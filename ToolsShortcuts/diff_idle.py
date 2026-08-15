"""Locate the crowd mod's IDLE GRUP + the PairedFrontPushKick record and
dump its subrecords, then dump ours, for a field-by-field diff."""
import struct

def find_grup(data, label4):
    off = 0
    t, size, _, _, _ = struct.unpack_from("<4sIHHI", data, off)
    off += 24 + size
    while off < len(data):
        gt = data[off:off+4]
        if gt != b"GRUP":
            # not top level; skip record
            rt, rsize = struct.unpack_from("<4sI", data, off)
            off += 24 + rsize
            continue
        gsize = struct.unpack_from("<I", data, off+4)[0]
        lab = data[off+8:off+12]
        gtype = struct.unpack_from("<I", data, off+12)[0]
        if lab == label4 and gtype == 0:
            return off, gsize
        off += gsize

def dump_records(data, grup_off, grup_size, want_edid=None):
    end = grup_off + grup_size
    off = grup_off + 24
    out = []
    while off < end:
        rt, rsize = struct.unpack_from("<4sI", data, off)
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
        if want_edid is None or edid == want_edid or want_edid in edid:
            subs = []
            p = 0
            while p + 6 <= len(body):
                st = body[p:p+4]
                sl = struct.unpack_from("<H", body, p+4)[0]
                subs.append((st.decode(errors="replace"), sl))
                p += 6 + sl
            out.append((edid, subs, body))
        off += 24 + rsize
    return out

crowd = open(r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp", "rb").read()
ours = open(r"C:/Fallout4Adaption/data/TheLivingCommonwealthAnims.esp", "rb").read()

goff, gsize = find_grup(crowd, b"IDLE")
print(f"crowd IDLE grup @{goff} size={gsize}")
for edid, subs, body in dump_records(crowd, goff, gsize, want_edid=b"PUSH_PairedFrontPushKick"):
    print(f"  {edid}:")
    for st, sl in subs:
        val = body
        # find the payload for this tag
        p = 0
        while p + 6 <= len(body):
            if body[p:p+4] == st.encode() and struct.unpack_from("<H", body, p+4)[0] == sl:
                print(f"    {st} ({sl}) = {body[p+6:p+6+sl]!r}")
                break
            p += 6 + sl
        else:
            print(f"    {st} ({sl})")
    print()

ooff, osize = find_grup(ours, b"IDLE")
print(f"ours IDLE grup @{ooff} size={osize}")
for edid, subs, body in dump_records(ours, ooff, osize):
    print(f"  {edid}:")
    for st, sl in subs:
        p = 0
        while p + 6 <= len(body):
            if body[p:p+4] == st.encode() and struct.unpack_from("<H", body, p+4)[0] == sl:
                print(f"    {st} ({sl}) = {body[p+6:p+6+sl]!r}")
                break
            p += 6 + sl
