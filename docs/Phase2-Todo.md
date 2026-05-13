# Lux2ISO Phase 2 — Development Checklist

Track progress from current phase 1 hardware through breadboard prototype, firmware update, PCB redesign, and final assembly.

---

## Stage 1 — Planning & Design

- [ ] **Review phase 2 parts list** (`docs/Phase2-PartsList.md`) and confirm all parts before ordering
- [ ] **Finalize encoder UI design** — decide which encoder controls aperture vs. shutter speed, and decide what encoder button presses do (e.g., ENC press = hold to take reading, or cycle mode)
- [ ] **Sketch updated schematic blocks** (on paper or KiCad) for:
  - USB-C connector with CC1/CC2 5.1kΩ pull-downs
  - MCP73831 charger IC with PROG resistor and charge LED
  - JST Li-Po battery connector
  - 2× rotary encoders with 100nF debounce caps on A/B lines
  - Power path: USB-C → MCP73831 → LiPo → S1 switch → MCP1700 LDO → 3.3V rail
- [ ] **Decide encoder sizes** — product image shows two different-sized knobs; confirm if both are the same EC11-footprint encoder body or if one needs a different mechanical form factor
- [ ] **Plan incident dome fabrication method** (ping pong ball / 3D print / commercial)

---

## Stage 2 — Order Parts

- [ ] **Order rotary encoders** — Bourns PEC11R-4215F-S0024 × 2 (DigiKey)
- [ ] **Order TP4056 USB-C charger modules** × 2 (Amazon, for breadboard phase)
- [ ] **Order Li-Po battery** — 3.7V 500mAh with JST-PH connector (Adafruit 1578)
- [ ] **Order black OLED display** — SSD1306 128×64 white-on-black (Amazon)
- [ ] **Order 100nF ceramic caps** × 4 (encoder debounce) — likely already have in a cap kit
- [ ] **Order PCB discrete parts** (MCP73831, USB-C connector, CC resistors, PROG resistor, VBAT cap, JST connector, charge LED) — see parts list for DigiKey part numbers
- [ ] **Get incident dome material** (ping pong balls or filament for 3D print)

---

## Stage 3 — Breadboard Prototype

### Power System
- [ ] Wire the TP4056 USB-C module output to the breadboard power rail (VBAT+)
- [ ] Wire VBAT+ through the EG1218 power switch to MCP1700 VIN
- [ ] Verify MCP1700 outputs 3.3V under load with the Li-Po battery connected
- [ ] Test charging: plug in USB-C, confirm charge LED lights on TP4056 module
- [ ] Measure voltage at MCP1700 VOUT while charging — confirm 3.3V stable
- [ ] Test power switch: confirm system powers off cleanly while charging continues

### Rotary Encoders
- [ ] Wire ENC1 (A→PD2, B→PD3, SW→PD4) with 100nF caps on A and B to GND
- [ ] Wire ENC2 (A→PD5, B→PD6, SW→PD7) with 100nF caps on A and B to GND
- [ ] Enable ATmega internal pull-ups on all encoder pins in firmware
- [ ] Write and test quadrature decoding for ENC1 (use interrupt-driven or polling)
- [ ] Write and test quadrature decoding for ENC2
- [ ] Confirm CW rotation increments index, CCW decrements
- [ ] Confirm index wraps correctly at boundaries (e.g., past f/22 wraps to f/1)
- [ ] Test encoder push-button debounce on SW pins

### Display
- [ ] Wire new black OLED module — same I2C pinout as phase 1
- [ ] Confirm `display.begin()` initializes on 0x3C address
- [ ] Verify display layout renders correctly with new hardware

### BH1750 & Full System Integration
- [ ] Wire BH1750 module to I2C (SDA/SCL) and confirm reads are live
- [ ] Run full system on breadboard: spin encoders → aperture/shutter changes → ISO recalculates → display updates
- [ ] Do a full power cycle test from battery (no USB)

---

## Stage 4 — Firmware Updates

