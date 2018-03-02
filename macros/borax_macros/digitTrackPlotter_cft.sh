#!/bin/bash


usage ()
{
	echo "required input: -preamp <1|2> -actinide <cf252|cm244> -noMaskTree </path/to/noMaskTree.trees.root> -cftTree </path/to/cftTree.trees.root>"
	echo s
	echo "other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\" "
	echo
	exit
}
cuts="1&&fitType==5"
pidType="zCut"
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
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		echo "-driftVel $driftVel"
	fi
done

if [[ -z $preamp ]]; then
	echo "-preamp not set"
	usage
fi


plotdir=`pwd`/digitTrackPlotter_cft.${actinide}.${preamp}preamp.$pidType.pidCut$pidCut.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/digitTrackPlotter_cft.{sh,C} $plotdir

echo root -l -q -b digitTrackPlotter_cft.C\+\($preamp,\"$actinide\",\"$cuts\",\"$pidType\",\"$driftVel\"\)
time root -l -q -b digitTrackPlotter_cft.C\+\($preamp,\"$actinide\",\"$cuts\",\"$pidType\",\"$driftVel\"\)
wait

mkdir backups pngs grid_plots
mv digitTrackPlotter_cft.{sh,C} backups/.
mv *.png pngs/.

cd pngs
ls run10000205*_LVEn.png|while read i
	do
	gm montage -mode concatenate $i ${i%_LVEn.png}.png montage.${i%_LVEn.png}.png
	rm $i ${i%_LVEn.png}.png
done
gm convert -loop 0 -delay 20 montage.*png LVEn_trkDig.gif
cd -
