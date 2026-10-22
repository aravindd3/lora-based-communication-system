# Hardware files

## Editable design

`hardware/kicad/` is the complete editable KiCad project. Use `lora.kicad_pro` as the entry point. The `.kicad_sch` and `.kicad_pcb` files should remain in the same directory as the project file and its local libraries.

## Review material

`hardware/review/` contains the supplied electrical-rule and design-rule reports, together with PDF schematic and board reviews. `pcb-front-preview.png` is a quick visual preview of the front layer.

## Deliberately excluded

- `.history/` is KiCad's local recovery history and should not be versioned.
- `~lora.kicad_pro.lck` is a temporary KiCad lock file.
- `lora-reviewed.zip` duplicates the editable project and review material.

The source package does not contain manufacturing Gerbers or drill files. Add them under `hardware/fabrication/` only after the board is approved for fabrication.
