# Film ISO Meter

A simple DIY light meter for film photographers.

This project uses a light sensor and microcontroller to measure ambient light and recommend an appropriate ISO based on selected aperture and shutter speed. The goal is to create a small, standalone tool that helps film photographers quickly determine which film speed to use in a given lighting condition.

---

## Features

- Measures ambient light using a digital light sensor
- Calculates exposure value (EV)
- Recommends an ISO for a chosen aperture and shutter speed
- Designed to run on inexpensive hardware
- Open source and easy to modify

---

## Hardware

The minimal prototype uses the following components:

- Microcontroller (Arduino Nano / ESP32 / Raspberry Pi Pico)
- BH1750 ambient light sensor
- OLED display (SSD1306)
- Breadboard and jumper wires
- Optional battery power

---

## How It Works

1. The light sensor measures the scene brightness in **lux**.
2. The firmware converts lux into an **exposure value (EV)**.
3. Using the selected **aperture** and **shutter speed**, the program calculates the recommended **ISO**.

This helps photographers decide which film (ISO 100, 200, 400, etc.) is appropriate for the current lighting conditions.

---

## Project Goals

- Create a low-cost alternative to commercial light meters
- Build a simple tool specifically for film photographers
- Provide an open platform for experimentation with exposure calculations

---

## Future Improvements

- Rotary encoder for selecting shutter speed and aperture
- Improved exposure algorithms
- Pocket-sized enclosure
- Battery-powered portable design
- Additional exposure modes

