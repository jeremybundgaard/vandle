#!/bin/bash

usage ()
{
	echo '***************************************************************************'
	echo '*******  README  *******  README  *******  README  *******  README  *******'
	echo '***************************************************************************'
	echo
	echo '**********  USAGE  **********'
	echo "required input: -TPCSummaryDigitTree </full/path/to/TPCSummaryDigitTree> \""
	echo "-TPCSummaryTree </full/path/to/TPCSummaryTree> \""
	echo "-pidFile </full/path/to/pidFile> \""
	echo "-pidCut <e.g. alphaCut_long_adc> \""
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
	if [[ "${array[$i-1]}" = "-launch_dir" ]]; then
		launch_dir="${array[$i]}"
		echo "-launch_dir $launch_dir"
	fi
	if [[ "${array[$i-1]}" = "-pidCut" ]]; then
		pidCut="${array[$i]}"
		echo "-pidCut $pidCut"
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
if [[ -z $pidCut ]]; then
	echo "-pidCut file doesn't exist"
	usage
fi

plotdir=`pwd`/plots_digitTrackPlotter_pidCuts.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir

echo "root -l -q -b $launch_dir/digitTrackPlotter_pidCuts.C\+\($preamp,\"$actinide\",\"$pidCut\",\"$cuts\",\"$TPCSummaryTree\",\"$TPCSummaryDigitTree\",\"$pidFile\"\)"
time  root -l -q -b $launch_dir/digitTrackPlotter_pidCuts.C\+\($preamp,\"$actinide\",\"$pidCut\",\"$cuts\",\"$TPCSummaryTree\",\"$TPCSummaryDigitTree\",\"$pidFile\"\)
