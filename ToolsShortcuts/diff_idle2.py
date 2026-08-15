"""Byte-compare the crowd's push-kick record payloads against ours."""
import struct

def subrecords(body):
    subs = []
    p = 0
    while p + 6 <= len(body):
        st = body[p:p+4]
        sl = struct.unpack_from("<H", body, p+4)[0]
        subs.append((st, body[p+6:p+6+sl]))
        p += 6 + sl
    return subs

def find_record(data, edid_want):
    off = 0
    t, size, _, _, _ = struct.unpack_from("<4sIHHI", data, off)
    off += 24 + size
    while off < len(data):
        if data[off:off+4] != b"GRUP":
            rt, rsize = struct.unpack_from("<4sI", data, off)
            body = data[off+24:off+24+rsize]
            edid = b""
            for st, pl in subrecords(body):
                if st == b"EDID":
                    edid = pl.rstrip(b"\x00")
            if edid_want in edid:
                return body
            off += 24 + rsize
        else:
            gsize = struct.unpack_from("<I", data, off+4)[0]
            off += gsize
    return None

crowd = open(r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp", "rb").read()
ours = open(r"C:/Fallout4Adaption/data/TheLivingCommonwealthAnims.esp", "rb").read()

cb = find_record(crowd, b"PUSH_PairedFrontPushKick_AttackerLead")
ob = find_record(ours, b"LCW_PairedFrontPushKick")

cs = {st: pl for st, pl in subrecords(cb)}
os_ = {st: pl for st, pl in subrecords(ob)}

for tag in [b"DNAM", b"ENAM", b"ANAM", b"DATA", b"GNAM"]:
    c = cs.get(tag)
    o = os_.get(tag)
    same = "SAME" if c == o else "DIFF"
    print(f"{tag.decode():5} {same}  crowd={c!r}")
    if c != o:
        print(f"           ours ={o!r}")
