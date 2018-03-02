#!/bin/bash

if [ $# -lt 1 ]||[ $# -gt 2 ] ;then

	echo "number of args = $#"
	echo "1 arg USAGE: ./anaHistMaker.SH </absolute/path/to/runListInputFile>"
	echo "2 arg USAGE: ./anaHistMaker.SH </absolute/path/to/runListInputFile> <nEvents>"
	exit
fi

runListInputFile=$1
nEvents=-1
if [ $# -gt 1 ];then
	nEvents=$2
fi

tmp=`basename $runListInputFile`
outputdir=histograms.${tmp:10:9}
plotdir=$outputdir/plots
mkdir $outputdir $plotdir
cp $ana/macros/anaHistMaker.C $outputdir
cp $ana/macros/plotMaker.C $outputdir
cd $outputdir

outputRootHistogramFile=histograms.`basename $runListInputFile .txt`.nEvents-$nEvents.root
logFile=log.`basename $runListInputFile .txt`.nEvents-$nEvents
echo "began " `date` >> $logFile
root -b -q anaHistMaker.C\+\(\"$runListInputFile\"\,\"$outputRootHistogramFile\",$nEvents\) >> $logFile
echo `date` >> $logFile



root -q -b plotMaker.C\+\(\"$outputRootHistogramFile\"\)
sleep 5

mv *png $plotdir/.
rm anaHistMaker* plotMaker*

cd -