#!/usr/bin/env python3
"""
Build TheLivingCommonwealthAnims.esp — three unconditional paired IDLE records:
  0x01000800  TLC_Kick       (cloned from vanilla PairedFrontPushKick)
  0x01000801  TLC_BodySlam   (cloned from vanilla PairedFrontBodySlam)
  0x01000802  TLC_Push       (cloned from vanilla PairedPushFrontFront)

The kick record is byte-identical to the crowd mod's proven recipe.
The body slam and push are the same recipe: clone the vanilla IDLE, strip
any CTDA conditions, point at MeleeBehavior.hkx + dyn_Activation, and keep
the vanilla animation file path (GNAM).

FormIDs use plugin index 0x01 so they resolve as
TheLivingCommonwealthAnims.esp load-order-independent via LookupFormID.
"""

import struct, sys, os

ESM = r"B:\SteamLibrary\steamapps\common\Fallout 4\Data\Fallout4.esm"
OUT = r"C:\Fallout4Adaption\data\TheLivingCommonwealthAnims.esp"

# Vanilla EDIDs to clone (attacker half — paired kick/push/body slam)
# + their _Human halves (victim side, played cross-targeted)
RECORDS = [
    # EDID,                     our_FormID,  vanilla_EDID_to_clone
    ("TLC_Kick",                0x01000800, b"PairedFrontPushKick"),
    ("TLC_Kick_Human",          0x01000803, b"PairedFrontPushKick_Human"),
    ("TLC_BodySlam",            0x01000801, b"PairedFrontBodySlam"),
    ("TLC_BodySlam_Human",      0x01000804, b"PairedFrontBodySlam_Human"),
    ("TLC_Push",                0x01000802, b"PairedPushFrontFront"),
    ("TLC_Push_Human",          0x01000805, b"PairedPushFrontFront_Human"),
]


def read_esm(path):
    with open(path, "rb") as f:
        return f.read()


def find_record_by_edid(data, vanilla_edid):
    """Find an IDLE record by its EDID string in the ESM data."""
    # Strategy: find the EDID string, verify EDID subrecord header before it,
    # then find the IDLE record header before the EDID subrecord.

    edid_terminated = vanilla_edid + b"\x00"
    pos = 0

    while True:
        idx = data.find(edid_terminated, pos)
        if idx == -1:
            return None

        # Check for EDID subrecord header (4-byte tag + 2-byte size) before the string
        edid_header_pos = idx - 6
        if data[edid_header_pos:edid_header_pos + 4] != b"EDID":
            pos = idx + 1
            continue

        edid_size = struct.unpack("<H", data[edid_header_pos + 4:edid_header_pos + 6])[0]
        if edid_size != len(vanilla_edid) + 1:  # includes null terminator
            pos = idx + 1
            continue

        # Walk backwards from the EDID subrecord start to find the IDLE record header.
        # The record header is 16 bytes (FO4 standard): type(4) + dataSize(4) + flags(4) + formid(4)
        # But some records have 20 or 24 byte headers. Try walking back.
        for back in range(16, 32):
            candidate = edid_header_pos - back
            if candidate < 0:
                break
            if data[candidate:candidate + 4] == b"IDLE":
                # Verify: dataSize should be plausible (the rest of the record from header end to end)
                data_size = struct.unpack("<I", data[candidate + 4:candidate + 8])[0]
                formid = struct.unpack("<I", data[candidate + 8:candidate + 12])[0]

                # Sanity check: the EDID subrecord should be right after the header
                # If header is 16 bytes: record body = data_size, EDID at offset 0 in body
                # If header is 20 bytes: EDID at offset 4 in body
                # If header is 24 bytes: EDID at offset 8 in body

                # The EDID subrecord should appear at candidate + header_size
                # and data_size = total - header_size
                expected_body_at_16 = candidate + 16
                expected_body_at_20 = candidate + 20
                expected_body_at_24 = candidate + 24

                if expected_body_at_16 == edid_header_pos:
                    header_size = 16
                elif expected_body_at_20 == edid_header_pos:
                    header_size = 20
                elif expected_body_at_24 == edid_header_pos:
                    header_size = 24
                else:
                    continue

                # Good match — extract the full record
                record_end = candidate + header_size + data_size
                record_bytes = data[candidate:record_end]

                print(f"  Found {vanilla_edid.decode()!r} at {candidate:#x}, "
                      f"header={header_size}, dataSize={data_size}, FormID={formid:#010x}")
                return record_bytes

        pos = idx + 1

    return None


