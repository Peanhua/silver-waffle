#ifndef SUBSYSTEM_JOBS_HH_
#define SUBSYSTEM_JOBS_HH_

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
  void StopThreads() override;
  void Stop()        override;

  unsigned int AddJob(job_func_t callback);
  bool         IsJobFinished(unsigned int job_id);
  void         ReleaseJob(unsigned int job_id);

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
};


extern SubsystemJobs * Jobs;

#endif
