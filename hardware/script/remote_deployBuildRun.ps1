param (
    [string]$mode = "build"  # Default mode is to build, change to "run" if any argument is provided
)
# Define variables
$localProjectPath = "D:\repos\projects\NeoLightFX"
$remoteUser = "rpi"
$remoteHost = "192.168.1.109"
$remotePath = "/home/rpi/shared/NeoLightFX"
$remoteBuildPath = "${remotePath}/build"
$appName = "runRpi3App"

if ($args.Length -eq 0) {
    # No argument provided, perform full build
    # Delete previous files and create directory
    ssh -t ${remoteUser}@${remoteHost} @"
rm -rf ${remotePath}
mkdir -p ${remotePath}
"@

    # SCP command to copy files
    scp -r ${localProjectPath}/* ${remoteUser}@${remoteHost}:${remotePath}

    # SSH command to build the project
    ssh -t ${remoteUser}@${remoteHost} @"
rm -rf ${remoteBuildPath}
mkdir -p ${remoteBuildPath}
cd ${remoteBuildPath}
cmake ${remotePath}
cmake --build .
"@
}

# SSH command to run the app
ssh -t ${remoteUser}@${remoteHost} "cd ${remoteBuildPath} && sudo ./${appName}"


# Use
# 1. Deploy, Build, Run
# .\remote_deployBuildRun.ps1
# 2. Run only
# .\remote_deployBuildRun.ps1 -action run
