#!/usr/bin/env python3
"""
Parse the NG Address Library format properly and find correct IDs for SUP F4SE.
The NG format uses a different encoding than the old format.
"""

import struct
from pathlib import Path

# Target offsets we need to find (from the 1.11.191 database, based on fuzzy matching)
# These are the NEW offsets in the NG build
TARGET_OFFSETS = {
    0x412090: "REFR_HasKeywordHelper",
    0x1F6A70: "WorkShopExtra_RemoveItem",
    0x1F6CF0: "WorkShopExtra_AddGridConnection",
    0x1F7430: "WorkShopExtra_RemoveGridConnection",
    0x1F7780: "WorkShopExtra_AreDirectlyConnected",
    0x240140: "WorkShopExtra_CalculateCapacityAndLoad",
    0x1F7A50: "WorkShopExtra_GetPowerGridForTarget",
    0x1F84B0: "WorkShopExtra_CleanUpEmptyPowerGrids",
    0x207A20: "WorkShop_DeleteWorkshopItem",
    0x200F60: "WorkShop_DisconnectSpline",
    0x1FC360: "Workshop_ItemInterruptsPower",
    0x1FC0D0: "Workshop_ItemIsWirelessPowerReceiver",
    0x2018F0: "Workshop_IsWithinRadiatorRange",
    0xCC3460: "Main_RadioUpdate",
    0xC4BE80: "Main_OnRadioAddStation",
    0x5D69E0: "Main_OnQuestComplete",
    0x5D6A90: "Main_OnQuestActive",
    0x5D66C0: "Main_OnQuestFailed",
    0x125B4A2: "Main_OnConsoleCommand",
    0x4E2A20: "Main_OnCompileScript",
    0xEA72D0: "Main_OnSePlayerMarker",
    0xE73F00: "Main_OnKnockExplosion",
    0xD59F40: "Main_OnGetUpFromKnock",
    0xCC1C60: "Main_OnPlayerRadioState",
    0x3C1170: "Main_UpdateSeenDataForPlayerLoc",
    0x4115B0: "Main_OnSetWantsDelete",
    0xD5EFF0: "TaskInterface_ProcessWorkshopSwitchEvent",
    0xD73620: "Actor_Jump",
    0xDA8C50: "Actor_IsInAir",
    0xD89D50: "Actor_IsDead",
    0xD89DD0: "Actor_CanBeKnockedDown",
    0x152E80: "Main_GetFormByEditorID",
    0xCC19D0: "Main_GetRadioStationName",
    0x23780: "Main_GetActorByHandle",
    0x2B0530: "Main_OnApplyDismemberment",
    0x204400: "Main_GetTerminalLinkKeyword",
}


