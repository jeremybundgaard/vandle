#!/bin/bash

if [[ $# != 1 ]]; then
	echo "USAGE: $macros/ghostTrackTrimmer.sh <TPCSummaryTree.root>"
	exit
fi

# sleep $((RANDOM % 100 + 1))s
TPCSummaryTree=$1
otherTreeCuts="1"
TPCSummaryTree_basename=`basename $TPCSummaryTree .trees.root`
TPCSummaryTree_dirname=`dirname $TPCSummaryTree`
plotdir=$TPCSummaryTree_dirname/ghostTrackTrimmer.`basename $TPCSummaryTree .root`.`date +%d%b%y_%H%M.%S`
mkdir -p $plotdir
mkdir -p $plotdir/ghostTail $plotdir/no_ghostTail
cd $plotdir
cp $macros/ghostTrackTrimmer.{sh,C} $plotdir
logfile=logfile.`date +%d%b%y_%H%M.%S`.txt
echo root -l -q -b ghostTrackTrimmer.C\+\(\"$TPCSummaryTree\",\"$TPCSummaryTree_basename\",\"$otherTreeCuts\"\)
time root -l -q -b ghostTrackTrimmer.C\+\(\"$TPCSummaryTree\",\"$TPCSummaryTree_basename\",\"$otherTreeCuts\"\)
# time root -l -q -b ghostTrackTrimmer.C\+\(\"$TPCSummaryTree\",\"$TPCSummaryTree_basename\",\"$otherTreeCuts\"\) 2>&1 | tee $logfile
wait

rm ghostTrackTrimmer_C*
