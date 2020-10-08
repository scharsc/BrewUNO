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
                BrewSettingsService *brewSettingsService);

  HeaterServiceStatus Compute(double input, double target, double heaterPercentage);

private:
  boolean IsOn();
  void SwitchOff();
  void SwitchOn();
  boolean StopCompute();

  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
};
#endif