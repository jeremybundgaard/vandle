#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "ornl2016_vandlePlot.sh -file_paths /full/path/to/root_tree.txt "
	echo 'other inputs eg: -cuts \"vandle_QDC!=0\"'
	echo
	exit
}

cuts=""

array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-h" ]] || [[ "${array[$i-1]}" = "--help" ]]; then
		usage
	fi
	if [[ "${array[$i-1]}" = "-file_paths" ]]; then
		file_paths="${array[$i]}"
		echo "-file_paths $file_paths"
	fi
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
done

if [[ ! -f $file_paths ]]; then
	usage
fi

ORNL2016_workDir=`pwd`/ornl2016_vandlePlots_`date +%d%b%y_%H%M.%S`
mkdir $ORNL2016_workDir; cd $ORNL2016_workDir

# cat $file_paths|while read i
# do
# 	$macros/vandlePlots.sh -vandle_tree ${i} &
# done

cat $file_paths|while read i
do
	$macros/vandlePlots.sh -vandle_tree ${i} &
	# $macros/vandlePlots_varyNeutron_TCut.sh -vandle_tree ${i} &
	# $macros/ornl2016_gammaPlots.sh -gamma_tree ${i} &
done

# nLock="`ls $ORNL2016_workDir/*LOCK|wc`"
# while [ "`ls $ORNL2016_workDir/*LOCK|wc -l`" ]
# do
# 	echo "waiting for these jobs to finish `ls $ORNL2016_workDir/*LOCK`"
# 	sleep 1
# done


# gm montage -mode concatenate -tile 3x3 \
# $ORNL2016_workDir/*/92*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/93*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/94*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/95*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/96*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/97*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/98*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/99*QDCvsTOF_grid.png \
# $ORNL2016_workDir/*/100*QDCvsTOF_grid.png \
# $ORNL2016_workDir/combine_ornl2016_neutronTOF.png
#
# cd - > /dev/null 2>&1
#
# gm montage -mode concatenate -tile 3x3 \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{92}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{93}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{94}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{95}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{96}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{97}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{98}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{99}*QDCvsTOF_grid.png` \
# `ls $ORNL2016_workDir/ornl2016_vandlePlots*/{100}*QDCvsTOF_grid.png` \
# $ORNL2016_workDir/combine_ornl2016_neutronTOF.png
