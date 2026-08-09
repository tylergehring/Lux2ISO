# Lux2ISO Phase 2 - Windows Firmware Setup

This guide walks through getting the Phase 2 firmware onto your PCB using a Windows laptop, PowerShell, an Arduino Nano as the ISP programmer, and the ATmega328P-PU running from its internal 8 MHz oscillator.

This setup does not use the external crystal, even if it is already soldered onto the board.

## What this guide assumes

- Windows laptop
- PowerShell
- Arduino Nano available to use as an ISP programmer
- ATmega328P-PU soldered into the Phase 2 PCB
- The Lux2ISO repo is present on your machine
- You want the ATmega328P to run on its internal 8 MHz clock

## Confirmed Arduino CLI path

Use this exact executable path in all commands below:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe"
```

You already confirmed it works with:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" version
```

## Overview

You will do the following:

1. Install the Arduino AVR core and required libraries
2. Install `avrdude`
3. Flash `ArduinoISP` onto the Nano
4. Wire the Nano to the Lux2ISO PCB ICSP header
5. Verify the ATmega328P responds over ISP
6. Set fuses for internal 8 MHz operation
7. Compile the Lux2ISO firmware for 3.3V 8 MHz
8. Flash the compiled firmware to the ATmega328P

## Step 1 - Install the Arduino AVR core

Run:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" config init
& "C:\Program Files\Arduino CLI\arduino-cli.exe" core update-index
& "C:\Program Files\Arduino CLI\arduino-cli.exe" core install arduino:avr
```

## Step 2 - Install the required libraries

The firmware in [firmware/main/main.ino](/home/tylegehr/repos/Lux2ISO/firmware/main/main.ino) depends on these libraries:

- `BH1750`
- `Adafruit SSD1306`
- `Adafruit GFX Library`

Install them with:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" lib update-index
& "C:\Program Files\Arduino CLI\arduino-cli.exe" lib install "BH1750"
& "C:\Program Files\Arduino CLI\arduino-cli.exe" lib install "Adafruit SSD1306"
& "C:\Program Files\Arduino CLI\arduino-cli.exe" lib install "Adafruit GFX Library"
```

## Step 3 - Install avrdude

Do not rely on the Arduino IDE bundled copy. Install a standalone version.

For a 64-bit Windows machine:

```powershell
Invoke-WebRequest -Uri "https://github.com/avrdudes/avrdude/releases/download/v8.0/avrdude-v8.0-windows-x64.zip" -OutFile "$env:TEMP\avrdude.zip"
Expand-Archive "$env:TEMP\avrdude.zip" -DestinationPath "C:\avrdude"
```

Verify it runs:

```powershell
& "C:\avrdude\avrdude.exe" -?
```

If your laptop is ARM64 instead of x64, use the ARM64 `avrdude` release instead.

## Step 4 - Plug in the Arduino Nano and find the COM port

Run:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" board list
```

Note the Nano port, such as `COM3` or `COM4`.

All commands below use `COM3` as an example. Replace that with your real port.

## Step 5 - Flash ArduinoISP onto the Nano

On some Windows installs, the Arduino AVR core is present but the local `ArduinoISP` example files are not. The most reliable PowerShell-only approach is to download the official sketch directly from the Arduino examples repository.

Create a local sketch folder and download `ArduinoISP.ino` into it:

```powershell
$ArduinoISP = "$env:USERPROFILE\Documents\ArduinoISP"
New-Item -ItemType Directory -Force -Path $ArduinoISP | Out-Null
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/arduino/arduino-examples/main/examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino" -OutFile "$ArduinoISP\ArduinoISP.ino"
Get-ChildItem $ArduinoISP
```

That should leave you with this file:

```text
C:\Users\tyler\Documents\ArduinoISP\ArduinoISP.ino
```

Compile the ArduinoISP example:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" compile --fqbn arduino:avr:nano:cpu=atmega328 --export-binaries "$ArduinoISP"
```

