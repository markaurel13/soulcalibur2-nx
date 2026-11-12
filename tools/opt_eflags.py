#!/usr/bin/env python3
import sys
import os
import re

PRESERVE_FUNCS = {
    "sub_00145D98",
    "sub_00145DBC",
    "sub_00145F81",
    "sub_00146500",
    "sub_0014651D",
    "sub_00148EC4",
    "sub_00149055",
}

# Regex matching:
# g_eflags = (g_eflags & ~0x...) | <carry_term> | ... ; /* flags across call boundary */
# Note that <carry_term> is always of form: (something ? 0x1u : 0u) or (something ? 1u : 0u) or (0)
# Specifically, it sets bit 0 (0x1u or 1u).
RE_EFLAGS_LINE = re.compile(
    r"^(\s*)g_eflags\s*=\s*\(g_eflags\s*&\s*~0x[0-9A-Fa-f]+u\)\s*\|\s*(\([^;]+?\?\s*0?x?1u\s*:\s*0u\))\s*\|.+?/\*\s*flags across call boundary\s*\*/"
)

def process_file(path):
    print(f"Processing {path}...")
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    out_lines = []
    current_func = None
    func_pattern = re.compile(r"^void (sub_[0-9A-Fa-f]+)\(void\)")
    replacements = 0

    for line in lines:
        m = func_pattern.match(line)
        if m:
            current_func = m.group(1)
        
        if current_func in PRESERVE_FUNCS:
            out_lines.append(line)
            continue
            
        if "flags across call boundary" in line and "g_eflags =" in line:
            m_eflags = RE_EFLAGS_LINE.match(line)
            if m_eflags:
                indent = m_eflags.group(1)
                carry_term = m_eflags.group(2)
                # Normalize carry_term: e.g. ((_cf) ? 1u : 0u)
                new_line = f"{indent}g_eflags = (g_eflags & ~1u) | {carry_term}; /* flags across call boundary (carry-only opt) */\n"
                out_lines.append(new_line)
                replacements += 1
            else:
                out_lines.append(line)
        else:
            out_lines.append(line)

    with open(path, "w", encoding="utf-8") as f:
        f.writelines(out_lines)
        
    print(f"Replaced {replacements} eflags lines in {path}")

if __name__ == "__main__":
    for arg in sys.argv[1:]:
        process_file(arg)
