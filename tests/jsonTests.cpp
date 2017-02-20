#include <gtest/gtest.h>
#include "data/jsonPresentation.h"

TEST(JSONPresentation, Basic) {
  Document expected;

  expected["a"] = "b";
  expected["c"] = "d";

  jsonPresentation converter;

  auto stringPresentation = converter.toString(expected);
  auto actual = converter.fromString(stringPresentation);

  ASSERT_EQ(expected, actual);
}

TEST(JSONPresentation, MalformedJSON) {
  jsonPresentation converter;

  try {
    converter.fromString("foobar}{");
    FAIL();
  } catch (const std::exception& ) {

  }
}