def strip_ctda(record_bytes, header_size):
    """Remove CTDA/CTDX/STCP subrecords from an IDLE record body."""
    body = bytearray(record_bytes[header_size:])
    result = bytearray()
    i = 0
    while i < len(body):
        if i + 6 > len(body):
            result.extend(body[i:])
            break
        tag = body[i:i + 4]
        size = struct.unpack("<H", body[i + 4:i + 6])[0]
        if tag in (b"CTDA", b"CTDX", b"STCP"):
            i += 6 + size
        else:
            result.extend(body[i:i + 6 + size])
            i += 6 + size
    return bytes(result)


def build_record(edid_name, our_formid, vanilla_record, vanilla_header_size):
    """Build a new IDLE record with our EDID and FormID, no conditions."""
    # Extract body, strip conditions
    body = strip_ctda(vanilla_record, vanilla_header_size)

    # Replace EDID subrecord
    if body[:4] == b"EDID":
        old_edid_size = struct.unpack("<H", body[4:6])[0]
        edid_bytes = edid_name.encode("ascii") + b"\x00"
        new_edid_sub = b"EDID" + struct.pack("<H", len(edid_bytes)) + edid_bytes
        body = new_edid_sub + body[6 + old_edid_size:]

    # Build 16-byte FO4 record header (same as our existing kick ESP)
    data_size = len(body)
    header = struct.pack("<4s", b"IDLE")
    header += struct.pack("<H", data_size)
    header += struct.pack("<H", 0x0000)  # flags
    header += struct.pack("<I", our_formid)
    header += struct.pack("<I", 0x00000000)  # version control

    return header + body


def main():
    esm = read_esm(ESM)

    # Build IDLE records
    idle_records = bytearray()
    for edid_name, our_fid, vanilla_edid in RECORDS:
        raw = find_record_by_edid(esm, vanilla_edid)
        if raw is None:
            print(f"WARNING: vanilla EDID {vanilla_edid.decode()!r} not found!")
            continue

        # Determine header size by checking where EDID starts
        # Find EDID in the record
        edid_pos_in_record = raw.find(b"EDID")
        # EDID should be at offset header_size
        # Try 16, 20, 24
        header_size = 16
        for hs in [16, 20, 24]:
            if hs < len(raw) and raw[hs:hs + 4] == b"EDID":
                header_size = hs
                break

        record = build_record(edid_name, our_fid, raw, header_size)
        idle_records.extend(record)
        print(f"  Built {edid_name}: FormID {our_fid:#010x}, {len(record)} bytes")

        # Show GNAM/ENAM for verification
        body = record[16:]  # our output header is always 16 bytes
        bi = 0
        while bi + 6 < len(body):
            tag = body[bi:bi + 4]
            sz = struct.unpack("<H", body[bi + 4:bi + 6])[0]
            if tag in (b"GNAM", b"ENAM"):
                val = body[bi + 6:bi + 6 + sz]
                ascii_val = val.rstrip(b"\x00").decode("ascii", errors="replace")
                print(f"    {tag.decode()}: {ascii_val!r}")
            bi += 6 + sz

    # TES4 header
    tes4_hedr_body = (
        b"HEDR" + struct.pack("<I", 44)
        + struct.pack("<I", 800)  # version
        + struct.pack("<I", 1)  # record count
        + b"\x00" * 36
    )
    master_name = b"Fallout4.esm\x00"
    tes4_masters = b"MAST" + struct.pack("<I", len(master_name)) + master_name
    tes4_body = tes4_hedr_body + tes4_masters
    tes4_header = (
        struct.pack("<4s", b"TES4")
        + struct.pack("<I", len(tes4_body))
        + struct.pack("<I", 0x0001)
        + struct.pack("<I", 0)
        + struct.pack("<I", 0)
    )
    tes4_record = tes4_header + tes4_body

    # GRUP for IDLE records
    grp_body = bytes(idle_records)
    grp_header = (
        struct.pack("<4s", b"GRUP")
        + struct.pack("<I", 16 + len(grp_body))
        + b"IDLE"
        + struct.pack("<I", 0)
    )
    grp = grp_header + grp_body

    # Write ESP
    esp = tes4_record + grp
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "wb") as f:
        f.write(esp)

    print(f"\nWrote {len(esp):,} bytes to {OUT}")
    print(f"  Records: {len(idle_records) // 1} IDLE bytes in group")


if __name__ == "__main__":
    main()
