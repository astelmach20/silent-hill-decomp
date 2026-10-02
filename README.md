<picture>
  <source media="(prefers-color-scheme: dark)" srcset="docs/SHDecompLogo.png">
  <source media="(prefers-color-scheme: light)" srcset="docs/SHDecompLogo-NTSC.png">
  <img alt="Silent Hill Decompilation Project" src="docs/SHDecompLogo-NTSC.png">
</picture>

<div align="center">

A matching decompilation of <i>Silent Hill</i> (PlayStation, US v1.1).

![USA code](https://img.shields.io/badge/USA%20code-100%25%20matched-brightgreen)
![USA data](https://img.shields.io/badge/USA%20data-mostly%20in%20C-yellowgreen)
[![Upstream code](https://decomp.dev/shdecompilations/silent-hill-decomp.svg?mode=shield&measure=matched_code_percent&label=upstream%20code)](https://decomp.dev/shdecompilations/silent-hill-decomp)

</div>

This is a fork of [shdecompilations/silent-hill-decomp](https://github.com/shdecompilations/silent-hill-decomp). The C source compiles back to a byte-identical copy of the original game. It is not a PC port.

> [!NOTE]
> **AI was used in this fork.** The upstream project does not accept AI-generated code. This fork used AI to finish the last function matches and to move the remaining raw data into C. Upstream has not reviewed this work. The "upstream code" badge above tracks the original repo, not this fork.

## Status

**US v1.1 (`SLUS-00707`)**
- **Code: 100% matched.** Every function is written in C. There are no `INCLUDE_ASM` stubs, and `configs/USA` has no raw `asm` segments. The last function matched was `func_8009E198` (`libkpad`).
- **Caveat:** `func_8009E198` only matches with an explicit-register `@hack` (`register ... asm("$n")`). Upstream rejected this in [#496](https://github.com/shdecompilations/silent-hill-decomp/pull/496) and wants a clean match. A few upstream functions already use the same kind of hack.
- **Data:** nearly all `.data`/`.rodata` is defined in C. Still raw:
  - `map0_s02` anim info (`0x6414`-`0x6500`)
  - one 2-byte padding word in `map1_s06`
  - linker padding bytes in `bodyprog`, `map3_s06`, `map4_s05` and `map5_s01`, which `tools/postbuild.py` patches back in
  - `.bss` segments are left as-is

**Other versions**
- **JAP2:** the build works, and the Japanese text and inventory functions match. Three of those six matches use register hacks.
- **EUR, JAP0, JAP1, JAP2:** their configs still contain raw `asm` segments.

**Still to do (on every version):** naming the `func_XXXXXXXX` functions and `D_XXXXXXXX` data, turning raw data into real structs, making the build shiftable (no hard-coded addresses), and documenting how the game's systems work.

## Layout

The game is split into a small main executable and many overlays, which are loaded into memory as needed.

| File | Source | Contents |
|-|-|-|
| `SLUS_007.07` | `src/main` | Main executable. Loads `B_KONAMI.BIN` and `BODYPROG.BIN`. |
| `BODYPROG.BIN` | `src/bodyprog` | Engine and core game logic. |
| `B_KONAMI.BIN` | `src/screens/b_konami` | Boot screens. |
| `STREAM.BIN` | `src/screens/stream` | FMV playback. |
| `SAVELOAD.BIN` | `src/screens/saveload` | Save/load screen. |
| `OPTION.BIN` | `src/screens/options` | Options screen. |
| `STF_ROLL.BIN` | `src/screens/credits` | Credits. |
| `MAP#_S##.BIN` | `src/maps/map#_s##` | Per-area scripts, events and enemy AI (listed below). |

<details>
<summary>Map overlays</summary>

| Overlay | Area |
|-|-|
| `MAP0_S00` | Intro sequence in Old Silent Hill |
| `MAP0_S01` | Cafe in Old Silent Hill |
| `MAP0_S02` | Bonus unlockable areas in Old Silent Hill |
| `MAP1_S00` | School: first floor, courtyard and basement |
| `MAP1_S01` | School: second floor |
| `MAP1_S02` | School (Otherworld): first floor and courtyard |
| `MAP1_S03` | School (Otherworld): second floor and roof |
| `MAP1_S04` | Unused |
| `MAP1_S05` | School boss fight |
| `MAP1_S06` | School: first floor and basement after the boss |
| `MAP2_S00` | Old Silent Hill |
| `MAP2_S01` | Church |
| `MAP2_S02` | Central Silent Hill |
| `MAP2_S03` | Unused |
| `MAP2_S04` | Police station in Central Silent Hill |
| `MAP3_S00` | Hospital: start, up to meeting Kaufmann |
| `MAP3_S01` | Hospital: first floor and basement after Kaufmann |
| `MAP3_S02` | Hospital: Green Lion Antique Shop cutscene |
| `MAP3_S03` | Hospital (Otherworld): third and second floors |
| `MAP3_S04` | Hospital (Otherworld): first floor |
| `MAP3_S05` | Hospital (Otherworld): basement |
| `MAP3_S06` | Hospital: first floor after the Otherworld section |
| `MAP4_S00` | Unused |
| `MAP4_S01` | Green Lion Antique Shop (normal and Otherworld) |
| `MAP4_S02` | Central Silent Hill (Otherworld) |
| `MAP4_S03` | Mall and boss fight |
| `MAP4_S04` | Hospital first floor: Lisa cutscenes |
| `MAP4_S05` | Central Silent Hill (Otherworld): Floatstinger boss |
| `MAP4_S06` | Unused |
| `MAP5_S00` | Sewers, lower and upper levels |
| `MAP5_S01` | Resort Area |
| `MAP5_S02` | Annie's Bar and Indian Runner |
| `MAP5_S03` | Norman's Motel |
| `MAP6_S00` | Resort Area (Otherworld) |
| `MAP6_S01` | Boat at Lakeside Pier |
| `MAP6_S02` | Lakeside Pier and lighthouse |
| `MAP6_S03` | Sewer to Lakeside Amusement Park |
| `MAP6_S04` | Cybil boss fight; Dahlia takes Alessa |
| `MAP6_S05` | Unused |
| `MAP7_S00` | Nowhere: hospital first floor, Lisa cutscene |
| `MAP7_S01` | Nowhere (other parts) |
| `MAP7_S02` | Nowhere: Alessa vs. Dahlia cutscene |
| `MAP7_S03` | Final boss |

For the unused overlays, see [upstream issue #335](https://github.com/shdecompilations/silent-hill-decomp/issues/335#issuecomment-3393749791).
</details>

## Docs

- [Organization](docs/Organization.md): game files and repo layout
- [Coding Conventions](docs/Coding%20Conventions.md)
- [Analysis Guide](docs/Analysis%20Guide.md): Ghidra, IDA and decomp.me setup
- [File Formats](docs/File%20Formats.md)
- [Game Information](docs/Game%20Information.md): SDK, libraries and known releases

Build setup is covered on the upstream [wiki](https://github.com/shdecompilations/silent-hill-decomp/wiki). For questions about the original project, use `#silent-hill` on the [PS1/PS2 Decompilation Discord](https://discord.gg/VwCPdfbxgm).
