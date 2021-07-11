#ifndef SUBSYSTEM_SETTINGS_HH_
#define SUBSYSTEM_SETTINGS_HH_

#include "Subsystem.hh"
#include <map>


class SubsystemSettings : public Subsystem
{
public:
  SubsystemSettings();

  bool Start() override;
  void Stop() override;

  bool   GetBool(const std::string & name) const;
  void   SetBool(const std::string & name, bool value);
  int    GetInt(const std::string & name) const;
  double GetDouble(const std::string & name) const;

private:
  std::map<std::string, bool> _bool_values;
  std::map<std::string, int>  _int_values;
};

extern SubsystemSettings * Settings;

#endif
