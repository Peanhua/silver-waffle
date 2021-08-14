#ifndef ASTAR_HH_
#define ASTAR_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include <vector>

class NavigationMap;


class AStar
{
public:

  class PathComponent
  {
  public:
    PathComponent(PathComponent * source, const glm::ivec2 & position, double cost);
    
    bool               CompareTo(const PathComponent & other) const;
    void               SetIsOpen(bool is_open);
    const glm::ivec2 & GetPosition() const;
    double             GetCost() const;
    void               SetCost(double cost);
    PathComponent *    GetSource() const;
    void               SetSource(PathComponent * source);
    
  private:
    PathComponent * _source;
    glm::ivec2      _position;
    double          _cost;
    bool            _is_open;
  };



  AStar(NavigationMap * navigation_map);

  std::vector<glm::ivec2> * FindPath(const glm::ivec2 & start, const glm::ivec2 & destination);
  bool                      IsAtDestination(const glm::ivec2 & position, const glm::ivec2 & destination);
  double                    EstimateCost(const glm::ivec2 & from, const glm::ivec2 & to);
  void                      GetNeighbors(const glm::ivec2 & position, std::vector<glm::ivec2> & neighbor_positions);
  
private:
  NavigationMap *              _navigation_map;
  std::vector<PathComponent *> _all;
};

#endif
