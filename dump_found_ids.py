#!/usr/bin/env python3
"""
Parse the NG Address Library and dump entries to find the pattern.
Based on the log, ID 4472849 maps to offset 0x1F6A70 (relative),
and 2232980 maps to 0xD5EFF0 (relative).
Let's verify this and find more mappings.
"""

import struct
from pathlib import Path

def parse_simple_format(filepath):
    """Try simple ID:offset pairs format used by some Address Library versions."""
    print(f"Parsing: {filepath}")

    with open(filepath, 'rb') as f:
        data = f.read()

    # Header: count (8 bytes)
    count = struct.unpack('<Q', data[:8])[0]
    print(f"Count from first 8 bytes: {count}")

    if count > 10000000:  # Too big, probably not this format
        print("Count too large, trying alternate header...")
        # Try 4-byte count
        count = struct.unpack('<I', data[:4])[0]
        print(f"Count from first 4 bytes: {count}")

    if count > 10000000:
        return None

    # Try reading entries as (id, offset) pairs
    entries = {}
    pos = 8

    # Try 8+8 format
    for i in range(min(count, 1000000)):
        if pos + 16 > len(data):
            break
        entry_id = struct.unpack('<Q', data[pos:pos+8])[0]
        entry_offset = struct.unpack('<Q', data[pos+8:pos+16])[0]
        pos += 16

        if entry_offset < 0x10000000:  # Reasonable offset
            entries[entry_id] = entry_offset

        if i < 10:
            print(f"Entry {i}: ID={entry_id}, offset=0x{entry_offset:X}")

    return entries

def main():
    db_path = Path(r"E:\games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-11-191-0.bin")

    if not db_path.exists():
        print("Database not found!")
        return

    print(f"File size: {db_path.stat().st_size} bytes\n")

    with open(db_path, 'rb') as f:
        data = f.read()

    # Show first 64 bytes in hex
    print("First 64 bytes of file:")
    for i in range(0, 64, 16):
        hex_part = ' '.join(f'{b:02X}' for b in data[i:i+16])
        print(f"  {i:04X}: {hex_part}")

    # The log showed these were found:
    # WorkShopExtra_RemoveItem: ID=4472849 -> 0x7FF78D1CA700
    # TaskInterface_ProcessWorkshopSwitchEvent: ID=2232980 -> 0x7FF79884FF00
    # Base address was 0x7FF78B260000
    # So offsets are: 0x1F6A700 and 0xD5EFF00 -- wait, those are too big
    # Actually 0x7FF78D1CA700 - 0x7FF78B260000 = 0x1F6A700... that's 32MB offset
    # But the expected offset was 0x1F6A70 which is only 2MB

    # Let me check: 0x1F6A700 / 16 = 0x1F6A70? No, 0x1F6A700 / 16 = 0x1F6A70
    # So the database stores offset*16 and the code divides by 16 wrong?

    print("\n\nAnalyzing the working IDs:")
    print("ID 4472849 resolved to 0x7FF78D1CA700")
    print("Base addr: 0x7FF78B260000")
    print("Relative offset: 0x7FF78D1CA700 - 0x7FF78B260000 = ", hex(0x7FF78D1CA700 - 0x7FF78B260000))

    print("\nID 2232980 resolved to 0x7FF79884FF00")
    print("Relative offset: 0x7FF79884FF00 - 0x7FF78B260000 = ", hex(0x7FF79884FF00 - 0x7FF78B260000))

    # These offsets are 16x what we expected - the C++ code has a bug!
    # It's multiplying by 16 but should be dividing, or not multiplying at all

    # What we need:
    # WorkShopExtra_RemoveItem should be at 0x1F6A70
    # But we got 0x1F6A700 = 0x1F6A70 * 16

    print("\n\nConclusion: The offset is being multiplied by 16 incorrectly!")
    print("0x1F6A700 / 16 = ", hex(0x1F6A700 // 16))
    print("0xD5EFF00 / 16 = ", hex(0xD5EFF00 // 16))

if __name__ == "__main__":
    main()
