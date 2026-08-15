import struct, sys

path = r"B:/Modding/MO2/Fallout 4/mods/Get Out Of My Face (Push Away Companions and NPCs)/PushAwayCompanions.esp"
data = open(path, "rb").read()
print("file size:", len(data))

def subrecords(buf, off, end):
    subs = []
    while off + 6 <= end:
        tag = buf[off:off+4].decode("latin1")
        size = struct.unpack("<H", buf[off+4:off+6])[0]
        body = buf[off+6:off+6+size]
        subs.append((tag, size, body))
        off += 6 + size
    return subs

off = 0
# TES4
assert data[off:off+4] == b"TES4"
hdr_size = struct.unpack("<I", data[off+4:off+8])[0]
off += 8 + hdr_size

while off + 24 <= len(data):
    typ = data[off:off+4].decode("latin1")
    size = struct.unpack("<I", data[off+4:off+8])[0]
    if typ == b"GRUP":
        label = struct.unpack("<I", data[off+16:off+20])[0]
        print(f"\nGRUP {label:08X} size={size} at {off}")
        if label == 0x00000010:  # IDLE group? need to check
            pass
        off += 24 + (size - 24)
        continue
    flags = struct.unpack("<H", data[off+12:off+14])[0]
    formid = struct.unpack("<I", data[off+16:off+20])[0]
    body_start = off + 24
    body_end = body_start + size - 24
    subs = subrecords(data, body_start, body_end)
    tags = " ".join(s[0] for s in subs)
    edid = ""
    for s in subs:
        if s[0] == "EDID":
            edid = s[2].rstrip(b"\x00").decode("latin1", "replace")
            break
    if "IDLE" in typ or "IDLE" in tags:
        print(f"  IDLE {formid:08X} {edid!r} [{tags}]")
    off += 24 + (size - 24)
