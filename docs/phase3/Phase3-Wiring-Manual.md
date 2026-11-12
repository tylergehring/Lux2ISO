# Lux2ISO Phase 3 - From-Empty-Schematic Wiring Manual

Use this manual with the [current Phase 3 BOM](Phase3-BOM). Reference designators and connector assignments below are NEW: do not copy them from the old schematic image.

## What this build does

- One EG1218 slide switch turns the meter on and off.
- One encoder adjusts shutter or aperture; its push button selects which value is edited.
- An ATmega328P runs at 3.3 V using its internal 8 MHz clock.
- A BH1750 module and a 128x32 OLED share I2C.
- An Adafruit 4410 USB-C charger charges an Adafruit 1578 protected 500 mAh battery at 100 mA.
- The meter MUST be OFF while charging. The charger is not a load-sharing/power-path device.

**Important change from previous instructions:** this build does NOT use a TP4056 module. The battery's protection is inside the purchased battery pack. The pack plugs into the charger's existing JST-PH socket. The charger and main board share GND. There are no separate main-board B-/OUT- or BAT_NEG nets in this version.

This is a design and assembly manual, not a claim that a PCB has passed ERC, electrical testing, or charging qualification. Do not delete any files automatically: save a backup of your old KiCad project before starting.

## 1. Prepare the empty project

1. Create a new KiCad project in a new folder.
2. Open its empty schematic and use one sheet for the main PCB.
3. Place the power block on the left, U1 in the middle, and module connectors on the right.
4. Use real regulator, MCU, resistor, capacitor, switch, and encoder symbols. Only use connector symbols for actual connectors.
5. Enter the exact manufacturer part number from the BOM in each component's fields.
6. Keep the reference designators below while following the instructions. If you reannotate later, regenerate the assembly documentation too.
7. Use wires to connect pin endpoints. Place net labels ON the wires; nearby text does not make a connection.
8. Crossings connect only when KiCad shows the intended junction. Use explicit junctions for branches.

### Main-board symbols to place

| References | Quantity | Component |
|---|---:|---|
| U1 | 1 | ATmega328P-PU, DIP-28 |
| U2 | 1 | MCP1700-3302E/TO, TO-92 |
| S1 | 1 | EG1218 SPDT slide switch |
| SW1 | 1 | PEC11R-4215F-S0024 encoder with push switch |
| R1, R4, R5, R6, R7, R8 | 6 | 10 kohm resistors |
| R2, R3 | 2 | 4.7 kohm resistors |
| R9 | 1 | 1 kohm resistor |
| C1, C2 | 2 | 2.2 uF ceramic capacitors |
| C3, C4, C5, C8 | 4 | 100 nF ceramic capacitors |
| C6, C7 | 2 | 10 nF ceramic capacitors |
| J1 | 1 | 2x3 ICSP header |
| J2 | 1 | 2-pin JST-XH header, charger connection |
| J3 | 1 | 4-pin JST-XH header, OLED connection |
| J4 | 1 | 4-pin JST-XH header, BH1750 connection |

The off-board assemblies are A1 (charger), BT1 (protected battery), A2 (OLED), and A3 (sensor). Document them in a wiring note or separate assembly drawing; do not assign their internal surface-mount parts footprints on your main PCB.

## 2. Define the nets

| Net | Meaning |
|---|---|
| SYS_IN | A1 VBAT output, before the slide switch; NOT USB 5 V |
| VSYS | Switched battery rail feeding U2 VIN |
| +3.3V | U2 regulated output |
| GND | Common return: charger GND and all main-board/module grounds |
| RESET | U1 reset and ICSP reset |
| MOSI, MISO, SCK | ICSP signals |
| SDA, SCL | Shared I2C signals |
| ENC_A_RAW, ENC_B_RAW | Encoder contacts before filtering |
| ENC1_A, ENC1_B | Filtered encoder signals at the MCU |
| ENC_SW_RAW | Push-switch contact before filtering |
| ENC1_SW | Filtered push-switch signal at the MCU |

