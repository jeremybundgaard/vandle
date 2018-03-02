#!/bin/bash

cuts="1"
driftVel="4.42"
launch="local"
n=100000
echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
	if [[ "${array[$i-1]}" = "-launch" ]]; then
		launch="${array[$i]}"
		echo "-launch $launch"
	fi
	if [[ "${array[$i-1]}" = "-n" ]]; then
		n="${array[$i]}"
		echo "-n $n"
	fi
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		echo "-driftVel $driftVel"
	fi
done

pidFile1="$pid/cf252_pidCut_preamp1_zCut70_fragThresh1.7.driftVel4.42.root"
pidFile2="$pid/cf252_pidCut_preamp2_zCut30_fragThresh1.3.driftVel4.42.root"
driftVel=driftVel${driftVel}

topdir=`pwd`/cf252_all_cftAnaPlots_`date +%d%b%y_%H%M.%S`
echo $topdir
mkdir $topdir
cd $topdir

if [[ "$n" = "-1" ]]; then
	noMaskTree1=$trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/noMask1_noGhostTail.root
	noMaskTree2=$trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/noMask2_noGhostTail.root
	cftTree1=$trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/cft1_noGhostTail.root
	cftTree2=$trees/noGhostTails/cf252.nEv-1.driftVel4.42.houghKalman_hadd_trees/cft2_noGhostTail.root
else
	noMaskTree1=$trees/noGhostTails/cf252.nEv100000.driftVel4.42.houghKalman_hadd_trees/noMask1_noGhostTail.root
	noMaskTree2=$trees/noGhostTails/cf252.nEv100000.driftVel4.42.houghKalman_hadd_trees/noMask2_noGhostTail.root
	cftTree1=$trees/noGhostTails/cf252.nEv100000.driftVel4.42.houghKalman_hadd_trees/cft1_noGhostTail.root
	cftTree2=$trees/noGhostTails/cf252.nEv100000.driftVel4.42.houghKalman_hadd_trees/cft2_noGhostTail.root
fi

preamp=1
launchScript1=launchScript1_preamp${preamp}.sh
echo "#!/bin/bash" \
$'\n\n' \
$macros/cftAnaPlots.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree $noMaskTree1 \
-cftTree $cftTree1 \
-driftVel $driftVel \
-pidFile $pidFile1 \
>> $launchScript1
chmod 750 $launchScript1

preamp=2
launchScript2=launchScript2_preamp${preamp}.sh
echo "#!/bin/bash" \
$'\n\n' \
$macros/cftAnaPlots.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree $noMaskTree2 \
-cftTree $cftTree2 \
-driftVel $driftVel \
-pidFile $pidFile2 \
>> $launchScript2
chmod 750 $launchScript2

if [[ "$launch" = "msub" ]]; then
	msub -q pbatch -l walltime=3600 $launchScript2
	# msub -q pbatch -l walltime=3600 $launchScript1
else
	time ./$launchScript2 &
	mv $launchScript2 cftAnaPlots.*/backups/.
fi

find $topdir >/dev/null &

exit
