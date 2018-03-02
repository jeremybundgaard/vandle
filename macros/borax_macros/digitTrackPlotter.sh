#!/bin/bash

usage ()
{
	echo '***************************************************************************'
	echo '*******  README  *******  README  *******  README  *******  README  *******'
	echo '***************************************************************************'
	echo
	echo 'the below example should run on aztec as the TFiles are in the /p/lscratchd/tpcuser dir'
	echo
	echo '$NIFFTE/sandbox/jbundgaard/digitTrackPlotter/digitTrackPlotter.sh -TPCSummaryDigitTree /p/lscratchd/tpcuser/cft_data/thesisData/trees/digTrkTreeDir/noMask2.driftVel4.42_trk_dig.2k.trees.root -TPCSummaryTree /p/lscratchd/tpcuser/cft_data/thesisData/trees/cf252.noMask2.nEv2k.trees.root'
	echo
	echo '**********  USAGE  **********'
	echo "required input: -TPCSummaryDigitTree </full/path/to/TPCSummaryDigitTree> -TPCSummaryTree </full/path/to/TPCSummaryTree>"
	echo
	echo
	echo 'to create a TPCSummaryDigitTree reconstruct a raw file using the TPCSummaryDigitTree module'
	echo 'an existing hough kalman reco config file => $NIFFTE/sandbox/jbundgaard/digitTrackPlotter/digitTrack_houghKalman.xml'
 	echo '*****these trees get big quickly ~2gb/100kevents*****'
	echo 'they have tracks and digits and are NOT populated efficiently (ie relationally)'
	echo 'every branch has an entry for each digit in an event.'
	echo
	echo 'other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\"'
	echo
	exit
}

cuts="1"

echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
	if [[ "${array[$i-1]}" = "-h" ]]; then
		usage
	fi
	if [[ "${array[$i-1]}" = "--help" ]]; then
		usage
	fi
	if [[ "${array[$i-1]}" = "-preamp" ]]; then
		preamp="${array[$i]}"
		echo "-preamp $preamp"
	fi
	if [[ "${array[$i-1]}" = "-actinide" ]]; then
		actinide="${array[$i]}"
		echo "-actinide $actinide"
	fi
	if [[ "${array[$i-1]}" = "-TPCSummaryTree" ]]; then
		TPCSummaryTree="${array[$i]}"
		echo "-TPCSummaryTree $TPCSummaryTree"
	fi
	if [[ "${array[$i-1]}" = "-TPCSummaryDigitTree" ]]; then
		TPCSummaryDigitTree="${array[$i]}"
		echo "-TPCSummaryDigitTree $TPCSummaryDigitTree"
	fi
	if [[ "${array[$i-1]}" = "-pidFile" ]]; then
		pidFile="${array[$i]}"
		echo "-pidFile $pidFile"
	fi
done

if [[ ! -f $TPCSummaryDigitTree ]]; then
	echo "-TPCSummaryDigitTree file doesn't exist"
	usage
fi
if [[ ! -f $TPCSummaryTree ]]; then
	echo "-TPCSummaryTree file doesn't exist"
	usage
fi
if [[ ! -f $pidFile ]]; then
	echo "-pidFile file doesn't exist"
	usage
fi

plotdir=`pwd`/plots_digitTrackPlotter.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp ~/macros/digitTrackPlotter.{sh,C} $plotdir

echo root -l -q -b digitTrackPlotter.C\+\($preamp,\"$actinide\",\"$cuts\",\"$TPCSummaryTree\",\"$TPCSummaryDigitTree\",\"$pidFile\"\)
time root -l -q -b digitTrackPlotter.C\+\($preamp,\"$actinide\",\"$cuts\",\"$TPCSummaryTree\",\"$TPCSummaryDigitTree\",\"$pidFile\"\)
# wait
#
# mkdir backups pngs
# mv digitTrackPlotter.{sh,C} backups/.
# mv *.png pngs/.
