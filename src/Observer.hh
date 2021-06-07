#ifndef OBSERVER_HH_
#define OBSERVER_HH_

#include <vector>


template<typename T> class Subject;



template<typename T> class Observer
{
public:
  virtual ~Observer<T>()
  {
    for(auto s : _subjects)
      s->RemoveObserver(this);
  }

  void Observe(Subject<T> * subject)
  {
    subject->AddObserver(this);
    _subjects.push_back(subject);
  }
  
  void StopObserving(Subject<T> * subject)
  {
    for(unsigned int i = 0; i < _subjects.size(); i++)
      if(_subjects[i] == subject)
        _subjects[i] = nullptr;
    
    while(_subjects.size() > 0 && !_subjects[_subjects.size() - 1])
      _subjects.pop_back();
    
    subject->RemoveObserver(this);
  }

  virtual void OnNotifyUpdate(T *) = 0;
  
private:
  std::vector<Subject<T> *> _subjects;
};



template<typename T> class Subject
{
public:
  void AddObserver(Observer<T> * observer)
  {
    _observers.push_back(observer);
  }
  
  void RemoveObserver(Observer<T> * observer)
  {
    for(unsigned int i = 0; i < _observers.size(); i++)
      if(_observers[i] == observer)
        _observers[i] = nullptr;
    
    while(_observers.size() > 0 && !_observers[_observers.size() - 1])
      _observers.pop_back();
  }
  
  void NotifyObservers()
  {
    for(auto o : _observers)
      if(o)
        o->OnNotifyUpdate(static_cast<T *>(this));
  }

private:
  std::vector<Observer<T> *> _observers;
};

#endif
