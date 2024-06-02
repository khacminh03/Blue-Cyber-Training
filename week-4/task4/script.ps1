$Query = "SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' AND TargetInstance.Name = 'Calculator.exe'"
$Action = {
    Start-Process notepad.exe
}

Register-WmiEvent -Query $Query -Action $Action -SourceIdentifier "CalculatorStarted"
Write-Output "Listening for Calculator startup events. Press 'Enter' to exit."
$null = Read-Host
Unregister-Event -SourceIdentifier "CalculatorStarted"
