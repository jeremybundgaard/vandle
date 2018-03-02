#!/bin/bash


usage ()
{
	echo "required input: -preamp <1|2> -noMaskTree </path/to/noMaskTree.trees.root>"
	echo
	echo "other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\" "
	echo
	exit
}

cuts="&&1"

echo 
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-preamp" ]]; then
		preamp="${array[$i]}"
		echo "-preamp:  $preamp"
	fi
	if [[ "${array[$i-1]}" = "-noMaskTree" ]]; then
		noMaskTree="${array[$i]}"
		echo "-noMaskTree:  $noMaskTree"
	fi
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts:  $cuts"
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


pidCut_alphaLinear_plotdir=pidCut_alphaLinear_preamp${preamp}plots.`date +%d%b%y_%H%M.%S`
mkdir $pidCut_alphaLinear_plotdir
cp $macros/pidCut_alphaLinear.C $pidCut_alphaLinear_plotdir
cd $pidCut_alphaLinear_plotdir
echo root -l -q -b pidCut_alphaLinear.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)
time root -l -q -b pidCut_alphaLinear.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)
wait


mkdir pngs primitiveAndBackups
mv *.C primitiveAndBackups

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_frags_energyShift*.png| sort` \
`ls noMaskLvEn_frags_energyShift*.png| sort -r` \
noMaskLvEn_frags.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_alphas_energyShift*.png| sort` \
`ls noMaskLvEn_alphas_energyShift*.png| sort -r` \
noMaskLvEn_alphas.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_energyShift*.png| sort` \
`ls bothCuts_noMaskLvEn_energyShift*.png| sort -r` \
bothCuts_noMaskLvEn_linEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_linearZoom_energyShift*.png| sort` \
`ls bothCuts_noMaskLvEn_linearZoom_energyShift*.png| sort -r` \
bothCuts_noMaskLvEn_linLowEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_full_logEnergy_energyShift*.png| sort` \
`ls bothCuts_noMaskLvEn_full_logEnergy_energyShift*.png| sort -r` \
bothCuts_noMaskLvEn_full_logEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_full_linearEnergy_energyShift*.png| sort` \
`ls bothCuts_noMaskLvEn_full_linearEnergy_energyShift*.png| sort -r` \
bothCuts_noMaskLvEn_full_linearEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
wait
mv *.png pngs/.
wait
tar czf pngs.tar.gz pngs
rm -rf pngs
# rm AlphaBand_proj.lengthBin*.png noMaskLvEn_cutPoints.y*.png 
# rm lengthProj_alphas_meanShifted_energyShift*.png noMaskLvEn_alphas_meanShifted_energyShift*.png
# rm bothCuts_noMaskLvEn*.png
rm *_C{.d,.so,_ACLiC_dict_rdict.pcm}



# plotdir1stPreamp=pidCut2_edge.`date +%d%b%y_%H%M.%S`
# mkdir $plotdir1stPreamp
# cp $macros/pidCut/pidCut2_edge.C $plotdir1stPreamp
# cd $plotdir1stPreamp
# root -l -q -b pidCut2_edge.C\+

# for i in `seq 150 5 395`
# do
# 	gm montage -mode concatenate \
# 	projx.LvEn.ybin$i*.png \
# 	LvEn.y$i*.png \
# 	montage.projx.LvEn.$i.png
# done
# gm convert -loop 0 -delay 3 \
# `ls montage.projx.LvEn.*| sort` \
# `ls montage.projx.LvEn.*| sort -r` \
# montage.projx.LvEn.gif

# rm projx.LvEn.ybin*.png LvEn.y*.png montage.projx.LvEn*.png

# cd $cft/pidCut/.
# plotdir2ndPreamp=rotateToExpoBasis.`date +%d%b%y_%H%M.%S`
# mkdir $plotdir2ndPreamp
# cp $macros/rotateToExpoBasis.C $plotdir2ndPreamp
# cd $plotdir2ndPreamp
# root -l -q -b rotateToExpoBasis.C\+ 



# for (( nDig = 170; nDig <= 250; nDig+=5 )); do
# 	montage -geometry 1000x700 -tile 1x2 \
# 	LvEn.y${nDig}.zoom.png projx.LvEn.ybin${nDig}.zoom.png montage.LvEn.plusProj.y${nDig}.png
# 	echo "created montage.LvEn.plusProj.y${nDig}.png"
# done


# convert -delay 20 -loop 0 montage.LvEn.plusProj.y*.png montage.LvEn.plusProj.gif
# echo "created montage.LvEn.plusProj.gif"
# convert -delay 20 -loop 0 projx.LvEn.ybin*.zoom.png projx.LvEn.gif
# echo "created projx.LvEn.gif"
# convert -delay 20 -loop 0 LvEn.y*.zoom.png LvEn.gif
# echo "created LvEn.gif"

# cd -
# plotdir2ndPreamp=pidCut.2ndPreamp.`date +%d%b%y_%H%M.%S`
# mkdir $plotdir2ndPreamp
# cp $macros/pidCut/pidCut2ndPreampHK.C $plotdir2ndPreamp
# cd $plotdir2ndPreamp
# root -l -q -b pidCut2ndPreampHK.C\+ &


# for (( nDig = 170; nDig <= 250; nDig+=5 )); do
# 	montage -geometry 1000x700 -tile 1x2 \
# 	LvEn.y${nDig}.zoom.png projx.LvEn.ybin${nDig}.zoom.png montage.LvEn.plusProj.y${nDig}.png
# 	echo "created montage.LvEn.plusProj.y${nDig}.png"
# done

# convert -delay 20 -loop 0 montage.LvEn.plusProj.y*.png montage.LvEn.plusProj.gif
# echo "created montage.LvEn.plusProj.gif"
# convert -delay 20 -loop 0 projx.LvEn.ybin*.zoom.png projx.LvEn.gif
# echo "created projx.LvEn.gif"
# convert -delay 20 -loop 0 LvEn.y*.zoom.png LvEn.gif
# echo "created LvEn.gif"

open .
