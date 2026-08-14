# Lux2ISO Phase 3 - Co-Design Plan (PCB + Housing)

## Phase 3 Objective

Design the next Lux2ISO hardware revision so the PCB, display, controls, battery, and housing are engineered as one system.

Primary outcome:
- A reliable handheld unit that is easy to assemble, mechanically robust, electrically stable, and visually aligned with the intended product look.

---

## Design Principles for Phase 3

1. Electrical reliability first.
2. Mechanical fit is part of the circuit design, not an afterthought.
3. Every off-board connection must be strain-relieved and polarity-safe.
4. Build for repeatable assembly and serviceability.
5. Validate each subsystem with measurable pass/fail checks before full integration.

---

## Goals From Phase 2 Learnings

### G1. Use internal MCU clock strategy (no external crystal dependency)

What to do:
- Keep ATmega328P on internal 8 MHz RC clock workflow.
- Remove external crystal and its load capacitor footprint from the new board unless kept as optional fallback pads.
- Keep the known-good fuse strategy documented in flashing instructions.

Why:
- Reduces lockout risk and parts count.
- Matches proven bring-up path from Phase 2.

Acceptance criteria:
- Signature read succeeds repeatedly.
- MCU boots and runs firmware after power cycles without external clock parts.

---

### G2. Remove redundant capacitors already present on breakout modules

What to do:
- Verify breakout module schematics for BH1750 and OLED modules.
- Remove duplicate support components on main PCB where module already includes them.
- Keep mandatory local decoupling near MCU and regulator.

Why:
- Avoid unnecessary parts and layout crowding.
- Prevent accidental over-constraining of module power filtering.

Acceptance criteria:
- BOM reflects only required board-level passives.
- Sensor and display remain stable during live operation.

---

### G3. Widen regulator pin holes and footprint tolerance

What to do:
- Increase plated through-hole diameter and annular ring for MCP1700 TO-92 pins.
- Confirm footprint against actual measured lead diameter from purchased parts.

Why:
- Improves assembly fit and reduces rework risk.

Acceptance criteria:
- Regulator drops in without force.
- No lifted pads or stressed leads during soldering.

---

### G4. Regulator capacitors in parallel to ground (never in series with supply)

What to do:
- Place VIN bypass cap from VIN to GND.
- Place VOUT bypass cap from VOUT to GND.
- Position both caps physically close to regulator pins.

Why:
- Required for regulator stability and clean rail behavior.

Acceptance criteria:
- 3.3 V rail remains stable under display refresh and input activity.
- No brownout/reset events during normal use.

---

### G5. Add PCB screw mounting holes

What to do:
- Add at least 4 mounting holes with clear keep-out from traces and copper pour.
- Match hole size and location to standoff hardware and housing bosses.

Why:
- Enables rigid, repeatable mounting and shock resistance.

Acceptance criteria:
- PCB fits housing bosses without slotting or force.
- Board remains secure under light drop and shake handling.

---

### G6. Use longer display module to match product image and window geometry

What to do:
- Select an elongated OLED with confirmed controller, I2C address behavior, and supply compatibility.
- Update UI layout to physical active area dimensions.
- Design bezel/window geometry around the real active area, not only pixel count.

Why:
- Visual product quality depends on active-area aspect ratio and alignment.

Acceptance criteria:
- Display text fits without clipping.
- Window alignment and margins look intentional from normal viewing angles.

---

### G7. Evaluate single-dial interaction (if encoder has push switch)

What to do:
- Keep one encoder with push for mode switching (aperture edit, shutter edit, menu).
- Define fallback to dual-encoder if single-dial usability is poor.

Why:
- Can reduce panel complexity and save space.

Acceptance criteria:
- Core operation (set aperture + shutter and read ISO) can be completed quickly with no confusion.
- User test can complete exposure setup in <=10 seconds after brief instruction.

---

### G8. Replace loose module cabling with more permanent interconnects

What to do:
- Replace friction-only loose jumpers with locking connectors or soldered harnesses plus strain relief.
- Polarize all connectors to prevent reversed insertion.
- Standardize wire gauge, color coding, and connector part families.

Why:
- Reduces intermittent faults and assembly errors.

Acceptance criteria:
- Connectors remain seated through handling and vibration.
- No intermittent power/display/sensor failures during movement test.

---

### G9. Route cables with enclosure-first intent

What to do:
- Reserve cable channels and bend radii in housing CAD.
- Keep cables clear of buttons, encoder shafts, diffuser path, and screw bosses.
- Include strain-relief anchors and tie points.

Why:
- Prevents pinch damage and assembly frustration.

Acceptance criteria:
- Housing closes without pinching wires.
- No cable contact with moving controls.

---

## Additional Important Phase 3 Goals

### G10. Add debug and measurement access

What to do:
- Add labeled test pads for VBAT, 3V3, GND, RESET, SDA, SCL.
- Keep ICSP header accessible after assembly.

Acceptance criteria:
- Critical rails/signals can be measured without disassembly.

