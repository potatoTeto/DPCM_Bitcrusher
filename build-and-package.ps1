param (
    [string]$Version = ""
)

# Project name variants
$projectNameLower = "dpcm_bitcrusher"
$projectNameUpper = "DPCM_Bitcrusher"

# Normalize version format (e.g. v1.0.0 or [v1.0.0])
$versionTag = $Version.Trim('[', ']').TrimStart('v', 'V')
$VersionSuffix = if ($versionTag) { "_v$versionTag" } else { "" }
Write-Host "Packaging with version tag: $VersionSuffix"

# Paths
$projectRoot = $PSScriptRoot
$buildFolder = Join-Path $projectRoot "Builds\VisualStudio2022"
$publishFolder = Join-Path $projectRoot "publish"
$config = "Release"
$platform = "x64"
$sevenZipPath = "C:\Program Files\7-Zip\7z.exe"  # Change if 7-Zip is installed elsewhere
$solutionPath = Join-Path $buildFolder "$projectNameUpper.sln"

# Ensure publish folder exists
if (-not (Test-Path $publishFolder)) {
    New-Item -ItemType Directory -Path $publishFolder | Out-Null
}

# Build entire solution only if needed (skip if up-to-date)
Write-Host "`n[BUILD] Checking if build is needed..."
$lastBuildTime = (Get-Item (Join-Path $buildFolder "x64\$config")).LastWriteTime
$srcFiles = Get-ChildItem -Path $projectRoot -Include *.cpp,*.h,*.cs -Recurse
$newestSrcTime = ($srcFiles | Sort-Object LastWriteTime -Descending | Select-Object -First 1).LastWriteTime

if ($newestSrcTime -gt $lastBuildTime) {
    Write-Host "[BUILD] Source files changed since last build. Building solution..."
    & msbuild $solutionPath "/t:Rebuild" "/p:Configuration=$config;Platform=$platform"
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Solution build failed. Aborting packaging."
        exit $LASTEXITCODE
    }
} else {
    Write-Host "[BUILD] Build is up-to-date. Skipping build."
}

# Paths for outputs and archives
$vst3FilePath = Join-Path $buildFolder "x64\$config\VST3\$projectNameLower.vst3\Contents\x86_64-win\$projectNameLower.vst3"
$standaloneFolder = Join-Path $buildFolder "x64\$config\Standalone Plugin"

$vst3ArchiveName = "${projectNameUpper}_VST3$VersionSuffix.7z"
$standaloneArchiveName = "${projectNameUpper}_StandalonePlugin$VersionSuffix.7z"

$vst3ArchivePath = Join-Path $publishFolder $vst3ArchiveName
$standaloneArchivePath = Join-Path $publishFolder $standaloneArchiveName

# Package VST3 plugin file at root of archive
if (Test-Path $vst3FilePath) {
    Write-Host "[ARCHIVE] Creating VST3 archive with .vst3 plugin file..."

    $tempVst3Folder = Join-Path $publishFolder "temp_vst3"
    if (Test-Path $tempVst3Folder) { Remove-Item -Recurse -Force $tempVst3Folder }
    New-Item -ItemType Directory -Path $tempVst3Folder | Out-Null

    Copy-Item $vst3FilePath -Destination $tempVst3Folder

    & "$sevenZipPath" a -t7z -mx=9 $vst3ArchivePath (Join-Path $tempVst3Folder "*")

    Remove-Item -Recurse -Force $tempVst3Folder

    Write-Host "[DONE] VST3 packaged as $vst3ArchiveName"
} else {
    Write-Warning "VST3 file not found: $vst3FilePath"
}

# Package Standalone Plugin files inside versioned folder in archive
$standaloneFiles = @(
    "$projectNameLower.exe",
    "$projectNameLower.exe.recipe",
    "${projectNameLower}_StandalonePlugin.log",
    "resources.res"
) | ForEach-Object { Join-Path $standaloneFolder $_ }

$existingStandaloneFiles = $standaloneFiles | Where-Object { Test-Path $_ }
if ($existingStandaloneFiles.Count -eq 0) {
    Write-Warning "No standalone plugin files found to package."
} else {
    Write-Host "[ARCHIVE] Creating StandalonePlugin archive..."

    $tempStandaloneFolder = Join-Path $publishFolder "temp_standalone"
    $finalStandaloneFolder = Join-Path $tempStandaloneFolder "$projectNameLower$VersionSuffix"

    if (Test-Path $tempStandaloneFolder) { Remove-Item -Recurse -Force $tempStandaloneFolder }
    New-Item -ItemType Directory -Path $finalStandaloneFolder | Out-Null

    foreach ($file in $existingStandaloneFiles) {
        Copy-Item -Path $file -Destination $finalStandaloneFolder
    }

    & "$sevenZipPath" a -t7z -mx=9 $standaloneArchivePath (Join-Path $tempStandaloneFolder "*")

    Remove-Item -Recurse -Force $tempStandaloneFolder

    Write-Host "[DONE] StandalonePlugin packaged as $standaloneArchiveName"
}

Write-Host "`n=== All builds complete. Files stored in: $publishFolder ==="
