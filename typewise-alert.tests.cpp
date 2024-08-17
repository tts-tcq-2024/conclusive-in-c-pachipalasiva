#include <gtest/gtest.h>
#include "typewise-alert.h"
#include <gmock/gmock.h>


TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLow) {
  TemperatureRange range = {0, 35};  // lower limit 0, upper limit 35
  EXPECT_EQ(inferBreach(-1, range), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToHIGH) {
  TemperatureRange range = {0, 35};  // lower limit 0, upper limit 36
  EXPECT_EQ(inferBreach(36, range), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToNormal) {
  TemperatureRange range = {0, 35};  // lower limit 0, upper limit 36
  EXPECT_EQ(inferBreach(20, range), NORMAL);
}
TEST(TypewiseAlertTest, ClassifyTemperatureBreachPassiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 35), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachHiActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 45), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachMedActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 40), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

// Function to test checkAndAlert
TEST(TypewiseAlertTest, ClassifyTemperatureBreachMedActiveCooling) {
    BatteryCharacter batteryChar;

    // Test PASSIVE_COOLING
    batteryChar.coolingType = PASSIVE_COOLING;
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    checkAndAlert(TO_CONTROLLER, batteryChar, 20);
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);

    checkAndAlert(TO_EMAIL, batteryChar, -1);
    checkAndAlert(TO_EMAIL, batteryChar, 20);
    checkAndAlert(TO_EMAIL, batteryChar, 36);

    // Test HI_ACTIVE_COOLING
    batteryChar.coolingType = HI_ACTIVE_COOLING;
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    checkAndAlert(TO_CONTROLLER, batteryChar, 30);
    checkAndAlert(TO_CONTROLLER, batteryChar, 46);

    checkAndAlert(TO_EMAIL, batteryChar, -1);
    checkAndAlert(TO_EMAIL, batteryChar, 30);
    checkAndAlert(TO_EMAIL, batteryChar, 46);

    // Test MED_ACTIVE_COOLING
    batteryChar.coolingType = MED_ACTIVE_COOLING;
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    checkAndAlert(TO_CONTROLLER, batteryChar, 25);
    checkAndAlert(TO_CONTROLLER, batteryChar, 41);

    checkAndAlert(TO_EMAIL, batteryChar, -1);
    checkAndAlert(TO_EMAIL, batteryChar, 25);
    checkAndAlert(TO_EMAIL, batteryChar, 41);

    // Note: We can't automatically verify the output of checkAndAlert
    // as it prints to stdout. Manual verification of the output is needed.
}
