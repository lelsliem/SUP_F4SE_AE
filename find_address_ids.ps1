# Script to find Address Library IDs for hardcoded offsets
# Searches the NG Address Library database for matching or nearby offsets

$dbPath = "E:\Games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-11-191-0.bin"

# Hardcoded offsets that need Address Library IDs (from old version)
$hardcodedOffsets = @{
    "g_Source" = 0x05A5F758
    "g_SUPplayerControls" = 0x059DA218
    "WorkshopDestroyItem_func" = 0x207B10
    "ConnectToRadiator" = 0x203C80
    "IsSwitch" = 0x328A80
    "GetOpenState" = 0x1439B0
    "WorkshopCalculatePowerRating" = 0x1F6170
    "WorkshopRecalculateResources" = 0x1F5D40
    "ItemIsPowerReceiver" = 0x1FC140
    "ItemContainsNode" = 0x241910
    "OnWorkshopHandleReferenceEvent_Hook" = 0x1F44D0
    "GetBSTObjectArenaScrapAllocBase_func" = 0x1B13830
    "g_TaskInterface" = 0x05AC64F0
    "UpdateMovingWirelessItem1" = 0x00201B10
    "DECLARE_EVENT_DISPATCHER_TESPlacementEvent" = 0x22A020
}

Write-Host "Reading Address Library database from: $dbPath"
Write-Host ""

$bytes = [System.IO.File]::ReadAllBytes($dbPath)
$count = [BitConverter]::ToUInt64($bytes, 0)
Write-Host "Database contains $count entries"
Write-Host ""

# Build a dictionary of offset -> ID
$offsetToId = @{}
$idToOffset = @{}

for ($i = 0; $i -lt $count; $i++) {
    $pos = 8 + ($i * 16)
    if ($pos + 16 -gt $bytes.Length) { break }

    $id = [BitConverter]::ToUInt64($bytes, $pos)
    $offset = [BitConverter]::ToUInt64($bytes, $pos + 8)

    $offsetToId[$offset] = $id
    $idToOffset[$id] = $offset
}

Write-Host "Loaded $($offsetToId.Count) entries"
Write-Host ""
Write-Host "=========================================="
Write-Host "Searching for hardcoded offsets..."
Write-Host "=========================================="
Write-Host ""

foreach ($name in $hardcodedOffsets.Keys) {
    $targetOffset = $hardcodedOffsets[$name]
    $targetHex = "0x{0:X}" -f $targetOffset

    Write-Host "--- $name (old offset: $targetHex) ---"

    # Check for exact match first
    if ($offsetToId.ContainsKey($targetOffset)) {
        $id = $offsetToId[$targetOffset]
        Write-Host "  EXACT MATCH: ID=$id (0x$($targetOffset.ToString('X')))"
    } else {
        # Search for nearby offsets (within 0x1000 range)
        $nearbyMatches = @()
        $searchRange = 0x200  # Search within +/- 512 bytes

        foreach ($offset in $offsetToId.Keys) {
            $diff = [Math]::Abs($offset - $targetOffset)
            if ($diff -le $searchRange -and $diff -gt 0) {
                $nearbyMatches += @{
                    ID = $offsetToId[$offset]
                    Offset = $offset
                    Diff = $diff
                }
            }
        }

        if ($nearbyMatches.Count -gt 0) {
            $sorted = $nearbyMatches | Sort-Object { $_.Diff }
            Write-Host "  No exact match. Nearby candidates:"
            $sorted | Select-Object -First 5 | ForEach-Object {
                $sign = if ($_.Offset -gt $targetOffset) { "+" } else { "-" }
                Write-Host ("    ID={0} -> 0x{1:X} ({2}0x{3:X})" -f $_.ID, $_.Offset, $sign, $_.Diff)
            }
        } else {
            Write-Host "  NO MATCHES within range. This might be a global variable, not a function."
        }
    }
    Write-Host ""
}

Write-Host "=========================================="
Write-Host "Done!"
