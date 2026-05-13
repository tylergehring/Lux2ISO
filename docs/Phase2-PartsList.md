# Lux2ISO Phase 2 — Parts List

Phase 2 upgrades: USB-C Li-Po charging, rotary encoders (replace buttons), black OLED display, incident light diffuser dome, and custom enclosure.

---

## What Carries Over from Phase 1

These parts are **unchanged** — no need to rebuy if you already have them.

| Ref | Part | Description |
|-----|------|-------------|
| U1 | ATmega328P-PU | Main microcontroller |
| Y1 | HC-49-U-S 8MHz | Crystal oscillator |
| C1, C2 | 27pF C0G | Crystal load caps |
| C3, C4 | 0.1µF | ATmega VCC/AVCC decoupling |
| C6 | 0.1µF | AREF decoupling |
| R1, R2 | 4.7kΩ | I2C SDA/SCL pull-ups |
| R3 | 10kΩ | RESET pull-up |
| C8, C9 | 1µF electrolytic | MCP1700 VIN/VOUT bypass caps |
| J1 | TSW-103-07-G-D | 6-pin ICSP programming header |
| J5 | Conn_01x05 | BH1750 module header |
| — | BH1750 module | Light sensor (I2C) |

---

## Phase 2 New & Replacement Parts

### Rotary Encoders (replace SW1 / SW2)

