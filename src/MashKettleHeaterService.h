#ifndef MashKettleHeaterService_h
#define MashKettleHeaterService_h

#include <HeaterService.h>

class MashKettleHeaterService : public HeaterService
{
public:
  MashKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);

protected:
  boolean IsOn();
  void SwitchOff();
  void SwitchOn();
  boolean StopCompute();
  void TurnOff();
};
#endif