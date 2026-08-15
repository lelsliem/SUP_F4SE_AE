$since = Get-Date -Year 2026 -Month 8 -Day 12 -Hour 22 -Minute 0 -Second 0
Get-WinEvent -FilterHashtable @{LogName='Application'; Id=1000; StartTime=$since} -ErrorAction SilentlyContinue |
    Where-Object { $_.Message -match 'fallout4' } |
    Select-Object -First 5 |
    ForEach-Object {
        Write-Output "=== $($_.TimeCreated) ==="
        ($_.Message -split "`n") |
            Select-String -Pattern 'Faulting application','Faulting module','Exception code','Fault offset' |
            ForEach-Object { $_.Line.Trim() }
    }
Write-Output "done"
