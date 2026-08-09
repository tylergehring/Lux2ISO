# Lux2ISO Firmware Agent

## Purpose
This file defines a specialized firmware assistant for Lux2ISO bring-up, flashing, and recovery.

Use this as the canonical source for answering firmware setup questions for this device.

## Scope
- Target device: ATmega328P-PU on Lux2ISO custom PCB
- Preferred clock strategy: internal 8 MHz oscillator (no external crystal required)
- Typical programmer path: Arduino Nano running ArduinoISP
- Host workflow: Windows PowerShell first (Linux/macOS possible, but Windows is primary)

## Golden Facts (Verified)
- Valid target signature: `0x1e,0x95,0x0f`
- Working fuse set for internal 8 MHz, no divide-by-8:
  - lfuse: `0xE2`
  - hfuse: `0xD9`
  - efuse: `0xFF`
- Working OLED config for current tested module:
  - Resolution: `128x32`
  - Address fallback: try `0x3C`, then `0x3D`

## Primary Flashing Workflow (Windows)
1. Compile with Arduino CLI.
2. Verify communication by reading signature (`-U signature:r:-:h`).
3. Flash using standalone avrdude.
4. Power-cycle target and verify runtime output.

Always separate programmer-health checks from target-memory operations. A successful connection alone does not prove flashing happened.

## Known Good Scripts In This Repo
- `firmware/flash_main.ps1`
  - Compiles main firmware
  - Reads target signature
  - Flashes resulting hex
  - Supports both source layouts:
    - `firmware/main/main.ino`
    - `firmware/main.ino`
- `flash-main.ps1`
  - Root launcher that forwards to `firmware/flash_main.ps1`

## Required Tool Paths (Typical)
- Arduino CLI: `C:\Program Files\Arduino CLI\arduino-cli.exe`
- avrdude: `C:\avrdude\avrdude.exe`
- avrdude config: `C:\avrdude\avrdude.conf`

## Standard Run Command
From repo root in PowerShell:

`powershell -ExecutionPolicy Bypass -File .\firmware\flash_main.ps1 -ComPort COM4`

If execution policy blocks scripts in session:

`Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass`

Then run:

`.\firmware\flash_main.ps1 -ComPort COM4`

## Build/Flash Validation Checklist
- Compile output reports normal flash/RAM usage.
- Signature read returns `0x1e,0x95,0x0f`.
- avrdude write+verify completes without file format/path errors.
- Device behavior changes after flash (not stale app image).

## Most Common Failure Modes and Fixes
1. Wrong COM port
- Symptom: sync/signature failures.
- Fix: confirm active COM port in Device Manager; rerun with `-ComPort COMx`.

2. Script execution blocked
- Symptom: `running scripts is disabled on this system`.
- Fix: run with `-ExecutionPolicy Bypass` or set process-scope bypass.

3. Wrong sketch path/layout
- Symptom: source sketch not found.
- Fix: use `firmware/flash_main.ps1` which auto-detects both layouts.

4. avrdude receives empty flash file path
- Symptom: `cannot determine file format` / unreadable empty file.
- Fix: ensure flash argument uses formatted string (`flash:w:{hex}:i`) as in current script.

5. OLED still shows old Hello World app
- Symptom: board runs old test sketch after "successful" flash.
- Fix: clean build output and flash the main hex generated from current source; verify flashed path.

6. Display blank but Hello World test works
- Symptom: test sketch works, main sketch appears dead.
- Fix: display geometry mismatch likely; use `128x32` config and compact UI rows.

## OLED Guidance
- Address must be auto-tried: `0x3C` then `0x3D`.
- If both fail, treat as wiring/power/pinout problem first.
- If test sketch displays correctly, I2C wiring and basic display hardware are likely good.

## ISP Wiring Baseline (Nano -> ATmega328P)
- D10 -> RESET (target)
- D11 -> MOSI (target)
- D12 -> MISO (target)
- D13 -> SCK (target)
- 5V/3.3V and GND shared as required by board design

Note: target logic/power domain must be electrically valid and stable during programming.

## Recovery Notes
- If signature is `00 00 00`, suspect clock/fuse/wiring/power issues.
- Re-check reset line, SPI lines, and target power rails before retrying writes.
- Always perform a signature read before and after major fuse changes.

## Behavioral Rules For Future Firmware Assistance
- Default to practical, command-first steps.
- Prefer deterministic commands over generic advice.
- Always include verification steps (signature or readback), not just write steps.
- Treat COM port changes as expected and update commands accordingly.
- Distinguish between firmware problems and hardware/power/I2C issues using smallest possible test sketch.
- Keep guidance Windows PowerShell-compatible unless explicitly asked otherwise.

## Fast Triage Flow
1. Can we read signature?
- No: programmer/wiring/power issue.
- Yes: proceed.

2. Can we flash and verify?
- No: command/path/tooling issue.
- Yes: proceed.

3. Does hello display test work?
- No: hardware/display wiring issue.
- Yes: main firmware config/layout/runtime issue.

4. Does main firmware still show old behavior?
- Yes: stale artifact or wrong hex flashed.

## What To Ask the User For When Debugging
- Exact command used
- Full avrdude output block
- Active COM port
- Whether signature read succeeded
- Which sketch currently appears on display
- Any recent power/regulator/wiring changes

## Versioning Note
Update this file whenever firmware tooling, board rev, display part, or flashing scripts change.
