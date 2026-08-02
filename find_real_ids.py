#!/usr/bin/env python3
"""
Parse the NG database in simple format and find IDs for our target offsets.
The database appears to use 8-byte ID + 8-byte offset pairs.
"""

import struct
from pathlib import Path

# Target offsets we need (divided by 16 for storage format)
# But wait - the working ones show offset*16 in the result, so the database stores raw offsets
TARGET_OFFSETS_RAW = {
    0x1F6A70: "WorkShopExtra_RemoveItem",  # This one worked with ID 4472849
    0xD5EFF0: "TaskInterface_ProcessWorkshopSwitchEvent",  # This worked with ID 2232980
    0x412090: "REFR_HasKeywordHelper",
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
    0x4E2A20: "Main_OnCompileScript",
    0xEA72D0: "Main_OnSePlayerMarker",
    0xE73F00: "Main_OnKnockExplosion",
    0xD59F40: "Main_OnGetUpFromKnock",
    0xCC1C60: "Main_OnPlayerRadioState",
    0x3C1170: "Main_UpdateSeenDataForPlayerLoc",
    0x4115B0: "Main_OnSetWantsDelete",
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

def main():
    db_path = Path(r"E:\games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-11-191-0.bin")

    with open(db_path, 'rb') as f:
        data = f.read()

    print(f"File size: {len(data)} bytes")

    # Try parsing as simple 8+8 format
    # Skip first 8 bytes (count), start reading pairs
    count = struct.unpack('<Q', data[:8])[0]
    print(f"Count: {count}")

    if count > 10000000:
        # Not the right interpretation
        # Try 4-byte values
        header = struct.unpack('<4I', data[:16])
        print(f"Header as 4 ints: {header}")
        count = header[0]

    offset_to_id = {}
    id_to_offset = {}

    # Try starting at offset 8 with (id, offset) pairs
    pos = 8
    entries = 0

    print("\nFirst 10 entries:")
    while pos + 16 <= len(data) and entries < count:
        entry_id = struct.unpack('<Q', data[pos:pos+8])[0]
        entry_offset = struct.unpack('<Q', data[pos+8:pos+16])[0]
        pos += 16

        # Sanity check - offsets should be reasonable
        if entry_offset > 0x20000000:  # > 512MB is suspicious
            continue

        offset_to_id[entry_offset] = entry_id
        id_to_offset[entry_id] = entry_offset

        if entries < 10:
            print(f"  ID={entry_id}, offset=0x{entry_offset:X}")

        entries += 1

    print(f"\nParsed {len(offset_to_id)} entries")

    # Verify the known working IDs
    print("\n\nVerifying known working IDs:")
    if 4472849 in id_to_offset:
        print(f"  ID 4472849 -> offset 0x{id_to_offset[4472849]:X}")
    else:
        print("  ID 4472849 NOT FOUND in id_to_offset")

    if 2232980 in id_to_offset:
        print(f"  ID 2232980 -> offset 0x{id_to_offset[2232980]:X}")
    else:
        print("  ID 2232980 NOT FOUND in id_to_offset")

    # Look for our target offsets
    print("\n\nSearching for target offsets:")
    found_count = 0
    for target_offset, name in sorted(TARGET_OFFSETS_RAW.items()):
        if target_offset in offset_to_id:
            print(f"  FOUND: {name} at 0x{target_offset:X} -> ID {offset_to_id[target_offset]}")
            found_count += 1
        else:
            # Try nearby
            for delta in range(-256, 257, 16):
                if target_offset + delta in offset_to_id:
                    print(f"  NEAR:  {name} at 0x{target_offset:X} -> nearby 0x{target_offset+delta:X} = ID {offset_to_id[target_offset+delta]}")
                    break
            else:
                print(f"  NOT FOUND: {name} at 0x{target_offset:X}")

    print(f"\nFound {found_count}/{len(TARGET_OFFSETS_RAW)} exact matches")

if __name__ == "__main__":
    main()
