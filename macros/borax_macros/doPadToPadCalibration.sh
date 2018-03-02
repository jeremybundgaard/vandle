#!/bin/bash

launchdir=`pwd`
dataType=$1
nEvents=-1
inputRawFile=/p/lscratche/jerbundg/data/thesisData/raw2013/cm/noMask/1stPreamp/niffte-run100001922_20Jun2013_11-04.raw
workdir=$launchdir/$dataType/p2pCal.nEvents$nEvents\_`date +%d%b%y_%H%M.%S`

if [ "$#" -gt 0 ];then
	if [[ $dataType = "real" ]]; then
		inputRawFile=/p/lscratche/jerbundg/data/thesisData/raw2013/cm/noMask/1stPreamp/niffte-run100001922_20Jun2013_11-04.raw
	fi
	if [[ $dataType = "sim" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_full_reco_66.root
	fi
	if [[ $dataType = "sim1p" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_pass4-full-map77-1p_reco_01.root
		truthMap=/p/lscratchd/tpcuser/data/simulation/maps/map077_1p.root
	fi
	if [[ $dataType = "sim3p" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_pass4-full-map77-3p_reco_01.root
		truthMap=/p/lscratchd/tpcuser/data/simulation/maps/map077_3p.root
	fi
	if [[ $dataType = "sim5p" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_pass4-full-map77-5p_reco_01.root
		truthMap=/p/lscratchd/tpcuser/data/simulation/maps/map077_5p.root
	fi
	if [[ $dataType = "sim20p" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_pass4-full-map77_reco_01.root
		truthMap=/p/lscratchd/tpcuser/data/simulation/maps/map077_20p.root
	fi
	if [[ $dataType = "simSpiral" ]]; then
		inputRawFile=/p/lscratchd/tpcuser/data/simulation/reco/alpha_iso_cent_pass4-full-spiral_reco_07.root
		truthMap=/p/lscratchd/tpcuser/data/simulation/maps/spiral.root
	fi
fi


if [ "$#" -eq 2 ];then
	nEvents=$2
fi
if [ "$#" -ne "1" ] && [ "$#" -ne "2" ]
then
	echo '$#='$#
	echo "USAGE: ./doPadToPadCalibration <dataType>"
	echo "USAGE: ./doPadToPadCalibration <dataType> <nEvents> "
	exit
fi

mkdir -p $dataType
workdir=$launchdir/$dataType/p2p.$dataType.n$nEvents\_`date +%d%b%y_%H%M.%S`
mkdir $workdir
logfile=$workdir/log.`basename $workdir`
cd $workdir

## set theta range: [20-70]
setThetaMin="int thetaMin=20;"
setThetaWindow="int thetaWindow=50;"
thmin=`grep "int thetaMin=" $NIFFTE/tpc/calib/TPCPadToPadCalibration.cxx`
sed -i "s/$thmin/$setThetaMin/g" $NIFFTE/tpc/calib/TPCPadToPadCalibration.cxx
thWindow=`grep "int thetaWindow=" $NIFFTE/tpc/calib/TPCPadToPadCalibration.cxx`
sed -i "s/$thWindow/$setThetaWindow/g" $NIFFTE/tpc/calib/TPCPadToPadCalibration.cxx

thmin=`grep "int thetaMin=" $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.cxx`
sed -i "s/$thmin/$setThetaMin/g" $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.cxx
thWindow=`grep "int thetaWindow=" $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.cxx`
sed -i "s/$thWindow/$setThetaWindow/g" $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.cxx

# compile any changes
cd $NIFFTE_BUILD
cmake -DCMAKE_INSTALL_PREFIX=$NIFFTE_INSTALL $NIFFTE
make install
cd -
export DB_FAIL_OK=1

## save copy of source for this run
mkdir $workdir/sourcebackups
cp $pad2pad/doPadToPadCalibration.sh $workdir/sourcebackups/.
cp $NIFFTE/tpc/calib/TPCPadToPadCalibration.{cxx,h} $workdir/sourcebackups/.
cp $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.{cxx,h} $workdir/sourcebackups/.

if [[ ! -d $pad2pad/sourcebackups ]]; then
	mkdir $pad2pad/sourcebackups
fi
backupdir=$pad2pad/sourcebackups/`date +%d%b%y_%H%M.%S`
mkdir $backupdir
cp $pad2pad/doPadToPadCalibration.sh $backupdir/.
cp $NIFFTE/tpc/calib/TPCPadToPadCalibration.{cxx,h} $backupdir/.
cp $NIFFTE/tpc/calib/TPCBuildAverageBraggCurve.{cxx,h} $backupdir/.


################################ TPCBuildAverageBraggCurve ###############################
mkdir $workdir/avgBC
cd $workdir/avgBC

echo ;echo "start building average bragg curves:   `date +%d%b%y_%H%M.%S` ";echo
echo "niffte -c $NIFFTE/config/examples/TPCBuildAverageBraggCurve.xml -i $inputRawFile -s 123 -n $nEvents -a trees.root -o dummy.root"
niffte -c $NIFFTE/config/examples/TPCBuildAverageBraggCurve.xml -i $inputRawFile -s 123 -n $nEvents -a trees.root -o dummy.root >> $logfile
wait
export THE_avgBraggCurve=`find $workdir/avgBC -name braggCurves.root`
echo; echo "created THE_avgBraggCurve: $THE_avgBraggCurve  `date +%d%b%y_%H%M.%S` "; echo

for i in `seq 10 10 100`
do
	echo "waiting for THE_avgBraggCurve to be written $i% done"
	sleep $i
done 

# ################################ TPCPadToPadCalibration ###############################

mkdir $workdir/scalefactors
cd $workdir/scalefactors
cp $truthMap $workdir/scalefactors/.

export p2p_truthMap=$truthMap

echo; echo "start computing scale factors:   `date +%d%b%y_%H%M.%S` "; echo 
echo "niffte -c $NIFFTE/config/examples/TPCPadToPadCalibration.xml -i $inputRawFile -s 123 -n $nEvents -a trees.root -o dummy.root"
niffte -c $NIFFTE/config/examples/TPCPadToPadCalibration.xml -i $inputRawFile -s 123 -n $nEvents -a trees.root -o dummy.root >> $logfile
echo; echo "scale factors computed:   `date +%d%b%y_%H%M.%S` "; echo

pngdir=$workdir/pngs_${dataType}
mkdir $pngdir
find $workdir -name "dummy.root" -exec rm {} \;
find $workdir -name "*png" -exec mv {} $pngdir \;
gm convert -loop 0 -delay 5 $pngdir/evt_*png $pngdir/perEvent_BraggCurves_IntegratedCharge.gif
rm $pngdir/evt_*png
mkdir $pngdir/lengthBinScalesPerEvt $pngdir/braggCurvePerEvt
find $pngdir -name "lengthBinScalesPerEvt*png" -exec mv {} $pngdir/lengthBinScalesPerEvt \;
find $pngdir -name "braggCurve*png" -exec mv {} $pngdir/braggCurvePerEvt \;
cd ${launchdir}
