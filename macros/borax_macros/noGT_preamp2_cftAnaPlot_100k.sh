~/macros/cftAnaPlots.sh \
-preamp 2 \
-actinide cf252 \
-pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root \
-noMaskTree $trees/drift/cf252.nEv100000.driftVel4.9.houghKalman/noMask2_noGhostTail.root  \
-cftTree $trees/drift/cf252.nEv100000.driftVel4.9.houghKalman/cft2_noGhostTail.root  \
-addGuide "nDigits" \
-driftVel 4.9 \
-cuts "eventID<20000"
sleep 1

# ~/macros/cftAnaPlots.sh \
# -preamp 2 \
# -actinide cf252 \
# -pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.42.root \
# -noMaskTree $trees/drift/cf252.nEv100000.driftVel4.42.houghKalman/noMask2_noGhostTail.root  \
# -cftTree $trees/drift/cf252.nEv100000.driftVel4.42.houghKalman/cft2_noGhostTail.root  \
# -driftVel 4.42 &
# sleep 1

# ~/macros/cftAnaPlots.sh \
# -preamp 2 \
# -actinide cf252 \
# -pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root \
# -noMaskTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/noMask2_noGhostTail.root  \
# -cftTree $trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/cft2_noGhostTail.root  \
# -driftVel 4.9
# sleep 1
#
# ~/macros/cftAnaPlots.sh \
# -preamp 2 \
# -actinide cf252 \
# -pidFile $pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.42.root \
# -noMaskTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/noMask2_noGhostTail.root  \
# -cftTree $trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/cft2_noGhostTail.root  \
# -driftVel 4.42
# sleep 1
