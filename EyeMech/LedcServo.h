#ifndef LEDC_SERVO_H
#define LEDC_SERVO_H

#include <Arduino.h>

// Direct-drive servo driver for classic ESP32 and ESP32-C3.
// One LEDC hardware PWM channel per servo, 50 Hz, 12-bit resolution.
// Drop-in replacement for the PCA9685 driver used in the ESP32-S3 build:
// same method names, same angle -> pulse mapping (150..600 counts, i.e.
// ~0.73..2.93 ms), so saved calibration values carry over unchanged.
//
// Wiring: one GPIO per servo (see SERVO_PIN[] in EyeMech.ino). Servos still
// need their own 5-6 V supply with a common ground to the ESP32 — the
// board's 3.3 V / 5 V pins cannot drive 6 servos.

#define LEDC_SERVO_FREQ_HZ  50
#define LEDC_SERVO_BITS     12
#define LEDC_SERVO_MIN      150   // counts for 0 degrees
#define LEDC_SERVO_MAX      600   // counts for 180 degrees
#define LEDC_SERVO_COUNT    6

class LedcServo {
public:
    LedcServo();
    void begin(const uint8_t pins[LEDC_SERVO_COUNT]);  // attach pins, 50 Hz, 12-bit
    void setPWMFreq(float freq_hz);                    // re-applies freq to all channels
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);  // 'on' ignored: LEDC has no phase offset
    void setServoAngle(uint8_t channel, int angle);
    bool isConnected();    // always true: no I2C device to ping
    void setDebug(bool enable);
private:
    float _freq = LEDC_SERVO_FREQ_HZ;
    bool _debug = false;
};

#endif