That should create a file named:

```text
C:\Users\tyler\Documents\ArduinoISP\build\arduino.avr.nano\ArduinoISP.ino.hex
```

Upload it to the Nano with the standalone `avrdude` you already installed:

```powershell
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c arduino `
  -P COM3 `
  -b 115200 `
  -D `
  -U flash:w:"$ArduinoISP\build\arduino.avr.nano\ArduinoISP.ino.hex":i
```

If you have an older Nano bootloader and the upload fails, retry using `atmega328old` and `57600` baud:

```powershell
& "C:\Program Files\Arduino CLI\arduino-cli.exe" compile --fqbn arduino:avr:nano:cpu=atmega328old --export-binaries "$ArduinoISP"
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c arduino `
  -P COM3 `
  -b 57600 `
  -D `
  -U flash:w:"$ArduinoISP\build\arduino.avr.nano.cpu.atmega328old\ArduinoISP.ino.hex":i
```

After the Nano is flashed, place a 10 uF electrolytic capacitor between the Nano `RESET` and `GND` pins.

- Capacitor positive lead -> `RESET`
- Capacitor negative lead -> `GND`

This prevents the Nano from auto-resetting when `avrdude` opens the serial port.

## Step 6 - Wire the Nano to the Lux2ISO PCB ICSP header

Make these connections:

| Nano pin | PCB ICSP signal |
|---|---|
| D10 | RESET |
| D11 | MOSI |
| D12 | MISO |
| D13 | SCK |
| 3V3 | VCC |
| GND | GND |

Important notes:

- Use `3V3`, not `5V`
- Leave the Li-Po battery disconnected during first programming
- The external crystal can remain soldered in place and will simply be ignored

## Step 7 - Verify the ATmega328P responds

Run:

```powershell
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c avrisp `
  -P COM3 `
  -b 19200 `
  -U signature:r:-:h
```

Expected result:

```text
1e 95 0f
```

If you run the command without `-U signature:r:-:h`, `avrdude` can exit successfully after talking only to the Nano programmer, which is not enough to prove the target ATmega328P is responding.

If you get `00 00 00` instead, do not continue. Re-check:

- RESET wiring from Nano D10
- MOSI, MISO, and SCK wiring
- VCC and GND
- ATmega orientation
- That the Nano really has `ArduinoISP` flashed

## Step 8 - Set fuses for internal 8 MHz clock

This is the key step for not using the external crystal.

Run:

```powershell
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c avrisp `
  -P COM3 `
  -b 19200 `
  -U lfuse:w:0xE2:m `
  -U hfuse:w:0xD9:m `
  -U efuse:w:0xFF:m
```

Fuse meanings:

- `lfuse = 0xE2` -> internal 8 MHz RC oscillator, no divide-by-8
- `hfuse = 0xD9` -> ISP enabled, reset pin left normal
- `efuse = 0xFF` -> brown-out disabled

After this, the chip runs from the internal 8 MHz oscillator and does not require the crystal.

## Step 9 - Compile the Lux2ISO firmware for 8 MHz

Open PowerShell in your Lux2ISO repo folder, then run:

```powershell
Set-Location "C:\path\to\Lux2ISO"
$SketchDir = ".\firmware\Lux2ISO"
$BuildDir = ".\firmware\build"
New-Item -ItemType Directory -Force -Path $SketchDir | Out-Null
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
Copy-Item .\firmware\main.ino "$SketchDir\Lux2ISO.ino" -Force
& "C:\Program Files\Arduino CLI\arduino-cli.exe" compile --fqbn arduino:avr:pro:cpu=8MHzatmega328 --output-dir $BuildDir $SketchDir
```

Replace `C:\path\to\Lux2ISO` with your actual repo path.

This extra copy step is required because Arduino sketches must have a main `.ino` file whose name matches the containing folder. Your Windows repo currently stores the firmware as `firmware\main.ino`, which `arduino-cli` will not compile directly.

