#!/bin/bash

nEvents=50000

array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-actinide" ]]; then
		actinide="${array[$i]}"
		# echo "-actinide $actinide"
		if [[ "$actinide" = "cf252" ]]; then
			jobtest=$((164-1))
			# echo $jobtest
		elif [[ "$actinide" = "cm244" ]]; then
			jobtest=$((17-1))
			# echo $jobtest
		fi
	fi
	if [[ "${array[$i-1]}" = "-c" ]]; then
		niffteConfigFile="${array[$i]}"
		# echo "-c $niffteConfigFile"
	fi
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		# echo "-driftVel $driftVel"
	fi
	if [[ "${array[$i-1]}" = "-n" ]]; then
		nEvents="${array[$i]}"
		# echo "-n $nEvents"
	fi
done

if [[ ! -f "$niffteConfigFile" ]] || [[ -z "$actinide" ]]; then
	echo
	echo "missing required input: ./scanDriftVel.sh -actinide <cf252|cm244> -c <config.xml>"
	echo "other inputs: -n <nEvents>"
	echo
	exit
fi


topdir=driftScan.${actinide}.nEvents${nEvents}.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir

set_nJobs ()
{
	nJobs=`showq -u jerbundg | grep "Total jobs:"`
	nJobs=${nJobs#Total jobs:}
	nJobs=${nJobs##*()}
}
set_nJobs

for driftVel_iter in `seq 4.5 0.05 5.5`;
do
	set_nJobs
	echo "nJobs:$nJobs jobtest:$jobtest"
	while [ "$nJobs" -gt "$jobtest" ]
	do
		echo "waiting for $((nJobs-jobtest)) jobs to finish as of `date`"
		sleep 60
		set_nJobs
		echo "nJobs:$nJobs jobtest:$jobtest"
	done

	$macros/recoThesisData.sh -c $houghkalman -actinide $actinide -driftVel $driftVel_iter -n $nEvents
	set_nJobs
	echo "nJobs:$nJobs jobtest:$jobtest"
	sleep 10

done

