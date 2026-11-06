# EYEMECH ε3.2 control code for ESP32-C3 — direct-drive servos, Full Web Dashboard Control
Based on Will Cogley's Eye Mechanism

<div align="center">
  <a href="https://youtube.com/shorts/8luMGPw0Tpo">
    <img src="https://img.youtube.com/vi/8luMGPw0Tpo/hqdefault.jpg" alt="EYEMECH ε3.2 demo — click to watch" width="360">
  </a>
  <br>
  <em>▶ Click to watch the demo</em>
</div>

## Overview

EYEMECH ε3.2 turns Will Cogley's animatronic eye mechanism into a self-contained wireless prop. An **ESP32-C3** drives six servos directly from its LEDC hardware PWM pins and hosts its own **offline Wi-Fi dashboard** — no app to install, no internet, no extra driver board, and no extra wiring beyond power and the six servo signal wires.

Open the dashboard on your phone and you get a live on-screen twin of the eyes, autonomous lifelike behaviour, a choreography editor, and a guided calibration wizard. Everything is controlled from the web page — there are no joysticks, switches, or potentiometers to wire up.


## Features

- **Self-hosted dashboard** — the device is its own Wi-Fi hotspot and web server. Connect and control it from any phone, tablet, or laptop, fully offline.
- **Live digital twin** — two on-screen SVG eyes mirror the real servos in real time, with a telemetry strip (mode, mood, per-eye aperture, blink, pan/tilt).
- **Personality Engine** — an autonomous "Alive" mode with moods (Calm, Alert, Curious, Sleepy, Skittish), natural blinks, spontaneous glances, and ambient gestures.
- **Choreography editor** — build and play your own keyframe sequences, or trigger ten built-in animations.
- **Guided calibration wizard** — a 14-step flow walks you through setting safe limits for every servo, saved permanently on the device.
- **Phone-friendly** — the layout reflows cleanly for small touch screens.

## What You Need

- ESP32-C3 dev board (e.g. ESP32-C3-DevKitM-1, or any C3 module with GPIO 3/4/5/6/7/10 broken out)
- 6 servos — 1 pan (left/right), 1 tilt (up/down), and 4 eyelids
- A 5–6V power supply for the servos (5 V / 4–5 A recommended — see WIRING.md)
- A small servo power distribution: perfboard, servo breakout, or Y-harness to split 5 V and GND to the six servos
- Will Cogley's 3D-printed [animatronic eye mechanism](https://makerworld.com/es/models/1184807-animatronic-eye-mechanism-e3-2)

> Earlier builds used a physical joystick, switches, and a trim pot. Those are **gone** — the web dashboard is now the only control surface.

## Wiring

Each servo signal wire goes straight to one ESP32 GPIO. The servos share one 5–6 V supply with a common ground to the ESP32 (see WIRING.md for the power distribution).

```
ESP32 GPIO      Servo
----------      ---------------------
GPIO 3          Left/Right pan
GPIO 4          Up/Down tilt
GPIO 5          Top-Left eyelid
GPIO 6          Bottom-Left eyelid
GPIO 7          Top-Right eyelid
GPIO 10         Bottom-Right eyelid

ESP32 GND  ---> servo supply GND (common ground, mandatory)
```

Pin map is a single table in the sketch (`SERVO_PIN[]` in `EyeMech.ino`) — change the numbers there only. The C3 has only GPIO 0–21 and exactly 6 LEDC channels, all used here. Pins avoid strapping pins (2/8/9), SPI flash pins (12–17), USB D−/D+ (18/19) and UART0 (20/21); spares are 0, 1, 11.

Give the servos their own 5–6 V supply with a common ground to the ESP32 — don't try to power them from the board's 3.3 V rail or its 5 V pin. The old PCA9685 board used to provide the servo power rail; without it, the power distribution is a small piece of perfboard or a servo breakout (details in WIRING.md).

Porting note: the angle-to-pulse mapping is identical to the classic-ESP32 direct-drive build and the original ESP32-S3 + PCA9685 build (0–180° → 150–600 counts at 50 Hz, 12-bit), so calibration values saved on either build carry over unchanged.

## Getting Started

### 1. Flash the firmware

The firmware is an Arduino sketch in the `EyeMech/` folder. The easiest path is [`arduino-cli`](https://arduino.github.io/arduino-cli/) with the ESP32 core installed:

```
arduino-cli compile --upload -p <YOUR_PORT> --fqbn esp32:esp32:esp32c3 ./EyeMech
```

One external library is required: **WebSockets by Links2004** (install it via the Library Manager). You can also open `EyeMech/EyeMech.ino` in the Arduino IDE and upload from there.

### 2. Connect to the eyes

Once flashed, the ESP32 creates its own Wi-Fi network:

1. **Find the network:** look for the Wi-Fi SSID `EyeMech-Controller`.
2. **Join it:** the password is `eyemech123`.
3. **Open the dashboard:** go to `http://192.168.4.1` (or `http://eyemech.local`) in any browser.

### 3. Calibrate (first time only)

Tap **Calibrate** and follow the 14-step wizard once to teach each servo its safe range. Your settings are saved on the device and survive power cycles.

## Using the Dashboard

Switch modes from the buttons under the digital twin.

**Sleep** — the resting state. The eyes drift slowly closed and "breathe." Any tap wakes them. The eyes also drop back to sleep on their own after 5 minutes of no activity.


**Alive** — the star of the show. The Personality Engine takes over: the eyes glance around, blink naturally, and shift mood on their own. Tap a **mood** (Calm, Alert, Curious, Sleepy, Skittish) to steer the feel, or fire a one-shot **expression** (wink, squint, wide, skeptical).


**Manual** — you take the wheel. Drag the virtual joystick to aim the gaze, and use the **Blink** button on demand. The **Record** button captures your moves into a choreography.


**Choreography** — build a sequence of keyframes, then play, loop, or stop it. Ten built-in animations are ready to go, and your own creations save in the browser.


**Calibrate** — the guided wizard for setting each servo's centre, travel, and safe limits. The eyes hold still while you work, and you can save, revert, or reset.


## How It Works

The firmware runs a simple state machine — Sleep, Alive, Manual, Calibration, and Perform — and serves a single web page straight from the chip's flash. The dashboard talks to the device over a small HTTP API and a WebSocket that streams the live eye state, which is what keeps the on-screen twin in sync with the real servos. Because the ESP32 is its own access point and web server, the whole thing works with no internet connection at all.


## Acknowledgments

The physical eye mechanism is **Will Cogley's** design — full credit for the hardware goes to his fantastic work:

- [Instructables Guide](https://www.instructables.com/Animatronic-Eye-Mechanism/)
- [Patreon](https://www.patreon.com/c/Will_Cogley/posts)
- [Documentation](https://willcogley.notion.site/EyeMech-3-2-1af24779b64d80b19edfdd795d4b90e5)
- [3D Models](https://makerworld.com/es/models/1184807-animatronic-eye-mechanism-e3-2)

This project is an independent ESP32 + web-dashboard control layer for that hardware.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request
