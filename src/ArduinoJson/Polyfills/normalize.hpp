// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
int normalize(T& value, T& error) {
  int powersOf10 = 0;

  if (value >= 1e256) {
    value /= 1e256;
    error *= 10;
    powersOf10 += 256;
  }
  if (value >= 1e128) {
    value /= 1e128;
    error *= 10;
    powersOf10 += 128;
  }
  if (value >= 1e64) {
    value /= 1e64;
    error *= 10;
    powersOf10 += 64;
  }
  if (value >= 1e32) {
    value /= 1e32;
    error *= 10;
    powersOf10 += 32;
  }
  if (value >= 1e16) {
    value /= 1e16;
    error *= 10;
    powersOf10 += 16;
  }
  if (value >= 1e8) {
    value /= 1e8;
    error *= 10;
    powersOf10 += 8;
  }
  if (value >= 1e4) {
    value /= 1e4;
    error *= 10;
    powersOf10 += 4;
  }
  if (value >= 1e2) {
    value /= 1e2;
    error *= 10;
    powersOf10 += 2;
  }
  if (value >= 1e1) {
    value /= 1e1;
    error *= 10;
    powersOf10 += 1;
  }

  if (value <= 1e-256) {
    value *= 1e256;
    error *= 10;
    powersOf10 -= 256;
  }
  if (value <= 1e-128) {
    value *= 1e128;
    error *= 10;
    powersOf10 -= 128;
  }
  if (value <= 1e-64) {
    value *= 1e64;
    error *= 10;
    powersOf10 -= 64;
  }
  if (value <= 1e-32) {
    value *= 1e32;
    error *= 10;
    powersOf10 -= 32;
  }
  if (value <= 1e-16) {
    value *= 1e16;
    error *= 10;
    powersOf10 -= 16;
  }
  if (value <= 1e-8) {
    value *= 1e8;
    error *= 10;
    powersOf10 -= 8;
  }
  if (value <= 1e-4) {
    value *= 1e4;
    error *= 10;
    powersOf10 -= 4;
  }
  if (value <= 1e-2) {
    value *= 1e2;
    error *= 10;
    powersOf10 -= 2;
  }
  if (value <= 1e-1) {
    value *= 1e1;
    error *= 10;
    powersOf10 -= 1;
  }

  return powersOf10;
}
}
}
