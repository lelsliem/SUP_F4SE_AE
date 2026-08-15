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

crowd = open(r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp", "rb").read()
ours = open(r"C:/Fallout4Adaption/data/TheLivingCommonwealthAnims.esp", "rb").read()

# crowd IDLE grup @33610 size=23444 (from diff_idle.py) — records start @33634
start, end = 33634, 33610 + 23444
cb = None
off = start
while off < end:
    rt, rsize = struct.unpack_from("<4sI", crowd, off)
    body = crowd[off+24:off+24+rsize]
    edid = b""
    for st, pl in subrecords(body):
        if st == b"EDID":
            edid = pl.rstrip(b"\x00")
    if b"PairedFrontPushKick" in edid:
        cb = body
        break
    off += 24 + rsize

# ours: find the IDLE grup
ooff = 0
t, size, _, _, _ = struct.unpack_from("<4sIHHI", ours, ooff)
ooff += 24 + size
osize = struct.unpack_from("<I", ours, ooff+4)[0]
ob = ours[ooff+24:ooff+24+osize]
# strip the IDLE record header
rt, rsize = struct.unpack_from("<4sI", ob, 0)
ob = ob[24:24+rsize]

cs = {st: pl for st, pl in subrecords(cb)}
os_ = {st: pl for st, pl in subrecords(ob)}

for tag in [b"DNAM", b"ENAM", b"ANAM", b"DATA", b"GNAM"]:
    c = cs.get(tag)
    o = os_.get(tag)
    same = "SAME" if c == o else "DIFF"
    print(f"{tag.decode():5} {same}  crowd={c!r}")
    if c != o:
        print(f"           ours ={o!r}")