| Qty | Ref | Part | Description | Source | ~Price |
|-----|-----|------|-------------|--------|--------|
| 2 | ENC1, ENC2 | Bourns PEC11R-4215F-S0024 | Rotary encoder, 24 detents, push-button, through-hole | [DigiKey PEC11R-4215F-S0024-ND](https://www.digikey.com/en/products/detail/bourns-inc/PEC11R-4215F-S0024/4499656) | ~$3.00 ea |

**Notes:**
- Each encoder uses 5 pins: A, B, SW (button), +3.3V, GND
- A and B are the quadrature outputs — rotating CW/CCW produces a phase-shifted pulse pair
- SW is a normally-open push button (shorts to GND when shaft is pressed)
- Use ATmega internal pull-ups for A, B, and SW lines
- Optional hardware debounce: 100nF cap on each of A and B to GND (strongly recommended — encoders are noisy)
- Left encoder = aperture, right encoder = shutter speed (matching the product image layout)

**Decoupling caps (new, for encoder debounce):**

| Qty | Ref | Part | Description | Source | ~Price |
|-----|-----|------|-------------|--------|--------|
| 4 | C_ENC1A, C_ENC1B, C_ENC2A, C_ENC2B | 100nF ceramic | Encoder signal debounce caps (A and B per encoder) | DigiKey / generic | ~$0.10 ea |

---

### USB-C Li-Po Charging System (replaces J2 battery connector + primary battery)

The power system changes from a raw battery + LDO to: **USB-C → charger IC → Li-Po → LDO (MCP1700, stays) → 3.3V**.

#### For Breadboarding (buy these first):

| Qty | Part | Description | Source | ~Price |
|-----|------|-------------|--------|--------|
| 1 | TP4056 USB-C Module (with protection) | Pre-built charger board — USB-C input, JST output, onboard protection circuit and status LEDs. Zero external components needed for breadboard. | [Amazon](https://www.amazon.com/s?k=TP4056+USB-C+lipo+charger+module) | ~$1.50 |
| 1 | 3.7V 500mAh Li-Po battery (JST-PH 2.0mm) | Rechargeable lithium polymer cell, fits a compact enclosure | [Adafruit 1578](https://www.adafruit.com/product/1578) or Amazon | ~$8.00 |

#### For the Final PCB (discrete components, integrates into custom board):

| Qty | Ref | Part | Description | Source | ~Price |
|-----|-----|------|-------------|--------|--------|
| 1 | U_CHG | MCP73831T-2ACI/OT | Li-Po charger IC, SOT-23-5, 500mA max | [DigiKey MCP73831T-2ACI/OTCT-ND](https://www.digikey.com/en/products/detail/microchip-technology/MCP73831T-2ACI-OT/964301) | ~$0.65 |
| 1 | J_USB | USB4085-GF-A | USB-C receptacle, SMD, mid-mount | [DigiKey 2073-USB4085-GF-ACT-ND](https://www.digikey.com/en/products/detail/gct/USB4085-GF-A/9859616) | ~$1.10 |
| 2 | R_CC1, R_CC2 | 5.1kΩ resistor | USB-C CC pull-downs — required for USB-C host to deliver 5V | DigiKey / generic | ~$0.10 ea |
| 1 | R_PROG | 3.9kΩ resistor | Sets MCP73831 charge current to ~330mA (safe for 500mAh cell) | DigiKey / generic | ~$0.10 |
| 1 | C_BAT | 4.7µF ceramic | MCP73831 VBAT bypass cap | DigiKey / generic | ~$0.15 |
| 1 | LED_CHG | Red LED (0805 or through-hole) | Charging status indicator (STAT pin) | DigiKey / generic | ~$0.10 |
| 1 | R_LED | 1kΩ | Current-limiting resistor for charge LED | DigiKey / generic | ~$0.10 |
| 1 | BT1 | JST-PH 2.0mm 2-pin connector | Li-Po battery connector (board side) | [DigiKey](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S2B-PH-K-S-LF-SN/926626) | ~$0.35 |

**MCP73831 charge current formula:** `I_CHG = 1000V / R_PROG`. At 3.9kΩ → ~256mA. For a 500mAh cell this is a ~0.5C charge rate, ideal.

**Why MCP73831 over TP4056?** Same Microchip ecosystem as the MCP1700 LDO already in the design, well-documented, DigiKey-stocked, and has a simple STAT output pin for a charge LED. Requires a separate protection circuit (DW01A/FS8205) if the Li-Po cell doesn't already have built-in protection. Most off-the-shelf Li-Po cells from Adafruit include a protection circuit.

---

### OLED Display (upgrade)

| Qty | Ref | Part | Description | Source | ~Price |
|-----|-----|------|-------------|--------|--------|
| 1 | J3/OLED | 0.96" SSD1306 128×64 OLED — **black PCB, white pixels** | Same I2C protocol and pinout, same library. Specify "white on black" when ordering to match the product image aesthetic. | [Amazon](https://www.amazon.com/s?k=SSD1306+0.96+OLED+128x64+white+black+PCB) | ~$6–9 |

**Note:** The electrical interface is identical to phase 1. Only the visual appearance changes (black PCB background vs. blue PCB). Firmware requires no changes for the display itself.

---

### Incident Light Diffuser Dome (new — mechanical)

This is the white translucent sphere visible in the product image, placed over the BH1750 sensor to collect light from a hemispherical field of view (incident metering).

| Option | Description | Source | ~Price |
|--------|-------------|--------|--------|
| **A (Quick)** | White ping pong ball — cut in half, file a small flat on the edge, glue or press-fit over BH1750 aperture | Sporting goods / Amazon | ~$2 for pack |
| **B (Clean)** | 3D print a 15–20mm hemisphere in white PETG or natural PLA. Design to press-fit over the BH1750 module. | Self-print or send to JLCPCB 3D | ~$1–3 |
| **C (Premium)** | Sekonic replacement diffuser dome — professional incident dome, very clean look | [Sekonic L-558 replacement part](https://www.bhphotovideo.com/c/search?q=sekonic+diffuser+dome) | ~$15–25 |

---

### Power Switch (keep or upgrade)

| Qty | Ref | Part | Description | Source | ~Price |
|-----|-----|------|-------------|--------|--------|
| 1 | S1 | EG1218 (existing) | SPDT slide switch — reuse from phase 1 | Existing stock | — |

The EG1218 remains between the Li-Po (+) and MCP1700 VIN. This cleanly disconnects the battery when the device is off. When USB-C is plugged in and the MCP73831 is charging, the battery charges regardless of switch position (the charger sits before the switch in the power path).

---

### Enclosure (new — mechanical)

| Option | Description |
|--------|-------------|
| **Breadboard phase** | No enclosure needed — bare breadboard for testing |
| **PCB phase** | Design a custom 3D-printed enclosure in KiCad/Fusion 360 to match the product image aesthetic: pill-shaped, black, cutouts for two encoder shafts, OLED window, BH1750 dome aperture, USB-C slot, and power switch |

---

## Phase 2 Complete Bill of Materials (Summary)

### Breadboard Prototype Shopping List

| # | Item | Qty | Source | ~Price |
|---|------|-----|--------|--------|
| 1 | Bourns PEC11R-4215F-S0024 rotary encoder | 2 | DigiKey | $6.00 |
| 2 | TP4056 USB-C Li-Po charger module (with protection) | 1 | Amazon | $1.50 |
| 3 | 3.7V 500mAh Li-Po battery (JST-PH) | 1 | Adafruit | $8.00 |
| 4 | SSD1306 128×64 OLED (black PCB, white pixels) | 1 | Amazon | $7.00 |
| 5 | 100nF ceramic caps (for encoder debounce) | 4+ | DigiKey / kit | $0.50 |
| 6 | White ping pong balls (incident dome prototype) | 1 pack | Amazon | $2.00 |
| 7 | 10µF electrolytic cap (bring-up) | 2 | DigiKey / kit | $0.20 |
| 8 | USB-to-TTL serial adapter (CH340 or FT232) | 1 | Amazon | $8.00 |
| 9 | DIP-28 socket (optional, for ATmega) | 1 | DigiKey | $0.50 |
| | **Estimated Breadboard Total** | | | **~$34** |

### PCB Integration Shopping List (additional, for final board)

| # | Item | Qty | Source | ~Price |
|---|------|-----|--------|--------|
| 1 | MCP73831T-2ACI/OT (charger IC) | 1 | DigiKey | $0.65 |
| 2 | USB4085-GF-A (USB-C connector) | 1 | DigiKey | $1.10 |
| 3 | 5.1kΩ resistors (CC pull-downs) | 2 | DigiKey | $0.20 |
| 4 | 3.9kΩ resistor (PROG / charge current) | 1 | DigiKey | $0.10 |
| 5 | 4.7µF ceramic cap (VBAT bypass) | 1 | DigiKey | $0.15 |
| 6 | JST-PH 2.0mm 2-pin connector | 1 | DigiKey | $0.35 |
| 7 | Red LED + 1kΩ resistor (charge indicator) | 1 ea | DigiKey | $0.20 |
| | **Estimated PCB Additional Total** | | | **~$3** |

---

## Board Bring-Up & Programming

Parts needed to flash and debug the ATmega328P on the bench before the PCB exists.

| Qty | Part | Purpose | Source | ~Price |
|-----|------|---------|--------|--------|
| 2 | **10µF electrolytic cap** (6.3V or higher) | One goes between RESET and GND on the Arduino-as-ISP programmer to prevent it from auto-resetting during flashing. Keep a spare — they're also useful as bulk supply decoupling on the breadboard power rail. | DigiKey / cap kit | ~$0.10 ea |
| 1 | **USB-to-TTL serial adapter** (CH340G or FT232RL, 3.3V capable) | Connects to ATmega UART (TX/RX) for `Serial.print()` debug output. Set adapter to **3.3V logic** — the ATmega runs at 3.3V and 5V logic will damage it. Also used to load a bootloader via UART if you ever want to skip the ICSP header. | [Amazon CH340](https://www.amazon.com/s?k=ch340+usb+ttl+3.3v) | ~$8 |
| 1 | **Arduino Uno or Nano** (if you don't already own one) | Used as the ISP programmer. Wire ICSP pins to J1 on the board, upload the `ArduinoISP` sketch, then flash fuses and firmware. | — | ~$10–25 |
| 1 | **DIP-28 IC socket** | Lets you seat/remove the ATmega without soldering it directly. Mandatory on breadboard, strongly recommended on the final PCB for rework. | [DigiKey](https://www.digikey.com/en/products/detail/on-shore-technology-inc/ED28DT/4147600) | ~$0.50 |

### Flashing Procedure Reminder

```
Arduino-as-ISP wiring:
  Arduino pin 10  → ATmega RESET (via 10µF cap: + to Arduino RESET, – to GND)
  Arduino pin 11  → ATmega MOSI (ICSP pin 4)
  Arduino pin 12  → ATmega MISO (ICSP pin 1)
  Arduino pin 13  → ATmega SCK  (ICSP pin 3)
  Arduino 3.3V    → ATmega VCC
  Arduino GND     → ATmega GND
```

**Important:** The 10µF cap goes on the **programmer Arduino's** RESET-to-GND, not on the target board. Its job is to prevent the Uno from resetting when avrdude opens the serial port, which would kick it out of ISP mode before the upload completes.

```bash
# Step 1 — Set fuses for external 8MHz crystal (do this once per new ATmega)
avrdude -p m328p -c stk500v1 -P /dev/ttyUSB0 -b 19200 \
  -U lfuse:w:0xE7:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m

# Step 2 — Flash firmware
avrdude -p m328p -c stk500v1 -P /dev/ttyUSB0 -b 19200 \
  -U flash:w:main.ino.hex:i
```

---

## GPIO Pin Assignment (Phase 2)

| ATmega328P Pin | Port | Phase 1 Use | Phase 2 Use |
|----------------|------|-------------|-------------|
| Pin 4 | PD2 / INT0 | (unused) | ENC1 — A signal (hardware interrupt) |
| Pin 5 | PD3 / INT1 | (unused) | ENC1 — B signal (hardware interrupt) |
| Pin 6 | PD4 | (unused) | ENC1 — push button (SW) |
| Pin 11 | PD5 | (unused) | ENC2 — A signal |
| Pin 12 | PD6 | BTN_SHUTTER | ENC2 — B signal |
| Pin 13 | PD7 | BTN_APERTURE | ENC2 — push button (SW) |
| Pin 23–24 | PC0, PC1 | (unused) | Available for future use |

**Interrupt note:** ENC1 uses INT0/INT1 (hardware interrupts) for reliable quadrature decoding at any rotation speed. ENC2 can use PCINT (pin-change interrupts) or polling. For the breadboard phase, polling every 1ms in a timer ISR is sufficient.
