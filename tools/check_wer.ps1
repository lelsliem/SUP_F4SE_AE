$cut = (Get-Date).AddHours(-2)
$evts = Get-WinEvent -FilterHashtable @{ LogName = 'Application'; Id = 1000, 1001 } -MaxEvents 60 -ErrorAction SilentlyContinue |
    Where-Object { $_.Message -match 'Fallout4' -and $_.TimeCreated -gt $cut }
foreach ($e in $evts) {
    $name = ''
    if ($e.Message -match 'Event Name:\s*(\S+)') { $name = $Matches[1] }
    $mod = ''
    if ($e.Message -match 'Faulting module name:\s*([^\s]+)') { $mod = $Matches[1] }
    $off = ''
    if ($e.Message -match 'Fault offset:\s*([^\s]+)') { $off = $Matches[1] }
    $code = ''
    if ($e.Message -match 'Exception code:\s*([^\s]+)') { $code = $Matches[1] }
    Write-Output ($e.TimeCreated.ToString('MM-dd HH:mm:ss') + " id=" + $e.Id + " " + $name + " module=" + $mod + " off=" + $off + " code=" + $code)
}
