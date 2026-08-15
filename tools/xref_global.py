#!/usr/bin/env python3
"""Find rip-relative references to a target RVA in Fallout4.exe .text."""
import struct
import sys

from capstone import CS_ARCH_X86, CS_MODE_64, Cs, x86_const

GAME = r"B:\SteamLibrary\steamapps\common\Fallout 4\Fallout4.exe"


def load_sections(path):
    d = open(path, "rb").read()
    e_lfanew = struct.unpack_from("<I", d, 0x3C)[0]
    coff = e_lfanew + 4
    machine, nsects = struct.unpack_from("<HH", d, coff)
    opt = coff + 20
    imgbase = struct.unpack_from("<Q", d, opt + 24)[0]
    optsize = struct.unpack_from("<H", d, coff + 16)[0]
    st = coff + 20 + optsize
    secs = []
    for i in range(nsects):
        name = d[st + i * 40 : st + i * 40 + 8].rstrip(b"\0").decode("latin1")
        vsize, vaddr, rsize, rptr = struct.unpack_from("<IIII", d, st + i * 40 + 8)
        secs.append((name, vaddr, vsize, rptr))
    return d, imgbase, secs


def main():
    target = int(sys.argv[1], 16)  # RVA of the global
    tol = int(sys.argv[2], 16) if len(sys.argv) > 2 else 0
    d, imgbase, secs = load_sections(GAME)
    text = next(s for s in secs if s[0] == ".text")
    name, vaddr, vsize, rptr = text
    code = d[rptr : rptr + vsize]
    md = Cs(CS_ARCH_X86, CS_MODE_64)
    md.detail = True
    lo = imgbase + vaddr
    count = 0
    for ins in md.disasm(code, lo):
        # rip-relative operand? capstone marks it as a memory operand with disp
        for op in ins.operands:
            if op.type == x86_const.X86_OP_MEM and op.mem.base == x86_const.X86_REG_RIP:
                ref = ins.address + ins.size + op.mem.disp
                refrva = ref - imgbase
                if abs(refrva - target) <= tol:
                    print(f"0x{refrva:#x} ({ins.address - imgbase:#x})  {ins.mnemonic} {ins.op_str}")
                    count += 1
    print(f"-- {count} references within +/-0x{tol:X} of 0x{target:X}")


if __name__ == "__main__":
    main()
