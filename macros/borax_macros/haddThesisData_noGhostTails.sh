#!/bin/bash

if [[ "$#" -ne "1" ]]; then
	echo "USAGE: haddThesisTrees.sh </full/path/to/topdir>"
	exit
fi

topdir=$1

find $topdir/noMask/1stPreamp -name "*_noGhostTail.root" -exec mv {} $topdir/noMask/1stPreamp/. \;
find $topdir/noMask/2ndPreamp -name "*_noGhostTail.root" -exec mv {} $topdir/noMask/2ndPreamp/. \;
find $topdir/cft/1stPreamp -name "*_noGhostTail.root" -exec mv {} $topdir/cft/1stPreamp/. \;
find $topdir/cft/2ndPreamp -name "*_noGhostTail.root" -exec mv {} $topdir/cft/2ndPreamp/. \;
wait

hadd $topdir/noMask1_noGhostTail.root $topdir/noMask/1stPreamp/niffte-run*_noGhostTail.root
hadd $topdir/noMask2_noGhostTail.root $topdir/noMask/2ndPreamp/niffte-run*_noGhostTail.root
hadd $topdir/cft1_noGhostTail.root $topdir/cft/1stPreamp/niffte-run*_noGhostTail.root
hadd $topdir/cft2_noGhostTail.root $topdir/cft/2ndPreamp/niffte-run*_noGhostTail.root
wait

# if [[ -f $topdir/cft/cft1.trees.root ]] && [[ -f $topdir/cft/cft2.trees.root ]] && [[ -f $topdir/noMask/noMask1.trees.root ]] && [[ -f $topdir/noMask/noMask1.trees.root ]] ; then
# 	find $topdir -type d -exec rm -rf {} \;
# fi
