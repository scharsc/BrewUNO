#include <MashKettleHeaterService.h>

MashKettleHeaterService::MashKettleHeaterService(TemperatureService *temperatureService,
                                                 ActiveStatus *activeStatus,
                                                 BrewSettingsService *brewSettingsService) : HeaterService(temperatureService,
                                                                                                           activeStatus,
                                                                                                           brewSettingsService)
{
}


uint8_t MashKettleHeaterService::GetBus()
{
  return HEATER_BUS;
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
    digitalWrite(GetBus(), LOW);
}