Identical local labels on this one sheet join electrically. Do not introduce VSYS+, 3V3, SYS_GND, or other spellings for the same nets.

## 3. Wire the charger and battery assembly

### 3.1 Configure A1 before connecting a battery

1. Identify A1 as Adafruit **4410**, the USB-C Micro-Lipo charger.
2. Leave its 500 mA solder jumper OPEN. Its default charge current is 100 mA.
3. Do not add external charge-programming resistors.
4. Do not add another USB-C receptacle, PD trigger, or CC resistors to your main PCB.
5. Leave A1's breakout pads marked 5V, D-, and D+ UNCONNECTED.
6. Use only its VBAT and GND breakout pads for the main-board power harness.

The manufacturer's drawing labels the five breakout positions as 1=5V, 2=D-, 3=D+, 4=GND, 5=VBAT. Identify the actual board pads by their markings and continuity, not by guessing left/right from a photograph.

### 3.2 Connect the battery

1. Use only the BOM's Adafruit **1578**, a protected 3.7 V nominal / 4.2 V full-charge, 500 mAh LiPo pack.
2. Inspect the pack and cable. Do not use a swollen, damaged, crushed, or punctured pack.
3. Check connector polarity with a voltmeter.
4. Plug the pack's existing JST-PH plug directly into A1's battery socket.
5. Do not remove the pack's protection PCB, unwrap it, splice into raw cell tabs, or solder directly to the cell.

The pack's external negative lead is already on its protected output side. Connecting A1 GND to main-board GND is correct for this architecture; it is not the earlier TP4056 protection-bypass mistake.

### 3.3 Wire J2 on the main board

| Main-board pin | Schematic net | Harness connection |
|---|---|---|
| J2.1 | SYS_IN | A1 VBAT |
| J2.2 | GND | A1 GND |

```text
Protected battery BT1
        |
        +-- factory JST-PH plug --> A1 battery socket

Ordinary USB-C cable ------------> A1 USB-C socket

A1 VBAT ---- two-wire harness ---- J2.1 SYS_IN
A1 GND  ------------------------- J2.2 GND
```

J2 is a charger-to-main-board connector, NOT a second battery socket. Do not connect the battery independently to both A1 and the main board.

### 3.4 USB-C compatibility

A1's documented schematic includes independent 5.1 kohm pulldowns on CC1 and CC2. This advertises a USB-C sink and permits a compliant USB-C source to provide its normal 5 V output.

Supported charging arrangements:

- USB-C wall charger and USB-C-to-USB-C cable.
- USB-C computer port and USB-C-to-USB-C cable.
- USB-A 5 V port and USB-A-to-USB-C cable.

A normal USB-PD wall charger is acceptable: this board does not negotiate a higher voltage and uses default 5 V. Do not use a trigger cable or adapter that forces 9 V, 12 V, or another higher voltage.

This is charging only, not USB firmware programming. Budget more than five hours for an empty 500 mAh pack at 100 mA because charging tapers near full; do not promise a fixed charge time.

## 4. Wire S1, the mechanical power switch

The EG1218 is SPDT ON-ON. Use one throw as ON and leave the other throw open to obtain OFF.

| Actual EG1218 terminal | Connection |
|---|---|
| 2, common | SYS_IN |
| 1, selected ON throw | VSYS |
| 3, unused throw | Unconnected; add no-connect marker |

1. Wire J2.1/SYS_IN to S1 terminal 2.
2. Wire S1 terminal 1 to a new VSYS wire.
3. Put a no-connect marker directly on terminal 3.
4. Do NOT connect terminal 3 to GND.
5. Check the chosen KiCad symbol: its COMMON must map to physical pad 2.

```text
J2.1 SYS_IN ---- S1 common 2
                            \---- throw 1 ---- VSYS ---- U2 VIN
                             ---- throw 3 ---- no connection
```

Confirm which actuator direction connects 2 to 1 using a continuity meter before labelling the enclosure ON/OFF.

The switch is rated 200 mA at 30 VDC. It carries operating current, not charging current. Keep normal and switching loads within the manufacturer's ratings. It is not a current limiter or short-circuit protector.

