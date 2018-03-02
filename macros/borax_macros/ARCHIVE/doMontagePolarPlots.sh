#!/bin/bash

plotdir=$1
cd $plotdir

for (( i = 0; i <= 80; i+=2 )); do

	thMin=$i
	thMax=$((i+10))
	if [[ ${i} -lt 10 ]]; then thMin=0$i; fi
	echo $thMin $thMax

	montage -geometry 1000x700 \
	noMaskLvEn.full.lin.thetaMin0$thMin*png cftLvEn.full.lin.thetaMin0$thMin*png \
	noMaskLvEn.full.log.zoom.thetaMin0$thMin*png cftLvEn.full.log.zoom.thetaMin0$thMin*png \
	LvEnSummary.thetaMin0$thMin.thetaMax0$thMax.png
	echo "created LvEnSummary.thetaMin0$thMin.thetaMax0$thMax.png"
	sleep 0.3;wait

	montage -geometry 1000x700 \
	noMaskLvEn.full.lin.plusFragCut.thetaMin0$thMin*png cftLvEn.full.lin.plusFragCut.thetaMin0$thMin*png \
	frags.full.noMask.thetaMin0$thMin*png frags.full.cft.thetaMin0$thMin*png \
	LvEnFragProj.thetaMin0$thMin.thetaMax0$thMax.png
	echo "created LvEnFragProj.thetaMin0$thMin.thetaMax0$thMax.png"
	sleep 0.3;wait

done