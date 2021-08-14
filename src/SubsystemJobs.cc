/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemJobs.hh"
#include <cassert>
#include <iostream>


SubsystemJobs * Jobs = nullptr;


SubsystemJobs::SubsystemJobs()
  : Subsystem("Jobs"),
    _jobs_pos(0),
    _next_job_id(0)
{
}


bool SubsystemJobs::Start()
{
  assert(!Jobs);
  Jobs = this;

  auto count = std::max(3u, std::thread::hardware_concurrency()) - 1;
  std::cout << GetName() << ": Using " << count << " job threads.\n";

  _processors.clear();
  
  for(unsigned int i = 0; i < count; i++)
    _processors.push_back(new Processor());

  std::atomic_thread_fence(std::memory_order_release);
  _next_job_id++; // done for the fence

  return true;
}


void SubsystemJobs::StopThreads()
{
  for(auto p : _processors)
    p->Stop();
}


void SubsystemJobs::Stop()
{
  for(auto j : _jobs)
    delete j;

  for(auto p : _processors)
    delete p;
}


void SubsystemJobs::WaitForSignal()
{
  std::unique_lock<std::mutex> lock(_signal_mutex);
  _signal.wait_for(lock, std::chrono::seconds(1));
}


void SubsystemJobs::SignalOneProcessor()
{
  _signal.notify_one();
}


void SubsystemJobs::SignalAllProcessors()
{
  _signal.notify_all();
}


unsigned int SubsystemJobs::AddJob(job_func_t callback)
{
  unsigned int id;
  {
    std::lock_guard lock(_jobs_mutex);

    id = _next_job_id;
    assert(id);

    auto job = new Job(id, callback);

    bool done = false;
    for(unsigned int i = 0; !done && i < _jobs.size(); i++)
      if(!_jobs[i])
        {
          _jobs[i] = job;
          done = true;
        }
    if(!done)
      _jobs.push_back(job);

    std::atomic_thread_fence(std::memory_order_release);
    _next_job_id++; // done here for the fence
    if(_next_job_id == 0)
      _next_job_id++;
  }

  SignalOneProcessor();

  return id;
}


Job * SubsystemJobs::GetNextJob()
{
  Job * job = nullptr;
  if(_jobs.size() > 0)
    {
      std::lock_guard lock(_jobs_mutex);
      for(unsigned int i = 0; !job && i < _jobs.size(); i++)
        {
          _jobs_pos++;
          if(_jobs_pos >= _jobs.size())
            _jobs_pos = 0;

          auto j = _jobs[_jobs_pos];
          if(j && !j->_busy && j->_active)
            job = j;
        }
    }

  if(!job)
    Jobs->WaitForSignal();
    
  return job;
}


bool SubsystemJobs::IsJobFinished(unsigned int job_id)
{
  std::lock_guard lock(_jobs_mutex);
  for(auto j : _jobs)
    if(j && j->_id == job_id)
      return !j->_active;
  return true;
}


void SubsystemJobs::ReleaseJob(unsigned int job_id)
{
  std::lock_guard lock(_jobs_mutex);
  for(unsigned int i = 0; i < _jobs.size(); i++)
    if(_jobs[i] && _jobs[i]->_id == job_id)
      {
        delete _jobs[i];
        _jobs[i] = nullptr;
        return;
      }
}




Processor::Processor()
  : _exit_thread(false),
    _job(nullptr)
{
  _thread = std::thread([this]()
  {
    bool exit = _exit_thread;
    while(!exit)
      {
        std::atomic_thread_fence(std::memory_order_acquire);
        auto job = Jobs->GetNextJob();
        exit = _exit_thread.load(std::memory_order_acquire);
        if(!exit && job && !job->_busy)
          {
            job->_busy = true;
            job->_active = job->_callback();
            job->_busy = false;
          }
      }
  });
}


void Processor::Stop()
{
  _exit_thread.store(true, std::memory_order_release);
  Jobs->SignalAllProcessors();
  _thread.join();
}




Job * Processor::GetJob() const
{
  return _job;
}


Job::Job(unsigned int id, job_func_t callback)
  : _id(id),
    _callback(callback),
    _active(true),
    _busy(false)
{
}
