# Lux2ISO Phase 2 — Project Overview

## What This Device Does

Lux2ISO is a **handheld incident light meter** built for film photographers. It measures the light falling on a scene and tells the photographer what ISO film speed would yield a correct exposure at their chosen aperture and shutter speed settings.

The photographer dials in the two exposure variables they are considering — aperture and shutter speed — and the meter shows them the ISO that those settings require. By comparing that number to the ISO of the film loaded in their camera, they immediately know whether their chosen settings will produce a correct exposure or whether they need to adjust.

---

## The User Scenario

A photographer loads a roll of **ISO 200** film into their camera. Because the film is fixed, the ISO is a constant for the entire roll. They will travel to different environments — indoors, outdoors, shade, direct sun — and need to find the right aperture and shutter speed combination for each shot.

**Each time they go to take a photo:**

1. **Point the meter at the scene.** The BH1750 sensor (behind the incident diffuser dome) continuously reads the ambient illuminance in lux.

2. **Dial in a starting aperture** with the left rotary encoder (e.g., f/5.6). The left encoder maps to the aperture table: f/1 → f/1.4 → f/2 → … → f/22.

3. **Dial in a starting shutter speed** with the right rotary encoder (e.g., 1/125s). The right encoder maps to the shutter speed table: 1s → 1/2 → 1/4 → … → 1/1000.

4. **Read the display.** The OLED shows:
   - The live lux reading for the scene
   - The currently dialed aperture and shutter speed
   - The **recommended ISO** for those exact settings in that exact light

5. **Interpret the result.** The displayed ISO is the answer to: *"What ISO film would give a perfect exposure at these settings?"*
   - If the meter shows **ISO 400** but the camera has **ISO 200** loaded: the chosen settings let in half as much light as needed. Open the aperture one stop (e.g., f/5.6 → f/4) or halve the shutter speed (e.g., 1/125 → 1/60) to bring the reading down to ISO 200.
   - If the meter shows **ISO 100** but the camera has **ISO 200** loaded: the chosen settings let in too much light. Close the aperture one stop or double the shutter speed.
   - When the meter displays **ISO 200**, the dialed aperture and shutter speed combination is exactly right for the film loaded.

6. **Set the camera and shoot.** Transfer the aperture and shutter speed shown on the meter directly to the camera.

---

## Why This Approach

Most light meters output a single recommended combination (e.g., "shoot at f/8, 1/125s for ISO 200"). Lux2ISO takes the inverse approach: the photographer is in control of aperture and shutter speed and the meter outputs the ISO that results from their choice. This gives the photographer immediate intuition about the light environment:

- A reading of ISO 800 in a dim room means the room is only bright enough for ISO 800 film at those settings — if shooting ISO 200, they must open up or slow down.
- A reading of ISO 100 outdoors means the scene is very bright — plenty of headroom for ISO 200 film.

The two-encoder design (see [Phase2-PartsList.md](Phase2-PartsList.md)) directly maps to the two independent exposure variables the photographer controls, making the adjustment feel natural and tactile.

---

## The Math Behind the Reading

The meter solves the standard photographic exposure equation for ISO, given measured lux and chosen settings:

$$ISO = \frac{250 \cdot N^2}{E \cdot t}$$

Where:
- $N$ = f-number (aperture) dialed on the left encoder
- $t$ = shutter speed in seconds dialed on the right encoder
- $E$ = lux measured by the BH1750 sensor
- 250 = incident meter calibration constant

The result is snapped to the nearest standard ISO stop (100, 200, 400, 800, 1600, 3200).

See [exposure-math.md](exposure-math.md) for the full derivation.

---

## Phase 2 Hardware That Enables This Interaction

The following Phase 2 changes (detailed in [Phase2-PartsList.md](Phase2-PartsList.md)) are what make this interaction possible:

| Component | Role in the interaction |
|-----------|------------------------|
| **Left rotary encoder** (Bourns PEC11R-4215F-S0024) | Continuously dials aperture — CW increases f-number (less light), CCW decreases (more light) |
| **Right rotary encoder** (Bourns PEC11R-4215F-S0024) | Continuously dials shutter speed — CW goes to faster speeds (less light), CCW to slower |
| **100nF debounce caps** (×4, one per A/B line) | Keep encoder signals clean so each detent registers as exactly one step |
| **BH1750 sensor** (carried over from Phase 1) | Measures the real ambient lux continuously as the user holds the meter |
| **Incident diffuser dome** (ping pong ball / 3D print) | Spreads the sensor's acceptance angle to a hemisphere, collecting light from the full scene rather than a single direction — this is what makes it an *incident* meter |
| **SSD1306 OLED display** (black PCB, white pixels) | Shows lux, dialed aperture, dialed shutter speed, and the resulting ISO simultaneously |
| **Li-Po battery + USB-C charging** (TP4056 module for breadboard; MCP73831 on final PCB) | Makes the meter portable and rechargeable for field use |

---

## Display Layout (Phase 2 Target)

```
┌────────────────────────────┐
│ ISO: 200                   │  ← large — the number to match
│                            │
│  f/5.6     1/125s          │  ← bold row — dialed settings
│                            │
│  1024 lx  Bright Outdoors  │  ← smaller — sensor reading + label
└────────────────────────────┘
```

ISO is shown largest because it is the primary output the user compares to their loaded film. Aperture and shutter speed are shown as a single row since they are dialed together. Lux and scene label provide secondary context.

---

## Quick Reference: What the ISO Reading Means

| Display shows | Your film is ISO 200 | Action |
|---------------|----------------------|--------|
| ISO 200 | ✓ Perfect match | Shoot at the dialed settings |
| ISO 400 | Needs 1 stop more light | Open aperture one stop OR halve shutter speed |
| ISO 800 | Needs 2 stops more light | Open aperture two stops OR quarter shutter speed (or split) |
| ISO 100 | 1 stop too much light | Close aperture one stop OR double shutter speed |
| ISO 50 | 2 stops too much light | Close aperture two stops OR use a faster shutter speed |

Each stop doubles or halves the light reaching the film. Aperture and shutter stops are interchangeable (the Exposure Value relationship) — one stop on aperture equals one stop on shutter speed.

---

## Scope: What Lux2ISO Does Not Do

- **It does not set your camera.** It only gives you the numbers to transfer manually to your camera's aperture ring and shutter speed dial.
- **It does not account for reciprocity failure.** Very long exposures (beyond ~1s) with some film stocks require additional exposure compensation not calculated here.
- **It does not adjust for film push/pull processing.** If you are intentionally over- or under-exposing to push or pull in development, apply that offset mentally to the ISO target.
