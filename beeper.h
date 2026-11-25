#pragma once
// Speaker: NS4168 amplifier on I2S (needs esp32 core 3.x for ESP_I2S.h).
// Pins from the board's spec sheet. The speaker plugs into the 2-pin "SPEAK" connector.
#include <ESP_I2S.h>
#include <math.h>

#define SPK_BCLK  42
#define SPK_LRCK  2
#define SPK_DOUT  41

static I2SClass beeperI2s;
static bool beeperOk = false;
static const int BEEP_RATE = 16000;

inline void beeperBegin() {
  beeperI2s.setPins(SPK_BCLK, SPK_LRCK, SPK_DOUT);
  // Stereo with the same sound on both channels: the NS4168 plays the left or the
  // right channel depending on how the board wires it, so this works either way.
  beeperOk = beeperI2s.begin(I2S_MODE_STD, BEEP_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
  Serial.println(beeperOk ? "Speaker: I2S started (BCLK 42, LRCK 2, DOUT 41)"
                          : "Speaker: I2S FAILED to start");
}

// Plays a tone (blocks for its length). volume 0-100. Short fade in/out avoids clicks.
inline void playTone(int freq, int ms, int volume) {
  if (!beeperOk) return;
  int16_t buf[512];                 // 256 stereo frames
  const int total = BEEP_RATE * ms / 1000, fade = BEEP_RATE / 200;   // 5 ms fades
  const float amp = 30000.0f * constrain(volume, 0, 100) / 100.0f;
  const float step = 2.0f * PI * freq / BEEP_RATE;
  float phase = 0;
  for (int done = 0; done < total;) {
    int n = min(256, total - done);
    for (int i = 0; i < n; i++) {
      int k = done + i;
      float env = 1.0f;
      if (k < fade) env = (float)k / fade;
      else if (k > total - fade) env = (float)(total - k) / fade;
      int16_t v = (int16_t)(amp * env * sinf(phase));
      buf[2 * i] = v;               // left
      buf[2 * i + 1] = v;           // right
      phase += step;
      if (phase > 2.0f * PI) phase -= 2.0f * PI;
    }
    beeperI2s.write((uint8_t *)buf, n * 2 * sizeof(int16_t));
    done += n;
  }
}

inline void playSilence(int ms) {
  if (!beeperOk) return;
  int16_t buf[512] = {0};
  for (int left = BEEP_RATE * ms / 1000; left > 0; left -= 256)
    beeperI2s.write((uint8_t *)buf, min(256, left) * 2 * sizeof(int16_t));
}

// Three short high beeps (~1 second).
inline void playAlarm(int volume) {
  for (int i = 0; i < 3; i++) { playTone(2000, 180, volume); playSilence(120); }
  playSilence(100);
}

// Start-up sound: two rising notes, clearly audible.
inline void playChirp(int volume) {
  playTone(1200, 150, volume);
  playSilence(50);
  playTone(1800, 200, volume);
  playSilence(150);
}
