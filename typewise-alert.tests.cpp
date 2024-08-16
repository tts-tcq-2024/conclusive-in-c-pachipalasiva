#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLimits) {
  // Test for inferBreach function
  void inferBreach() {
  TemperatureRange range = {0, 35};

   ASSERT_EQ(inferBreach(12, 20), TOO_LOW);
   ASSERT_EQ(inferBreach(25, 20), NORMAL);
   ASSERT_EQ(inferBreach(35, 20), TOO_HIGH);
}
}
