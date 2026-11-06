# Wiring — EyeMech ε3.2, ESP32-C3 direct drive

No PCA9685, no I2C. Six servo signal wires go straight to ESP32-C3 GPIOs,
driven by the chip's LEDC hardware PWM (50 Hz, 12-bit). The C3 has exactly
6 LEDC channels — all six are used here, none spare.

## Signal pins

| CH | Function           | ESP32-C3 GPIO | LEDC ch |
|----|--------------------|---------------|---------|
| 0  | Left/Right pan     | 3             | 0       |
| 1  | Up/Down tilt       | 4             | 1       |
| 2  | Top-Left eyelid    | 5             | 2       |
| 3  | Bottom-Left eyelid | 6             | 3       |
| 4  | Top-Right eyelid   | 7             | 4       |
| 5  | Bottom-Right eyelid| 10            | 5       |

Change the pin map in one place only: `SERVO_PIN[]` at the top of
`EyeMech/EyeMech.ino`. LEDC channel numbers follow the table order
automatically.

## Pins you must NOT use for servos (ESP32-C3)

- **2 / 8 / 9** — strapping pins (boot mode). GPIO 8 is also the onboard
  RGB LED on the DevKitM-1. A servo pulling one of these low at boot can
  stop the board starting, or start it in download mode.
- **12–17** — SPI flash. Occupied on all standard C3 modules.
- **18 / 19** — USB D− / D+. Used for flashing and the serial console on
  most C3 boards.
- **20 / 21** — UART0 TX/RX, the serial console.

Spares if you need to move a servo: **0, 1, 11**. Note that small C3
boards (Xiao ESP32C3, LOLIN C3 Mini) break out fewer pins — check that
GPIO 3/4/5/6/7/10 are all available on your board before wiring.

## Power — read this first

```
Servo 5-6 V supply (+) ----+----> all six servo red wires
                           |
ESP32-C3 GND --------------+----> all six servo brown/black wires
                           |
                           +----> servo supply GND (common ground)

ESP32-C3 GPIO 3/4/5/6/7/10 --> servo signal wires (one each)
```

- **Servos must have their own 5–6 V supply.** A 5 V / 4–5 A supply is
  recommended: six small servos can pull ~0.7 A each at stall, ~4 A peak
  together. A 5 V / 2 A phone charger will brown out and the C3 will
  reboot mid-blink.
- **Common ground is mandatory.** Servo supply GND, ESP32-C3 GND and all
  servo GND wires must be one net, or the signal has no reference and
  the servos will jitter or ignore commands.
- **Do not power the servos from the board's 5 V pin or 3.3 V rail.**
  Split power on a small piece of perfboard, a servo breakout board, or
  a 6-way Y-harness. Keep the servo power wires short and twisted with
  ground where possible.
- The C3 itself is powered separately as usual (USB or its own 5 V in).

## Signal voltage

ESP32-C3 GPIO is 3.3 V logic. Most hobby servos (SG90, MG90S, MG996R)
accept a 3.3 V signal without complaint. If a servo ignores commands or
jitters while the others work, that servo wants a 5 V signal — add a
simple level shifter (e.g. 74HCT245, or a single NPN + pull-up per
channel) on that signal line only.

## First power-on checklist

1. Signal wires to the GPIOs above, servo power disconnected.
2. Flash the sketch, join the `EyeMech-Controller` AP, open
   `http://192.168.4.1` — the dashboard should load and the digital twin
   should respond.
3. Connect the common ground first, then servo power. The eyes should
   wake in Sleep mode.
4. Run **Calibrate** (14-step wizard) once — or reuse calibration saved
   from the classic-ESP32 or PCA9685 build, the pulse mapping is identical.
