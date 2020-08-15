#ifndef MashKettleHeaterService_h
#define MashKettleHeaterService_h

#include <HeaterService.h>

class MashKettleHeaterService : public HeaterService
{
public:
  MashKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);

protected:
  boolean StopCompute();
  uint8_t GetBus();
  void TurnOff();
};
#endif