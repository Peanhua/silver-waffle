#ifndef SUBSYSTEM_JOBS_HH_
#define SUBSYSTEM_JOBS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Subsystem.hh"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

typedef std::function<bool()> job_func_t;

class Job
{
public:
  Job(unsigned int id, job_func_t callback);
  
  unsigned int      _id;
  job_func_t        _callback;
  std::atomic<bool> _active;
  std::atomic<bool> _busy;
};



class Processor
{
public:
  Processor();
  void  Stop();
  Job * GetJob() const;
private:
  std::atomic<bool>  _exit_thread;
  std::thread        _thread;
  std::atomic<Job *> _job;
  std::mutex         _job_mutex;
};


class SubsystemJobs : public Subsystem
{
public:

  
  SubsystemJobs();

  bool Start()       override;
  void Tick(double deltatime) override;
  void StopThreads() override;
  void Stop()        override;

  unsigned int AddJob(job_func_t callback);
  bool         IsJobFinished(unsigned int job_id);
  void         ReleaseJob(unsigned int job_id);
  void         ReleaseJobNoLock(unsigned int job_id);

  void  WaitForSignal();
  Job * GetNextJob();
  
  void SignalOneProcessor();
  void SignalAllProcessors();
  
private:
  
  std::vector<Processor *> _processors;

  std::condition_variable _signal;
  std::mutex              _signal_mutex;
  
  std::deque<Job *>      _jobs;
  unsigned int           _jobs_pos;
  std::mutex             _jobs_mutex;

  std::atomic<unsigned int> _next_job_id;

  void ReleaseSomeInactiveJobs();
};


extern SubsystemJobs * Jobs;

#endif
