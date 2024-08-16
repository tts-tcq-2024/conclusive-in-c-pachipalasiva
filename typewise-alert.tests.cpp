#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLimits) {
  // Test for inferBreach function
  TemperatureRange range = {20, 30};
  ASSERT_EQ(inferBreach(12, range), TOO_LOW);
}

