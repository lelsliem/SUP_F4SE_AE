#!/usr/bin/env python3
"""Attribute each SUP crash dump's exception address to a module and RVA."""
import glob
import os
import struct
import time


def read_cstr(buf, off):
    if off <= 0 or off + 4 > len(buf):
        return ""
    n = struct.unpack_from("<I", buf, off)[0]
    raw = buf[off + 4 : off + 4 + n * 2]
    return raw.decode("utf-16-le", "replace").split("\x00", 1)[0]


def parse(path):
    d = open(path, "rb").read()
    if d[:4] != b"MDMP":
        return None
    nstreams = struct.unpack_from("<I", d, 8)[0]
    dir_rva = struct.unpack_from("<I", d, 12)[0]
    streams = {}
    for i in range(nstreams):
        stype, dsize, rva = struct.unpack_from("<III", d, dir_rva + i * 12)
        streams[stype] = (rva, dsize)
    excaddr = None
    mods = []
    if 4 in streams:
        rva, _ = streams[4]
        cnt = struct.unpack_from("<I", d, rva)[0]
        p = rva + 4
        for _ in range(cnt):
            base, size, _cs, _ts, name_rva = struct.unpack_from("<QIIII", d, p)
            name = read_cstr(d, name_rva)
            mods.append((base, size, name))
            p += 108
    if 6 in streams:
        excaddr = struct.unpack_from("<Q", d, streams[6][0] + 24)[0]
    for base, size, name in sorted(mods, reverse=True):
        if excaddr and base <= excaddr < base + size:
            return (name.split("\\")[-1], hex(excaddr - base))
    return (None, hex(excaddr) if excaddr else None)


def main():
    rows = []
    for p in sorted(glob.glob("C:/Users/mrlma/Documents/My Games/Fallout4/F4SE/SUP_Crash_*.dmp")):
        mod, rva = parse(p)
        ts = time.strftime("%H:%M", time.localtime(os.path.getmtime(p)))
        rows.append((ts, os.path.basename(p), mod, rva))
    for ts, name, mod, rva in rows:
        print(f"{name}  {ts}  -> {mod} {rva}")


if __name__ == "__main__":
    main()
