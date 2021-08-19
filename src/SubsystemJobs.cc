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

  auto count = std::max(4u, std::thread::hardware_concurrency());
  count--; // Main thread.
  count--; // Audio threads (OpenAL, MusicPlayer).
  count += 2; // One or more threads are usually not 100% busy with CPU.
  std::cout << GetName() << ": Using " << count << " job threads.\n";

  _processors.clear();

  // Dedicate the first 2 for quick jobs:
  for(unsigned int i = 0; i < count; i++)
    _processors.push_back(new Processor(i < 2));

  std::atomic_thread_fence(std::memory_order_release);
  _next_job_id++; // done for the fence

  return true;
}


void SubsystemJobs::Tick([[maybe_unused]] double deltatime)
{
  ReleaseSomeInactiveJobs();
}


void SubsystemJobs::ReleaseSomeInactiveJobs()
{
  if(_jobs.size() == 0)
    return;

  auto CanBeReleased = [](const Job * job)
  {
    return job && !job->_active;
  };
  
  auto first = _jobs.front();
  if(!first || CanBeReleased(first))
    {
      std::lock_guard lock(_jobs_mutex);
      while(_jobs.size() > 0)
        {
          auto job = _jobs.front();
          if(job && !CanBeReleased(job))
            break;

          if(job)
            ReleaseJobNoLock(job->_id);
          _jobs.pop_front();
        }
    }
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


unsigned int SubsystemJobs::AddJob(bool is_quick, job_func_t callback)
{
  unsigned int id;
  {
    std::lock_guard lock(_jobs_mutex);

    id = _next_job_id;
    assert(id);

    auto job = new Job(id, is_quick, callback);
    _jobs.push_back(job);

    std::atomic_thread_fence(std::memory_order_release);
    _next_job_id++; // done here for the fence
    if(_next_job_id == 0)
      _next_job_id++;
  }

  SignalOneProcessor();

  return id;
}


Job * SubsystemJobs::GetNextJob(bool only_quick)
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
          if(j && j->_active && !j->_busy.load(std::memory_order_acquire))
            if(!only_quick || j->_is_quick)
              job = j;
        }
      if(job)
        job->_busy = true;
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
  ReleaseJobNoLock(job_id);
}


void SubsystemJobs::ReleaseJobNoLock(unsigned int job_id)
{
  for(unsigned int i = 0; i < _jobs.size(); i++)
    if(_jobs[i] && _jobs[i]->_id == job_id)
      {
        delete _jobs[i];
        _jobs[i] = nullptr;
        return;
      }
}







Processor::Processor(bool quick_jobs_only)
  : _quick_jobs_only(quick_jobs_only),
    _exit_thread(false),
    _job(nullptr)
{
  std::cout << "Starting job thread for " << (quick_jobs_only ? "quick" : "slow") << " jobs.\n";
  _thread = std::thread([this]()
  {
    bool exit = _exit_thread;
    while(!exit)
      {
        std::atomic_thread_fence(std::memory_order_acquire);
        auto job = Jobs->GetNextJob(_quick_jobs_only);
        exit = _exit_thread.load(std::memory_order_acquire);

        if(!exit && job)
          {
            job->_active = job->_callback();
            job->_busy.store(false, std::memory_order_release);
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


Job::Job(unsigned int id, bool is_quick, job_func_t callback)
  : _id(id),
    _is_quick(is_quick),
    _callback(callback),
    _active(true),
    _busy(false)
{
}
