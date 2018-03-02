#!/bin/bash

topdir=$cft/summaryPlots/zCut_cftAnaPlots.cf252.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir

pidType="zCut"
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
done

cd $topdir
preamp=1
mkdir preamp$preamp
cd preamp$preamp
launchScript=launchScript_preamp${preamp}_pidType${pidType}_pidCut${pidCut}.sh
echo $macros/cftAnaPlots.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree $trees/cf252.noMask$preamp.trees.root \
-cftTree $trees/cf252.cft$preamp.trees.root \
-pidType $pidType \
>> $launchScript
chmod 750 $launchScript
msub -q pbatch -l walltime=100:00 $launchScript
# $launchScript &
cd -

cd $topdir
preamp=2
mkdir preamp$preamp
cd preamp$preamp
launchScript=launchScript_preamp${preamp}_pidType${pidType}_pidCut${pidCut}.sh
echo $macros/cftAnaPlots.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree $trees/cf252.noMask$preamp.trees.root \
-cftTree $trees/cf252.cft$preamp.trees.root \
-pidType $pidType \
>> $launchScript
chmod 750 $launchScript
msub -q pbatch -l walltime=100:00 $launchScript
# $launchScript &
cd -
