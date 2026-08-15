# Lux2ISO Phase 3 Pilot Plan (90 Days)

## Objective

Run a controlled pilot to validate product reliability, manufacturing repeatability, and business viability before scaling to a 1000-unit order.

## Pilot Scope

- Pilot volume: 50 to 100 units
- Build model: outsourced PCB assembly + outsourced housing print + in-house final assembly
- Target customer segment: early adopters (photo/video users willing to provide structured feedback)
- Pilot duration: 90 days from first shipment

## Success Criteria (Go/No-Go)

A pilot is considered successful if all conditions below are met:

1. Hardware return rate is less than 3.0%.
2. Early-life failure rate (first 30 days) is less than 1.5%.
3. Gross margin after packaging and expected returns is at least 55%.
4. Average support burden is less than 15 minutes per sold unit over 90 days.
5. Functional test pass rate at ship is at least 98%.
6. No unresolved safety-critical power or charging defects are found.

If any two of these fail, do not scale. Run corrective actions and re-pilot.

## Pilot Timeline

## Phase 0: Pre-Pilot Setup (Week 0-2)

### Deliverables

1. Frozen BOM and approved alternates
2. Finalized PCB test checklist
3. Finalized calibration procedure
4. Assembly work instructions with photos
5. Support/returns workflow and ticket template

### Exit Gate

- All work instructions are reviewed and a dry-run build of 5 units is completed.

## Phase 1: EV Build and Process Lock (Week 3-4)

### Volume

- 10 engineering validation units

### Focus

1. Validate outsourced PCB assembly quality
2. Validate housing fit and stack-up tolerance
3. Validate connector harness repeatability
4. Validate battery/charger behavior across charge-discharge cycles

### Required Tests Per Unit

1. Power rail validation (VBAT, VSYS, 3.3V)
2. Programmer signature + firmware flash verification
3. OLED bring-up and full UI traversal
4. Encoder input verification (A/B/SW)
5. Sensor sanity check against reference lighting scene
6. Soft-power long-press shutdown and restart behavior

### Exit Gate

- At least 9 of 10 units pass all tests with no safety-critical defects.

## Phase 2: Pilot Production Build (Week 5-7)

### Volume

- Build remaining units to reach 50-100 total pilot units

### Process Controls

1. Incoming inspection for PCBs and housings
2. Final assembly checklist sign-off per unit
3. Unit serialization and test record storage
4. 24-hour burn-in sample check on 10% of units

### Metrics Tracked

1. First-pass yield
2. Rework rate
3. Assembly time per unit
4. Defect category counts (power, display, encoder, charging, mechanical)

### Exit Gate

- First-pass yield at least 95%
- Rework rate at most 8%

## Phase 3: Controlled Market Release (Week 8-12)

### Release Strategy

1. Ship in two batches (for example 30 then 20-70)
2. Collect structured user feedback at day 7 and day 30
3. Prioritize warranty and reliability signal collection over growth

### Customer Data to Capture

1. Setup success rate
2. Reported defects by category
3. Battery/charging complaints
4. Accuracy confidence feedback
5. NPS-like satisfaction score

### Support SLA

1. Initial response within 24 hours
2. Triage resolution path documented for top 5 failure modes

## Unit Economics Model for Pilot Decision

Use this at the end of week 12:

- Per-unit COGS (pilot actual)
- Packaging and outbound shipping cost
- Payment processing fees
- Return and warranty reserve
- Net contribution per unit at current price

### Decision Thresholds

1. Net contribution remains positive after return reserve.
2. Gross margin remains at least 55% at planned sell price.
3. No single recurring defect exceeds 1% of shipped units.

## Risk Register and Mitigations

1. Through-hole assembly variability
- Mitigation: add stricter incoming QC and final test fixtures.

2. Harness/crimp inconsistency
- Mitigation: pre-crimp supplier option or crimp pull-test sampling.

3. Battery/charging safety incidents
- Mitigation: mandatory charge/discharge verification and thermal spot checks.

4. Mechanical fit variance from printed housing
- Mitigation: define dimensional tolerance checks and reject criteria.

5. Calibration drift or sensor variance
- Mitigation: reference-light calibration procedure and periodic audit samples.

## Scale-Up Gate to 1000 Units

Proceed to a 1000-unit order only if all are true:

1. Pilot meets all success criteria.
2. Defect Pareto is stable and top issue has corrective action verified.
3. Assembly and test takt time is documented and forecast capacity is known.
4. Supplier lead times and alternates are confirmed for critical parts.
5. Cash flow model supports inventory, warranty reserve, and support load.

## Immediate Next Actions (This Week)

1. Freeze current BOM and mark approved alternates.
2. Create a one-page final test checklist for every unit.
3. Build 5 dry-run units and record cycle time and defects.
4. Set up a simple returns and support ticket tracker.
5. Define pilot target price and warranty policy before first shipment.
