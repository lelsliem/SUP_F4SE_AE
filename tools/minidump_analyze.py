#!/usr/bin/env python3
"""Tiny Windows minidump analyzer — exception code, faulting module, rough stack.

Parses a MINIDUMP_HEADER + stream directory, the exception stream, the module
list, and the faulting thread's stack memory. No external dependencies.
"""
import struct, sys

try:
    sys.stdout.reconfigure(errors='replace')
except AttributeError:
    pass

def read_cstr(buf, off):
    n = struct.unpack_from('<I', buf, off)[0]
    raw = buf[off+4:off+4+n*2]
    return raw.decode('utf-16-le', 'replace').split('\x00', 1)[0]

def main(path):
    with open(path, 'rb') as f:
        buf = f.read()

    if buf[:4] != b'MDMP':
        print('Not a minidump (bad signature).')
        return 1

    nstreams = struct.unpack_from('<I', buf, 8)[0]
    dir_rva = struct.unpack_from('<I', buf, 12)[0]
    streams = {}
    for i in range(nstreams):
        stype, dsize, rva = struct.unpack_from('<III', buf, dir_rva + i*12)
        streams[stype] = (rva, dsize)

    print(f'== minidump {path} — {nstreams} streams ==')

    # ---- modules (type 4) ----
    modules = []
    if 4 in streams:
        rva, dsize = streams[4]
        count = struct.unpack_from('<I', buf, rva)[0]
        p = rva + 4
        for _ in range(count):
            # MINIDUMP_MODULE64 (what MiniDumpWriteDump emits for x64): BaseOfImage(8)
            # SizeOfImage(4) CheckSum(4) TimeDateStamp(4) ModuleNameRVA(4)
            # VersionInfo(52) CvRecord(8) MiscRecord(8) Reserved(8+8) = 108 bytes
            base, size, _cs, _ts, name_rva = struct.unpack_from('<QIIII', buf, p)
            name = read_cstr(buf, name_rva)
            modules.append((base, size, name))
            p += 108
        print(f'\n== {len(modules)} modules loaded ==')
        for base, size, name in modules:
            if any(k in name.lower() for k in ('sup', 'fallout4', 'nistron', 'f4se', 'dll')):
                print(f'  {base:016x} +{size:06x}  {name}')

    # ---- exception stream (type 6) ----
    if 6 in streams:
        rva, dsize = streams[6]
        tid = struct.unpack_from('<I', buf, rva)[0]
        code, _flags, _rec, addr, nparams = struct.unpack_from('<IIQQI', buf, rva+8)
        print(f'\n== EXCEPTION ==')
        print(f'  thread id    : {tid}')
        print(f'  code         : 0x{code:08X}')
        print(f'  address      : 0x{addr:016X}')
        # map faulting address to module
        for base, size, name in modules:
            if base <= addr < base + size:
                print(f'  in module    : {name}  (base 0x{base:x})')
                print(f'  module RVA   : 0x{addr-base:06X}')
                break
        else:
            print('  in module    : <no module contains address>')

        # ---- faulting thread stack (thread list type 3) ----
        if 3 in streams:
            rva, dsize = streams[3]
            tcount = struct.unpack_from('<I', buf, rva)[0]
            p = rva + 4
            tinfo = None
            for _ in range(tcount):
                # MINIDUMP_THREAD: tid(4) sus(4) prioClass(4) prio(4) teb(8)
                #   stack: StartOfMemoryRange(8) DataSize(4) Rva(4)
                #   context: DataSize(4) Rva(4)  = 48 bytes
                _tid, _sus, _prio, _pri, _teb, _s_start, s_size, s_rva, _ctx_size, _ctx_rva = \
                    struct.unpack_from('<IIIIQQIIII', buf, p)
                if _tid == tid:
                    tinfo = (s_rva, s_size)
                    break
                p += 48
            if tinfo:
                s_rva, s_size = tinfo
                stack = buf[s_rva:s_rva+s_size]
                print(f'\n== faulting thread stack ({s_size} bytes) — return addresses in modules ==')
                # scan for qwords that fall inside any module (coarse but effective)
                hits = []
                for off in range(0, len(stack)-7, 8):
                    val = struct.unpack_from('<Q', stack, off)[0]
                    for base, size, name in modules:
                        if base <= val < base + size:
                            hits.append((off, val, base, name))
                            break
                # report hits in stack order (keep duplicates — they mark frames)
                for off, val, base, name in hits:
                    print(f'  +0x{off:04x}  0x{val:016x}  {name}+0x{val-base:06X}')
    else:
        print('\nNo exception stream in dump.')

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1]))
