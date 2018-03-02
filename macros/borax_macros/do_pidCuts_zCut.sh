#!/bin/bash

curdir=`pwd`

if [[ "$#" != "1" ]]; then
  echo "requires: ql_pidCut_zCut.sh <cm244|cf252>"
  exit 0
fi

if [[ "$1" != "cf252" ]] && [["$1" != "cm244" ]]; then
  echo "incorrect name"
  echo "requires: ql_pidCut_zCut.sh <cm244|cf252>"
  exit "00"
fi

if [[ "$1" == "cf252" ]]; then
  # noMask1_dv442=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/noMask1_noGhostTail.root
  noMask2_dv442=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/noMask2_noGhostTail.root
  # noMask1_dv49=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask1_noGhostTail.root
  noMask2_dv49=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask2_noGhostTail.root
  cd $cft/pid/zCut
  # $macros/pidCut_zCut.sh -actinide "cf252" -driftVel 4.42  -preamp 1 -noMaskTree ${noMask1_dv442} &
  $macros/pidCut_zCut.sh -actinide "cf252" -driftVel 4.42  -preamp 2 -noMaskTree ${noMask2_dv442} &
  # $macros/pidCut_zCut.sh -actinide "cf252" -driftVel 4.9 -preamp 1 -noMaskTree ${noMask1_dv49} &
  # $macros/pidCut_zCut.sh -actinide "cf252" -driftVel 4.9 -preamp 2 -noMaskTree ${noMask2_dv49} &
  cd $curdir
fi
if [[ "$1" == "cm244" ]]; then
  noMask1_dv442=$trees/cm244.nEv-1.driftVel4.42.houghKalman.24Jun16_1144.39/noMask1.trees.root
  cd $cft/pid/zCut
  $macros/pidCut_zCut.sh -actinide "cm244" -driftVel 4.42  -preamp 1 -noMaskTree ${noMask1_dv442}
  wait
  cd $curdir
  exit
fi

find $cft/pid/zCut >/dev/null &
