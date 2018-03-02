#!/bin/bash

topdir=digitScan_MinThresh_`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir

echo ~/macros/digitScan_MinThresh.sh \
-preamp 2 \
-actinide cf252 \
-pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root \
-noMaskTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/noMask2_noGhostTail.root  \
-cftTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/cft2_noGhostTail.root  \
-driftVel 4.9 \
>> launch_preamp2_driftVel4.9.sh
chmod 750 launch_preamp2_driftVel4.9.sh
msub -q pbatch -l walltime=44444 launch_preamp2_driftVel4.9.sh
sleep 1

echo ~/macros/digitScan_MinThresh.sh \
-preamp 2 \
-actinide cf252 \
-pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.42.root \
-noMaskTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/noMask2_noGhostTail.root  \
-cftTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/cft2_noGhostTail.root  \
-driftVel 4.42 \
>> launch_preamp2_driftVel4.42.sh
chmod 750 launch_preamp2_driftVel4.42.sh
msub -q pbatch -l walltime=44444 launch_preamp2_driftVel4.42.sh
sleep 1

echo ~/macros/digitScan_MinThresh.sh \
-preamp 1 \
-actinide cf252 \
-pidFile $pid/pidCut_preamp1_zCut70_fragThresh2.5.driftVel4.9.root \
-noMaskTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/noMask1_noGhostTail.root  \
-cftTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/cft1_noGhostTail.root  \
-driftVel 4.9 \
>> launch_preamp1_driftVel4.9.sh
chmod 750 launch_preamp1_driftVel4.9.sh
msub -q pbatch -l walltime=44444 launch_preamp1_driftVel4.9.sh
sleep 1

echo ~/macros/digitScan_MinThresh.sh \
-preamp 1 \
-actinide cf252 \
-pidFile $pid/pidCut_preamp1_zCut70_fragThresh2.5.driftVel4.42.root \
-noMaskTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/noMask1_noGhostTail.root  \
-cftTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/cft1_noGhostTail.root  \
-driftVel 4.42 \
>> launch_preamp1_driftVel4.42.sh
chmod 750 launch_preamp1_driftVel4.42.sh
msub -q pbatch -l walltime=44444 launch_preamp1_driftVel4.42.sh
sleep 1
