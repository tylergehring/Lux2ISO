# Exposure Math: Lux to ISO

## Step 1 — Lux → EV at ISO 100

$$EV_{100} = \log_2\!\left(\frac{E}{2.5}\right)$$

- $E$ = illuminance in lux
- The constant 2.5 comes from the standard incident meter calibration constant $C = 250$, normalized to ISO 100: $100 / 250 = 0.4$, so $1/0.4 = 2.5$

---

## Step 2 — Solve for Required ISO

Given a chosen aperture $N$ (f-number) and shutter speed $t$ (seconds), the required ISO is:

$$ISO = \frac{C \cdot N^2}{E \cdot t} = \frac{250 \cdot N^2}{E \cdot t}$$

Or equivalently using EVs:

$$EV_{cam} = \log_2\!\left(\frac{N^2}{t}\right)$$

$$ISO = 100 \cdot 2^{\,(EV_{cam} - EV_{100})}$$

Both forms are equivalent and give the same result.

---

## Example

Scene: $E = 1000\,\text{lx}$, aperture $f/5.6$ ($N = 5.6$), shutter $1/60\,\text{s}$ ($t = 0.01\overline{6}$)

$$ISO = \frac{250 \cdot 5.6^2}{1000 \cdot (1/60)} = \frac{250 \cdot 31.36}{16.67} \approx 470$$

→ Round to nearest standard stop: **ISO 400**

---

## Standard ISO Stops

`100, 200, 400, 800, 1600, 3200`

To snap a raw ISO value to the nearest standard stop:

$$ISO_{snapped} = 100 \cdot 2^{\,\text{round}(\log_2(ISO_{raw}\,/\,100))}$$

---

## Firmware Formula

```cpp
float iso_raw = (250.0 * aperture * aperture) / (lux * shutter_speed);
// then snap iso_raw to nearest standard ISO stop
```
