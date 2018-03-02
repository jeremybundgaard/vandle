#!/bin/bash

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
	if [[ "${array[$i-1]}" = "-pidType" ]]; then
		pidType="${array[$i]}"
		echo "-pidType $pidType"
	fi
done
if [[ -z $pidType ]]; then
	echo "-pidType not set"
	usage
fi

topdir=$cft/polar/polarCft_${pidType}.cf252.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir

cd $topdir
mkdir pidCuts
cd pidCuts

preamp=1
mkdir preamp$preamp
cd preamp$preamp
launchScript=launchScript_preamp${preamp}_pidCut${pidCut}.sh
echo $macros/polarCft.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree ${trees}/cf252.noMask$preamp.trees.root \
-cftTree ${trees}/cf252.cft$preamp.trees.root \
-pidType $pidType \
>> $launchScript
chmod 750 $launchScript
# ./$launchScript &
msub -q pbatch -l walltime=3600 $launchScript
cd -

preamp=2
mkdir preamp$preamp
cd preamp$preamp
launchScript=launchScript_preamp${preamp}_pidCut${pidCut}.sh
echo $macros/polarCft.sh \
-preamp $preamp \
-actinide cf252 \
-noMaskTree ${trees}/cf252.noMask$preamp.trees.root \
-cftTree ${trees}/cf252.cft$preamp.trees.root \
-pidType $pidType \
>> $launchScript
chmod 750 $launchScript
# ./$launchScript
msub -q pbatch -l walltime=3600 $launchScript
cd -
