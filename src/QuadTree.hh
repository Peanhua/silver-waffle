#ifndef QUADTREE_HH_
#define QUADTREE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Object.hh"
#include "RingBuffer.hh"


template <typename T> class Result
{ // Groups container objects into one.
  // todo: Add separate iterator, begin() and end() methods.
public:
  Result()
    : _current_list(0),
      _current_list_index(0)
  {
  }


  void Clear()
  {
    _lists.clear();
  }

  
  void Add(RingBuffer<T> * list)
  {
    for(auto l : _lists)
      assert(l != list);
    
    _lists.push_back(list);
  }
  

  void Rewind()
  {
    _current_list = 0;
    _current_list_index = 0;
  }

  
  T Next()
  {
    if(_current_list >= _lists.size())
      return nullptr;
    
    auto cur = _lists[_current_list];
    if(_current_list_index >= cur->size())
      {
        _current_list++;
        _current_list_index = 0;
        return Next();
      }

    auto object = cur->at(_current_list_index);
    _current_list_index++;
    
    if(object)
      return object;
    else
      return Next();
  }
  
private:
  std::vector<RingBuffer<T> *> _lists;
  unsigned int _current_list;
  unsigned int _current_list_index;
};


/*
** Keep the objects in a 2d grid array, evenly spaced.
** Add(), Remove(), and Move() are called from the objects.
** Results from GetAll() and GetNearby() use a Result object that contains pointers to the grid elements, and thus are always ready and up to date.
*/
class QuadTree
{
public:
  QuadTree(const glm::vec3 & dimensions, float spacing)
    : _dimensions(dimensions),
      _spacing(spacing)
  {
  }

  virtual ~QuadTree()
  {
  }

  void Initialize()
  {
    assert(_grid.size() == 0);

    for(int i = 0; i < _width * _height; i++)
      {
        auto rb = new RingBuffer<Object *>();
        _grid.push_back(rb);
        _all.Add(rb);
        
        auto r = new Result<Object *>();
        _nearby.push_back(r);
      }

    _grid.push_back(new RingBuffer<Object *>()); // outer space objects
    _grid.push_back(new RingBuffer<Object *>()); // too big objects

    const auto outerspace = static_cast<unsigned int>(_width * _height);
    const auto toobig     = static_cast<unsigned int>(_width * _height + 1);

    _all.Add(_grid[outerspace]);
    _all.Add(_grid[toobig]);

    for(int yyy = 0; yyy < _height; yyy++)
      for(int xxx = 0 ; xxx < _width; xxx++)
        {
          auto nearby = _nearby[static_cast<unsigned int>(xxx + yyy * _width)];
          for(int yy = -1 ; yy <= 1; yy++)
            for(int xx = -1; xx <= 1; xx++)
              {
                int x = xx + xxx;
                int y = yy + yyy;
                if(x >= 0 && x < _width  &&  y >= 0 && y < _height)
                  nearby->Add(_grid[static_cast<unsigned int>(x + y * _width)]);
              }
          nearby->Add(_grid[outerspace]);
          nearby->Add(_grid[toobig]);
        }
  }

  void Add(Object * object)
  {
    auto rb = _grid[GetIndex(object)];
    rb->Add(object);
  }
  
  void Remove(Object * object)
  {
    auto rb = _grid[GetIndex(object)];
    rb->Remove(object);
  }
  
  void Move(Object * object, const glm::vec3 & previous_position)
  {
    unsigned int prevpos;
    if(IsObjectFit(object))
      prevpos = GetIndex(previous_position);
    else
      prevpos = static_cast<unsigned int>(_width * _height + 1);
    
    auto newpos = GetIndex(object);
    if(prevpos != newpos)
      {
        _grid[prevpos]->Remove(object);
        _grid[newpos]->Add(object);
      }
  }

  Result<Object *> & GetAll()
  {
    _all.Rewind();
    return _all;
  }
  
  Result<Object *> & GetNearby(const glm::vec3 & position)
  {
    Result<Object *> * rv;

    const auto ind = GetIndex(position);
    if(ind < static_cast<unsigned int>(_width * _height))
      rv = _nearby[ind];
    else
      rv = &_all;
    
    rv->Rewind();
    
    return *rv;
  }
  
protected:
  glm::vec3                    _dimensions;
  float                        _spacing;
  std::vector<RingBuffer<Object *> *> _grid; // this could be std::array<RingBuffer<Object *> *, GetGridWidth() * GetGridHeight()>
  int                          _width;
  int                          _height;
  std::vector<Result<Object *> *>     _nearby; // Contains pointers to 3x3 closest grid elements (ie. 8 neighbors & self). Could also be std::array
  Result<Object *>                    _all;

  bool IsObjectFit(const Object * object) const
  {
    return static_cast<float>(object->GetVisualBoundingSphereRadius()) < _spacing;
  }
  
  unsigned int GetIndex(const Object * object) const
  {
    if(IsObjectFit(object))
      return GetIndex(object->GetPosition());
    else
      return static_cast<unsigned int>(_width * _height + 1);
  }

  virtual unsigned int GetIndex(const glm::vec3 & position) const = 0;
};


class QuadTreeXZ : public QuadTree
{
public:
  QuadTreeXZ(const glm::vec3 & dimensions, float spacing)
    : QuadTree(dimensions, spacing)
  {
    _width = 1 + static_cast<int>(dimensions.x / spacing);
    _height = 1 + static_cast<int>(dimensions.z / spacing);
    Initialize();
  }
  
protected:
  unsigned int GetIndex(const glm::vec3 & position) const override
  {
    auto x = static_cast<unsigned int>((position.x + _dimensions.x * 0.5f) / _spacing);
    auto y = static_cast<unsigned int>((position.z + _dimensions.z * 0.5f) / _spacing);
    if(/*x >= 0 &&*/ x < static_cast<unsigned int>(_width) &&
       /*y >= 0 &&*/ y < static_cast<unsigned int>(_height)  )
      return x + y * static_cast<unsigned int>(_width);
    else
      return static_cast<unsigned int>(_width * _height);
  }
};

class QuadTreeXY : public QuadTree
{
public:
  QuadTreeXY(const glm::vec3 & dimensions, float spacing)
    : QuadTree(dimensions, spacing)
  {
    _width = 1 + static_cast<int>(dimensions.x / spacing);
    _height = 1 + static_cast<int>(dimensions.y / spacing);
    Initialize();
  }
  
protected:
  unsigned int GetIndex(const glm::vec3 & position) const override
  {
    auto x = static_cast<unsigned int>((position.x + _dimensions.x * 0.5f) / _spacing);
    auto y = static_cast<unsigned int>((position.y + _dimensions.y * 0.5f) / _spacing);
    if(/*x >= 0 &&*/ x < static_cast<unsigned int>(_width) &&
       /*y >= 0 &&*/ y < static_cast<unsigned int>(_height)  )
      return x + y * static_cast<unsigned int>(_width);
    else
      return static_cast<unsigned int>(_width * _height);
  }
};

#endif