## 5. Wire U2 and its capacitors

Use the TO-92 MCP1700-3302E/TO pin mapping below. Other package variants have different mappings.

| U2 physical pin | Function | Net |
|---|---|---|
| 1 | GND | GND |
| 2 | VIN | VSYS |
| 3 | VOUT | +3.3V |

1. Connect U2.1 to GND.
2. Connect U2.2 to S1.1/VSYS.
3. Connect U2.3 to the +3.3V rail.
4. Connect C1, 2.2 uF, between VSYS and GND.
5. Connect C2, 2.2 uF, between +3.3V and GND.

```text
VSYS ----+---- U2 VIN
         |
       C1 2.2uF
         |
        GND

U2 VOUT -+---- +3.3V
         |
       C2 2.2uF
         |
        GND

U2 GND ------- GND
```

Both capacitors are nonpolarized ceramics. They are branches to ground, never series components. Place C1 beside VIN and C2 beside VOUT on the PCB.

The MCP1700 datasheet specifies at least 1 uF output capacitance and output-capacitor ESR of 0 to 2 ohms. The selected 2.2 uF X7R parts provide capacitance margin.

For the wider lead spacing, use `Package_TO_SOT_THT:TO-92_Inline_Wide` as a starting footprint. Its pads are spaced 2.54 mm apart. Check pad numbers, actual package orientation, drill size, and formed-lead fit before ordering.

## 6. Wire U1 power, AREF, and reset

All numbers in this section are physical DIP-28 pins, NOT Arduino digital-pin numbers.

| U1 physical pin | Name | Connection |
|---|---|---|
| 7 | VCC | +3.3V |
| 20 | AVCC | +3.3V |
| 8 | GND | GND |
| 22 | GND | GND |
| 21 | AREF | C5 to GND only |
| 1 | PC6/RESET | RESET net |

1. Connect both VCC and AVCC to +3.3V.
2. Connect both ground pins to GND.
3. Connect C3, 100 nF, from U1.7 to GND.
4. Connect C4, 100 nF, from U1.20 to GND.
5. Connect C5, 100 nF, from U1.21/AREF to GND.
6. Do not wire AREF directly to +3.3V.
7. Connect R1, 10 kohm, from RESET to +3.3V.
8. Label the U1.1 wire RESET.

Place C3/C4 immediately beside the respective power pins, with short ground returns. Do not substitute one distant capacitor for both.

No crystal is installed. Pins 9/PB6 and 10/PB7 are unused. Internal-8-MHz fuse configuration is mandatory; do not use the historical README's external-crystal configuration.

## 7. Wire SW1 encoder rotation

The selected encoder has three rotation contacts A/B/C and two separate momentary-switch contacts. Its mechanical mounting tabs are not substitutes for terminal C.

Use a rotary-encoder-with-switch symbol, but verify its pin-to-pad mapping against the actual Bourns part. Do not assume your previous custom six-pin block represented the five electrical contacts correctly.

### 7.1 Common

Connect encoder terminal C to GND.

### 7.2 Channel A

1. Label encoder terminal A's wire ENC_A_RAW.
2. Connect R4, 10 kohm, between +3.3V and ENC_A_RAW.
3. Connect R5, 10 kohm, between ENC_A_RAW and a new net ENC1_A.
4. Connect C6, 10 nF, between ENC1_A and GND.
5. Connect ENC1_A to U1 physical pin 4, PD2 / Arduino D2.

```text
+3.3V
  |
 R4 10k
  |
  +---- ENC_A_RAW ---- SW1 terminal A
  |
 R5 10k
  |
  +---- ENC1_A ------- U1.4 / PD2
  |
 C6 10nF
  |
 GND
```

### 7.3 Channel B

1. Label encoder terminal B's wire ENC_B_RAW.
2. Connect R6, 10 kohm, between +3.3V and ENC_B_RAW.
3. Connect R7, 10 kohm, between ENC_B_RAW and a new net ENC1_B.
4. Connect C7, 10 nF, between ENC1_B and GND.
5. Connect ENC1_B to U1 physical pin 5, PD3 / Arduino D3.

