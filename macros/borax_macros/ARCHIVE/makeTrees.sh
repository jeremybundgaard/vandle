#!/bin/bash

if [ $# -lt 1 ]||[ $# -gt 2 ] ;then

	echo "number of args = $#"
	echo "1 arg USAGE: ./makeTrees.sh <runListInputFile>"
	echo "2 arg USAGE: ./makeTrees.sh <runListInputFile> <nEvents>"
	exit
fi

runListInputFile=$1
nEvents=-1
if [ $# -gt 1 ];then
	nEvents=$2
fi

outputdir=trees.`basename $runListInputFile .list`
mkdir $outputdir 
cp $macros/makeTrees.C $outputdir/.
cp $runListInputFile $outputdir/.
cd $outputdir

outputRootHistogramFile=histograms.`basename $runListInputFile .list`.nEvents-$nEvents.root
logFile=log.`basename $runListInputFile .list`.nEvents-$nEvents
echo "began " `date` >> $logFile
root -b -q makeTrees.C\+\(\"$runListInputFile\"\,\"$outputRootHistogramFile\",$nEvents\) >> $logFile
echo `date` >> $logFile

rm anaHistMaker_*

cd - 