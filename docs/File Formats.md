## File Formats
Custom formats are used for models, levels, cutscenes and other assets. Some are documented. Others are still mostly unknown, or were only documented on sites that no longer exist.

### Data Files
Formats seen so far, with known or suspected purpose and any parser or docs.

| File Type | Purpose | Parser/Documentation |
|-|-|-|
| .ANM | Animation data | [anm.ksy](https://github.com/laura-a-n-n/silent-hill-museum/blob/main/ksy/sh1anm.ksy) |
| .BIN | Overlay code, loaded into memory when needed by the game. | - |
| .CMP | LZSS-compressed data, unused by game. | [lzss.c](/src/screens/b_konami/lzss.c) |
| .DAT | Data for demo playback, contains button states for each frame. | - |
| .DMS | Cutscene keyframe data. | [sh1_dms.bt](file_formats/sh1_dms.bt) |
| .ILM | Skeletal models. | [sh1_model.bt](https://github.com/Sparagas/Silent-Hill/blob/main/010%20Editor%20-%20Binary%20Templates/sh1_model.bt) by Sparagas |
| .IPD | Local static models. | [sh1_model.bt](https://github.com/Sparagas/Silent-Hill/blob/main/010%20Editor%20-%20Binary%20Templates/sh1_model.bt) by Sparagas, [sh_ipd2obj](https://github.com/belek666/sh_ipd2obj) by belek666 |
| .KDT | Konami MIDI tracker files. | [kdt-tool](https://github.com/Nisto/kdt-tool) by Nisto |
| .PLM | Global static models. | [sh1_model.bt](https://github.com/Sparagas/Silent-Hill/blob/main/010%20Editor%20-%20Binary%20Templates/sh1_model.bt) by Sparagas, [sh_ipd2obj](https://github.com/belek666/sh_ipd2obj) by belek666 |
| .TIM | PsyQ SDK texture container. | SDK `filefrmt.pdf` |
| .TMD | PsyQ SDK 3D models, used exclusively on the item screen. | SDK `filefrmt.pdf` |
| .VAB | PsyQ SDK audio container. | SDK `filefrmt.pdf` |
| XA/* | XA audio & FMV video. | SDK `filefrmt.pdf` |
| Savegame | Player save data. | [ps1_memory_card.bt](https://github.com/Sparagas/Silent-Hill/blob/main/010%20Editor%20-%20Binary%20Templates/ps1_memory_card.bt) by Sparagas |

### `SILENT.` & `HILL.` Containers
The game keeps all of its data merged together inside the `SILENT.` and `HILL.` files on the game disc. `SILENT.` contains data/overlay files, while `HILL.` contains XA audio & video.

The containers have no header or file table of their own. The [file table](/src/main/filetable.c.USA.inc) is stored, slightly encoded, in the main executable. The game refers to files by table index and uses the table to find the sector to read.

> [!NOTE]  
> [`tools/silentassets/extract.py`](/tools/silentassets/extract.py) parses the file table and extracts files from both containers.

### Folder Paths
The file table still includes folder names, though the game doesn't seem to use them:

| Folder Name | File Types | Purpose |
|-|-|-|
| 1ST/ | .TIM, .BIN | B_KONAMI.BIN boot logo overlay & main BODYPROG.BIN engine overlay, both encrypted with XOR key. |
| ANIM/ | .ANM, .DMS | ANM files for each character model, and .DMS files for each cutscene location. |
| BG/ | .TIM, .IPD, .BIN | Textures & models for level graphics, along with two unknown .BIN files. |
| CHARA/ | .TIM, .ILM | Textures & skeletal models for each character. |
| ITEM/ | .TIM, .TMD, .PLM | Textures & models for in-game items. |
| MISC/ | .DAT | Data related to demo playback. |
| SND/ | .KDT, .VAB | MIDI audio & audio samples. |
| TEST/ | .CMP, .DMS, .ILM, .IPD, .TIM, .TMD | Mostly leftovers from ITF, uncertain if any are used by game. |
| TIM/ | .TIM | Full-screen textures. |
| VIN/ | .BIN | Overlay binaries for maps & screens. |
| XA/ | - | XA audio banks & FMV videos. |

### Sources

https://github.com/SILENTpavel/SHresearch/blob/master/README.md

https://github.com/Sparagas/Silent-Hill/tree/main
