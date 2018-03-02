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


pidCuts_stdDev_plotdir=pidCut${preamp}plots.`date +%d%b%y_%H%M.%S`
mkdir $pidCuts_stdDev_plotdir
cp $macros/pidCuts.C $pidCuts_stdDev_plotdir
cd $pidCuts_stdDev_plotdir
echo root -l -q -b pidCuts.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)
time root -l -q -b pidCuts.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)
wait

open .
for i in `seq 200 5 375`
do
	gm montage -mode concatenate \
	AlphaBand_proj.lengthBin$i*.png \
	noMaskLvEn_cutPoints.y$i*.png \
	montage.projx.LvEn.$i.png
	rm AlphaBand_proj.lengthBin$i*.png
	rm noMaskLvEn_cutPoints.y$i*.png
done
gm convert -loop 0 -delay 3 \
`ls montage.projx.LvEn.*| sort` \
`ls montage.projx.LvEn.*| sort -r` \
montage.projx.LvEn.gif

mkdir pngs primitiveAndBackups
mv *.C primitiveAndBackups
mv montage.projx.LvEn*.png pngs/.

# for i in `seq 2 9`
# do
# 	gm montage -mode concatenate -tile 1x \
# 	lengthProj_alphas_meanShifted_stdDev$i.png \
# 	noMaskLvEn_alphas_meanShifted_stdDev$i.png \
# 	montage.alphas_meanShifted_stdDev$i.png
# done

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_frags_stdDev?.png| sort` \
`ls noMaskLvEn_frags_stdDev?.png| sort -r` \
noMaskLvEn_frags.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_alphas_stdDev?.png| sort` \
`ls noMaskLvEn_alphas_stdDev?.png| sort -r` \
noMaskLvEn_alphas.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls montage.alphas_meanShifted_stdDev?.png| sort` \
`ls montage.alphas_meanShifted_stdDev?.png| sort -r` \
alphas_meanShifted.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_linEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_linearZoom_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_linearZoom_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_linLowEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_full_logEnergy_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_full_logEnergy_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_full_logEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_full_linearEnergy_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_full_linearEnergy_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_full_linearEnergy.gif

find . -name "*\(?\)*" -exec rm {} \;
wait
mv *.png pngs/.
wait
tar czf pngs.tar.gz pngs
rm -rf pngs
# rm AlphaBand_proj.lengthBin*.png noMaskLvEn_cutPoints.y*.png 
# rm lengthProj_alphas_meanShifted_stdDev*.png noMaskLvEn_alphas_meanShifted_stdDev*.png
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
