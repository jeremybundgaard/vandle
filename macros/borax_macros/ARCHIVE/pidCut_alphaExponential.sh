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
echo

if [[ -z $preamp ]]; then
	echo "-preamp not set"
	usage
fi
if [[ ! -f $noMaskTree ]]; then
	echo "file doesn't exist: $noMaskTree"
	exit
fi


pidCut_alphaExponential_plotdir=pidCut_alphaExponential_preamp${preamp}plots.`date +%d%b%y_%H%M.%S`
mkdir $pidCut_alphaExponential_plotdir
cp $macros/pidCut_alphaExponential.C $pidCut_alphaExponential_plotdir
cd $pidCut_alphaExponential_plotdir
echo root -l -q -b pidCut_alphaExponential.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)>>log.stdout.txt
time root -l -q -b pidCut_alphaExponential.C\+\($preamp,\"$noMaskTree\"\,\"$cuts\"\)>>log.stdout.txt
wait

for i in `seq -w 0 2 100`
do
	gm montage -mode concatenate -tile 2x2 \
	noMaskLvEn_above_zCut$i.png noMaskLvEn_below_zCut$i.png \
	aaa.noMaskLvEn$i.png
done
rm noMaskLvEn_above_zCut*.png noMaskLvEn_below_zCut*.png \
gm convert -loop 0 -delay 20 \
`ls aaa.noMaskLvEn*.png|sort` `ls aaa.noMaskLvEn*.png|sort -r` zCut.gif
wait
gm convert -loop 0 -delay 20 \
`ls noMaskLvEn_alphas_zCut*.png|sort` `ls noMaskLvEn_alphas_zCut*.png|sort -r` zCut_alphas.gif
wait
gm convert -loop 0 -delay 20 \
`ls noMaskLvEn_all_zCut*.png|sort` `ls noMaskLvEn_all_zCut*.png|sort -r` zCut_all.gif
wait

for i in `seq -w 070 5 430`
do
	gm montage -mode concatenate \
	AlphaBand_proj.lengthBin$i*.png \
	noMaskLvEn_cutPoints.y$i*.png \
	montage.projx.LvEn.$i.png
	rm AlphaBand_proj.lengthBin$i*.png
	rm noMaskLvEn_cutPoints.y$i*.png
done

open .

gm convert -loop 0 -delay 3 \
`ls montage.projx.LvEn.*| sort` \
`ls montage.projx.LvEn.*| sort -r` \
montage.projx.LvEn.gif
wait
mkdir pngs backups
mv *.C backups
rm montage.projx.LvEn*.png pngs/.


find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_frags_stdDev?.png| sort` \
`ls noMaskLvEn_frags_stdDev?.png| sort -r` \
noMaskLvEn_frags.gif
wait
find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls noMaskLvEn_alphas_stdDev?.png| sort` \
`ls noMaskLvEn_alphas_stdDev?.png| sort -r` \
noMaskLvEn_alphas.gif
wait
wait
find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_linEnergy.gif
wait
find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_linearZoom_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_linearZoom_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_linLowEnergy.gif
wait
find . -name "*\(?\)*" -exec rm {} \;
gm convert -loop 0 -delay 10 \
`ls bothCuts_noMaskLvEn_full_logEnergy_stdDev?.png| sort` \
`ls bothCuts_noMaskLvEn_full_logEnergy_stdDev?.png| sort -r` \
bothCuts_noMaskLvEn_full_logEnergy.gif
wait

find . -name "*\(?\)*" -exec rm {} \;
wait
mv *.png pngs/.
wait

rsync pidCut_alphaExponential${preamp}.root $pidCut_alphaExponential_plotdir
rm *_C{.d,.so,_ACLiC_dict_rdict.pcm}