- [ ] **Remove button code** — delete `BTN_SHUTTER`, `BTN_APERTURE` defines and `buttonPressed()` function
- [ ] **Add encoder library** — install [Encoder by Paul Stoffregen](https://github.com/PaulStoffregen/Encoder) via Arduino Library Manager (or implement manual quadrature ISR)
- [ ] **Rewire encoder pin defines:**
  ```cpp
  #define ENC_SHUTTER_A  2   // PD2 / INT0
  #define ENC_SHUTTER_B  3   // PD3 / INT1
  #define ENC_SHUTTER_SW 4   // PD4
  #define ENC_APERTURE_A 5   // PD5
  #define ENC_APERTURE_B 6   // PD6
  #define ENC_APERTURE_SW 7  // PD7
  ```
- [ ] **Implement encoder delta reading** — on each loop, read delta from each encoder and increment/decrement the corresponding index. Wrap at table boundaries.
- [ ] **Add encoder button handling** — decide behavior (e.g., press shutter encoder = hold measurement, press aperture encoder = toggle EV mode)
- [ ] **Update display layout** — match the product image layout: ISO on top, F-stop and shutter speed on a single bold row. Consider removing the raw lux readout line or making it a secondary screen.
- [ ] **Test encoder acceleration** (optional) — if rotation is fast, jump multiple stops per tick for faster traversal of tables
- [ ] **Verify no timing issues** — encoder ISR + BH1750 I2C reads + OLED updates must not block each other. Run BH1750 reads at 500ms intervals, decouple from encoder ISR.
- [ ] **Commit updated firmware** to repository

---

## Stage 5 — KiCad Schematic Redesign

- [ ] **Open existing KiCad project**
- [ ] **Delete SW1, SW2** (tactile buttons) from schematic
- [ ] **Delete J2** (2-pin battery connector — will be replaced by JST Li-Po connector)
- [ ] **Add USB-C connector** (USB4085-GF-A or equivalent) with VBUS, GND, CC1, CC2 pins
- [ ] **Add 5.1kΩ CC pull-down resistors** on CC1 and CC2 to GND
- [ ] **Add MCP73831T** charger IC:
  - VDD → USB-C VBUS
  - VBAT → Li-Po+ (and to switch/MCP1700)
  - PROG → 3.9kΩ → GND
  - STAT → 1kΩ → LED → GND
  - GND → GND
- [ ] **Add JST-PH 2.0mm battery connector** for Li-Po cell
- [ ] **Add 4.7µF VBAT bypass cap** on MCP73831 VBAT pin
- [ ] **Add ENC1 symbol** (3 signal pins + power pins) connected to PD2/PD3/PD4
- [ ] **Add ENC2 symbol** connected to PD5/PD6/PD7
- [ ] **Add 100nF debounce caps** on ENC1-A/B and ENC2-A/B to GND
- [ ] **Run ERC** (Electrical Rules Check) — resolve all errors
- [ ] **Update schematic BOM** and net labels
- [ ] **Export schematic PDF** to `docs/`

---

## Stage 6 — PCB Layout

- [ ] **Update PCB from new schematic** (push from schematic to PCB in KiCad)
- [ ] **Place USB-C connector** — on board edge, accessible from the side of the enclosure
- [ ] **Place MCP73831** — near USB-C connector, short traces to VBUS and VBAT
- [ ] **Place Li-Po JST connector** — accessible to battery wiring
- [ ] **Place rotary encoders** — match physical positions to enclosure design (left=aperture, right=shutter)
- [ ] **Place 100nF encoder debounce caps** — directly at encoder A/B pins
- [ ] **Run DRC** (Design Rules Check) — resolve all errors
- [ ] **Review copper pours** — refresh GND pour on B.Cu
- [ ] **Double-check power path trace widths** — 1mm for power (VBUS, VBAT, 3V3), 0.5mm for signals
- [ ] **Generate Gerber files**
- [ ] **Order PCB** (JLCPCB, PCBWay, OSHPark, etc.)

---

## Stage 7 — PCB Assembly & Test

- [ ] **Solder SMD components first** (MCP73831, USB-C connector, CC resistors, decoupling caps)
- [ ] **Solder through-hole components** (ATmega, crystal, caps, encoder headers, ICSP header, JST connector, switch)
- [ ] **Visual inspection** — check for solder bridges especially on USB-C and MCP73831
- [ ] **Power test before flashing** — measure 3.3V rail with no MCU activity
- [ ] **Flash fuses** (if using new ATmega):
  ```bash
  avrdude -p m328p -c usbtiny -U lfuse:w:0xE7:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
  ```
- [ ] **Flash phase 2 firmware** via ICSP header
- [ ] **Functional test** — encoders, display, BH1750, USB-C charging all operational
- [ ] **Charge cycle test** — charge from empty, monitor temperature, confirm charge LED goes out at full

---

## Stage 8 — Enclosure

- [ ] **Design enclosure** in Fusion 360 or FreeCAD:
  - Pill / rounded-rectangular body (match product image)
  - Cutouts: 2× encoder shafts (with nut recesses), OLED window, BH1750 dome aperture, USB-C slot, slide switch access
  - Wall thickness: 2–3mm
  - PCB mounting bosses (2.5mm screws or snap-fit)
- [ ] **3D print prototype** in black PLA or PETG
- [ ] **Test fit** — check encoder knob clearance, OLED alignment, USB-C port access
- [ ] **Install incident dome** in BH1750 aperture (glue or press-fit)
- [ ] **Final assembly** — PCB into enclosure, attach encoder knobs, close case

---

## Stage 9 — Final Validation

- [ ] Measure a known light source (e.g., direct sunlight ≈ 100,000 lux) and verify ISO output is reasonable
- [ ] Compare readings against a known reference meter or smartphone light meter app
- [ ] Field test with a film camera — take a frame at the recommended exposure and evaluate results
- [ ] Update README.md with phase 2 schematic, photos, and updated hardware table
- [ ] Archive phase 1 schematic in `docs/`

---

## Key Decisions to Make Before Starting

| Decision | Options |
|----------|---------|
| Encoder button behavior | Hold to measure / mode cycle / confirm selection |
| Incident dome fabrication | Ping pong ball / 3D print / commercial |
| PCB manufacturer | JLCPCB (cheapest) / OSHPark (domestic, US) / PCBWay |
| Enclosure manufacturing | FDM 3D print / SLA print for cleaner finish / machined aluminum |
| Battery capacity | 500mAh (small) / 1000mAh (more runtime, bigger footprint) |
