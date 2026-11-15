<p align="center">
  <img src="switch/assets/icon.jpg" alt="Soulcalibur II NX" width="280" />
</p>

# Soulcalibur II NX for Nintendo Switch

[![Platform: Nintendo Switch](https://img.shields.io/badge/Platform-Nintendo%20Switch-E60012.svg?logo=nintendoswitch&logoColor=white)](https://github.com/markaurel13/soulcalibur2-nx)
[![Performance: 60 FPS](https://img.shields.io/badge/Performance-60%20FPS-brightgreen.svg)]()
[![Resolution: 1080p Docked / 720p Handheld](https://img.shields.io/badge/Resolution-1080p%20%7C%20720p-blue.svg)]()
[![Author: markaurel13](https://img.shields.io/badge/Author-markaurel13-orange.svg)](https://github.com/markaurel13)

A high-performance native port of **Soulcalibur II** (Xbox NTSC-U) for the Nintendo Switch, built via static recompilation.

Unlike traditional emulation, the original game runs natively on Nintendo Switch hardware with zero emulation overhead—delivering full-speed **60 FPS** gameplay, sharp **1080p** visuals on TV, and **Spawn** as the exclusive Xbox guest fighter.

---

## ✨ Features

- **Full-Speed 60 FPS on Stock Clocks:** Plays smoothly at 60 FPS in both Handheld and Docked modes with zero overclocking required.
- **16:9 Native Widescreen:** True panoramic 16:9 gameplay with an expanded field of view and ZERO character stretching—filling both the Switch screen and your TV edge-to-edge on Full HD
- **Faithful Xbox Visuals:** Dynamic lighting, real-time shadows, stage effects, and character models rendered natively on Switch hardware at Full HD.
- **Original Audio & Full FMV Cutscenes:** Crisp stereo sound, character voices, background music, and full video cinematics (`movie.afs` & `bgm.afs`).
- **2-Player Local Battles:** Challenge friends in VS mode using Handheld, Gamepad, or Joy-Cons.
- **All Classic Modes Included:** Arcade, VS Battle, Team Battle, Time Attack, Survival, Practice, and Weapon Master, with automatic saving to your microSD card.

---

## 📁 Installation (Nintendo Switch)

1. Download the latest `soulcalibur2.nro` from the [Releases](https://github.com/markaurel13/soulcalibur2-nx/releases) page.
2. Create the `/switch/soulcalibur2/` folder on your Nintendo Switch microSD card (`sdmc:/`) and place `soulcalibur2.nro` inside.
3. Copy your own legal **Soulcalibur II (Xbox NTSC-U)** game files into `sdmc:/switch/soulcalibur2/`:
   - `Default.xbe`
   - `root.olk`
   - `movie.afs`
   - `bgm.afs`

Your microSD card directory structure should look like this:

```text
sdmc:/
└── switch/
    └── soulcalibur2/
        ├── soulcalibur2.nro        # Nintendo Switch native executable (from Releases)
        ├── Default.xbe             # Original Xbox executable
        ├── root.olk                # Game assets archive
        ├── movie.afs               # FMV cutscenes
        ├── bgm.afs                 # Background music
        └── save/                   # Auto-created for save data
```

> [!TIP]
> You can also place `Default.xbe`, `root.olk`, `movie.afs`, and `bgm.afs` inside `sdmc:/switch/soulcalibur2/disc/` if you prefer keeping assets in a subfolder.

---

## 🎮 Controls

Standard Nintendo Switch controller layout (Xbox face buttons mapped positionally: **B** = A, **A** = B, **Y** = X, **X** = Y, **Plus (+)** = Start, **Minus (-)** = Back).

---

## 🛠️ Building from Source

### Prerequisites
- [devkitPro](https://devkitpro.org/) with `devkitA64`, `libnx`, `deko3d`, `uam`, `cmake`, and `ninja`
- Python 3 (for lifting the Xbox executable)

### 1. Extract & Lift the Game Binary
Place your clean Xbox NTSC-U dump files (`Default.xbe`, `root.olk`, `movie.afs`, `bgm.afs`) into `private/disc/` and run:

```bash
python3 tools/build_game.py lift
```

### 2. Build `soulcalibur2.nro`
```bash
cmake -S switch -B build/switch -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Switch.cmake -G Ninja
cmake --build build/switch --target soulcalibur2_nro
```

The compiled executable will be generated at `build/switch/soulcalibur2.nro`.

---

## 🙏 Credits & Upstream

- **Upstream Recompilation Project:** [tmcleroy/soulcalibur2-recomp](https://github.com/tmcleroy/soulcalibur2-recomp)
- **Switch Graphics & SDK:** [fincs/deko3d](https://github.com/fincs/deko3d) & [devkitPro / libnx](https://github.com/switchbrew/libnx)

---

## ⚖️ Legal Notice

This repository contains **no copyrighted game assets, ROMs, XBEs, or binary data**. You must supply your own legally obtained copy of **Soulcalibur II (Xbox NTSC-U)** to run or build this project.
