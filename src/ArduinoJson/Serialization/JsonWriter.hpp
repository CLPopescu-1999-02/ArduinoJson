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
#include "../TypeTraits/FloatTraits.hpp"

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

  int8_t getDecimalPlaces(JsonFloat value) {
    int8_t result = 0;
    JsonFloat error = 1e-6;
    while (value > error && result < 6) {
      // Extract digit
      value *= 10.0;
      error *= 10.0;
      char currentDigit = char(value);
      value -= static_cast<JsonFloat>(currentDigit);
      result++;
    }
    return result;
  }

  void writeFloat(JsonFloat value) {
    const uint8_t maxDecimalPlaces = 9;
    const uint32_t maxDecimalPart = 1000000000;

    if (Polyfills::isNaN(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (Polyfills::isInfinity(value)) return writeRaw("Infinity");

    // TODO: use short
    int powersOf10 = Polyfills::normalize(value);

    uint32_t integralPart = static_cast<uint32_t>(value);
    JsonFloat remainder = value - static_cast<JsonFloat>(integralPart);

    uint32_t decimalPart = uint32_t(remainder * maxDecimalPart);
    remainder = remainder * maxDecimalPart - JsonFloat(decimalPart);

    // rounding
    if (remainder > 0.5) {
      decimalPart++;
      if (decimalPart >= maxDecimalPart) {
        decimalPart -= maxDecimalPart;
        integralPart++;
        if (powersOf10 > 0 && integralPart >= 10) {
          powersOf10++;
          integralPart /= 10;
        }
      }
    }

    writeInteger(integralPart);
    if (decimalPart) {
      writeRaw('.');
      writeDecimals(decimalPart, maxDecimalPlaces);
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

  template <typename UInt>
  void writeInteger(UInt value) {
    char buffer[22];
    char *ptr = buffer + sizeof(buffer) - 1;

    *ptr = 0;
    do {
      *--ptr = static_cast<char>(value % 10 + '0');
      value /= 10;
    } while (value);

    writeRaw(ptr);
  }

  void writeDecimals(uint32_t value, int8_t n) {
    // remove trailing zeros
    while (value % 10 == 0 && n > 0) {
      value /= 10;
      n--;
    }

    if (!n) {
      writeRaw('0');
      return;
    }

    char buffer[22];
    char *ptr = buffer + sizeof(buffer) - 1;

    *ptr = 0;
    while (n--) {
      *--ptr = static_cast<char>(value % 10 + '0');
      value /= 10;
    }

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
