#include "typewise-alert.h"
#include <stdio.h>

typedef struct {
  int lowerLimit;
  int upperLimit;
} TemperatureRange;

// Function to get the temperature range based on cooling type
TemperatureRange getTemperatureRange(CoolingType coolingType) {
  TemperatureRange ranges[] = {
    {0, 35}, // PASSIVE_COOLING
    {0, 45}, // HI_ACTIVE_COOLING
    {0, 40}  // MED_ACTIVE_COOLING
  };
  return ranges[coolingType];
}

// Function to infer breach type based on value and limits
BreachType inferBreach(double value, TemperatureRange range) {
  if (value < range.lowerLimit) {
    return TOO_LOW;
  }
  if (value > range.upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

// Function to classify temperature breach based on cooling type and temperature
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  TemperatureRange range = getTemperatureRange(coolingType);
  return inferBreach(temperatureInC, range);
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}
void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if (breachType == TOO_LOW) {
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too low\n");
  } else if (breachType == TOO_HIGH) {
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too high\n");
  }
}
