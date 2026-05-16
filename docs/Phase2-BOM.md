# Lux2ISO Phase 2 — Complete Bill of Materials

All parts required to build Phase 2 from scratch. Links are verified against live DigiKey and Amazon listings.
Prices are unit prices at qty 1 in USD; verify current pricing before ordering.

---

## DigiKey Order

> Primary order: all ICs, passives, connectors, encoders, and PCB parts.

### Microcontroller & Clock

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 1 | ATMEGA328P-PU | ATmega328P-PU, DIP-28, 8-bit MCU | $2.89 | [Link](https://www.digikey.com/en/products/detail/microchip-technology/ATMEGA328P-PU/1914589) |
| 1 | ED281DT | DIP-28 IC socket, 0.3" row spacing | $0.35 | [Link](https://www.digikey.com/en/products/detail/on-shore-technology-inc/ED281DT/4147600) |
| 1 | HC-49-U-S8000000ABJB | 8.000 MHz HC-49/U-S crystal | ~$0.50 | [Link](https://www.digikey.com/en/products/detail/citizen-finedevice-co-ltd/HC-49-U-S8000000ABJB/284205) |
| 2 | K270J15C0GF5TL2 | 27 pF C0G ceramic disc cap (crystal load caps) | ~$0.40 ea | [Link](https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/K270J15C0GF5TL2/286460) |

### 3.3 V Power Regulation

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 1 | MCP1700-3302E/TO | 3.3 V LDO, 250 mA, TO-92 **— use inline PCB footprint** | ~$0.50 | [Link](https://www.digikey.com/en/products/detail/microchip-technology/MCP1700-3302E-TO/652680) |
| 2 | ECE-A1HKA010 | 1 µF 50 V radial electrolytic (VIN + VOUT bypass, C8/C9) | ~$0.30 ea | [Link](https://www.digikey.com/en/products/detail/panasonic-industry/ECE-A1HKA010/6929) |

> **MCP1700 soldering note:** Use the **TO-92 inline footprint** on the PCB (`Package_TO_SOT_THT:TO-92_Inline` in KiCad). Place the three holes in a straight line at **2.54 mm (0.1") pitch** instead of the standard cramped triangle. Bend the two outer leads of the TO-92 outward to match before inserting. This gives standard 0.1" clearance between adjacent pads, eliminating solder bridging. Do **not** use the standard triangle footprint.  
> **Note:** MCP1700 datasheet requires ≥1 µF on both VIN and VOUT pins. Any radial aluminum electrolytic rated ≥10 V will work; substitute with any in-stock 1 µF radial if ECE-A1HKA010 is unavailable.

### USB-C Li-Po Charging (PCB)

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 1 | MCP73831T-2ACI/OT | Li-Po charger IC, SOT-23-5, 500 mA | $0.76 | [Link](https://www.digikey.com/en/products/detail/microchip-technology/MCP73831T-2ACI-OT/964301) |
| 1 | USB4085-GF-A | USB Type-C 2.0 through-hole right-angle receptacle | $0.91 | [Link](https://www.digikey.com/en/products/detail/gct/USB4085-GF-A/9859662) |
| 2 | CF14JT5K10 | 5.1 kΩ 1/4 W resistor (USB-C CC1/CC2 pull-downs) | $0.10 ea | [Link](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT5K10) |
| 1 | CF14JT3K90 | 3.9 kΩ 1/4 W resistor (MCP73831 PROG pin, sets 128 mA charge current) | $0.10 | [Link](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT3K90) |
| 1 | WP7113ID | Red diffused 5 mm LED, 1.9 V forward voltage (charge indicator) | $0.21 | [Link](https://www.digikey.com/en/products/detail/kingbright/WP7113SRD-D/1747663) |
| 1 | CF14JT1K00 | 1 kΩ 1/4 W resistor (LED current limiter) | $0.10 | [Link](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT1K00/1741314) |
| 1 | ESH475M050AC3AA | 4.7 µF 50 V radial electrolytic (MCP73831 VBAT bypass) | ~$0.20 | [Link](https://www.digikey.com/en/products/detail/kemet/ESH475M050AC3AA) |
| 1 | B2B-PH-K-S | JST PH 2 mm 2-pin vertical battery header (STAT pin — optional) | $0.11 | [Link](https://www.digikey.com/en/products/detail/jst-sales-america-inc/B2B-PH-K-S/926611) |

> **USB4085-GF-A note:** This is a **through-hole right-angle** USB-C connector, NOT SMD mid-mount. It mounts on the PCB edge with pins through the board.  
> **Charge current:** R_PROG = 1000 V / I_charge → 3.9 kΩ ≈ 256 mA. Replace with 2 kΩ for 500 mA charging on full PCB build.

### Rotary Encoders

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 2 | PEC11R-4215F-S0024 | Bourns 24-detent incremental encoder, integrated push switch, 15 mm shaft | $2.27 ea | [Link](https://www.digikey.com/en/products/detail/bourns-inc/PEC11R-4215F-S0024/4499665) |
| 4 | K104K15X7RF5TL2 | 100 nF X7R ceramic cap (encoder contact debounce, one per encoder phase A/B) | ~$0.25 ea | [Link](https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/K104K15X7RF5TL2/286538) |

> **GPIO assignments:**
> - Encoder 1 (ISO/EV): A = PD2, B = PD3, SW = PD4
> - Encoder 2 (Aperture): A = PD5, B = PD6, SW = PD7

### I2C Pull-ups & MCU Decoupling

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 2 | CF14JT4K70 | 4.7 kΩ 1/4 W resistor (I2C SDA/SCL pull-ups) | $0.10 ea | [Link](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT4K70/1741428) |
| 1 | CF14JT10K0 | 10 kΩ 1/4 W resistor (ATmega RESET pull-up) | $0.10 | [Link](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT10K0/1741265) |
| 3 | K104K15X7RF5TL2 | 100 nF X7R ceramic cap (MCU VCC, AVCC, and AREF bypass; reuse same part as encoder debounce) | ~$0.25 ea | [Link](https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/K104K15X7RF5TL2/286538) |

### Connectors, Headers & Switch

| Qty | Part Number | Description | Unit Price | DigiKey Link |
|-----|------------|-------------|-----------|-------------|
| 1 | TSW-103-07-G-D | 2×3 2.54 mm ICSP header (Arduino-as-ISP programming) | ~$1.50 | [Link](https://www.digikey.com/en/products/detail/samtec-inc/TSW-103-07-G-D/1101274) |
| 1 | PH1-40-UA | 40-pin 2.54 mm breakaway single-row header (cut to fit BH1750, OLED, encoder signal pins) | ~$0.65 | [Link](https://www.digikey.com/en/products/detail/adam-tech/PH1-40-UA/22118795) |
| 1 | EG1218 | SPDT slide switch (power switch) | ~$0.50 | [Link](https://www.digikey.com/en/products/detail/e-switch/EG1218/101726) |

---

## Amazon / Adafruit Order

| Qty | Description | Source | Link | Est. Price |
|-----|------------|--------|------|-----------|
| 1 | BH1750FVI ambient light sensor module | Amazon | [HiLetgo BH1750FVI](https://www.amazon.com/HiLetgo-BH1750FVI-intensity-illumination-arduino/dp/B00M0F29OS) | ~$3–5 |
| 1 | SSD1306 0.96" OLED display, **black PCB**, 128×64, I2C | Amazon | Search "SSD1306 0.96 OLED black PCB I2C 128x64" | ~$4–6 |
| 1 | TP4056 USB-C Li-Po charger module with protection circuit | Amazon | Search "TP4056 USB-C lipo charger module with protection" | ~$1–2 |
| 1 | 3.7 V 500 mAh Li-Po battery, JST PH 2 mm connector | Adafruit | [Adafruit #1578](https://www.adafruit.com/product/1578) | ~$8 |

> **TP4056 note:** The TP4056 breakout is the breadboard/prototype charging solution. The Phase 2 PCB uses the MCP73831T instead. Only buy the TP4056 module if prototyping on breadboard before the PCB is ready.  
> **OLED note:** Look specifically for the **black PCB** version — some SSD1306 breakouts have a blue PCB and slightly different pinout labels.

---

## Summary: DigiKey Cart Totals

| Category | Parts | Est. Subtotal |
|----------|-------|--------------|
| MCU & Clock | ATMEGA328P-PU, socket, crystal, 27 pF caps | ~$5.00 |
| Power Regulation | MCP1700, 1 µF caps × 2 | ~$1.50 |
| USB-C Charging IC | MCP73831T, USB4085, resistors, LED, 4.7 µF cap | ~$4.00 |
| Rotary Encoders × 2 | PEC11R + 4 debounce caps | ~$6.00 |
| Passives & Decoupling | 0.1 µF × 7, 4.7 kΩ × 2, 10 kΩ, 1 kΩ | ~$3.00 |
| Connectors & Switch | ICSP header, pin header, JST, slide switch | ~$3.50 |
| **DigiKey Subtotal** | | **~$23** |
| Amazon/Adafruit | BH1750, OLED, Li-Po battery | **~$15–20** |
| **Total (Phase 2)** | | **~$38–45** |

> Prices exclude shipping. DigiKey orders over $35 USD typically qualify for free standard shipping.

---

## GPIO Pin Reference

```
ATmega328P-PU Phase 2 Pin Assignments
──────────────────────────────────────
PC4 (SDA)  → I2C — BH1750 & SSD1306
PC5 (SCL)  → I2C — BH1750 & SSD1306
PD2 (INT0) → Encoder 1 Phase A
PD3 (INT1) → Encoder 1 Phase B
PD4        → Encoder 1 Switch
PD5        → Encoder 2 Phase A
PD6        → Encoder 2 Phase B
PD7        → Encoder 2 Switch
PC6 (RESET)→ 10 kΩ pull-up to VCC; ICSP pin
```

---

## Verified Part Notes

- **PEC11R-4215F-S0024**: Use DigiKey ID **/4499665**. An older incorrect listing at /4499656 maps to a different encoder variant — double-check the part number before checkout.
- **USB4085-GF-A**: Through-hole right-angle connector. Requires 4 through-hole pads on the PCB edge plus 2 anchor pads. **Not SMD/mid-mount.**
- **CF14JT3K90 / CF14JT5K10**: DigiKey direct-page IDs vary by catalog update. Links above use manufacturer slug + part number format — DigiKey will resolve to the correct product. Verify part number on the product page before adding to cart.
- **ESH475M050AC3AA**: KEMET 4.7 µF 50 V. If unavailable, any 4.7 µF radial electrolytic ≥10 V (e.g. Panasonic ECE-A1CKS4R7) is a functional substitute for the VBAT bypass position.
- **ECE-A1HKA010**: Panasonic 1 µF 50 V. If unavailable, any 1 µF radial electrolytic ≥10 V works for MCP1700 bypass.
- **MCP1700-3302E/TO footprint**: Use `Package_TO_SOT_THT:TO-92_Inline` (KiCad) — 3 holes in a straight line at 2.54 mm pitch. Bend the outer two leads outward before inserting. Avoids solder bridging that occurs with the standard TO-92 triangle footprint.
