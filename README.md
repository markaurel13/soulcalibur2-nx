<p align="center">
  <img src="switch/assets/icon.jpg" alt="Soulcalibur II NX" width="280" />
</p>

# Soulcalibur II NX for Nintendo Switch

[![Platform: Nintendo Switch](https://img.shields.io/badge/Platform-Nintendo%20Switch-E60012.svg?logo=nintendoswitch&logoColor=white)](https://github.com/markaurel13/soulcalibur2-nx)
[![Performance: 60 FPS](https://img.shields.io/badge/Performance-60%20FPS-brightgreen.svg)]()
[![Resolution: 1080p Docked / 720p Handheld](https://img.shields.io/badge/Resolution-1080p%20%7C%20720p-blue.svg)]()
[![Author: markaurel13](https://img.shields.io/badge/Author-markaurel13-orange.svg)](https://github.com/markaurel13)

A high-performance native port of **Soulcalibur II** (Xbox NTSC-U) for the Nintendo Switch, built via static recompilation.

Unlike traditional emulation, the original game runs natively on Nintendo Switch hardware with zero emulation overhead—delivering full-speed **60 FPS** gameplay, sharp **1080p** visuals on TV, and **all console-exclusive guest fighters (Spawn, Link, and Heihachi)** playable together.

---

## ✨ Features

- **All Console-Exclusive Fighters Included:** Play with **Spawn** (Xbox), **Link** (GameCube), and **Heihachi** (PS2) all in one definitive roster.
- **Full-Speed 60 FPS on Stock Clocks:** Plays smoothly at 60 FPS in both Handheld and Docked modes with zero overclocking required.
- **16:9 Native Widescreen:** True panoramic 16:9 gameplay with an expanded field of view and ZERO character stretching—filling both the Switch screen and your TV edge-to-edge on Full HD
- **Faithful Xbox Visuals:** Dynamic lighting, real-time shadows, stage effects, and character models rendered natively on Switch hardware at Full HD.
- **Original Audio & Full FMV Cutscenes:** Crisp stereo sound, character voices, background music, and full video cinematics (`movie.afs` & `bgm.afs`).
- **2-Player Local Battles:** Challenge friends in VS mode using Handheld, Gamepad, or Joy-Cons.
- **All Classic Modes Included:** Arcade, VS Battle, Team Battle, Time Attack, Survival, Practice, and Weapon Master, with automatic saving to your microSD card.

---

## 📁 Installation (Nintendo Switch)

1. **Prepare Game Assets (Web Installer):**
   - Open the **[Soulcalibur II NX Web Installer](https://markaurel13.github.io/soulcalibur2-nx/)** in your browser (runs 100% locally—no Python, Java, or extra tools required).
   - Drop your own legal **USA (NTSC-U)** copies of all three discs:
     - **Xbox** (`.iso` / `.xiso`) — Base game + Spawn
     - **GameCube** (`.iso` / `.gcm` / `.ciso` / `.nkit.iso`) — Extracts Link
     - **PlayStation 2** (`.iso`) — Extracts Heihachi
   - *(Optional)* Drop a compatible Dolphin **HD Textures Pack** archive (`.7z` / `.zip` / `.rar`) — unused textures are skipped and combined textures are converted automatically into `switch/soulcalibur2/textures/`.
   - Click **Build Switch Package** to generate `soulcalibur2_nx_package.zip` and extract it to the root of your Nintendo Switch microSD card (`sdmc:/`). *(In browsers without direct disk saving like Brave/Firefox, if HD Textures push the package over 1.85 GB, it will download `soulcalibur2_nx_package.zip` + `soulcalibur2_nx_textures.zip` separately — extract both to `sdmc:/`.)*

2. **Install the Latest `soulcalibur2.nro`:**
   - Go to the **[Releases](https://github.com/markaurel13/soulcalibur2-nx/releases)** page and download the latest `soulcalibur2.nro`.
   - Copy `soulcalibur2.nro` into `sdmc:/switch/soulcalibur2/`.

Your microSD card directory structure should look like this:

```text
sdmc:/
└── switch/
    └── soulcalibur2/
        ├── soulcalibur2.nro        # Latest Nintendo Switch executable (from Releases)
        ├── Default.xbe             # Original Xbox executable
        ├── guests.olk              # Combined game assets + Link & Heihachi (from Web Installer)
        ├── movie.afs               # FMV cutscenes
        ├── bgm.afs                 # Background music
        └── textures/               # Optional converted HD textures (from Web Installer)
```

> [!TIP]
> Launch **Soulcalibur II NX** from the Homebrew Menu using **title override** (hold `R` while launching a game) for full RAM access.

---

## 🎮 Controls

All Xbox buttons are mapped positionally to the Nintendo Switch controller:

| Switch Input | Xbox Equivalent | In-Game Action (Default) |
| :--- | :--- | :--- |
| **Left Stick / D-Pad** | Left Stick / D-Pad | Movement (8-Way Run) / Menu Navigation |
| **`B`** | `A` | Guard (`G`) / Confirm |
| **`A`** | `B` | Kick (`K`) / Cancel |
| **`Y`** | `X` | Horizontal Attack (`A`) |
| **`X`** | `Y` | Vertical Attack (`B`) |
| **`L`** | `Black` | `A + B + K` (Soul Charge) |
| **`R`** | `White` | `B + K` (Special / Taunt) |
| **`ZL`** | `LT` (Left Trigger) | `A + G` (Throw) |
| **`ZR`** | `RT` (Right Trigger) | `B + G` (Throw) |
| **`Plus (+)`** | `Start` | Start / Pause Menu |
| **`Minus (-)`** | `Back` | Back / Practice Options (Tap) · **Hold (1s) to open Mod Menu** |
| **`L3` / `R3`** *(Stick Clicks)* | `L-Thumb` / `R-Thumb` | Custom / Macro Bindings |

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
