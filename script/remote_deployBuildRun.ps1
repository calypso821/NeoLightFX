param (
    [string]$mode = "build"  # Default mode is to build, change to "run" if any argument is provided
)
# Define variables
$localProjectPath = "D:/repos/projects/NeoLightFX"
$remoteUser = "rpi"
$remoteHost = "192.168.1.109"
$remotePath = "/home/rpi/shared/NeoLightFX"
$remoteBuildPath = "${remotePath}/build"
$appName = "NeoLightFX_Hardware"

# Path to the private SSH key (update as necessary)
$privateKeyPath = "/home/matic/.ssh/id_rsa"

if ($args.Length -eq 0) {
    # No argument provided, perform full build
    # Create remotePath directory (if does not exist)
    ssh -t ${remoteUser}@${remoteHost} @"
mkdir -p ${remotePath}
echo 'Target directory: ${remotePath}'
"@

    # Replace backslashes with forward slashes and convert to WSL path
    $unixStyleLocalProjectPath = ${localProjectPath} -replace '\\', '/'
    $wslLocalProjectPath = wsl.exe wslpath -u ${unixStyleLocalProjectPath}

    # Convert the private key path to a WSL-compatible path
    #$unixStylePrivateKeyPath = ${privateKeyPath} -replace '\\', '/'
    #$wslPrivateKeyPath = wsl.exe wslpath -u ${unixStylePrivateKeyPath}

    # Use rsync to copy only modified files and delete removed files
    # Exclude multiple directories and hidden files
    # wsl rsync -av --delete --exclude='build' --exclude='.*' ${wslLocalProjectPath}/ ${remoteUser}@${remoteHost}:${remotePath}/

    $rsyncCommand = "rsync -av --delete --exclude='build' --exclude='.*' -e 'ssh -i ${privateKeyPath}' ${wslLocalProjectPath}/ ${remoteUser}@${remoteHost}:${remotePath}/"

    # Use wsl to execute the rsync command
    wsl bash -c "$rsyncCommand"

    # Ensure the remoteBuildPath directory exists and build the project
    ssh -t ${remoteUser}@${remoteHost} @"
mkdir -p ${remoteBuildPath}
cd ${remoteBuildPath}
cmake -DHARDWARE=ON ${remotePath}
cmake --build . -- -j2
"@
}

# SSH command to run the app
ssh -t ${remoteUser}@${remoteHost} "cd ${remoteBuildPath} && sudo ./${appName}"


# Use
# 1. Deploy, Build, Run
# .\remote_deployBuildRun.ps1
# 2. Run only
# .\remote_deployBuildRun.ps1 -action run
