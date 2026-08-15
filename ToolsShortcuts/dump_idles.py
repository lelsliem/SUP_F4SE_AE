"""Dump the two vanilla paired-push IDLE records (0x47FC3, 0x6571F) from
Fallout4.esm: EDID, DNAM, ENAM, ANAM, and the CTDA conditions — the
ground truth for why PlayIdle refuses them outside combat, and what the
crowd mod's unconditional copies stripped."""
import struct, sys

ESM = r"B:/SteamLibrary/steamapps/common/Fallout 4/Data/Fallout4.esm"
WANT = {0x00047FC3, 0x0006571F, 0x00047FB2, 0x00065720, 0x00047FBA, 0x00065723}

def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def u16(b, o): return struct.unpack_from("<H", b, o)[0]

def main():
    with open(ESM, "rb") as f:
        data = f.read()
    print(f"esm size {len(data)}")
    found = {}

    # Walk top-level records (TES4 + GRUPs). The IDLE records live inside
    # a GRUP 'IDLE' inside the top-level (record groups are nested by type
    # at the top level for a master).
    def walk(off, end, depth=0):
        while off + 24 <= end:
            rtype = data[off:off+4]
            usize = u32(data, off+4)
            hdr = 24
            if rtype == b"GRUP":
                glabel = data[off+8:off+12]
                if glabel == b"IDLE":
                    rec_off = off + hdr
                    while rec_off + 24 <= off + 24 + usize:
                        rt = data[rec_off:rec_off+4]
                        rs = u32(data, rec_off+4)
                        rid = u32(data, rec_off+12)
                        if rt == b"IDLE" and (rid & 0xFFFFFF) in WANT:
                            body = data[rec_off+hdr : rec_off+hdr+rs]
                            found.setdefault(rid & 0xFFFFFF, body)
                        rec_off += hdr + rs
                off += hdr + usize
            else:
                off += hdr + usize
        if depth > 8:
            return

    # Top level: TES4 then GRUPs. Find top-level GRUPs by scanning from 24.
    # Top-level groups are 4-byte type + 4-byte size + label (4) + ts (4).
    off = 24
    toplevel_groups = []
    while off + 24 <= len(data):
        rtype = data[off:off+4]
        usize = u32(data, off+4)
        if rtype == b"GRUP":
            toplevel_groups.append((off, usize))
        off += 24 + usize
        if off > len(data):
            break
    for goff, gsize in toplevel_groups:
        walk(goff, goff + 24 + gsize, 1)

    for rid in sorted(found):
        body = found[rid]
        print(f"\n=== IDLE 0x{rid:08X} ({len(body)} bytes) ===")
        o = 0
        while o + 6 <= len(body):
            st = body[o:o+4]
            ss = u16(body, o+4)
            sub = body[o+6 : o+6+ss]
            tag = st.decode("latin1")
            if tag == "EDID":
                print(f"  EDID {sub.rstrip(b'\x00').decode('latin1')}")
            elif tag == "DNAM":
                print(f"  DNAM {sub.rstrip(b'\x00').decode('latin1')}")
            elif tag == "ENAM":
                print(f"  ENAM {sub.rstrip(b'\x00').decode('latin1')}")
            elif tag == "ANAM":
                print(f"  ANAM {sub.hex()}")
            elif tag == "CTDA":
                print(f"  CTDA ({len(sub)}b) {sub.hex()}")
            elif tag == "SCHR":
                print(f"  SCHR ({len(sub)}b)")
            else:
                print(f"  {tag} ({len(sub)}b) {sub.hex()[:40]}")
            o += 6 + ss

if __name__ == "__main__":
    main()
