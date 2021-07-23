#ifndef RINGBUFFER_HH_
#define RINGBUFFER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif
#include <vector>

// Requires the type T have a method: bool IsAlive().

template<typename T> class RingBuffer : public std::vector<T>
{
public:
#if defined(WITH_VALGRIND) || false
  // Constructor to silence Valgrind about uninitialized _pos in GetNextFreeIndex():
  RingBuffer()
    : std::vector<T>(),
      _pos(0)
  {
  }
#endif

    
  unsigned int GetNextFreeIndex()
  {
    for(std::vector<void *>::size_type i = 0; i < std::vector<T>::size(); i++)
      {
        _pos++;
        if(_pos >= std::vector<T>::size())
          _pos = 0;
        
        auto obj = std::vector<T>::at(_pos);
        if(!obj || !obj->IsAlive())
          return _pos;
      }
    return static_cast<unsigned int>(std::vector<T>::size());
  }

  unsigned int GetNextNullIndex()
  {
    for(std::vector<void *>::size_type i = 0; i < std::vector<T>::size(); i++)
      {
        _pos++;
        if(_pos >= std::vector<T>::size())
          _pos = 0;
        
        auto obj = std::vector<T>::at(_pos);
        if(!obj)
          return _pos;
      }
    return static_cast<unsigned int>(std::vector<T>::size());
  }

  void Add(T object)
  {
    auto ind = GetNextFreeIndex();
    if(ind < std::vector<T>::size())
      (*this)[ind] = object;
    else
      std::vector<T>::push_back(object);
  }

  void Remove(T object)
  {
    for(std::vector<void *>::size_type i = 0; i < std::vector<T>::size(); i++)
      if(std::vector<T>::at(i) == object)
        {
          (*this)[i] = nullptr;
          break;
        }
  }
  
private:
  unsigned int _pos;
};

#endif
