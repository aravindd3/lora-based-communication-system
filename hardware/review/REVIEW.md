# LoRa schematic and PCB review

**Status: corrected review copy; not released for fabrication.** The schematic passes KiCad ERC, and all PCB connections are routed. Four USB-C footprint clearance errors remain. Component selection, load capacity, USB impedance, and hardware operation still require validation.

The original files in `C:/Users/beoin/OneDrive/Documents/lora/lora/` were left unchanged. Open the included `lora.kicad_pro` to use the corrected schematic, board, and local libraries together. Keep the `libraries` folder and both library tables alongside the project.

## Verification results

Checks were run with KiCad 10.0.4, including zone refill and schematic-to-PCB comparison.

| Check | Initial result | Corrected result |
|---|---:|---:|
| Schematic ERC messages, including exclusions | 95 | 0 |
| PCB DRC violations | 156 | 4 |
| Unrouted PCB connections | 3 | 0 |
| Schematic-to-PCB discrepancies | 1 | 0 |

Many initial warnings concerned unavailable library definitions. The corrected project includes recovered libraries. Passing checks against those libraries establishes internal consistency; it does not independently certify the original custom footprint dimensions against component drawings.

The existing fabrication constraints were retained: 0.20 mm minimum track width, 0.30 mm minimum drill, 0.25 mm hole clearance, and 0.50 mm copper-to-board-edge clearance. No new DRC exclusions were added. Existing ignored-check categories are listed in the supplied reports.

## Corrections made

- Fixed the amplifier VDD connections, which had been connected to ground. U6 pins 7 and 8 now use SYS; C13 and C14 bypass SYS to ground. Corrected stacked symbol pins and electrical pin types.
- Corrected LoRa bypass capacitors C8 and C9 from ground-to-ground connections to 3.3 V-to-ground connections. Relocated them nearer the module supply connection while retaining the module's ground pins on ground.
- Connected USB protection U3 pin 5 to VBUS instead of ground. Connected the USB-C shield to ground.
- Grounded charger U1's charge-enable input explicitly. Assigned its four previously unassigned thermal holes to ground as pad 17, and enlarged the thermal drills to 0.30 mm with 0.60 mm copper pads.
- Enlarged the ESP32 module thermal drills from 0.20 to 0.30 mm; retained their 0.60 mm pads and the embedded antenna keepout.
- Corrected the I2C connector and resistors: J3 pins 2/3 connect to SDA/SCL, and R8/R9 pull those signals up to 3.3 V.
- Added speaker connector J6: pin 1 is S+ / OUTP, and pin 2 is S- / OUTN. Neither speaker output is a ground connection.
- Changed the ESP32 EN capacitor C1 from 10 uF to 1 uF, retaining its 10 kOhm pull-up.
- Removed faulty ground routing, rebuilt ground pours on both copper layers, and completed the corrected signal and supply connections.
- Raised all trace segments below 0.20 mm to at least 0.20 mm. Widened supply, battery, inductor, and speaker runs where clearance permitted. Main widened sections reach 0.60 mm; constrained escapes remain narrower.
- Fixed silkscreen collisions and edge issues, retained reference designators, and moved conflicting outline graphics to fabrication layers.
- Made footprint assignments consistent between schematic and PCB and provided local footprint variants for the edits.

The pin corrections follow the [TI BQ24074 datasheet](https://www.ti.com/lit/ds/symlink/bq24074.pdf), [ST USBLC6-2 datasheet](https://www.st.com/resource/en/datasheet/usblc6-2.pdf), and [Analog Devices MAX98357A datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/max98357a-max98357b.pdf). The EN RC change follows [Espressif's schematic guidance](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/schematic-checklist.html).

## Remaining fabrication and electrical decisions

1. **USB-C locating-hole clearance:** J2's original USB4105 footprint has 0.1944 mm copper clearance to its two non-plated locating holes, below the project's 0.25 mm requirement. Four errors represent the duplicated ground pads around those two holes. Resolve this with the fabricator and the exact connector land-pattern drawing, or choose a compatible connector/footprint that meets the rule. The holes and solder lands were not arbitrarily resized, and the rule was not weakened.

2. **Inductor L1:** the retained LQM18PNR47NFR part has a 1.1 A temperature-rise current rating. Its suitability for the TPS63802's actual peak current has not been established. TI requires selection using peak current at minimum input voltage, with saturation-current margin; its reference choices have substantially higher saturation ratings. A replacement can require a different footprint and regulator placement. The current switch-net routing remains approximately 8.4 mm per net and should be revisited with the selected inductor to minimize switching loops. See [Murata's part data](https://www.murata.com/en-US/products/productdetail?partno=LQM18PNR47NFR%23) and [TI's TPS63802 selection and layout guidance](https://www.ti.com/lit/ds/symlink/tps63802.pdf).

3. **Battery temperature and charge current:** R3 remains the original unspecified `R_US` on TS. Specify the battery temperature-sensing arrangement before assembly; a fixed resistor does not measure battery temperature. R10 = 1.13 kOhm programs approximately 0.79 A nominal fast-charge current. The retained EN1-high / EN2-low configuration selects USB500 mode, so the input budget also limits charging and system load. Confirm battery chemistry, allowable charge current, and source capability against the [BQ24074 datasheet](https://www.ti.com/lit/ds/symlink/bq24074.pdf).

4. **Trace current capacity:** 0.20 mm signals and up-to-0.60 mm power sections are geometric improvements, not an established current rating. Confirm copper weight, maximum rail currents, voltage drop, temperature rise, and via capacity. Some supply and switch-pad escapes remain 0.20 mm wide. The board's retained nominal thickness is 1.6 mm; copper weight was not specified.

5. **USB routing:** D+/D- are electrically connected and clearance-checked, but they are not certified as a controlled-impedance differential pair. The actual copper/dielectric stackup and a coordinated pair-routing review are required. Espressif specifies 90 Ohm differential impedance, with a tolerance of +/-10%, in its [USB layout guidance](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/pcb-layout-design.html). USB input inrush and the combined VBUS bypass capacitance also remain unverified.

6. **Assembly and hardware validation:** confirm the exact Ra-02 module against the retained Ra-01-derived footprint, custom IC land patterns, capacitor voltage ratings/effective capacitance, and speaker impedance/power. Check regulator startup/transients, charging behavior, USB enumeration, RF performance, and amplifier operation on hardware.

## Included files

- `lora.kicad_pro`, `lora.kicad_sch`, `lora.kicad_pcb`: corrected editable project.
- `libraries`, `sym-lib-table`, `fp-lib-table`: portable project libraries.
- `erc-final.txt/json`, `drc-final.txt/json`: final machine-generated checks.
- `schematic-review.pdf`, `pcb-front-review.pdf`, `pcb-back-review.pdf`: review plots.
- `pcb-front-preview.png`: cropped front-board preview.

No production Gerbers were generated because the remaining fabrication and electrical issues are unresolved.
