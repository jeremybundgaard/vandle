#!/bin/bash

actinide="cf252"
cuts="1"

usage ()
{
	echo "required input: -preamp <1|2> -actinide <cf252|cm244> -pidFile </full/path/to/pidFile.trees.root> -noMaskTree </full/path/to/noMaskTree.trees.root> -cftTree </full/path/to/cftTree.trees.root>"
	echo
	exit
}

cuts="1"
actinide="cf252"
addGuide="nDigits"
echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-preamp" ]]; then
		preamp="${array[$i]}"
		echo "-preamp $preamp"
	fi
	if [[ "${array[$i-1]}" = "-actinide" ]]; then
		actinide="${array[$i]}"
		echo "-actinide $actinide"
	fi
	if [[ "${array[$i-1]}" = "-pidFile" ]]; then
		pidFile="${array[$i]}"
		echo "-pidFile $pidFile"
	fi
	if [[ "${array[$i-1]}" = "-noMaskTree" ]]; then
		noMaskTree="${array[$i]}"
		echo "-noMaskTree $noMaskTree"
	fi
	if [[ "${array[$i-1]}" = "-cftTree" ]]; then
		cftTree="${array[$i]}"
		echo "-cftTree $cftTree"
	fi
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		echo "-driftVel $driftVel"
	fi
	if [[ "${array[$i-1]}" = "-addGuide" ]]; then
		addGuide="${array[$i]}"
		echo "-addGuide $addGuide"
	fi
done

if [[ -z $preamp ]]; then
	echo "-preamp not set"
	usage
fi
if [[ ! -f $pidFile ]]; then
	echo "file doesn't exist: $pidFile"
	exit
fi
if [[ ! -f $noMaskTree ]]; then
	echo "file doesn't exist: $noMaskTree"
	exit
fi
if [[ ! -f $cftTree ]]; then
	echo "file doesn't exist: $cftTree"
	exit
fi

topdir=`pwd`/digitScan_MinThresh.`basename $pidFile .root`.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir
cp $macros/makeNewTree.sh $topdir/.

# used to get number of jobs in the queue
set_nJobs ()
{
	nJobs=`showq -u jerbundg | grep "Total jobs:"`
	nJobs=${nJobs#Total jobs:}
	nJobs=${nJobs##*()}
}

for dig_min in `seq  0 5 150`
do
	cuts="$dig_min<nDigits"
	jobtest=190
	set_nJobs
	# pause the loop while the ques is unavailable
	echo "nJobs:$nJobs jobtest:$jobtest"
	while [ "$nJobs" -gt "$jobtest" ]
	do
		echo "waiting for $((nJobs-jobtest)) jobs to finish as of `date`"
		sleep 20
		set_nJobs
		echo "nJobs:$nJobs jobtest:$jobtest"
	done

	dig_min_dir=dig_min$(printf "%03d" $dig_min)
	mkdir $dig_min_dir
	cd $dig_min_dir

	$topdir/makeNewTree.sh $noMaskTree noMaskTree_cutTree.root $cuts
	mv makeNewTree*/noMaskTree_cutTree.root .
	wait
	rm -rf makeNewTree*
	$topdir/makeNewTree.sh $cftTree cftTree_cutTree.root $cuts
	mv makeNewTree*/cftTree_cutTree.root .
	wait
	rm -rf makeNewTree*

	echo sh $macros/cftAnaPlots.sh \
	-preamp $preamp \
	-actinide $actinide \
	-noMaskTree `pwd`/noMaskTree_cutTree.root \
	-cftTree `pwd`/cftTree_cutTree.root \
	-pidFile $pidFile \
	-cuts \"$cuts\" \
	-addGuide "nDigits" \
	-driftVel $driftVel >> launch_minDig_all_cftAnaPlots_pid.$dig_min.sh
	chmod 750 launch_minDig_all_cftAnaPlots_pid.$dig_min.sh
	cat launch_minDig_all_cftAnaPlots_pid.$dig_min.sh
	wait
	sleep 1
	# ./launch_minDig_all_cftAnaPlots_pid.$dig_min.sh
	msub -l walltime=4444 -q pbatch `pwd`/launch_minDig_all_cftAnaPlots_pid.$dig_min.sh

	set_nJobs
	cd -
	echo;echo
done
