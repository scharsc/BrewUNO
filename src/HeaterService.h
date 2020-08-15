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
  
    if (StopCompute())
    {
      TurnOff();
      status.PWMPercentage = (IsOn()) ? 100 : 0;
      return status;
    }

    if (input < target)
      SwitchOn();
    else
      SwitchOff();

    status.PWMPercentage = (IsOn()) ? 100 : 0;
    return status;
  }

protected:
  virtual boolean IsOn();
  virtual void SwitchOff();
  virtual void SwitchOn();
  virtual boolean StopCompute();
  virtual void TurnOff();

  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
};
#endif