This is the Bourns suggested filter topology powered from 3.3 V rather than its example's 5 V. The pull-up belongs on the RAW contact side of the series resistor. Moving it to the MCU side would create a voltage divider when the contact closes.

Configure PD2 and PD3 as INPUT, not INPUT_PULLUP. The internal pull-ups would interact with the series resistors and impair the LOW level. The filters do not eliminate the need for correct quadrature/detent handling in firmware.

## 8. Wire SW1 push button

The push contacts are electrically separate from A/B/C.

1. Connect push-switch contact 2 to GND.
2. Connect push-switch contact 1 to ENC_SW_RAW.
3. Connect R8, 10 kohm, from +3.3V to ENC_SW_RAW.
4. Connect R9, 1 kohm, from ENC_SW_RAW to ENC1_SW.
5. Connect C8, 100 nF, from ENC1_SW to GND.
6. Connect ENC1_SW to U1 physical pin 6, PD4 / Arduino D4.

```text
+3.3V
  |
 R8 10k
  |
  +---- ENC_SW_RAW ---- push switch ---- GND
  |
 R9 1k
  |
  +---- ENC1_SW ------- U1.6 / PD4
  |
 C8 100nF
  |
 GND
```

Use INPUT for PD4 too. Released is HIGH; pressed is LOW. A debounced short press changes edit mode. Do not connect this circuit to S1, SYS_IN, or regulator VIN.

## 9. Wire the shared I2C bus

1. Connect U1 physical pin 27 / PC4 / Arduino A4 to SDA.
2. Connect U1 physical pin 28 / PC5 / Arduino A5 to SCL.
3. Connect R2, 4.7 kohm, from SDA to +3.3V.
4. Connect R3, 4.7 kohm, from SCL to +3.3V.
5. Branch SDA and SCL to BOTH J3 and J4.

Use 100 kHz I2C. The specified Adafruit sensor board already has 10 kohm pull-ups; those remain installed. A main-board 4.7 kohm resistor in parallel with 10 kohm gives approximately 3.2 kohm before accounting for the display's pull-ups.

Before release, check the complete bus with both purchased modules connected: pull-ups must not go to 5 V, equivalent resistance must meet the devices' sink-current ratings, and standard-mode rise time must be at most 1 us. Use a conservative target of at least 1.1 kohm effective pull-up resistance at 3.3 V. Do not add arbitrary extra pull-ups or change module variants without rechecking.

## 10. Wire J3 and A2, the OLED

Use Winstar WEA012832FWPP3N00000, not an unspecified SSD1306 module with a potentially different connector order.

| Main-board J3 pin | Net | Winstar module pin |
|---|---|---|
| 1 | GND | 1, GND |
| 2 | +3.3V | 2, VIN |
| 3 | SCL | 3, SCL |
| 4 | SDA | 4, SDA |

1. Wire J3 as above.
2. Build a four-wire harness using one XHP-4 housing.
3. Solder and insulate the module ends at the corresponding labelled terminals.
4. Retain all components already on the Winstar module.
5. Configure firmware for 128x32 pixels and verify its actual I2C address during bring-up.

VIN on this display means its supply input. It connects to regulated +3.3V, NOT to the main-board VSYS or SYS_IN net.

## 11. Wire J4 and A3, the light sensor

Use Adafruit **4681**. It contains the BH1750, support components, regulator, level shifting, and pull-ups.

| Main-board J4 pin | Net | Adafruit 4681 labelled pad |
|---|---|---|
| 1 | GND | GND |
| 2 | +3.3V | VIN |
| 3 | SCL | SCL |
| 4 | SDA | SDA |

1. Wire J4 as above.
2. Build a second four-wire harness using the other XHP-4 housing.
3. Solder to the module's labelled through-hole pads, not to its small surface-mount components.
4. Leave 3Vo unconnected: it is a regulator OUTPUT, not this build's power input.
5. Leave ADDR/AD0 unconnected externally and its address solder jumper open. The module's default address is 0x23.
6. Leave the STEMMA QT connectors unused; no extra QT cable is needed.