The board target matters here. It must be:

- `arduino:avr:pro:cpu=8MHzatmega328`

That matches a 3.3V, 8 MHz ATmega328P build, which keeps timing and serial correct.

## Step 10 - Find the compiled hex file

After compiling, list the generated `.hex` files:

```powershell
Get-ChildItem .\firmware\build -Filter *.hex
```

You should see a file similar to:

```text
C:\path\to\Lux2ISO\firmware\build\Lux2ISO.ino.hex
```

## Step 11 - Flash the firmware to the ATmega328P

Run:

```powershell
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c avrisp `
  -P COM3 `
  -b 19200 `
  -U flash:w:"C:\path\to\Lux2ISO\firmware\build\Lux2ISO.ino.hex":i
```

Replace `C:\path\to\Lux2ISO` with your actual repo path.

## Step 12 - Power up and test

If programming succeeded, the board should:

- Show `Lux2ISO ready!` on the OLED
- Read light from the BH1750
- Let Encoder 1 change shutter speed
- Let Encoder 2 change aperture
- Let each encoder push switch reset that value to default

The firmware source is in [firmware/main/main.ino](/home/tylegehr/repos/Lux2ISO/firmware/main/main.ino).

## Current Phase 2 control mapping

This is how the current firmware is set up:

- Encoder 1: shutter speed
  - A = PD2
  - B = PD3
  - SW = PD4
- Encoder 2: aperture
  - A = PD5
  - B = PD6
  - SW = PD7

Encoder push actions:

- Encoder 1 press -> reset shutter speed to `1/125`
- Encoder 2 press -> reset aperture to `f/8`

## Troubleshooting

### `device signature = 000000`

Usually one of these:

- Wrong wiring on RESET, MOSI, MISO, or SCK
- Missing 10 uF cap on Nano RESET
- Wrong COM port
- Nano is not actually running `ArduinoISP`

### `programmer is not responding`

Usually one of these:

- Wrong COM port
- Bad USB cable
- Nano reset during the session instead of staying in ArduinoISP

### `cannot execute upload tool ... avrdude.exe: %1 is not a valid Win32 application`

This means the `arduino-cli upload` step tried to use an incompatible bundled `avrdude.exe`.

Use the standalone `C:\avrdude\avrdude.exe` commands from Step 5 instead of `arduino-cli upload`.

### OLED stays blank after successful flashing

Check:

- I2C wiring
- 3.3V rail
- OLED address if your module is not `0x3C`

### BH1750 reads invalid values

Check:

- SDA and SCL wiring
- Power and ground
- Pull-ups on the I2C bus

### Encoders move the wrong direction

Swap the A and B wires on that encoder.

## Re-flashing firmware later

Once the fuses are set, you normally do not need to touch them again. To update only the firmware later, re-run compile and then flash only the `.hex` file:

```powershell
$SketchDir = ".\firmware\Lux2ISO"
$BuildDir = ".\firmware\build"
New-Item -ItemType Directory -Force -Path $SketchDir | Out-Null
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
Copy-Item .\firmware\main.ino "$SketchDir\Lux2ISO.ino" -Force
& "C:\Program Files\Arduino CLI\arduino-cli.exe" compile --fqbn arduino:avr:pro:cpu=8MHzatmega328 --output-dir $BuildDir $SketchDir
& "C:\avrdude\avrdude.exe" `
  -C "C:\avrdude\avrdude.conf" `
  -p m328p `
  -c avrisp `
  -P COM3 `
  -b 19200 `
  -U flash:w:"C:\path\to\Lux2ISO\firmware\build\Lux2ISO.ino.hex":i
```

## Recommended first run order

1. Flash `ArduinoISP` to the Nano
2. Verify the ATmega328P signature
3. Set internal 8 MHz fuses
4. Compile the Lux2ISO firmware for `arduino:avr:pro:cpu=8MHzatmega328`
5. Flash the firmware
6. Test OLED, BH1750, and both encoders