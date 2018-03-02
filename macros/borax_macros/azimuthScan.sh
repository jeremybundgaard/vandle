
if [[ $# != 2 ]]; then
	echo "USAGE: $macros/azimuthScan.sh <1|2> <cf|cm>"
	exit
fi

preamp=$1
actinide=$2
phiStepSize=3
plotdir=`pwd`/azimuthScan.${actinide}.${preamp}preamp.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir

for i in `seq 0 2 360`;
do
	phiMin=$i
	phiMax=$((i + 3))
	thetadir=$plotdir/phiMin${phiMin}-phiMax${phiMax}
	mkdir $thetadir
	cd $thetadir
	cp $macros/azimuthScan.C $thetadir/.

	echo msub -q pbatch -l walltime=1:00:00 $macros/msubAzimuthScan.sh $preamp $actinide $phiMin $phiMax $phiStepSize $plotdir
	msub -q pbatch -l walltime=1:00:00 $macros/msubAzimuthScan.sh $preamp $actinide $phiMin $phiMax $phiStepSize $plotdir
	cd -
done

wait
touch i.waited

for i in `ls log.output.phiMin*`;do cat $i >> log.output; done
rm log.output.PhiMin*

find $plotdir -name "*.png" -exec mv {} $plotdir/. \;


if [[ "`uname`" == "Linux" ]]; then

	for i in `ls *phiMin000*`;
		do 
		convert -loop 0 -delay 10  ${i%phiMin*}* ${i%phiMin*}gif
	done
	# rm *png

	# for (( i = 0; i <= 360; i+=10 )); do

	# 	phiMin=$i
	# 	phiMax=$((i+10))
	# 	echo $phiMin $phiMax

	# 	montage -geometry 1000x700 \
	# 	noMaskLvEn.full.lin.phiMin$phiMin*png cftLvEn.full.lin.phiMin$phiMin*png \
	# 	noMaskLvEn.full.log.zoom.phiMin$phiMin*png cftLvEn.full.log.zoom.phiMin$phiMin*png \
	# 	LvEnSummary.phiMin$phiMin.phiMax$phiMax.png
	# 	echo "created LvEnSummary.phiMin$phiMin.phiMax$phiMax.png"
	# 	sleep 0.3;wait

	# 	montage -geometry 1000x700 \
	# 	noMaskLvEn.full.lin.plusFragCut.phiMin$phiMin*png cftLvEn.full.lin.plusFragCut.phiMin$phiMin*png \
	# 	frags.full.lin.noMask.phiMin$phiMin*png frags.full.lin.cft.phiMin$phiMin*png \
	# 	LvEnFragProj.phiMin$phiMin.phiMax$phiMax.png
	# 	echo "created LvEnFragProj.phiMin$phiMin.phiMax$phiMax.png"
	# 	sleep 0.3;wait

	# 	montage -geometry 1000x700 \
	# 	noMaskLvEn.lowEn.plusFragCut.phiMin$phiMin*png cftLvEn.lowEn.plusFragCut.phiMin$phiMin*png \
	# 	frags.lowEn.noMask.phiMin$phiMin*png frags.lowEn.cft.phiMin$phiMin*png \
	# 	LvEnFragProj.phiMin$phiMin.phiMax$phiMax.png
	# 	echo "created LvEnFragProj.phiMin$phiMin.phiMax$phiMax.png"
	# 	sleep 0.3;wait


	# done

fi

mkdir $plotdir/pngs
find $plotdir -name "*.png" -exec mv {} $plotdir/pngs/. \;