The harness order is defined by J4, not by the physical left-to-right order of the sensor's pads.

## 12. Wire J1, the ICSP header

| J1 pin | Signal | Destination |
|---|---|---|
| 1 | MISO | U1 physical pin 18 / PB4 |
| 2 | Target voltage | +3.3V |
| 3 | SCK | U1 physical pin 19 / PB5 |
| 4 | MOSI | U1 physical pin 17 / PB3 |
| 5 | RESET | U1 physical pin 1 / RESET |
| 6 | GND | GND |

Use numbered pins, not the visual left/right position of a mirrored symbol. Mark pin 1 clearly on the PCB.

For the intended programming method, disconnect USB charging, switch S1 ON, and power the target from its battery/regulator. Use a programmer supporting 3.3 V target logic, with its target-power output disabled; J1.2 is then the target-voltage reference/sense connection.

Do not connect a second powered supply to +3.3V. A conventional 5 V Nano's GPIO outputs are still 5 V even if its 3.3 V supply pin powers the target. Use proper programming-level compatibility rather than the older direct-Nano wiring instructions.

## 13. Mark unused MCU pins

For this exact board, mark these U1 pins unconnected:

| Physical pins | MCU functions |
|---|---|
| 2, 3 | PD0/RXD, PD1/TXD |
| 9, 10 | PB6, PB7; external oscillator not used |
| 11, 12, 13 | PD5, PD6, PD7 |
| 14, 15, 16 | PB0, PB1, PB2 |
| 23, 24, 25, 26 | PC0, PC1, PC2, PC3 |

There is no PWR_HOLD connection. Do not put no-connect markers on MISO, MOSI, SCK, RESET, SDA, SCL, or the encoder inputs.

## 14. Build the three harnesses

1. Take five ASXHSXH22K305 pre-crimped socket-to-socket leads.
2. Cut each at its midpoint to obtain ten approximately 152 mm pre-crimped tails.
3. Label each tail at both ends; all supplied wires are black.
4. Insert two tails into the XHP-2 housing for J2.
5. Insert four tails into an XHP-4 housing for J3.
6. Insert four tails into the remaining XHP-4 housing for J4.
7. Confirm each contact locks in place with a gentle pull test.
8. Before soldering module ends, meter each wire back to the numbered PCB pad.
9. Solder the stripped ends to the assigned module pads/terminals.
10. Insulate exposed joints and anchor each cable so pulling it does not load the solder joints.

The battery uses its own factory cable. The encoder and power switch mount directly to the main PCB; no additional encoder or switch harnesses are counted.

## 15. Add test points, mechanical references, and ERC declarations

Add copper test pads, without purchased pin hardware:

| Reference | Net |
|---|---|
| TP1 | SYS_IN |
| TP2 | VSYS |
| TP3 | +3.3V |
| TP4 | GND |
| TP5 | RESET |
| TP6 | SDA |
| TP7 | SCL |

Add four mechanical mounting-hole references H1-H4. Select their final diameter/location with the enclosure design; do not fabricate before that fit check.

Add PWR_FLAG declarations where passive connector/switch symbols prevent ERC from recognizing the external supply: typically GND and VSYS. SYS_IN may also need a declaration depending on the chosen symbols. U2's VOUT should be a power-output pin. A flag documents a real source; it does not generate power or fix an incorrect wire.

Run ERC and inspect every warning. Do not suppress warnings globally to make the report appear clean.

## 16. Mandatory firmware work before functional testing

The repository's existing [firmware](../../firmware/main/main.ino) still targets the old soft-power latch. It must be adapted before this board is considered operational:

