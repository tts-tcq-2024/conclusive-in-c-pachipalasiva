#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLimits) {
  // Test for inferBreach function
  EXPECT_EQ(inferBreach(-1,35), TOO_LOW);
}

