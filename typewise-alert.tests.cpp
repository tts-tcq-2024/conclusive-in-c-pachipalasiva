#include <gtest/gtest.h>
#include "typewise-alert.h"
#include <gmock/gmock.h>
#include <sstream>
#include <iostream>

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

// Mock functions to replace printf
MOCK_FUNCTION_VOID(printf, const char*, ...);

class TypewiseAlertTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect stdout to suppress output during tests
        freopen("/dev/null", "w", stdout);
    }

    void TearDown() override {
        // Restore stdout
        fclose(stdout);
        stdout = fdopen(1, "w");
    }
};

TEST_F(TypewiseAlertTest, CheckAndAlertToController) {
    BatteryCharacter batteryChar;
    
    // Test PASSIVE_COOLING
    batteryChar.coolingType = PASSIVE_COOLING;
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_LOW));
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, NORMAL));
    checkAndAlert(TO_CONTROLLER, batteryChar, 20);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_HIGH));
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);

    // Test HI_ACTIVE_COOLING
    batteryChar.coolingType = HI_ACTIVE_COOLING;
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_LOW));
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, NORMAL));
    checkAndAlert(TO_CONTROLLER, batteryChar, 30);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_HIGH));
    checkAndAlert(TO_CONTROLLER, batteryChar, 46);

    // Test MED_ACTIVE_COOLING
    batteryChar.coolingType = MED_ACTIVE_COOLING;
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_LOW));
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, NORMAL));
    checkAndAlert(TO_CONTROLLER, batteryChar, 25);
    
    EXPECT_CALL(printf, ("%x : %x\n", 0xfeed, TOO_HIGH));
    checkAndAlert(TO_CONTROLLER, batteryChar, 41);
}

TEST_F(TypewiseAlertTest, CheckAndAlertToEmail) {
    BatteryCharacter batteryChar;
    const char* recipient = "a.b@c.com";
    
    // Test PASSIVE_COOLING
    batteryChar.coolingType = PASSIVE_COOLING;
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too low\n"));
    checkAndAlert(TO_EMAIL, batteryChar, -1);
    
    // Normal case doesn't send an email, so no EXPECT_CALL
    checkAndAlert(TO_EMAIL, batteryChar, 20);
    
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too high\n"));
    checkAndAlert(TO_EMAIL, batteryChar, 36);

    // Test HI_ACTIVE_COOLING
    batteryChar.coolingType = HI_ACTIVE_COOLING;
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too low\n"));
    checkAndAlert(TO_EMAIL, batteryChar, -1);
    
    // Normal case doesn't send an email, so no EXPECT_CALL
    checkAndAlert(TO_EMAIL, batteryChar, 30);
    
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too high\n"));
    checkAndAlert(TO_EMAIL, batteryChar, 46);

    // Test MED_ACTIVE_COOLING
    batteryChar.coolingType = MED_ACTIVE_COOLING;
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too low\n"));
    checkAndAlert(TO_EMAIL, batteryChar, -1);
    
    // Normal case doesn't send an email, so no EXPECT_CALL
    checkAndAlert(TO_EMAIL, batteryChar, 25);
    
    EXPECT_CALL(printf, ("To: %s\n", recipient));
    EXPECT_CALL(printf, ("Hi, the temperature is too high\n"));
    checkAndAlert(TO_EMAIL, batteryChar, 41);
}

