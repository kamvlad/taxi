#include <gtest/gtest.h>
#include "json/json.hpp"

using json = nlohmann::json;

TEST(JSONTests, malformedJSON) {
  try {
    json::parse("hellofoo");
    FAIL();
  } catch (const std::invalid_argument &e) {
    // OK
  }
}