#pragma once
// Touch controller (built into the AXS15231B), I2C address 0x3B.
// We only need "is the screen being touched", so coordinates are ignored.
#include <Wire.h>

#define TOUCH_SDA   4
#define TOUCH_SCL   8
#define TOUCH_ADDR  0x3B

inline void touchBegin() {
  Wire.begin(TOUCH_SDA, TOUCH_SCL, 400000);
}

inline bool touchPressed() {
  static const uint8_t cmd[8] = {0xB5, 0xAB, 0xA5, 0x5A, 0x00, 0x00, 0x00, 0x08};
  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(cmd, sizeof cmd);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom((uint8_t)TOUCH_ADDR, (uint8_t)8) != 8) return false;
  uint8_t buf[8];
  for (int i = 0; i < 8; i++) buf[i] = Wire.read();
  uint8_t points = buf[1];          // number of fingers
  return points > 0 && points <= 2;
}
