~/macros/pidCut_zCut.sh \
-preamp 2 \
-noMaskTree /p/lscratche/jerbundg/data/thesisData/trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/noMask2_noGhostTail.root \
-driftVel 4.42 &
sleep 1

~/macros/pidCut_zCut.sh \
-preamp 2 \
-noMaskTree /p/lscratche/jerbundg/data/thesisData/trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/noMask2_noGhostTail.root \
-driftVel 4.9 &
sleep 1

~/macros/pidCut_zCut.sh \
-preamp 1 \
-noMaskTree /p/lscratche/jerbundg/data/thesisData/trees/drift/cf252.nEv-1.driftVel4.42.houghKalman/noMask1_noGhostTail.root \
-driftVel 4.42 &
sleep 1

~/macros/pidCut_zCut.sh \
-preamp 1 \
-noMaskTree /p/lscratche/jerbundg/data/thesisData/trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/noMask1_noGhostTail.root \
-driftVel 4.9 &
sleep 1
