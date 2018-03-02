#!/bin/bash


usage ()
{
	echo "required input: -preamp <1|2> -actinide <cf252|cm244> -noMaskTree </path/to/noMaskTree.trees.root> -cftTree </path/to/cftTree.trees.root>"
	echo s
	echo "other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\" "
	echo
	exit
}

cuts="1"
pidType="linear"
actinide="cf252"

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
	if [[ "${array[$i-1]}" = "-pidType" ]]; then
		pidType="${array[$i]}"
		echo "-pidType $pidType"
	fi
done

if [[ -z $preamp ]]; then
	echo "-preamp not set"
	usage
fi
if [[ ! -f $noMaskTree ]]; then
	echo "file doesn't exist: $noMaskTree"
	exit
fi
if [[ ! -f $cftTree ]]; then
	echo "file doesn't exist: $cftTree"
	exit
fi


plotdir=`pwd`/vertexCft.${actinide}.${preamp}preamp.pidType$pidType.pidCut$pidCut.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/vertexCft.{sh,C} $plotdir

echo root -l -q -b vertexCft.C\+\($preamp,\"$actinide\"\,\"$noMaskTree\"\,\"$cftTree\"\,\"$cuts\"\,\"$pidType\"\)
time root -l -q -b vertexCft.C\+\($preamp,\"$actinide\"\,\"$noMaskTree\"\,\"$cftTree\"\,\"$cuts\"\,\"$pidType\"\)
wait
mkdir backups pngs grid_plots
mv vertexCft.{sh,C} backups/.


find . -name "*\(?\)*.*" -exec rm {} \;
# mv 2x2grid*png grid_plots/.
# mv *.png pngs/.

# tar -zcf backups.tar.gz backups 
# tar -zcf pngs.tar.gz pngs 
# rm -rf backups pngs

rm vertexCft_pid_*
date