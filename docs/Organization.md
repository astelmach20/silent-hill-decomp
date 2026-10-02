# Organization

## Main Files
The game is made up of 3 files: the executable, and two containers, `SILENT.` and `HILL.`. `HILL.` holds only audio and FMVs. `SILENT.` holds everything else, including the code overlays.

### Code Files Explanation
- `SLUS_007.07` (`main`): the executable. It shows the content warning screen and loads `B_KONAMI.BIN` and `BODYPROG.BIN`.
- `1ST/B_KONAMI.BIN`: loads shared assets (Harry's model and animations, fonts) and shows the Konami/KCET splash screens.
- `1ST/BODYPROG.BIN`: the engine and most of the game logic.

The other overlays are in `VIN/`:
- `STREAM.BIN`: video playback.
- `OPTION.BIN`: options menu.
- `SAVELOAD.BIN`: save/load menu UI. Memory card and save data handling live in the engine.
- `STF_ROLL.BIN`: credits.
- `MAP#_S##.BIN`: map scripts and enemy AI, numbered roughly in playthrough order (`MAP0_S00` runs from the start to the cafe, then `MAP0_S01` loads).
  - `S##` is probably short for "stage". The second digit is the stage index. The first digit is always 0, except in the European release, where it encodes the language (0-5, skipping 1).

## Main Folders Structures
How the repo is laid out.

### Source Folder Structure
Decompiled C source. Code shared between maps is in `include/maps/shared/` (see below).
```
src/
├── maps/
│   └── map#_s##/
│       └── The map (level) scripts overlay (MAP#_S##.BIN).
├── screens/
│   ├── b_konami/
│   │   └── The boot screen overlay (B_KONAMI.BIN).
│   ├── credits/
│   │   └── The credits screen overlay (STF_ROLL.BIN).
│   ├── options/
│   │   └── The options screen overlay (OPTION.BIN).
│   ├── saveload/
│   │   └── The save/load screen overlay (SAVELOAD.BIN).
│   └── stream/
│       └── The video stream overlay (STREAM.BIN).
├── bodyprog/
│   └── The main game logic overlay (BODYPROG.BIN).
└── main/
    └── The game's main executable logic (SLUS_007.07).
```

### Include Folder Structure
Headers, plus code shared between map overlays.
```
include/
│   └── General game or tooling specific header files and macro defines.
├── maps/
│   └── map#/
│       └── Exclusive per-map overlay data and functions header files.
│   └── shared/
│       └── Shared decompiled functions among overlays.
├── bodyprog/
│   └── Headers for game's main game logic overlay (BODYPROG.BIN).
├── psyq/
│   └── Headers for PSY-Q SDK.
├── screens/
│   ├── b_konami/
│   │   └── Headers for the boot screen overlay (B_KONAMI.BIN).
│   ├── credits/
│   │   └── Headers for the credits screen overlay (STF_ROLL.BIN).
│   ├── options/
│   │   └── Headers for the options screen overlay (OPTION.BIN).
│   ├── saveload/
│   │   └── Headers for the save/load screen overlay (SAVELOAD.BIN).
│   └── stream/
│       └── Headers for the video stream overlay (STREAM.BIN).
└── main/
    └── Headers for game's main executable code (SLUS_007.07).
```
