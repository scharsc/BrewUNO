#include "HeaterService.h"

boolean _HeaterEnabled;

HeaterService::HeaterService(TemperatureService *temperatureService,
                             ActiveStatus *activeStatus,
                             BrewSettingsService *brewSettingsService)
    : _temperatureService(temperatureService),
      _activeStatus(activeStatus),
      _brewSettingsService(brewSettingsService)
{
    _HeaterEnabled = false;
}


boolean HeaterService::IsOn()
{
  return _HeaterEnabled;
}

void HeaterService::SwitchOff()
{
  if (_HeaterEnabled)
  {
    _HeaterEnabled = false;
    digitalWrite(HEATER_BUS, LOW);
  }
}

void HeaterService::SwitchOn()
{
  if (!_HeaterEnabled)
  {
    _HeaterEnabled = true;
    digitalWrite(HEATER_BUS, HIGH);
  }
}

boolean HeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted ||
         !(_activeStatus->ActiveStep == mash || _activeStatus->ActiveStep == boil) ||
         _activeStatus->PumpIsResting ||
         !_activeStatus->HeaterOn;
}

HeaterServiceStatus HeaterService::Compute(double input, double target, double heaterPercentage)
{
    HeaterServiceStatus status;
    status.PIDActing = false;
    status.PWM = 0;

    if (StopCompute())
    {
        SwitchOff();
        status.PWMPercentage = (IsOn()) ? 100 : 0;
        return status;
    }

    if (_activeStatus->ActiveStep == boil)
    {
        SwitchOn();
    }
    else if (_activeStatus->ActiveStep == mash)
    {
        if (input < target)
          SwitchOn();
        else
          SwitchOff();
    }

    status.PWMPercentage = (IsOn()) ? 100 : 0;
    return status;
}