#!/bin/bash

if [[ $# != 2 ]]; then
	echo "USAGE: $macros/compDiffRuns_cf252.sh <cfCFT1|cfCFT2|cfNoMask1|cfNoMask2> <inputTreeFile.root>"
	exit
fi

runType=$1
inputTreeFile=$2
plotdir=`pwd`/compDiffRuns_cf252.${runType}${recoType}.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/compDiffRuns_cf252.{sh,C} $plotdir

echo root -l -q -b compDiffRuns_cf252.C\+\(\"$runType\",\"$inputTreeFile\"\)
time root -l -q -b compDiffRuns_cf252.C\+\(\"$runType\",\"$inputTreeFile\"\)
wait

rm compDiffRuns_cf252_C*

