#!/usr/bin/env python3
"""Print the exception context (registers) from a SUP minidump."""
import struct
import sys


def main():
    for path in sys.argv[1:]:
        print("====", path.replace("\\", "/").split("/")[-1])
        d = open(path, "rb").read()
        sig, ver, nstreams, modtime, modlist, ds, dt = struct.unpack_from("<6IQ", d, 0)
        print("streams:", nstreams)
        for i in range(nstreams):
            stype, ssize, saddr = struct.unpack_from("<3I", d, 32 + i * 12)
            if stype != 6:
                continue
            tid, align = struct.unpack_from("<2I", d, saddr)
            code = struct.unpack_from("<I", d, saddr + 8)[0]
            npar = struct.unpack_from("<I", d, saddr + 12)[0]
            p = list(struct.unpack_from("<7Q", d, saddr + 16))
            # MINIDUMP_EXCEPTION: code, flags, record, address, nparams, params
            flags = struct.unpack_from("<I", d, saddr + 12)[0]
            excrec = struct.unpack_from("<Q", d, saddr + 16)[0]
            excaddr = struct.unpack_from("<Q", d, saddr + 24)[0]
            npar = struct.unpack_from("<I", d, saddr + 32)[0]
            p = list(struct.unpack_from("<7Q", d, saddr + 40))
            # ThreadContext descriptor sits after MINIDUMP_EXCEPTION (152 bytes):
            # saddr + 8 (header) + 152 = saddr + 160 -> RVA, DataSize
            ctxrva = struct.unpack_from("<I", d, saddr + 160)[0]
            print(f"exceptionRecord={excrec:#x} exceptionAddress={excaddr:#x} flags=0x{flags:X}")
            rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15 = (
                struct.unpack_from("<16Q", d, ctxrva + 0x80)
            )
            rip = struct.unpack_from("<Q", d, ctxrva + 0xF8)[0]
            print(f"thread={tid} code=0x{code:08X} nParams={npar}")
            print(f"RAX={rax:#x} RBX={rbx:#x} RCX={rcx:#x} RDX={rdx:#x}")
            print(f"RSP={rsp:#x} RBP={rbp:#x} RSI={rsi:#x} RDI={rdi:#x}")
            print(
                f"R8={r8:#x} R9={r9:#x} R10={r10:#x} R11={r11:#x} "
                f"R12={r12:#x} R13={r13:#x} R14={r14:#x} R15={r15:#x}"
            )
            print(f"RIP={rip:#x}")
            if code == 0xC0000005 and npar >= 2:
                print(f"AV: read/write={p[0]}, accessed={p[1]:#x}")


if __name__ == "__main__":
    main()
