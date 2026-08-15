"""Compare the crowd mod's ESP structure (GRUP headers + a PUSH record)
against our generated ESP byte-for-byte at the structural level."""
import struct, sys

def walk(path, label):
    data = open(path, "rb").read()
    print(f"=== {label}: {len(data)} bytes ===")
    off = 0
    # TES4
    t, size, flags, gap, fid = struct.unpack_from("<4sIHHI", data, off)
    print(f"  @0 {t.decode()} size={size} flags=0x{flags:04x} formid=0x{fid:08x}")
    off += 24 + size
    # GRUP
    gt, gsize = struct.unpack_from("<4sI", data, off)
    label4 = data[off+8:off+12]
    print(f"  @{off} GRUP {label4.decode()} size={gsize} (data to @{off+gsize})")
    off += 24
    n = 0
    while off < len(data):
        rt, rsize, rflags, rgap, rfid = struct.unpack_from("<4sIHHI", data, off)
        # read EDID
        body = data[off+24:off+24+rsize]
        edid = b""
        p = 0
        while p + 6 <= len(body):
            st = body[p:p+4]
            sl = struct.unpack_from("<H", body, p+4)[0]
            if st == b"EDID":
                edid = body[p+6:p+6+sl].rstrip(b"\x00")
                break
            p += 6 + sl
        print(f"  @{off} {rt.decode()} size={rsize} flags=0x{rflags:04x} fid=0x{rfid:08x} {edid.decode(errors='replace')}")
        off += 24 + rsize
        n += 1
        if n > 12:
            print("  ... (truncated)")
            break

if __name__ == "__main__":
    crowd = r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp"
    ours = r"C:/Fallout4Adaption/data/TheLivingCommonwealthAnims.esp"
    walk(crowd, "crowd mod")
    print()
    walk(ours, "ours")
