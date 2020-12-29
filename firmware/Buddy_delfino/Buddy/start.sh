#!/bin/bash
/opt/ti/uniflash_5.0.0/deskdb/content/TICloudAgent/linux/ccs_base/DebugServer/bin/DSLite flash \
    -c targetConfigs/TMS320F28377S.ccxml \
    -l "generated.ufsettings" \
    -s VerifyAfterProgramLoad="No verification" \
    -e -f -v CPU1_RAM/Buddy.out