1. Remove PWR_HOLD setup and shutdown control.
2. Remove shutdownSystem() and the long-press path that enters an infinite loop.
3. Configure encoder A, B, and push as INPUT rather than INPUT_PULLUP.
4. Keep debounced short-press edit-mode selection; a long hold must not freeze the MCU.
5. Suppress a button held during startup until it has been released.
6. Decode the encoder so one physical detent makes one intended adjustment; the current transition-at-a-time code requires qualification.
7. Set I2C to 100 kHz; verify addresses with real bus acknowledgement, not just a display-buffer allocation result.
8. Use a build and fuse configuration for internal 8 MHz. Select and verify a suitable brownout setting rather than carrying forward disabled brownout without review.

This manual does not change or validate firmware. Do not flash the old image and interpret its five-second freeze as a new hardware fault.

### Low-battery operating limit

The MCP1700 is not a boost converter. Below its required input headroom, +3.3V falls with battery voltage. The Adafruit sensor board specifies a 3 V minimum VIN, so do not claim valid operation below 3.0 V at the main regulated rail.

The battery protection cutoff is a last-resort protection function, not a precise operational cutoff for the meter. Before release, qualify minimum operating voltage and implement a suitable low-battery warning/stop behavior in firmware. Stop bench functional tests when the main rail drops below 3.0 V; do not deliberately deep-discharge the cell to test a software feature.

## 17. Check the complete schematic before layout

- [ ] Exactly two ICs, one slide switch, one switched encoder, nine resistors, eight capacitors, and four connectors on the main board.
- [ ] J2.1 is A1 VBAT, never A1 5V.
- [ ] All returns are GND in this protected-pack architecture.
- [ ] S1.2 receives SYS_IN; S1.1 feeds VSYS; S1.3 is unconnected.
- [ ] U2 pins are 1=GND, 2=VIN, 3=VOUT.
- [ ] C1/C2 are shunts to GND on the correct sides of U2.
- [ ] U1 VCC/AVCC and both grounds are connected.
- [ ] AREF connects only to its capacitor.
- [ ] Encoder RAW nets and filtered MCU nets are distinct.
- [ ] Encoder pull-ups are on the RAW side of the series resistors.
- [ ] SDA/SCL reach both modules and their pull-ups.
- [ ] Sensor VIN, not 3Vo, receives main-board power.
- [ ] ICSP numbering and unused-pin markers are correct.
- [ ] All connector and component footprint pad numbers match the actual parts.
- [ ] ERC is clean or every remaining warning has a specific reviewed explanation.

## 18. Layout and enclosure rules

- Put C1/C2 close to U2 and C3/C4 close to U1 supply pins.
- Keep power/ground routes short and wide enough for the measured load; use a continuous system-GND return where practical.
- Keep the encoder's filtered signal paths short; place filters near the MCU inputs.
- Verify U2's 2.54 mm wide footprint and S1's 2.50 mm terminal pitch separately.
- Place A1's existing USB-C socket at an enclosure opening. Provide mechanical retention so cable insertion does not pull on the harness.
- Keep charger heat and exposed solder joints away from the pouch cell. Provide insulation and strain relief.
- Keep the battery clear of screw tips and sharp edges; never clamp or compress the pouch.
- Position A3 behind the diffuser and away from OLED/charge-LED light leakage.
- Make S1 accessible and label the enclosure: "OFF TO CHARGE".
- Perform DRC and a physical/3D fit check before ordering the PCB or housing.

## 19. Bring-up and acceptance tests

Use a supervised bench and disconnect power before changing wiring. Never place a current-mode meter directly across a battery or supply.

### 19.1 Unpowered inspection

1. Disconnect battery, USB, programmer, and modules.
2. Inspect polarity, solder bridges, connector numbering, and component orientation.
3. Check for hard shorts between SYS_IN/GND, VSYS/GND, and +3.3V/GND. Allow capacitors to charge when interpreting resistance readings.
4. Verify S1 continuity: ON joins SYS_IN to VSYS; OFF opens that path.
5. Verify harness continuity pin-for-pin before connecting the modules.

### 19.2 Main-board power test without a battery

