

plotdir1stPreamp=plots.buildAlphaRidgeCut.1stPreamp.`date +%d%b%y_%H%M.%S`
mkdir $plotdir1stPreamp
cp $macros/buildAlphaRidgeCut1stPreamp.C $plotdir1stPreamp
cd $plotdir1stPreamp

root -l -q -b buildAlphaRidgeCut1stPreamp.C\+
wait

rm buildAlphaRidgeCut1stPreamp_*

for (( nDig = 65; nDig <= 250; nDig+=5 )); do
	montage -geometry 1000x700 -tile 1x2 \
	LvEn.y${nDig}.zoom.png projx.LvEn.ybin${nDig}.zoom.png montage.LvEn.plusProj.y${nDig}.png
	echo "created montage.LvEn.plusProj.y${nDig}.png"
done

convert -delay 20 -loop 0 montage.LvEn.plusProj.y*png montage.LvEn.plusProj.gif
echo "created montage.LvEn.plusProj.gif"
convert -delay 20 -loop 0 projx.LvEn.ybin*.zoom.png projx.LvEn.gif
echo "created projx.LvEn.gif"
convert -delay 20 -loop 0 LvEn.y*.zoom.png LvEn.gif
echo "created LvEn.gif"



cd -

plotdir2ndPreamp=plots.buildAlphaRidgeCut.2ndPreamp.`date +%d%b%y_%H%M.%S`
mkdir $plotdir2ndPreamp
cp $macros/buildAlphaRidgeCut2ndPreamp.C $plotdir2ndPreamp
cd $plotdir2ndPreamp

root -l -q -b buildAlphaRidgeCut2ndPreamp.C\+
wait

rm buildAlphaRidgeCut2ndPreamp_*

for (( nDig = 65; nDig <= 250; nDig+=5 )); do
	montage -geometry 1000x700 -tile 1x2 \
	LvEn.y${nDig}.zoom.png projx.LvEn.ybin${nDig}.zoom.png montage.LvEn.plusProj.y${nDig}.png
	echo "created montage.LvEn.plusProj.y${nDig}.png"
done

convert -delay 20 -loop 0 montage.LvEn.plusProj.y*png montage.LvEn.plusProj.gif
echo "created montage.LvEn.plusProj.gif"
convert -delay 20 -loop 0 projx.LvEn.ybin*.zoom.png projx.LvEn.gif
echo "created projx.LvEn.gif"
convert -delay 20 -loop 0 LvEn.y*.zoom.png LvEn.gif
echo "created LvEn.gif"

