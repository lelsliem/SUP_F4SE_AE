#!/usr/bin/env python3
"""
Parse Address Library binary file to find IDs for known offsets.
The offsets in Tomm_Addresses.h were for FO4 1.10.163.
We need to find the Address Library IDs that map to those offsets.
"""

import struct
import sys
from pathlib import Path

# Known offsets from Tomm_Addresses.h (FO4 v1.10.163)
KNOWN_OFFSETS = {
    0x4120B0: "REFR_HasKeywordHelper",
    0x1F69C0: "WorkShopExtra_RemoveItem",
    0x1F6E20: "WorkShopExtra_AddGridConnection",
    0x1F7440: "WorkShopExtra_RemoveGridConnection",
    0x1F77D0: "WorkShopExtra_AreDirectlyConnected",
    0x240160: "WorkShopExtra_CalculateCapacityAndLoad",
    0x1F7950: "WorkShopExtra_GetPowerGridForTarget",
    0x1F8550: "WorkShopExtra_CleanUpEmptyPowerGrids",
    0x207A30: "WorkShop_DeleteWorkshopItem",
    0x200F50: "WorkShop_DisconnectSpline",
    0x1FC360: "Workshop_ItemInterruptsPower",
    0x1FC140: "Workshop_ItemIsWirelessPowerReceiver",
    0x201930: "Workshop_IsWithinRadiatorRange",
    0x1F7840: "Workshop_IsPowered",
    0xCC3450: "Main_RadioUpdate",
    0xC4BEE0: "Main_OnRadioAddStation",
    0x5D69B0: "Main_OnQuestComplete",
    0x5D6AC0: "Main_OnQuestActive",
    0x5D6690: "Main_OnQuestFailed",
    0x125B4A0: "Main_OnConsoleCommand",
    0x4E2A60: "Main_OnCompileScript",
    0xEA7220: "Main_OnSePlayerMarker",
    0xEA7250: "Main_OnRemovePlayerMapMarker",
    0xE73F40: "Main_OnKnockExplosion",
    0xD59F00: "Main_OnGetUpFromKnock",
    0xCC1B90: "Main_OnPlayerRadioState",
    0xCC1CF0: "Main_IsPlayerRadioEnabled",
    0x3C10F0: "Main_UpdateSeenDataForPlayerLoc",
    0x4115C0: "Main_OnSetWantsDelete",
    0xD5F310: "TaskInterface_ProcessWorkshopSwitchEvent",
    0xD73770: "Actor_Jump",
    0xDA8AD0: "Actor_IsInAir",
    0xD89D40: "Actor_IsDead",
    0xD89DD0: "Actor_CanBeKnockedDown",
    0x152EB0: "Main_GetFormByEditorID",
    0xCC1950: "Main_GetRadioStationName",
    0x38457D0: "Address_TimeMultiplier",
    0x23870: "Main_GetActorByHandle",
    0x2B04A0: "Main_OnApplyDismemberment",
    0x204400: "Main_GetTerminalLinkKeyword",
}


def parse_addresslib_v1(filepath):
    """Parse Address Library format version 1 (simple format)."""
    with open(filepath, 'rb') as f:
        data = f.read()

    # First 8 bytes = count
    count = struct.unpack('<Q', data[:8])[0]
    print(f"Format v1: {count} entries")

    # Each entry is 16 bytes (8 byte ID + 8 byte offset)
    offset_to_id = {}
    for i in range(count):
        pos = 8 + i * 16
        if pos + 16 > len(data):
            break
        entry_id, entry_offset = struct.unpack('<QQ', data[pos:pos+16])
        offset_to_id[entry_offset] = entry_id

    return offset_to_id


