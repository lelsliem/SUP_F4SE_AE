#!/usr/bin/env python3
"""
Find Address Library IDs from the 1.10.163 database for known offsets.
The IDs are consistent across versions, so once found they work with any version.
"""

import struct
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


def parse_old_format(filepath):
    """Parse the old Address Library format (pre-NG)."""
    print(f"Parsing: {filepath}")
    print(f"File size: {filepath.stat().st_size} bytes")

    with open(filepath, 'rb') as f:
        data = f.read()

    pos = 0

    # Read format version
    fmt = struct.unpack('<i', data[pos:pos+4])[0]
    pos += 4
    print(f"Format version: {fmt}")

    if fmt == 1:
        # Format 1: version info then simple pairs
        ver_major = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_minor = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_build = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_sub = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name_len = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name = data[pos:pos+name_len].decode('utf-8', errors='ignore')
        pos += name_len
        count = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4

        print(f"Game version: {ver_major}.{ver_minor}.{ver_build}.{ver_sub}")
        print(f"Module name: {name}")
        print(f"Entry count: {count}")

        offset_to_id = {}
        for i in range(count):
            entry_id = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            entry_offset = struct.unpack('<Q', data[pos:pos+8])[0]; pos += 8
            offset_to_id[entry_offset] = entry_id

        return offset_to_id

    elif fmt == 2:
        # Format 2: compressed delta encoding
        ver_major = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_minor = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_build = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        ver_sub = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name_len = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        name = data[pos:pos+name_len].decode('utf-8', errors='ignore')
        pos += name_len
        ptr_size = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4
        addr_count = struct.unpack('<i', data[pos:pos+4])[0]; pos += 4

        print(f"Game version: {ver_major}.{ver_minor}.{ver_build}.{ver_sub}")
        print(f"Module name: {name}")
        print(f"Pointer size: {ptr_size}")
        print(f"Entry count: {addr_count}")

        offset_to_id = {}
        prev_id = 0
        prev_offset = 0

        for i in range(addr_count):
            type_byte = data[pos]; pos += 1
            offset_type = type_byte & 0xF
            id_type = type_byte >> 4

            # Read ID delta
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
                raise ValueError(f"Unknown ID type: {id_type}")

            # Read offset delta
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
                raise ValueError(f"Unknown offset type: {offset_type}")

            entry_id = prev_id + id_delta
            entry_offset = prev_offset + offset_delta

            offset_to_id[entry_offset] = entry_id
            prev_id = entry_id
            prev_offset = entry_offset

        return offset_to_id

    else:
        raise ValueError(f"Unknown format version: {fmt}")


def main():
    # Parse the 1.10.163 database
    db_path = Path(r"E:\Games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-10-163-0.bin")

    if not db_path.exists():
        print(f"ERROR: Database not found: {db_path}")
        return

    print("=" * 80)
    print("Parsing 1.10.163 Address Library Database")
    print("=" * 80)

    try:
        offset_to_id = parse_old_format(db_path)
    except Exception as e:
        print(f"ERROR: Failed to parse database: {e}")
        import traceback
        traceback.print_exc()
        return

    print(f"\nLoaded {len(offset_to_id)} address mappings\n")

    # Find IDs for known offsets
    print("=" * 80)
    print("Address Library IDs for SUP F4SE functions")
    print("=" * 80)
    print()

    found = []
    not_found = []

    for offset, name in sorted(KNOWN_OFFSETS.items(), key=lambda x: x[1]):
        if offset in offset_to_id:
            addr_id = offset_to_id[offset]
            found.append((name, addr_id, offset))
        else:
            not_found.append((name, offset))

    print(f"Found: {len(found)} / {len(KNOWN_OFFSETS)}")
    print()

    # Output C++ namespace format
    print("// Copy this to SUP_AddressLib.h in the IDs namespace:")
    print("namespace IDs {")
    for name, addr_id, offset in sorted(found, key=lambda x: x[0]):
        print(f"    constexpr uint64_t {name} = {addr_id};  // 0x{offset:X}")
    print("}")
    print()

    if not_found:
        print("// NOT FOUND (need different lookup method):")
        for name, offset in not_found:
            print(f"//   {name} = 0x{offset:X}")


if __name__ == "__main__":
    main()
