# Beginner’s Guide to Designing the Plastic Housing and Diffuser for the Lux2ISO

This guide is written for a first-time maker who wants to build a clean, functional plastic housing for a handheld incident light meter like Lux2ISO. The goal is to help you choose a practical process, design the parts in CAD, and make the first prototype without jumping straight into expensive injection molding.

## 1. What you are actually making

For this project, there are really two separate plastic parts to think about:

1. The outer housing
   - Holds the PCB, battery, OLED, encoders, and wiring
   - Should be ergonomic and durable
   - Can be made from a simple shell or two-part enclosure

2. The diffuser dome or light-facing front cover
   - The sensor needs a light-accepting front surface that collects light from the scene
   - This part should be translucent or semi-diffusing
   - A simple hemispherical or rounded dome shape is ideal

For a first version, do not start with full injection molding. Start with a 3D printed shell and a simple vacuum-formed or 3D printed diffuser.

---

## 2. Best manufacturing path for a beginner

### Recommended path for this project

Use this order:

1. Design the housing in CAD
2. 3D print a prototype housing
3. Make a simple diffuser dome using vacuum forming or a printed part
4. Test fit and ergonomics
5. Only later consider injection molding if you want a production version

### Why this is the best approach

- You can iterate quickly
- You avoid expensive tooling
- You can test the size, feel, and sensor behavior before committing to a mold
- You can learn the design rules that will still apply to later production methods

### Good beginner options

#### Option A: 3D printed housing + 3D printed diffuser (easiest)
Best for first prototype.
- Fastest
- Cheapest
- Great for iteration
- Good enough for functional testing

#### Option B: 3D printed housing + vacuum-formed diffuser dome
Best for a more polished, lightweight dome.
- Better finish than a plain printed dome
- More realistic for an incident light meter
- Requires a simple male form or plug

#### Option C: Simple silicone mold + cast plastic part
Good later if you want a small batch of identical housings or diffuser pieces.
- More work
- Better for small runs than one-off prints
- Not ideal for the first prototype

#### Option D: Injection molding
Best only when the product is proven and you want volume production.
- Requires expensive tooling
- Not beginner-friendly
- Not needed for the first version

---

## 3. What material to use

For this specific application, the most practical beginner materials are:

### For the housing
- PETG: best general-purpose choice
- ASA: better if you want more UV resistance and outdoor durability

### For the diffuser dome
- PETG is still a good starting point
- Clear PETG or translucent PETG can work well
- If you want optical diffusion, a slightly frosted or semi-opaque finish is better than fully clear

### If you want a more premium look
- Acrylic or polycarbonate can be used later, but they are more advanced and more expensive

For your first version, use PETG for the housing and either PETG or a vacuum-formed translucent sheet for the diffuser.

---

## 4. Where to design the housing and mold

You will design the parts in CAD software.

### Good beginner CAD programs

- Fusion 360: excellent for beginners and professionals, strong for parametric modeling
  - https://www.autodesk.com/products/fusion-360/overview
- Onshape: browser-based, easy collaboration and good for learning CAD
  - https://www.onshape.com/
- SolidWorks: very common in industry, but more advanced and often more expensive

### Best choice for you
If you are new to design, start with Fusion 360 or Onshape.

---

## 5. What to design first

Before you start modeling, define the core layout.

### Measure these things first

- PCB size and mounting points
- Battery size and location
- OLED display size and position
- Rotary encoder diameter and shaft clearance
- Sensor location and sensor window size
- Wiring path and connector access
- Hand size and comfort in the palm

### Basic shape ideas

For a handheld meter, a compact rectangular or slightly rounded body is easiest.

A good layout is:
- Front face: diffuser window and sensor opening
- Top or side: encoder knobs and display
- Bottom or back: battery compartment and switch

The housing should not trap the sensor behind a thick solid wall. The front opening should be clear and open enough for the light to reach the sensing element.

---

## 6. Design rules for the housing

### Keep the wall thickness reasonable
A good starting point is:
- 2 mm to 3 mm for a printed housing
- 1 mm to 2 mm for a thin diffuser shell

### Add clearance for electronics
Leave a little extra room around the PCB and battery so wires do not get pinched.

### Add mounting features
You can add:
- PCB standoffs
- Snap-fit tabs
- Bosses for screws or inserts
- A simple battery compartment cover

### Make the front face ergonomic
The front should feel natural in hand and allow the user to point it at the scene comfortably.

### Leave access for buttons and display
Make sure the display and encoder shafts have enough clearance and do not bind when assembled.

---

## 7. Design rules for the diffuser dome

The diffuser is one of the most important parts of the product.

### Shape ideas
A simple hemispherical or domed front surface works well.

Good starting geometry:
- A half-sphere or shallow dome
- A smooth outer curve
- A slightly rounded lip at the edge for strength

### Design goals
The diffuser should:
- Let light reach the sensor from a wide angle
- Reduce directional glare
- Spread light across the sensor surface
- Not be too thick or too opaque

### Important note
Do not make the diffuser too thick. A thick plastic dome can reduce light transmission and make the sensor less responsive.

