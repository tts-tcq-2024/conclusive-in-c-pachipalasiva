#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLow) {
  TemperatureRange range = {0, 35};  // lower limit 0, upper limit 35
  EXPECT_EQ(inferBreach(-1, range), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToHIGH) {
  TemperatureRange range = {0, 35}; 
  EXPECT_EQ(inferBreach(36, range), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToNormal) {
  TemperatureRange range = {0, 35};
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

TEST(TypeWiseAlertTestSuite, SendsLowTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}
TEST(TypeWiseAlertTestSuite, SendHighTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}
TEST(TypeWiseAlertTestSuite, SendNormalTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

// Test sendToController function
// Test case for sending TOO_LOW breach type to controller
TEST(SendToControllerTest, SendsTooLowBreach) {
    testing::internal::CaptureStdout();
    sendToController(TOO_LOW);  // Assuming TOO_LOW is represented by 1
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");
}

// Test case for sending TOO_HIGH breach type to controller
TEST(SendToControllerTest, SendsTooHighBreach) {
    testing::internal::CaptureStdout();
    sendToController(TOO_HIGH);  // Assuming TOO_HIGH is represented by 2
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");
}

// Test case for sending NORMAL breach type to controller
TEST(SendToControllerTest, SendsNormalBreach) {
    testing::internal::CaptureStdout();
    sendToController(NORMAL);  // Assuming NORMAL is represented by 0
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");
}
// Test case for getTemperatureRange function
TEST(TemperatureRangeTest, GetTemperatureRange) {
    TemperatureRange range;

    // Test for PASSIVE_COOLING
    range = getTemperatureRange(PASSIVE_COOLING);
    EXPECT_EQ(range.lowerLimit, 0);
    EXPECT_EQ(range.upperLimit, 35);

    // Test for HI_ACTIVE_COOLING
    range = getTemperatureRange(HI_ACTIVE_COOLING);
    EXPECT_EQ(range.lowerLimit, 0);
    EXPECT_EQ(range.upperLimit, 45);

    // Test for MED_ACTIVE_COOLING
    range = getTemperatureRange(MED_ACTIVE_COOLING);
    EXPECT_EQ(range.lowerLimit, 0);
    EXPECT_EQ(range.upperLimit, 40);
}


// Helper function to test checkAndAlert
void TestCheckAndAlert(AlertTarget alertTarget, CoolingType coolingType, double temperatureInC, const std::string& expectedOutput) {
    BatteryCharacter batteryChar = {coolingType, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(alertTarget, batteryChar, temperatureInC);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, expectedOutput);
}

// Test case for sending email when temperature is too high
TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooHigh) {
    TestCheckAndAlert(TO_EMAIL, MED_ACTIVE_COOLING, 41, "To: a.b@c.com\nHi, the temperature is too high\n");
}

// Test case for sending email when temperature is too low
TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooLow) {
    TestCheckAndAlert(TO_EMAIL, MED_ACTIVE_COOLING, -1, "To: a.b@c.com\nHi, the temperature is too low\n");
}

// Test case for sending to controller when temperature is too high
TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerTooHigh) {
    TestCheckAndAlert(TO_CONTROLLER, MED_ACTIVE_COOLING, 41, "feed : 1\n");
}

// Test case for sending to controller when temperature is too low
TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerTooLow) {
    TestCheckAndAlert(TO_CONTROLLER, MED_ACTIVE_COOLING, -1, "feed : 0\n");
}

// Test case for no alert when temperature is normal
TEST(TypeWiseAlertTestSuite, CheckAndAlertNoAlertWhenNormal) {
    TestCheckAndAlert(TO_EMAIL, MED_ACTIVE_COOLING, 25, "");
}
