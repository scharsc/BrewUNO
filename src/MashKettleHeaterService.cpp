#include <MashKettleHeaterService.h>

boolean _MashKettleHeaterEnabled;

MashKettleHeaterService::MashKettleHeaterService(TemperatureService *temperatureService,
                                                 ActiveStatus *activeStatus,
                                                 BrewSettingsService *brewSettingsService) : HeaterService(temperatureService,
                                                                                                           activeStatus,
                                                                                                           brewSettingsService)
{
  _MashKettleHeaterEnabled = false;
}

boolean MashKettleHeaterService::IsOn()
{
  return _MashKettleHeaterEnabled;
}

void MashKettleHeaterService::SwitchOff()
{
  if (_MashKettleHeaterEnabled)
  {
    _MashKettleHeaterEnabled = false;
    digitalWrite(HEATER_BUS, LOW);
    Serial.println("MashKettleHeaterService::SwitchOff");
  }
}

void MashKettleHeaterService::SwitchOn()
{
  if (!_MashKettleHeaterEnabled)
  {
    _MashKettleHeaterEnabled = true;
    digitalWrite(HEATER_BUS, HIGH);
    Serial.println("MashKettleHeaterService::SwitchOn");
  }
}

boolean MashKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted ||
         _activeStatus->ActiveStep != mash ||
         _activeStatus->PumpIsResting ||
         !_activeStatus->HeaterOn;
}

void MashKettleHeaterService::TurnOff()
{
  if (!_activeStatus->BrewStarted ||
      (_activeStatus->ActiveStep == boil && _activeStatus->EnableBoilKettle) ||
      (_activeStatus->ActiveStep == mash && _activeStatus->PumpIsResting) ||
      (_activeStatus->ActiveStep == mash && !_activeStatus->HeaterOn))
      {
        SwitchOff();
      }
}