def parse_addresslib_v2(filepath):
    """Parse Address Library format version 2 (compressed format)."""
    with open(filepath, 'rb') as f:
        data = f.read()

    pos = 0

    # Header
    fmt = struct.unpack('<i', data[pos:pos+4])[0]
    pos += 4

    if fmt == 1:
        # Version 1 format
        ver_major = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_minor = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_build = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_sub = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name_len = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        pos += name_len  # skip name
        count = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4

        print(f"Format v1: Version {ver_major}.{ver_minor}.{ver_build}.{ver_sub}, {count} entries")

        offset_to_id = {}
        for i in range(count):
            entry_id = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            entry_offset = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            offset_to_id[entry_offset] = entry_id

        return offset_to_id

    elif fmt == 2:
        # Version 2 compressed format
        ver_major = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_minor = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_build = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_sub = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name_len = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        pos += name_len  # skip name
        ptr_size = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        addr_count = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4

        print(f"Format v2: Version {ver_major}.{ver_minor}.{ver_build}.{ver_sub}")
        print(f"  Pointer size: {ptr_size}, Address count: {addr_count}")

        offset_to_id = {}
        prev_id = 0
        prev_offset = 0

        for i in range(addr_count):
            # Read type byte
            type_byte = data[pos]; pos += 1

            # Low 4 bits = offset type, high 4 bits = ID type
            offset_type = type_byte & 0xF
            id_type = type_byte >> 4

            # Read ID delta based on type
            if id_type == 0:
                id_delta = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            elif id_type == 1:
                id_delta = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
            elif id_type == 2:
                id_delta = struct.unpack('<H', data[pos:pos+2])[0]; pos += 2
            elif id_type == 3:
                id_delta = struct.unpack('<B', data[pos:pos+1])[0]; pos += 1
            elif id_type == 4:
                id_delta = 1
            else:
                print(f"Unknown ID type: {id_type}")
                break

            # Read offset delta based on type
            if offset_type == 0:
                offset_delta = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            elif offset_type == 1:
                offset_delta = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
            elif offset_type == 2:
                offset_delta = struct.unpack('<H', data[pos:pos+2])[0]; pos += 2
            elif offset_type == 3:
                offset_delta = struct.unpack('<B', data[pos:pos+1])[0]; pos += 1
            elif offset_type == 4:
                offset_delta = 1
            else:
                print(f"Unknown offset type: {offset_type}")
                break

            entry_id = prev_id + id_delta
            entry_offset = prev_offset + offset_delta

            offset_to_id[entry_offset] = entry_id

            prev_id = entry_id
            prev_offset = entry_offset

        return offset_to_id

    else:
        print(f"Unknown format: {fmt}")
        return {}


