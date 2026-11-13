#!/usr/bin/env python3
"""
tools/opt_direct_calls.py
Reemplaza RECOMP_ABI_CALL(va, sub_X) por sub_X() en recomp_0000..0007.c
para todas las direcciones que NO tienen hook manual en runtime/ o switch/.
Esto elimina ~9,750 llamadas/frame a recomp_lookup_manual y permite inlining GCC.
"""

import os
import re
import sys


MANUAL_SOURCES = [
    "build/src/recomp-runtime/program_manual.c",
    "build/src/recomp-runtime/sc2_hooks.c",
    "build/src/recomp-runtime/d3d8_resource_adapter.c",
    "build/src/recomp-runtime/d3d8_adapter.c",
    "build/src/recomp-runtime/d3d8_state_adapter.c",
    "build/src/recomp-runtime/d3d8_draw_adapter.c",
    "build/src/recomp-runtime/dsound_api_adapter.c",
    "build/src/recomp-runtime/dsound_effects_adapter.c",
    "build/src/recomp-runtime/dsound_buffer_adapter.c",
    "build/src/recomp-runtime/input_adapter.c",
    "build/src/recomp-runtime/free_camera_adapter.c",
]

HEX_RE = re.compile(r"0x[0-9a-fA-F]{4,8}")
ABI_CALL_RE = re.compile(
    r"RECOMP_ABI_CALL\(\s*(0x[0-9a-fA-F]+)[uU]?\s*,\s*(sub_[0-9a-fA-F]+)\s*\)"
)


def collect_manual_vas(repo_root: str) -> set[int]:
    vas: set[int] = set()
    for rel in MANUAL_SOURCES:
        path = os.path.join(repo_root, rel)
        if not os.path.exists(path):
            print(f"[opt_direct_calls] WARNING: missing {path}", file=sys.stderr)
            continue
        with open(path, "r", encoding="utf-8", errors="ignore") as f:
            for m in HEX_RE.finditer(f.read()):
                vas.add(int(m.group(0), 16))
    return vas


def process_file(path: str, manual_vas: set[int]) -> tuple[int, int]:
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()

    replaced = 0
    kept = 0

    def _sub(m: re.Match) -> str:
        nonlocal replaced, kept
        va = int(m.group(1), 16)
        fn = m.group(2)
        if va in manual_vas:
            kept += 1
            return m.group(0)
        replaced += 1
        return f"{fn}()"

    new_text = ABI_CALL_RE.sub(_sub, text)
    if new_text != text:
        with open(path, "w", encoding="utf-8") as f:
            f.write(new_text)
    return replaced, kept


def main() -> int:
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <file1.c> [file2.c ...]", file=sys.stderr)
        return 1

    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    manual_vas = collect_manual_vas(repo_root)
    print(f"[opt_direct_calls] Collected {len(manual_vas)} manual/protected hex constants")

    total_replaced = 0
    total_kept = 0
    for path in sys.argv[1:]:
        rep, kept = process_file(path, manual_vas)
        total_replaced += rep
        total_kept += kept
        print(f"[opt_direct_calls] {os.path.basename(path)}: {rep} direct, {kept} manual kept")

    print(f"[opt_direct_calls] Total: {total_replaced} direct calls, {total_kept} manual calls kept")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
