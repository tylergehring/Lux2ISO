# Lux2ISO — Programming & Debug Log

## Project Overview

Custom PCB with an ATmega328P-PU (DIP-28) soldered directly to the board. The chip runs at 8MHz using an external HC-49/US crystal (Y1). Power is supplied by a 3.3V MCP1700 LDO regulator. Programming is done via a 6-pin ICSP header (J1) using an Arduino Nano flashed with the ArduinoISP sketch as the programmer.

The machine running everything is a Windows PC running WSL2 (Ubuntu). Arduino IDE 2.x is installed on Windows.

---

## Issue 1: Arduino Nano not visible in WSL

**Symptom:** `ls /dev/ttyACM* /dev/ttyUSB*` returned nothing. `lsusb` only showed root hubs.

**Cause:** USB devices are not automatically passed through to WSL2. The Arduino stays on the Windows side.

**Fix:** Used `usbipd-win` to forward the USB device into WSL.
```powershell
# In Windows Admin PowerShell:
usbipd bind --busid 2-1
usbipd attach --wsl --busid 2-1
```
The Arduino Nano uses a CH340 USB-serial chip and appeared as `/dev/ttyUSB0` in WSL.

---

## Issue 2: Arduino IDE's bundled avrdude.exe is incompatible with this Windows machine

**Symptom:**
```
Failed uploading: cannot execute upload tool: fork/exec .../avrdude.exe: %1 is not a valid Win32 application.
```
Also: "This app can't run on your PC" when trying to run the bundled avrdude manually.

**Cause:** Arduino IDE 2.x ships with an avrdude binary compiled for a different CPU architecture (x64 vs ARM64, or vice versa) than the user's machine.

**Fix:** Downloaded a compatible avrdude binary manually from the avrdude GitHub releases:
```powershell
Invoke-WebRequest -Uri "https://github.com/avrdudes/avrdude/releases/download/v8.0/avrdude-v8.0-windows-x64.zip" -OutFile "$env:TEMP\avrdude.zip"
Expand-Archive "$env:TEMP\avrdude.zip" -DestinationPath "C:\avrdude"
```
All subsequent avrdude commands use `C:\avrdude\avrdude.exe` with `C:\avrdude\avrdude.conf`.

To manually flash the ArduinoISP sketch (since Arduino IDE's upload also fails):
```cmd
C:\avrdude\avrdude.exe -C C:\avrdude\avrdude.conf -p m328p -c arduino -P COM3 -b 115200 -D -U flash:w:"C:\Users\tyler\AppData\Local\arduino\sketches\6194F9AB1F71DC1BB882A2CE2925E2E3\ArduinoISP.ino.hex":i
```
Note: the hex file path comes from Arduino IDE verbose output after compilation.

---

## Issue 3: Fuse step succeeded, but ATmega328P now returns device signature `00 00 00`

**Symptom:**
```
Device signature = 00 00 00 (retrying)
Device signature = 00 00 00 (retrying)
Device signature = 00 00 00
Error: invalid device signature
Error: expected signature for ATmega328P is 1E 95 0F
```

**Background:** The fuse step was run successfully earlier:
```cmd
C:\avrdude\avrdude.exe -C C:\avrdude\avrdude.conf -p m328p -c avrisp -P COM3 -b 19200 -U lfuse:w:0xE7:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
```
These fuses configure the ATmega to use the **external full-swing crystal oscillator**. After this, the chip requires Y1 to be oscillating before it can respond to anything — including ISP programming.

**Root cause (suspected):** The external crystal (Y1, HC-49/US 8MHz) is not oscillating. Testing with a multimeter in AC voltage mode across the crystal pins showed **no voltage**, confirming no oscillation. All ISP signnectial conons (MOSI/MISO/SCK/RST/VCC/GND) tested with continuity and passed. AVCC (pin 20) confirmed receiving 3.3V.

**Recovery attempt:** Tried forcing a fuse reset back to internal oscillator using `-F`:
```cmd
C:\avrdude\avrdude.exe -C C:\avrdude\avrdude.conf -p m328p -c avrisp -P COM3 -b 19200 -F -U lfuse:w:0x62:m
```
Result: `0x62` write failed — chip is completely unresponsive over ISP.

**Current status:** The chip is alive but locked — it has no clock source so it cannot respond to ISP. Standard ISP recovery is not possible without a working clock.

---

## Current State

| Item | Status |
|---|---|
| Fuses | Set to external crystal (0xE7 / 0xD9 / 0xFF) |
| Crystal Y1 | Not oscillating — suspected dead (heat damage during soldering) |
| ATmega328P | Alive but locked out — requires working crystal to respond |
| ISP recovery | Not possible over standard ISP without a clock |
| Firmware | Compiled and ready, upload not yet possible |

---

## Next Steps / Recovery Options

### Option A: Replace Y1 (recommended first step)
Replace the HC-49/US 8MHz crystal. HC-49 crystals are sensitive to heat and can be killed if the iron is held too long. After replacement, attempt:
```cmd
C:\avrdude\avrdude.exe -C C:\avrdude\avrdude.conf -p m328p -c avrisp -P COM3 -b 19200 -F -U lfuse:w:0x62:m
```
If the chip responds, reset fuses to external crystal and flash firmware.

### Option B: High-Voltage Parallel Programming (HVPP)
HVPP can reset fuses without needing a clock at all. Requires a special HV programmer (12V on RESET pin + parallel data bus). More complex but guaranteed to recover a locked ATmega328P if the silicon is undamaged.

---

## Programmer Setup Reference

| Arduino Nano Pin | PCB J1 Pin |
|---|---|
| D10 | RST |
| D11 | MOSI |
| D12 | MISO |
| D13 | SCK |
| 3V3 | VCC |
| GND | GND |

- 1µF cap between RESET and GND on the Nano (to prevent auto-reset)
- Arduino Nano flashed with ArduinoISP sketch before connecting to PCB
- Port: COM3 (Windows) / `/dev/ttyUSB0` (WSL)
- Programmer type: `avrisp`, baud `19200`

---

## Firmware Notes

- Board setting in Arduino IDE: **Arduino Pro or Pro Mini (ATmega328P 3.3V 8MHz)**
- Programmer setting: **Arduino as ISP**
- Upload method: **Sketch → Upload Using Programmer** (not regular upload)
- Firmware compiled successfully: 20250 bytes (65%), 1045 bytes RAM (51%)
- Firmware location: `firmware/main/main.ino`
