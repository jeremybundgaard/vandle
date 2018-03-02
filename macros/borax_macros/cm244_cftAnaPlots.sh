#!/bin/bash


usage ()
{
	echo "required input: -preamp <1|2> -actinide <cf252|cm244> -pidFile </full/path/to/noMaskTree.trees.root> -noMaskTree </full/path/to/noMaskTree.trees.root> -cftTree </full/path/to/cftTree.trees.root>"
	echo s
	echo "other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\" "
	echo
	exit
}

cuts="1"
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


topdir=`pwd`/cm244_cftAnaPlots.${actinide}.${preamp}preamp.driftVel${driftVel}.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir
cp $macros/cm244_cftAnaPlots.{sh,C} $topdir

echo root -l -q -b cm244_cftAnaPlots.C\+\($preamp,\"$actinide\"\,\"$pidFile\"\,\"$noMaskTree\"\,\"$cftTree\"\,\"$cuts\"\,\"$driftVel\"\,\"$addGuide\"\)
time root -l -q -b cm244_cftAnaPlots.C\+\($preamp,\"$actinide\"\,\"$pidFile\"\,\"$noMaskTree\"\,\"$cftTree\"\,\"$cuts\"\,\"$driftVel\"\,\"$addGuide\"\)
wait

mkdir backups pngs grid_plots
mv cm244_cftAnaPlots.{sh,C} backups/.

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.fullEn.logx.png cftLvEn.cuts.fullEn.logx.png \
noMask_pidCuts_En_Stack_logxy_fullEn.png cft_pidCuts_En_Stack_logxy_fullEn.png \
cft_2x2grid_cuts_fullEn_logx.png
wait
echo "created cft.2x2grid.cuts.fullEn.logx.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.lowEn.png cftLvEn.cuts.lowEn.png \
noMask_pidCuts_En_Stack_logy_lowEn.png cft_pidCuts_En_Stack_logy_lowEn.png \
cft_2x2grid_cuts_lowEn_logx.png
wait
echo "created cft.2x2grid.cuts.lowEn.logx.png"

gm montage -mode concatenate -tile 2x2 \
noMaskCuts.asf.fullEn.logxy.png cftCuts.asf.fullEn.logxy.png \
cf252.preamp${preamp}.noMaskEn.fullEn.logxy.png cf252.preamp${preamp}.cftEn.fullEn.logxy.png \
cft_2x2grid_asf_fullEn_logxy.png
wait
echo "created cft.2x2grid.asf.fullEn.logx.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.fullEn.logx.png cftLvEn.fullEn.logx.png \
noMaskEn.fullEn.logx.png cftEn.fullEn.logx.png \
cft_2x2grid_noCuts_fullEn_logx.png
wait
echo "created cft.2x2grid.noCuts.fullEn.logx.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.lowEn.png cftLvEn.lowEn.png \
noMaskEn.lowEn.png cftEn.lowEn.png \
cft_2x2grid_noCuts_lowEn.png
wait
echo "created cft.2x2grid.noCuts.lowEn.png"

gm montage -mode concatenate -tile 2x2 \
noMaskCuts.asf.lowEn.png cftCuts.asf.lowEn.png \
cf252.preamp${preamp}.noMaskEn.lowEn.logy.png cf252.preamp${preamp}.cftEn.lowEn.logy.png \
cft_2x2grid_asf_lowEn_logx.png
wait
echo "created cft.2x2grid.asf.lowEn.logx.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.fullEn.logx.png cftLvEn.cuts.fullEn.logx.png \
noMask_cosTheta_Stack.png cft_cosTheta_Stack.png \
cft_2x2grid_cosTheta_Stack.png
wait
echo "created cft.2x2grid.cosTheta_Stack.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.lowEn.png noMaskLvEn.cuts.fullEn.logx.png \
noMask_pidCuts_En_Stack_logy_lowEn.png noMask_pidCuts_En_Stack_logxy_fullEn.png \
pidBins_2x2grid.png
wait
echo "created pidBins.2x2grid.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.lowEn.png noMaskLvEn.cuts.fullEn.logx.png \
noMask_pidCuts_En_Stack_logy_lowEn.png noMask_pidCuts_En_Stack_logxy_fullEn.png \
pidBins_2x2grid.png
wait
echo "created pidBins.2x2grid.png"

gm montage -mode concatenate -tile 2x2 \
noMaskLvEn.cuts.fullEn.logx.png noMask_cosTheta_Stack.png \
noAlphas_noFrags_noMaskLvEn.cuts.fullEn.logx.png noMask_NOalpha_NOfrag_cosTheta_Stack.png \
pidBins_2x2grid_noAlphasOrFrags.png
wait
echo "created pidBins.2x2grid.noAlphasOrFrags.png"


gm montage -mode concatenate -tile 2x3 \
noMaskLvEn_noCuts.png noMaskLvEn_wCuts.png \
Energy_vs_Azimuth_noCuts.png Energy_vs_Azimuth_wCuts.png \
Energy_vs_cosTheta_noCuts.png Energy_vs_cosTheta_wCuts.png \
3x2grid_with_and_wo_aziumth_cut.png
wait
echo "created 3x2grid.with_and_wo_aziumth_cut.png"

gm montage -mode concatenate -tile 2x2 \
normalization_noMask_LvsADC_fragCut_energy.png normalization_cft_Lvsenergy_fragCut_energy.png \
normalization_noMaskFragsenergy_fragCut_energy.png normalization_cftFragsADC_fragCut_energy.png \
2x2_frag_norm.png
wait
gm montage -mode concatenate -tile 1x2 \
2x2_frag_norm.png normalization_fragEn.png \
3x1grid_frag_norm_adc.png
wait
rm 2x2_frag_norm.png
echo "created 3x1grid.frag_norm_adc.png"

gm montage -mode concatenate -tile 1x2 \
cf252.preamp${preamp}.bothFragsCut.fullEn.logy.png \
cf252.preamp${preamp}.bothFragsCut.lowEn.logy.png \
1x2grid_cftEfficiency.png
wait
echo "created 1x2grid.cftEfficiency.png"

mv *grid*png grid_plots/.
mv *.png pngs/.

if [[ ! -z $addGuide ]]; then
	mv scanGuide.png grid_plots/.
	cd $topdir/grid_plots
	for quad in `ls 2x2*png`; do
		echo composite scanGuide.png -gravity center $quad ${quad%.png}scanGuide.png
		composite scanGuide.png -gravity center $quad ${quad%.png}scanGuide.png
		rm $quad
	done
	cd $topdir
fi

find $topdir -name scanGuide.png -exec rm {} \;
# tar -zcf backups.tar.gz backups
# tar -zcf pngs.tar.gz pngs
# rm -rf backups pngs

rm cm244_cftAnaPlots_* deleteme.root
date
exit 1
