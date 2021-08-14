#ifndef OBSERVER_HH_
#define OBSERVER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <vector>


template<typename T> class Subject;



template<typename T> class Observer
{
public:
  virtual ~Observer()
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
