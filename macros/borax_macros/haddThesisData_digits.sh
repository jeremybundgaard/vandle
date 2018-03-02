#!/bin/bash

if [[ "$#" -ne "1" ]]; then
	echo "USAGE: haddThesisTrees.sh </full/path/to/topdir>"
	exit
fi

topdir=$1

# find $topdir/noMask/1stPreamp -name "*.trees.root" -exec mv {} $topdir/noMask/1stPreamp/. \;
# find $topdir/noMask/2ndPreamp -name "*.trees.root" -exec mv {} $topdir/noMask/2ndPreamp/. \;
# find $topdir/cft/1stPreamp -name "*.trees.root" -exec mv {} $topdir/cft/1stPreamp/. \;
# find $topdir/cft/2ndPreamp -name "*.trees.root" -exec mv {} $topdir/cft/2ndPreamp/. \;
# wait

hadd $topdir/noMask2.bsDigitTrees.trees.root $topdir/noMask/2ndPreamp/niffte-run*.trees.root &
hadd $topdir/cft2.bsDigitTrees.trees.root $topdir/cft/2ndPreamp/niffte-run*.trees.root &
hadd $topdir/noMask1.bsDigitTrees.trees.root $topdir/noMask/1stPreamp/niffte-run*.trees.root &
hadd $topdir/cft1.bsDigitTrees.trees.root $topdir/cft/1stPreamp/niffte-run*.trees.root &
wait

# if [[ -f $topdir/cft/cft1.trees.root ]] && [[ -f $topdir/cft/cft2.trees.root ]] && [[ -f $topdir/noMask/noMask1.trees.root ]] && [[ -f $topdir/noMask/noMask1.trees.root ]] ; then
# 	find $topdir -type d -exec rm -rf {} \;
# fi