A good starting thickness is about 1 mm to 1.5 mm if printed, or thinner if vacuum-formed.

---

## 8. Beginner workflow: from idea to first prototype

### Step 1: Create a simple body sketch
In Fusion 360 or Onshape, create a rough body shape for the meter.

Start with:
- a rectangular or rounded body
- a front opening for the diffuser
- a cutout for the display
- space for the PCB and battery

### Step 2: Place the electronics in the model
Import the PCB outline or create a simple placeholder block.

Place these items in the model:
- PCB
- battery
- display
- encoders
- sensor location

This makes the housing fit the hardware instead of just looking nice on paper.

### Step 3: Add the front diffuser opening
Create a circular or oval opening on the front.

You can then add:
- a shallow dome
- a rounded front face
- a simple cover ring around the diffuser

### Step 4: Add wall thickness and fillets
Round the edges slightly so the part feels better in hand and is easier to print.

### Step 5: Split the housing into two halves
For a practical enclosure, split it into:
- a front half
- a back half

The front half can hold the diffuser and display.
The back half can hold the battery and PCB.

### Step 6: Export the files
Export:
- a printable STL for the housing
- a printable STL for the diffuser or mold master

### Step 7: Print and test
Print the parts and check:
- fit around the PCB
- clearance for the encoders
- display visibility
- battery access
- sensor window alignment

---

## 9. If you want to make a molded diffuser dome

If you want a more polished diffuser, use a simple mold approach.

### The easiest beginner method: vacuum forming
This is a good bridge between a printed prototype and an actual molded part.

#### How it works
1. Make a 3D printed master form of the dome
2. Place the sheet plastic over the form
3. Heat it until soft
4. Pull it over the form with vacuum
5. Cool it and trim it

#### Why it is good for this project
- Much simpler than true injection molding
- Great for a dome-shaped diffuser
- Lets you make a lightweight translucent part

### What to use for the vacuum-forming master
Use a 3D printed male form shaped like the outside of the diffuser.

You can then make the final part around that form.

---

## 10. If you want a real mold instead of a printed part

A real mold is usually made in two halves.

### Basic mold design principles
When designing a mold, follow these rules:
- Add draft angles so the part can be removed
- Keep wall thickness consistent
- Avoid deep, narrow features
- Avoid sharp internal corners if possible
- Leave room for ejection and assembly

### For a beginner, keep the part simple
Do not start with a highly complex shape with lots of undercuts.

A simple rounded housing with a front diffuser window is much easier than a highly sculpted shell.

---

## 11. Good first version design for Lux2ISO

A very reasonable first design is:

- A rounded rectangular handheld body
- A front diffuser window with a shallow dome
- A top-facing OLED display
- Two rotary encoder openings on the front or top
- A simple battery compartment on the rear
- A two-part shell with a simple snap fit or screw mounting

This keeps the design simple while still feeling like a real product.

---

## 12. Recommended websites to visit

These are useful resources for learning the design and manufacturing process.

### CAD and design
- Fusion 360: https://www.autodesk.com/products/fusion-360/overview
- Onshape: https://www.onshape.com/

### Design-for-manufacturing and molding basics
- Protolabs design resources: https://www.protolabs.com/resources/
- Fictiv design resources: https://www.fictiv.com/resources

### 3D printing and prototyping
- Formlabs: https://www.formlabs.com/
- Prusa: https://www.prusa3d.com/

### Vacuum forming / thermoforming
- Protolabs resources: https://www.protolabs.com/resources/
- Fictiv resources: https://www.fictiv.com/resources

### Search terms to use on those sites
- "designing for injection molding"
- "design for manufacturability"
- "vacuum forming design guide"
- "3D printed housing design"
- "draft angle injection molding"

---

## 13. Suggested learning path for you

If you are just starting, follow this order:

1. Learn basic CAD in Fusion 360 or Onshape
2. Model the electronics and housing envelope
3. Make a simple printed prototype shell
4. Add a diffuser opening and test fit
5. Iterate until the form feels right
6. Then consider vacuum forming or a more advanced mold

---

## 14. What to avoid at first

Avoid these beginner mistakes:
- Designing the whole housing before measuring the electronics
- Making the diffuser too thick
- Using a shape with too many undercuts
- Ignoring draft angles if you plan to mold later
- Making the housing too small for the battery and connectors
- Forgetting that the display and encoders need clearance and a comfortable grip

---

## 15. A very practical first milestone

Your first milestone should be:

- one printable enclosure shell
- one diffuser window or simple dome
- a fit check with the PCB and battery
- a test of how the meter feels in your hand

If that works, you have already built the foundation for a better version.

---

## 16. My recommendation for you

For your project, I would start with:

- a 3D printed PETG housing
- a simple front diffuser dome
- a rounded ergonomic body
- a two-part shell for easy assembly

That gives you a realistic prototype that is affordable and fast to build.

If you want, the next step can be to help you create a very simple starter CAD layout for the housing in Fusion 360 or Onshape, with the PCB, battery, display, and encoders placed in the model.