### G11. Power integrity and protection review

What to do:
- Confirm battery polarity protection strategy.
- Verify charger module placement, thermal room, and wiring labels (B+/B-, OUT+/OUT-).
- Confirm decoupling near MCU VCC/AVCC/AREF.

Acceptance criteria:
- No unsafe heating in normal charge/use conditions.
- Stable runtime under battery and USB-powered states.

### G12. Design-for-assembly and service

What to do:
- Keep connector and switch orientation obvious.
- Add silkscreen polarity labels and revision marking.
- Ensure battery replacement and ICSP access are possible without full teardown.

Acceptance criteria:
- Assembly can be done consistently with a written checklist.

---

## Phase 3 Execution Plan

## Step 1 - Lock Requirements (Electrical + Mechanical)

Deliverables:
- Final interaction decision: single-dial vs dual-dial.
- Display module selection shortlist with dimensions and pin order.
- Connector strategy (part family, pitch, lock type).
- Preliminary enclosure constraints: max PCB size, display window, battery envelope.

Exit criteria:
- Requirement freeze document approved before schematic edits.

## Step 2 - Schematic Revision (Phase 3A)

Tasks:
- Remove external crystal path (or keep optional pads only).
- Clean redundant module-support parts.
- Enforce regulator cap topology and values.
- Add connector symbols and pin labels matching chosen harness approach.
- Add test points and mounting hole references.

Exit criteria:
- ERC clean (or only reviewed intentional warnings).

## Step 3 - PCB Layout Revision (Phase 3B)

Tasks:
- Update regulator footprint hole size.
- Place mounting holes and board edge constraints from enclosure model.
- Keep power loop short around regulator and decoupling caps.
- Place connector exits to match cable channels and strain relief locations.
- Verify keep-outs for standoffs, screws, and control shaft clearances.

Exit criteria:
- DRC clean.
- 3D fit check passes against enclosure geometry.

## Step 4 - Housing CAD Co-Design (Phase 3C)

Tasks:
- Position standoff bosses to match PCB mounting holes.
- Define display bezel, diffuser opening, and control shaft locations.
- Add cable channels, clamp features, and service access cutouts.
- Validate wall thickness and printability/manufacturability.

Exit criteria:
- Dry-fit model review complete with no mechanical collisions.

## Step 5 - Prototype Build + Bring-Up (Phase 3D)

Tasks:
- Assemble one engineering prototype with final harness approach.
- Program and verify signature/fuse workflow.
- Run OLED address probe and full UI check on selected display.
- Perform movement and handling test for connector retention.

Exit criteria:
- Prototype meets functional and mechanical acceptance criteria.

## Step 6 - Verification, Documentation, and Release (Phase 3E)

Tasks:
- Create a validation checklist and test report.
- Update flashing docs to internal-clock defaults.
- Update BOM, assembly drawings, and wiring diagram.
- Freeze release package: schematic, PCB, Gerbers, enclosure files, firmware revision.

Exit criteria:
- Release package complete and reproducible.

---

## Validation Checklist (Pass/Fail)

Power and programming:
- [ ] VBAT in expected range under load.
- [ ] 3.3 V rail within tolerance during display refresh.
- [ ] Signature read consistent.
- [ ] Firmware flash and boot repeatable.

Display and UI:
- [ ] Correct OLED address handling (0x3C/0x3D as needed).
- [ ] Geometry matches selected long display.
- [ ] Readability acceptable in expected lighting.

Inputs and UX:
- [ ] Dial interaction validated (single or dual strategy).
- [ ] Push-switch mode logic reliable (if single-dial design chosen).

Mechanical and interconnect:
- [ ] PCB mounts without stress.
- [ ] Housing closes with zero cable pinch.
- [ ] Connectors remain stable under handling.
- [ ] Control shafts and buttons move freely.

---

## Risks and Mitigations

1. Display module mismatch (pin order/address/geometry)
- Mitigation: buy 2-3 candidate modules and run bench qualification before freezing PCB.

2. Single-dial UX complexity
- Mitigation: test with quick user flow script before committing enclosure geometry.

3. Cable interference with housing closure
- Mitigation: include cable mockups in CAD and reserve bend/anchor space early.

4. Regulator instability from poor placement
- Mitigation: enforce close cap placement and validate rail under dynamic load.

5. Assembly variability
- Mitigation: add polarized connectors, silkscreen labels, and assembly test checklist.

---

## Suggested Milestones

- M1: Requirements freeze and parts down-select.
- M2: Schematic complete and reviewed.
- M3: PCB routed, enclosure aligned, and fit-checked.
- M4: First integrated prototype assembled.
- M5: Validation pass and release package archived.

---

## Definition of Done (Phase 3)

Phase 3 is complete when:
- The board and housing assemble cleanly without ad-hoc rework.
- The unit powers, flashes, and runs reliably on the internal-clock strategy.
- The selected long display and input method provide the intended product experience.
- Interconnects are mechanically secure and serviceable.
- Documentation is complete enough for repeat build and future revision handoff.
