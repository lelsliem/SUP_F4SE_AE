import struct

path = r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp"
data = open(path, "rb").read()

def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def u16(b, o): return struct.unpack_from("<H", b, o)[0]

def subs(body):
    out = []
    o = 0
    while o + 6 <= len(body):
        tag = body[o:o+4].decode("latin1")
        ss = u16(body, o+4)
        out.append((tag, body[o+6:o+6+ss]))
        o += 6 + ss
    return out

# walk top-level GRUPs, find IDLE group, list all records
off = 24
while off + 24 <= len(data):
    rtype = data[off:off+4]
    usize = u32(data, off+4)
    if rtype == b"GRUP":
        glabel = data[off+8:off+12]
        if glabel == b"IDLE":
            roff = off + 24
            rend = off + 24 + usize
            print(f"IDLE group at {off} size {usize}")
            while roff + 24 <= rend:
                rt = data[roff:roff+4]
                rs = u32(data, roff+4)
                if rt == b"IDLE":
                    body = data[roff+24:roff+24+rs]
                    s = subs(body)
                    edid = ""
                    enam = ""
                    gnam = ""
                    anam = ""
                    for tag, val in s:
                        if tag == "EDID": edid = val.rstrip(b"\x00").decode("latin1","replace")
                        if tag == "ENAM": enam = val.rstrip(b"\x00").decode("latin1","replace")
                        if tag == "GNAM": gnam = val.rstrip(b"\x00").decode("latin1","replace")
                        if tag == "ANAM": anam = val.hex()
                    print(f"  {rt.decode():4s} {roff+16:8d} {u32(data, roff+12):08X} {edid!r} ENAM={enam!r} GNAM={gnam!r} ANAM={anam}")
                roff += 24 + rs
    off += 24 + usize
    if off >= len(data):
        break
