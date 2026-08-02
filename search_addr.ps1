$dbPath = "E:\Games\steamapps\common\Fallout 4\Data\F4SE\Plugins\version-1-11-191-0.bin"
$bytes = [System.IO.File]::ReadAllBytes($dbPath)
$count = [BitConverter]::ToUInt64($bytes, 0)

# Search F4SE event range for TESActivateEvent
$targets = @{
    "F4SE Event Range Start" = 0x00530000
}

$range = 0x2000  # Wider range to find activate event

foreach ($name in $targets.Keys) {
    $targetOffset = $targets[$name]
    Write-Host "`n=== $name (0x$($targetOffset.ToString('X'))) ==="

    $found = $false
    for ($i = 0; $i -lt $count; $i++) {
        $pos = 8 + ($i * 16)
        if ($pos + 16 -gt $bytes.Length) { break }
        $id = [BitConverter]::ToUInt64($bytes, $pos)
        $offset = [BitConverter]::ToUInt64($bytes, $pos + 8)

        if ([Math]::Abs($offset - $targetOffset) -le $range) {
            $diff = $offset - $targetOffset
            $sign = if ($diff -ge 0) { "+" } else { "" }
            Write-Host "  ID=$id Offset=0x$($offset.ToString('X')) ($sign$diff)"
            $found = $true
        }
    }
    if (-not $found) {
        Write-Host "  No matches in range"
    }
}
