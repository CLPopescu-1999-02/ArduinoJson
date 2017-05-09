// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>
#include <limits>

void check(JsonVariant variant, const std::string &expected) {
  char buffer[256] = "";
  size_t returnValue = variant.printTo(buffer, sizeof(buffer));
  REQUIRE(expected == buffer);
  REQUIRE(expected.size() == returnValue);
}

TEST_CASE("JsonVariant::printTo()") {
  SECTION("Empty") {
    check(JsonVariant(), "");
  }

  SECTION("Null") {
    check(static_cast<char *>(0), "null");
  }

  SECTION("String") {
    check("hello", "\"hello\"");
  }

  SECTION("Double Zero") {
    check(0.0, "0");
  }

  SECTION("Double") {
    check(3.14159265358979323846, "3.1415927");
  }

  SECTION("Infinity") {
    check(std::numeric_limits<double>::infinity(), "Infinity");
  }

  SECTION("MinusInfinity") {
    check(-std::numeric_limits<double>::infinity(), "-Infinity");
  }

  SECTION("SignalingNaN") {
    check(std::numeric_limits<double>::signaling_NaN(), "NaN");
  }

  SECTION("QuietNaN") {
    check(std::numeric_limits<double>::quiet_NaN(), "NaN");
  }

  SECTION("Integer") {
    check(42, "42");
  }

  SECTION("NegativeLong") {
    check(-42, "-42");
  }

  SECTION("UnsignedLong") {
    check(4294967295UL, "4294967295");
  }

  SECTION("True") {
    check(true, "true");
  }

  SECTION("OneFalse") {
    check(false, "false");
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("NegativeInt64") {
    check(-9223372036854775807 - 1, "-9223372036854775808");
  }

  SECTION("PositiveInt64") {
    check(9223372036854775807, "9223372036854775807");
  }

  SECTION("UInt64") {
    check(18446744073709551615, "18446744073709551615");
  }
#endif
}
