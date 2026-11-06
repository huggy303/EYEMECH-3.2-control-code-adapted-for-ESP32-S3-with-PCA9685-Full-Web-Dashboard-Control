#include "LedcServo.h"

LedcServo::LedcServo() {}

void LedcServo::begin(const uint8_t pins[LEDC_SERVO_COUNT]) {
    for (uint8_t ch = 0; ch < LEDC_SERVO_COUNT; ch++) {
        ledcSetup(ch, (uint32_t)_freq, LEDC_SERVO_BITS);
        ledcAttachPin(pins[ch], ch);
        ledcWrite(ch, 0);
    }
}

void LedcServo::setPWMFreq(float freq_hz) {
    _freq = freq_hz;
    for (uint8_t ch = 0; ch < LEDC_SERVO_COUNT; ch++) {
        ledcSetup(ch, (uint32_t)_freq, LEDC_SERVO_BITS);
    }
}

void LedcServo::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    (void)on;  // LEDC has no phase-offset register; only the OFF count sets the pulse.
    if (channel < LEDC_SERVO_COUNT) ledcWrite(channel, off);
}

void LedcServo::setServoAngle(uint8_t channel, int angle) {
    if (channel >= LEDC_SERVO_COUNT) return;
    // Same mapping as the PCA9685 build: 0-180 deg -> 150-600 counts.
    int pulse = LEDC_SERVO_MIN + (angle * (LEDC_SERVO_MAX - LEDC_SERVO_MIN) / 180);
    ledcWrite(channel, pulse);

    if (_debug) {
        Serial.printf("[PWM] CH%-2u  angle=%3d  pulse=%3d  LEDC\n",
                      channel, angle, pulse);
    }
}

bool LedcServo::isConnected() {
    return true;  // no I2C device to ping; the driver is always "connected"
}

void LedcServo::setDebug(bool enable) {
    _debug = enable;
}
