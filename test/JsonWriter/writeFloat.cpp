// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <catch.hpp>
#include <limits>
#include <string>

#include <ArduinoJson/Serialization/DynamicStringBuilder.hpp>
#include <ArduinoJson/Serialization/JsonWriter.hpp>

using namespace ArduinoJson::Internals;

void check(double input, const std::string& expected) {
  std::string output;
  DynamicStringBuilder<std::string> sb(output);
  JsonWriter<DynamicStringBuilder<std::string> > writer(sb);
  writer.writeFloat(input);
  REQUIRE(writer.bytesWritten() == output.size());
  CHECK(output == expected);
}

TEST_CASE("JsonWriter::writeFloat()") {
  SECTION("NaN") {
    double nan = std::numeric_limits<double>::signaling_NaN();
    check(nan, "NaN");
  }

  SECTION("Infinity") {
    double inf = std::numeric_limits<double>::infinity();
    check(inf, "Infinity");
    check(-inf, "-Infinity");
  }

  SECTION("#.#") {
    check(0.0, "0.0");
    check(0.1, "0.1");
    check(0.9, "0.9");

    check(9.0, "9.0");
    check(9.1, "9.1");
    check(9.9, "9.9");
  }

  SECTION("#.##") {
    check(0.01, "0.01");
    check(0.99, "0.99");

    check(9.01, "9.01");
    check(9.99, "9.99");
  }

  SECTION("#.###") {
    check(0.001, "0.001");
    check(0.999, "0.999");

    check(9.001, "9.001");
    check(9.999, "9.999");
  }

  SECTION("#.####") {
    check(0.0001, "0.0001");
    check(0.9999, "0.9999");

    check(9.0001, "9.0001");
    check(9.9999, "9.9999");
  }

  SECTION("##.#") {
    check(10.0, "10.0");
    check(10.1, "10.1");
    check(10.9, "10.9");

    check(99.0, "99.0");
    check(99.1, "99.1");
    check(99.9, "99.9");
  }

  SECTION("###.#") {
    check(100.0, "100.0");
    check(100.1, "100.1");
    check(100.9, "100.9");

    check(999.0, "999.0");
    check(999.1, "999.1");
    check(999.9, "999.9");
  }

  SECTION("####.#") {
    check(1000.0, "1000.0");
    check(1000.1, "1000.1");
    check(1000.9, "1000.9");

    check(9999.0, "9999.0");
    check(9999.1, "9999.1");
    check(9999.9, "9999.9");
  }

  SECTION("######.#") {
    check(100000.0, "100000.0");
    check(100000.1, "100000.1");
    check(999999.9, "999999.9");
  }

  SECTION("########.#") {
    check(1000000.0, "1e6");
    check(1000000.1, "1e6");
    check(9999999.9, "1e7");
  }

  SECTION("#e#") {
    check(1e6, "1e6");
    check(1e-6, "1e-6");
    check(9e9, "9e9");
    check(9e-9, "9e-9");
  }

  SECTION("#e###") {
    check(1e308, "1e308");
    check(1e-308, "1e-308");
    check(-1e308, "-1e308");
    check(-1e-308, "-1e-308");
  }
  /*
    SECTION("Pi") {
      check(3.14159265359, "3.14159265359");
    }*/
}