def parse_addresslib_simple(filepath):
    """Parse Address Library using the simple format from CommonLibF4.
    Format: 8-byte count, then N x (8-byte id, 8-byte offset) pairs.
    """
    with open(filepath, 'rb') as f:
        data = f.read()

    # First 8 bytes = count of entries
    count = struct.unpack('<Q', data[:8])[0]

    # Sanity check - count should be reasonable
    expected_size = 8 + count * 16
    if count > 10000000 or expected_size > len(data) + 1000:
        print(f"Count {count} seems invalid for file size {len(data)}")
        return None

    print(f"Simple format: {count} entries expected")

    offset_to_id = {}
    pos = 8
    for i in range(min(count, (len(data) - 8) // 16)):
        entry_id = struct.unpack('<Q', data[pos:pos+8])[0]
        entry_offset = struct.unpack('<Q', data[pos+8:pos+16])[0]
        pos += 16
        offset_to_id[entry_offset] = entry_id

    return offset_to_id


def main():
    # Try to find the Address Library database
    db_paths = [
        Path(r"D:\projects\Address Library - All In One-47327-1-11-191-1765967714\F4SE\Plugins\version-1-11-191-0.bin"),
        Path(r"D:\projects\SUP F4SE\version-1-10-163-0.bin"),
    ]

    db_path = None
    for p in db_paths:
        if p.exists():
            db_path = p
            break

    if not db_path:
        print("Address Library database not found!")
        print("Checked paths:")
        for p in db_paths:
            print(f"  {p}")
        return

    print(f"Using database: {db_path}")
    print(f"File size: {db_path.stat().st_size} bytes")
    print()

    # Try parsing with different formats
    offset_to_id = None

    # Try simple format first (CommonLibF4 style)
    try:
        offset_to_id = parse_addresslib_simple(db_path)
    except Exception as e:
        print(f"Simple parse failed: {e}")

    if not offset_to_id:
        try:
            offset_to_id = parse_addresslib_v2(db_path)
        except Exception as e:
            print(f"V2 parse failed: {e}")

    if not offset_to_id:
        try:
            offset_to_id = parse_addresslib_v1(db_path)
        except Exception as e:
            print(f"V1 parse failed: {e}")

    if not offset_to_id:
        print("ERROR: Could not parse Address Library database!")
        print("The file format may be different from expected.")
        return

    print(f"\nLoaded {len(offset_to_id)} address mappings")
    print()

    # Look up our known offsets
    print("=" * 80)
    print("Looking up Address Library IDs for known offsets...")
    print("NOTE: These offsets are for FO4 1.10.163, database is for 1.11.191")
    print("      Offsets may not match directly - use as reference only")
    print("=" * 80)
    print()

    found = []
    not_found = []

    # Also create ID to offset mapping for reverse lookup
    id_to_offset = {v: k for k, v in offset_to_id.items()}

    for offset, name in sorted(KNOWN_OFFSETS.items()):
        if offset in offset_to_id:
            addr_id = offset_to_id[offset]
            found.append((name, addr_id, offset, offset))
            print(f"FOUND (exact): {name}")
            print(f"       Offset: 0x{offset:X} -> ID: {addr_id}")
        else:
            # Try fuzzy search - look for nearby offsets
            # Game update shifts addresses, typically by a consistent delta in each region
            nearby = []
            for db_offset, db_id in offset_to_id.items():
                # Check if within reasonable range (functions can shift by several KB)
                diff = db_offset - offset
                if -0x50000 < diff < 0x50000:  # Within ~320KB
                    nearby.append((abs(diff), diff, db_offset, db_id))

            if nearby:
                nearby.sort()  # Sort by absolute difference
                closest_diff, signed_diff, closest_offset, closest_id = nearby[0]
                if closest_diff < 0x100:  # Very close match (within 256 bytes)
                    found.append((name, closest_id, offset, closest_offset))
                    print(f"FOUND (fuzzy, delta={signed_diff:+d}): {name}")
                    print(f"       Offset: 0x{offset:X} -> 0x{closest_offset:X} -> ID: {closest_id}")
                else:
                    not_found.append((name, offset, nearby[:3]))  # Keep top 3 candidates
            else:
                not_found.append((name, offset, []))

    print()
    print("=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print(f"Found: {len(found)} / {len(KNOWN_OFFSETS)}")
    print()

    if found:
        print("// Address Library IDs (found):")
        for name, addr_id, orig_offset, db_offset in found:
            if orig_offset == db_offset:
                print(f"constexpr uint64_t {name} = {addr_id};  // 0x{orig_offset:X}")
            else:
                print(f"constexpr uint64_t {name} = {addr_id};  // 0x{orig_offset:X} -> 0x{db_offset:X}")

    print()
    if not_found:
        print("// Offsets NOT found in database (need manual lookup):")
        for item in not_found:
            name = item[0]
            offset = item[1]
            candidates = item[2] if len(item) > 2 else []
            print(f"// {name} = 0x{offset:X}")
            if candidates:
                print(f"//   Nearest candidates:")
                for _, diff, cand_offset, cand_id in candidates[:3]:
                    print(f"//     ID {cand_id} at 0x{cand_offset:X} (delta: {diff:+d})")


if __name__ == "__main__":
    main()