def parse_ng_addresslib(filepath):
    """Parse the NG Address Library format."""
    print(f"Parsing: {filepath}")
    print(f"File size: {filepath.stat().st_size} bytes")

    with open(filepath, 'rb') as f:
        data = f.read()

    # Read header - 4 ints
    header = struct.unpack('<4i', data[:16])
    print(f"Header: {header}")

    count = header[0]
    print(f"Entry count from header: {count}")

    # NG format: after 16-byte header, entries are delta-encoded
    # Each entry: 1 byte type, then variable length ID delta, then variable length offset delta

    pos = 16
    entries = {}
    prev_id = 0
    prev_offset = 0

    for i in range(count):
        if pos >= len(data):
            print(f"Ran out of data at entry {i}")
            break

        type_byte = data[pos]
        pos += 1

        lo = type_byte & 0xF  # ID encoding type
        hi = type_byte >> 4   # Offset encoding type

        # Decode ID delta
        if lo == 0:
            if pos + 8 > len(data): break
            id_val = struct.unpack('<Q', data[pos:pos+8])[0]
            pos += 8
        elif lo == 1:
            id_val = prev_id + 1
        elif lo == 2:
            if pos + 1 > len(data): break
            delta = struct.unpack('<B', data[pos:pos+1])[0]
            pos += 1
            id_val = prev_id + delta
        elif lo == 3:
            if pos + 1 > len(data): break
            delta = struct.unpack('<b', data[pos:pos+1])[0]
            pos += 1
            id_val = prev_id - delta
        elif lo == 4:
            if pos + 2 > len(data): break
            delta = struct.unpack('<H', data[pos:pos+2])[0]
            pos += 2
            id_val = prev_id + delta
        elif lo == 5:
            if pos + 2 > len(data): break
            delta = struct.unpack('<h', data[pos:pos+2])[0]
            pos += 2
            id_val = prev_id - delta
        elif lo == 6:
            if pos + 2 > len(data): break
            id_val = struct.unpack('<H', data[pos:pos+2])[0]
            pos += 2
        elif lo == 7:
            if pos + 4 > len(data): break
            id_val = struct.unpack('<I', data[pos:pos+4])[0]
            pos += 4
        else:
            print(f"Unknown ID type {lo} at entry {i}")
            break

        # Decode offset delta
        if hi == 0:
            if pos + 8 > len(data): break
            offset_val = struct.unpack('<Q', data[pos:pos+8])[0]
            pos += 8
        elif hi == 1:
            offset_val = prev_offset + 1
        elif hi == 2:
            if pos + 1 > len(data): break
            delta = struct.unpack('<B', data[pos:pos+1])[0]
            pos += 1
            offset_val = prev_offset + delta
        elif hi == 3:
            if pos + 1 > len(data): break
            delta = struct.unpack('<b', data[pos:pos+1])[0]
            pos += 1
            offset_val = prev_offset - delta
        elif hi == 4:
            if pos + 2 > len(data): break
            delta = struct.unpack('<H', data[pos:pos+2])[0]
            pos += 2
            offset_val = prev_offset + delta
        elif hi == 5:
            if pos + 2 > len(data): break
            delta = struct.unpack('<h', data[pos:pos+2])[0]
            pos += 2
            offset_val = prev_offset - delta
        elif hi == 6:
            if pos + 2 > len(data): break
            offset_val = struct.unpack('<H', data[pos:pos+2])[0]
            pos += 2
        elif hi == 7:
            if pos + 4 > len(data): break
            offset_val = struct.unpack('<I', data[pos:pos+4])[0]
            pos += 4
        else:
            print(f"Unknown offset type {hi} at entry {i}")
            break

        # NG format stores offset / 16, so multiply back
        real_offset = offset_val * 16

        entries[real_offset] = id_val
        prev_id = id_val
        prev_offset = offset_val

        if i < 5 or i % 100000 == 0:
            print(f"Entry {i}: ID={id_val}, offset=0x{real_offset:X}")

    print(f"\nParsed {len(entries)} entries")
    return entries


def main():
    db_path = Path(r"E:\games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-11-191-0.bin")

    if not db_path.exists():
        db_path = Path(r"D:\projects\Address Library - All In One-47327-1-11-191-1765967714\F4SE\Plugins\version-1-11-191-0.bin")

    if not db_path.exists():
        print("Database not found!")
        return

    entries = parse_ng_addresslib(db_path)

    print("\n" + "=" * 80)
    print("Looking up IDs for target offsets...")
    print("=" * 80 + "\n")

    found = []
    not_found = []

    for offset, name in sorted(TARGET_OFFSETS.items()):
        if offset in entries:
            addr_id = entries[offset]
            found.append((name, addr_id, offset))
            print(f"FOUND: {name}")
            print(f"       Offset: 0x{offset:X} -> ID: {addr_id}")
        else:
            # Try nearby offsets (within 16 bytes - might be off by one slot)
            nearby = None
            for delta in range(-16, 17, 16):
                test_offset = offset + delta
                if test_offset in entries:
                    nearby = (entries[test_offset], test_offset, delta)
                    break

            if nearby:
                addr_id, actual_offset, delta = nearby
                found.append((name, addr_id, actual_offset))
                print(f"FOUND (nearby, delta={delta}): {name}")
                print(f"       Offset: 0x{offset:X} -> 0x{actual_offset:X} -> ID: {addr_id}")
            else:
                not_found.append((name, offset))
                print(f"NOT FOUND: {name} at 0x{offset:X}")

    print("\n" + "=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print(f"Found: {len(found)} / {len(TARGET_OFFSETS)}")
    print()

    if found:
        print("// Correct Address Library IDs for SUP_AddressLib.h:")
        print("namespace IDs {")
        for name, addr_id, offset in sorted(found, key=lambda x: x[0]):
            print(f"    constexpr uint64_t {name} = {addr_id};  // 0x{offset:X}")
        print("}")

    if not_found:
        print("\n// Still not found:")
        for name, offset in not_found:
            print(f"//   {name} at 0x{offset:X}")


if __name__ == "__main__":
    main()
