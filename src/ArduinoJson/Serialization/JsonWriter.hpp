// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <stdint.h>
#include "../Data/Encoding.hpp"
#include "../Data/JsonFloat.hpp"
#include "../Data/JsonInteger.hpp"
#include "../Polyfills/attributes.hpp"
#include "../Polyfills/math.hpp"
#include "../Polyfills/normalize.hpp"

namespace ArduinoJson {
namespace Internals {

// Writes the JSON tokens to a Print implementation
// This class is used by:
// - JsonArray::writeTo()
// - JsonObject::writeTo()
// - JsonVariant::writeTo()
// Its derived by PrettyJsonWriter that overrides some members to add
// indentation.
template <typename Print>
class JsonWriter {
 public:
  explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

  // Returns the number of bytes sent to the Print implementation.
  // This is very handy for implementations of printTo() that must return the
  // number of bytes written.
  size_t bytesWritten() const {
    return _length;
  }

  void beginArray() {
    writeRaw('[');
  }
  void endArray() {
    writeRaw(']');
  }

  void beginObject() {
    writeRaw('{');
  }
  void endObject() {
    writeRaw('}');
  }

  void writeColon() {
    writeRaw(':');
  }
  void writeComma() {
    writeRaw(',');
  }

  void writeBoolean(bool value) {
    writeRaw(value ? "true" : "false");
  }

  void writeString(const char *value) {
    if (!value) {
      writeRaw("null");
    } else {
      writeRaw('\"');
      while (*value) writeChar(*value++);
      writeRaw('\"');
    }
  }

  void writeChar(char c) {
    char specialChar = Encoding::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }

  void writeFloat(JsonFloat value) {
    JsonFloat error = 1e-10;

    if (Polyfills::isNaN(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (Polyfills::isInfinity(value)) return writeRaw("Infinity");

    int powersOf10;  // TODO: use short
    if (value > 0 && (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD ||
                      value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD)) {
      powersOf10 = Polyfills::normalize(value, error);
    } else {
      powersOf10 = 0;
    }

    // Extract the integer part of the value and print it
    JsonUInt int_part = static_cast<JsonUInt>(value + error);
    JsonFloat remainder = value - static_cast<JsonFloat>(int_part);
    writeInteger(int_part);

    if (remainder > error) {
      // Print the decimal point, but only if there are digits beyond
      writeRaw('.');

      // Extract digits from the remainder one at a time
      while (remainder >= error) {
        // Extract digit
        remainder *= 10.0;
        error *= 10.0;
        char currentDigit = char(remainder + error);
        remainder -= static_cast<JsonFloat>(currentDigit);

        // Print
        writeRaw(char('0' + currentDigit));
      }
    } else if (powersOf10 == 0) {
      writeRaw(".0");  // so that isFloat() will return true
    }

    if (powersOf10 < 0) {
      writeRaw("e-");
      writeInteger(-powersOf10);
    }

    if (powersOf10 > 0) {
      writeRaw('e');
      writeInteger(powersOf10);
    }
  }

  void writeInteger(JsonUInt value) {
    char buffer[22];
    char *ptr = buffer + sizeof(buffer) - 1;

    *ptr = 0;
    do {
      *--ptr = static_cast<char>(value % 10 + '0');
      value /= 10;
    } while (value);

    writeRaw(ptr);
  }

  void writeRaw(const char *s) {
    _length += _sink.print(s);
  }
  void writeRaw(char c) {
    _length += _sink.print(c);
  }

 protected:
  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}
}
