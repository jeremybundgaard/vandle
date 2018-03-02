#!/bin/bash

if [[ $# != 1 ]]; then
	echo "USAGE: $macros/ghostTrackTrimmer.sh /full/path/to/topDir" # topdir contains the noMask directory
	exit
fi

topdir=$1
cd $topdir
echo $topdir
find $topdir -name "*.trees.root" |while read TPCSummaryTree
do
	msub -q pbatch -l walltime=1:00:00 $macros/ghostTrackTrimmer.sh $TPCSummaryTree
	echo
done
