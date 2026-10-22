## Project Team

* [Aravind VS](https://github.com/USERNAME)
* [Vyshnav Harilal](https://github.com/USERNAME)
* [Sidharth P](https://github.com/USERNAME)
* [Jagadjith U](https://github.com/Jagadjith-U)
* [Arun K](https://github.com/USERNAME)
* [Alwin Sunny Jude](https://github.com/Jude-00-05)

# LoRa-Based Communication System

Repository for the firmware and custom KiCad hardware design of a portable LoRa-based communication device.

## Repository layout

- `firmware/` — ESP32/Arduino sketches.
- `hardware/kicad/` — editable KiCad project and required local libraries.
- `hardware/review/` — ERC/DRC reports and schematic/PCB review exports.
- `docs/` — project documentation.

## Hardware design

Open `hardware/kicad/lora.kicad_pro` in KiCad. Keep the `libraries/` folder and the `sym-lib-table` and `fp-lib-table` files beside the project files; they provide the custom symbols and footprints needed by the design.

The supplied board package includes editable schematic and PCB files, local libraries, review PDFs, and ERC/DRC results. It does not include production Gerber or drill files.

## Firmware

The `firmware/` folder contains the available Arduino sketches for microphone, OLED interface, push-to-talk, and LoRa-control functions. Each sketch is in a folder matching its `.ino` filename so it can be opened in the Arduino IDE.

## Important hardware note

The supplied KiCad design uses an ESP32-S3-WROOM-1 footprint and an Ai-Thinker Ra-01-derived LoRa footprint. Confirm the actual populated modules, pin assignments, and component values against the assembled board before any new hardware revision or fabrication order.
