#include <gtest/gtest.h>
#include "data/JSONPresentation.h"

TEST(JSONPresentation, Basic) {
  Document expected;

  expected["a"] = "b";
  expected["c"] = "d";

  JSONPresentation converter;

  auto stringPresentation = converter.toString(expected);
  auto actual = converter.fromString(stringPresentation);

  ASSERT_EQ(expected, actual);
}

TEST(JSONPresentation, MalformedJSON) {
  JSONPresentation converter;

  try {
    converter.fromString("foobar}{");
  } catch (const std::exception& ) {

  }
}