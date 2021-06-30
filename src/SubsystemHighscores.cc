#include "SubsystemHighscores.hh"
#include "HighscoreEntry.hh"
#include "SubsystemAssetLoader.hh"
#include <algorithm>
#include <fstream>


SubsystemHighscores * Highscores = nullptr;


SubsystemHighscores::SubsystemHighscores()
  : Subsystem("Highscores")
{
}


bool SubsystemHighscores::Start()
{
  if(Highscores)
    return false;

  Read();
  Highscores = this;

  return true;
}


void SubsystemHighscores::Stop()
{
  Write();
  if(Highscores == this)
    Highscores = nullptr;
}


void SubsystemHighscores::Add(HighscoreEntry * entry)
{
  _entries.push_back(entry);
  std::sort(_entries.begin(), _entries.end(),
            [](const HighscoreEntry * a, const HighscoreEntry * b)
            {
              return a->GetScore() > b->GetScore();
            });
}


const std::vector<HighscoreEntry *> & SubsystemHighscores::Get() const
{
  return _entries;
}


void SubsystemHighscores::Read()
{
  auto data = AssetLoader->LoadJson("Highscores");
  if(!(*data)["entries"].is_array())
    return;

  for(auto entryjson : (*data)["entries"].array_items())
    Add(new HighscoreEntry(entryjson));
}


void SubsystemHighscores::Write()
{
  std::vector<json11::Json> entries;
  for(auto e : _entries)
    entries.push_back(e->ToJson());
  
  json11::Json data = json11::Json::object
    {
      { "entries", json11::Json(entries) }
    };
  
  std::ofstream file("Highscores.json");
  file << data.dump();
}