1. Leave J2's charger harness disconnected. Do not backfeed a bench supply into A1 or the battery.
2. Feed J2.1/J2.2 from a current-limited bench supply set to 3.7 V, initially with only the main board connected.
3. Start with a conservative 50 mA limit. Investigate a current-limit event before increasing it.
4. With S1 ON, verify VSYS is close to the input and +3.3V is approximately 3.3 V.
5. At room temperature with adequate input headroom, use 3.20-3.40 V as a bring-up acceptance window; inspect ripple/transients under load too.
6. Switch OFF and verify the system rail decays near zero after capacitors discharge. Disconnect programmers during this check.
7. Attach the display and sensor with power OFF, then repeat. Measure actual run/startup load and check against the 200 mA switch rating.
8. Repeat with a 4.2 V source and sweep downward without a battery to characterize dropout. Stop claiming valid operation below 3.0 V at +3.3V.

### 19.3 Programming and controls

1. Use verified 3.3 V-compatible ISP and the updated firmware described above.
2. Verify the MCU signature and internal-clock fuse readback.
3. Check sensor acknowledgement at 0x23 and the actual display address.
4. Check SDA/SCL rise times and LOW levels with both modules connected.
5. Turn the encoder one detent at a time in both directions; verify one intended setting change per detent.
6. Test faster turns, short presses, long holds, and power-on with the button held.
7. Repeat mechanical power-off/power-on at least ten times.

### 19.4 Battery and USB-C charging

1. Remove the bench supply and programmer. Set S1 OFF.
2. Check that A1 is still configured for 100 mA and the battery polarity matches.
3. Plug BT1 into A1 and reconnect A1's VBAT/GND harness to J2.
4. At room temperature, charge from a known-good USB-A-to-C cable/source.
5. Repeat using a normal USB-C-to-C source, reversing the USB-C plug orientation as part of the test.
6. Verify approximately 5 V at A1's USB input. Verify the charge indicator behaves as documented.
7. With S1 OFF, the MCU/display must remain unpowered while the battery charges.
8. Verify the charge current is consistent with the configured 100 mA rate and tapers near full. USB input current also includes board/LED consumption and is not exactly battery charge current.
9. If measuring battery current, use a correctly polarized inline fixture installed with all sources disconnected. Never short the pack with an ammeter.
10. Confirm the battery approaches its specified 4.2 V charge voltage and the charger reports completion. Do not continue if voltage exceeds the battery specification.
11. Monitor for abnormal heating, swelling, smell, or unstable charging; disconnect USB and stop testing if any occurs. Follow the battery's temperature and handling limits.
12. Unplug USB before turning S1 ON. Confirm normal operation from the battery.

A successful connector fit or charging LED alone is not proof of correct termination, protection, or USB-C compatibility. Record the measured results before treating the design as ready for a pilot.

## 20. Source documents

- [Adafruit 4410 USB-C charger](https://www.adafruit.com/product/4410)
- [Adafruit charger schematics: use PID 4410](https://learn.adafruit.com/adafruit-microlipo-and-minilipo-battery-chargers/downloads)
- [Adafruit 1578 protected battery, charging/handling limits](https://www.adafruit.com/product/1578)
- [Adafruit 4681 pinouts](https://learn.adafruit.com/adafruit-bh1750-ambient-light-sensor/pinouts)
- [E-Switch EG1218 manufacturer drawing](https://configured-product-images.s3.amazonaws.com/2D/specs/EG1218.pdf)
- [Microchip MCP1700 datasheet, distributor-hosted copy: pin table and capacitor requirements](https://www.mantech.co.za/datasheets/products/mcp1700t-3302e-tt-250219a.pdf)
- [Bourns PEC11R datasheet, distributor-hosted copy: suggested filter circuit](https://www.electrokit.com/upload/quick/71/2d/4349_PEC11R-4015F-N0024.pdf)
- [Winstar WEA012832F module family pinout](https://www.winstar.com.tw/products/oled-module/graphic-oled-display/0_91-oled.html)
- [KiCad wide TO-92 footprint](https://raw.githubusercontent.com/KiCad/kicad-footprints/master/Package_TO_SOT_THT.pretty/TO-92_Inline_Wide.kicad_mod)
