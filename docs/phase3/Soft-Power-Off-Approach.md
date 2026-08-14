# Lux2ISO Phase 3 - Soft Power Off Using Encoder Push Button

## Goal

Replace the external latching power switch with an encoder-button-driven soft power system:

- Short press: switch UI mode (shutter or aperture)
- Long press (for example 5 seconds): request clean shutdown
- Hardware latch then removes system power completely

This gives one-button UX while still allowing true power off.

---

## Why this is needed

Firmware sleep alone cannot fully remove power from the board.
A hardware latch is required so firmware can tell hardware to cut the rail.

Without a latch:
- You can sleep the MCU
- But battery is still connected to the system rail

With a latch:
- Button pulse turns system on
- MCU keeps system on through a HOLD signal
- MCU can release HOLD to turn fully off

---

## High-Level Power Flow

1. Battery and charger module provide source power (SYS_IN and OUT-).
2. A high-side load switch stage gates SYS_IN to VSYS.
3. Encoder push button provides the initial ON pulse.
4. MCU asserts HOLD GPIO after boot to keep power enabled.
5. On long press shutdown, firmware clears HOLD.
6. Load switch turns off and removes power from regulator and logic rail.

---

## New Hardware Required

## Core power-latch parts

1. High-side P-channel MOSFET for load switching
- Example class: low-Rds(on) logic-level PMOS rated for your battery voltage and current
- Source to SYS_IN, drain to VSYS

2. NPN transistor or small N-MOSFET to pull PMOS gate low
- Used by latch logic and MCU HOLD path

3. Gate pull-up resistor on PMOS gate
- Pulls PMOS gate to source by default (power off)

4. HOLD resistor network
- Allows MCU GPIO to sustain ON state after boot

5. Optional RC on gate/control node
- Debounce and clean startup pulse shaping

## Recommended protection and usability parts

6. Series resistor from MCU HOLD pin to latch control node
- Protects MCU pin and reduces transient stress

7. Optional Schmitt trigger buffer for clean button signal (if noisy)
- Useful if long cable run to encoder

8. Optional TVS/ESD handling at external button line if exposed

---

## Signals and Net Naming

Use explicit names to avoid confusion:

- BAT_POS, BAT_NEG: battery connector only
- SYS_IN: charger OUT+
- SYS_GND: charger OUT-
- VSYS: switched system rail after latch
- PWR_HOLD: MCU output that keeps latch on
- ENC_SW: encoder push signal to MCU input

Important:
- Keep BAT_NEG and SYS_GND distinct in schematic intent unless your exact charger module internally shorts them.

---

## Functional Behavior Specification

## Power-on sequence

1. User presses encoder button.
2. Latch hardware briefly enables VSYS.
3. MCU boots.
4. Early in startup firmware sets PWR_HOLD active.
5. System remains on after button release.

## Runtime button behavior

1. Short press (for example <300 ms): toggle shutter/aperture mode.
2. Long press (>=5 s): initiate shutdown.

## Shutdown sequence

1. Show shutdown state on display.
2. Save current mode/settings if needed.
3. Put BH1750 in power-down.
4. Send OLED display-off command.
5. Clear PWR_HOLD.
6. Latch drops VSYS to 0 V.

---

## Firmware Requirements

1. Reserve one GPIO for PWR_HOLD output.
2. Configure ENC_SW input with pull-up.
3. Implement robust button timing state machine:
- debounce
- short press detection
- long press threshold
4. Assert PWR_HOLD as early as possible in setup.
5. During shutdown, release PWR_HOLD only after peripherals are quiesced.

---

## Integration Notes For Current Lux2ISO Design

1. Remove current external latching power switch from main power path.
2. Keep encoder push as both UI and power-control input.
3. Feed regulator VIN from VSYS (switched rail), not directly from SYS_IN.
4. Keep regulator input/output bypass caps close to regulator pins.
5. Keep ICSP accessible; system must be powered during programming.

---

## Validation Checklist

1. Power-on from button press works reliably (10 out of 10 attempts).
2. MCU always asserts PWR_HOLD before startup pulse expires.
3. Short press changes UI mode only.
4. Long press always performs clean shutdown.
5. VSYS drops to near 0 V after shutdown.
6. No ghost current paths through peripherals after off.
7. Device restarts correctly after full off.

---

## Risks and Mitigations

1. Risk: latch chatter or false trigger
- Mitigation: RC shaping and clean debounce strategy

2. Risk: MCU fails to assert HOLD quickly enough at boot
- Mitigation: keep startup pulse long enough and set HOLD early in setup

3. Risk: button noise causes accidental shutdown
- Mitigation: long threshold >=5 s with stable-hold validation

4. Risk: inability to recover after firmware fault
- Mitigation: include hidden battery disconnect jumper or service pads

---

## Suggested Prototype Plan

1. Build latch on breadboard with existing Lux2ISO board power input.
2. Validate ON, HOLD, and OFF behavior with bench supply first.
3. Measure off-state current at battery.
4. Integrate latch block into schematic after proven behavior.
5. Re-run full bring-up and flashing workflow on integrated board.
