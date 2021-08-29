/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "AStar.hh"
#include "NavigationMap.hh"
#include <cassert>
#include <iostream>
#include <queue>



AStar::PathComponent::PathComponent(PathComponent * source, const glm::ivec2 & position, double cost)
  : _source(source),
    _position(position),
    _cost(cost),
    _is_open(true)
{
}
    
bool AStar::PathComponent::CompareTo(const PathComponent & other) const
{
  return _cost < other._cost;
}

void AStar::PathComponent::SetIsOpen(bool is_open)
{
  _is_open = is_open;
}

const glm::ivec2 & AStar::PathComponent::GetPosition() const
{
  return _position;
}

double AStar::PathComponent::GetCost() const
{
  return _cost;
}

void AStar::PathComponent::SetCost(double cost)
{
  _cost = cost;
}

AStar::PathComponent * AStar::PathComponent::GetSource() const
{
  return _source;
}

void AStar::PathComponent::SetSource(PathComponent * source)
{
  _source = source;
}



AStar::AStar(NavigationMap * navigation_map)
  : _navigation_map(navigation_map)
{
}

std::vector<glm::ivec2> * AStar::FindPath(const glm::ivec2 & start, const glm::ivec2 & destination)
{
  auto SortOpenList = [this, &destination](const PathComponent * left, const PathComponent * right)
  {
    auto lcost = left->GetCost() + EstimateCost(left->GetPosition(), destination);
    auto rcost = right->GetCost() + EstimateCost(right->GetPosition(), destination);
    return lcost > rcost;
  };
  std::priority_queue<PathComponent *, std::vector<PathComponent *>, decltype(SortOpenList)> open(SortOpenList);
  
  _all.clear();
  
  std::vector<glm::ivec2> neighbors;
  
  auto first = new PathComponent(nullptr, start, 0);
  open.push(first);
  _all.push_back(first);

  PathComponent * last = nullptr;
  while(!last && open.size() > 0)
    {
      auto current = open.top();
      open.pop();
      current->SetIsOpen(false);
      
      GetNeighbors(current->GetPosition(), neighbors);
      for(auto neighbor_pos : neighbors)
        {
          double new_neighbor_cost = current->GetCost() + 1;
          if(_navigation_map->Get(current->GetPosition()) == '.')
            new_neighbor_cost += 1;
          
          auto IsSamePosition = [&neighbor_pos](const PathComponent * pc) { return pc->GetPosition() == neighbor_pos; };
          auto existing = std::find_if(_all.cbegin(), _all.cend(), IsSamePosition);
          
          if(existing != _all.cend())
            {
              if((*existing)->GetCost() > new_neighbor_cost)
                {
                  (*existing)->SetCost(new_neighbor_cost);
                  (*existing)->SetSource(current);
                }
            }
          else
            {
              auto neighbor = new PathComponent(current, neighbor_pos, new_neighbor_cost);
              open.push(neighbor);
              _all.push_back(neighbor);
            }
        }
      
      
      if(IsAtDestination(current->GetPosition(), destination))
        last = current;
    }

  if(!last)
    return nullptr;

  
  auto rv = new std::vector<glm::ivec2>();
  while(last)
    {
      rv->emplace_back(last->GetPosition());
      last = last->GetSource();
    }
  std::reverse(rv->begin(), rv->end());
  
  return rv;
}


bool AStar::IsAtDestination(const glm::ivec2 & position, const glm::ivec2 & destination)
{
  return EstimateCost(position, destination) < 0.1;
}

double AStar::EstimateCost(const glm::ivec2 & from, const glm::ivec2 & to)
{
  return glm::distance(glm::vec2(from), glm::vec2(to));
}

void AStar::GetNeighbors(const glm::ivec2 & position, std::vector<glm::ivec2> & neighbor_positions)
{
  auto AddPos = [this, &position, &neighbor_positions](int x, int y)
  {
    auto pos = position + glm::ivec2(x, y);
    auto d = _navigation_map->Get(pos);
    if(d == ' ' || d == '.')
      neighbor_positions.emplace_back(pos);
  };

  neighbor_positions.clear();

  AddPos(-1,  0);
  AddPos( 1,  0);
  AddPos( 0, -1);
  AddPos( 0,  1);
}
