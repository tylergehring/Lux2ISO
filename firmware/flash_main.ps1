param(
    [string]$ComPort = "COM4",
    [string]$ArduinoCliPath = "C:\Program Files\Arduino CLI\arduino-cli.exe",
    [string]$AvrdudePath = "C:\avrdude\avrdude.exe",
    [string]$AvrdudeConf = "C:\avrdude\avrdude.conf",
    [string]$Fqbn = "arduino:avr:pro:cpu=8MHzatmega328"
)

$ErrorActionPreference = "Stop"

$scriptDir = $PSScriptRoot

$layout = $null
if (Test-Path (Join-Path $scriptDir "main\main.ino")) {
    # Script is located in the firmware folder; sketch in firmware/main/main.ino
    $layout = @{
        FirmwareDir = $scriptDir
        SourceIno   = Join-Path $scriptDir "main\main.ino"
    }
} elseif (Test-Path (Join-Path $scriptDir "main.ino")) {
    # Script is located in the firmware folder; sketch in firmware/main.ino
    $layout = @{
        FirmwareDir = $scriptDir
        SourceIno   = Join-Path $scriptDir "main.ino"
    }
} elseif (Test-Path (Join-Path $scriptDir "firmware\main\main.ino")) {
    # Script is located at repo root; sketch in firmware/main/main.ino
    $layout = @{
        FirmwareDir = Join-Path $scriptDir "firmware"
        SourceIno   = Join-Path $scriptDir "firmware\main\main.ino"
    }
} elseif (Test-Path (Join-Path $scriptDir "firmware\main.ino")) {
    # Script is located at repo root; sketch in firmware/main.ino
    $layout = @{
        FirmwareDir = Join-Path $scriptDir "firmware"
        SourceIno   = Join-Path $scriptDir "firmware\main.ino"
    }
} else {
    throw "Could not find main sketch from script location: $scriptDir"
}

$firmwareDir = $layout.FirmwareDir
$sourceIno = $layout.SourceIno

$repoDir = Split-Path -Parent $firmwareDir
$sketchDir = Join-Path $firmwareDir "Lux2ISO"
$sketchIno = Join-Path $sketchDir "Lux2ISO.ino"
$buildDir = Join-Path $firmwareDir "build_main"
$hexPath = Join-Path $buildDir "Lux2ISO.ino.hex"

if (-not (Test-Path $ArduinoCliPath)) {
    throw "arduino-cli not found at: $ArduinoCliPath"
}
if (-not (Test-Path $AvrdudePath)) {
    throw "avrdude not found at: $AvrdudePath"
}
if (-not (Test-Path $AvrdudeConf)) {
    throw "avrdude.conf not found at: $AvrdudeConf"
}
if (-not (Test-Path $sourceIno)) {
    throw "Source sketch not found at: $sourceIno"
}

Write-Host "== Preparing sketch =="
New-Item -ItemType Directory -Force -Path $sketchDir | Out-Null
Copy-Item $sourceIno $sketchIno -Force

Write-Host "== Cleaning build output =="
Remove-Item $buildDir -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

Write-Host "== Compiling main firmware =="
& $ArduinoCliPath compile --fqbn $Fqbn --output-dir $buildDir $sketchDir
if ($LASTEXITCODE -ne 0) {
    throw "Compile failed"
}

if (-not (Test-Path $hexPath)) {
    throw "Compiled hex not found at: $hexPath"
}

Write-Host "== Reading target signature on $ComPort =="
& $AvrdudePath -C $AvrdudeConf -p m328p -c avrisp -P $ComPort -b 19200 -U signature:r:-:h
if ($LASTEXITCODE -ne 0) {
    throw "Signature read failed"
}

Write-Host "== Flashing main firmware =="
$flashArg = "flash:w:{0}:i" -f $hexPath
& $AvrdudePath -C $AvrdudeConf -p m328p -c avrisp -P $ComPort -b 19200 -U $flashArg
if ($LASTEXITCODE -ne 0) {
    throw "Flash failed"
}

Write-Host ""
Write-Host "Flash complete."
Write-Host "Hex flashed: $hexPath"
Write-Host "Power-cycle the target board if needed."
