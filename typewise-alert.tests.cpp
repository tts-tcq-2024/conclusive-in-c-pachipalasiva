#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite,InfersBreachAccordingToLimits) {
  // Test for inferBreach function
  void test_inferBreach() {
  TemperatureRange range = {0, 35};

  assert(inferBreach(-1, range) == TOO_LOW);
  assert(inferBreach(36, range) == TOO_HIGH);
  assert(inferBreach(20, range) == NORMAL);
}
}
