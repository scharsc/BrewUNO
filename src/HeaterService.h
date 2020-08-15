#ifndef HeaterService_h
#define HeaterService_h

#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <enum.h>
#include <BrewSettingsService.h>

struct HeaterServiceStatus
{
  double PWM;
  double PWMPercentage;
  boolean PIDActing;
};

class HeaterService
{
public:
  HeaterService(TemperatureService *temperatureService,
                ActiveStatus *activeStatus,
                BrewSettingsService *brewSettingsService) : _temperatureService(temperatureService),
                                                            _activeStatus(activeStatus),
                                                            _brewSettingsService(brewSettingsService)

  {
  }

  HeaterServiceStatus Compute(double input, double target, double heaterPercentage)
  {
    HeaterServiceStatus status;
    status.PIDActing = false;
    status.PWM = 0;
    status.PWMPercentage = 0;
  
    uint8_t _heaterBus = GetBus();

    if (StopCompute())
    {
      TurnOff();
      return status;
    }

    if (_activeStatus->ActiveStep == boil)
    {
      digitalWrite(_heaterBus, (_brewSettingsService->BoilPowerPercentage > 0) ? HIGH : LOW);
      return status;
    }

    digitalWrite(_heaterBus, (heaterPercentage > 0) ? HIGH : LOW);

    return status;
  }

protected:
  virtual boolean StopCompute();
  virtual uint8_t GetBus();
  virtual void TurnOff();

  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
};
#endif