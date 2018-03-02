#!/bin/bash

inputTreeFile_digit=/p/lscratche/jerbundg/data/thesisData/trees/digTrees/cf252.nEv500000.driftVel4.9.bsDigitTrees_RawToHDKK.11Oct16_0912.41/noMask2.bsDigitTrees.trees.root
# inputTreeFile_digit=/p/lscratche/jerbundg/data/thesisData/trees/digTrees/cf252.nEv50000.driftVel4.9.bsDigitTrees_RawToHDKK.11Oct16_0813.50/noMask2.bsDigitTrees.trees.root
bname_digit=`basename $inputTreeFile_digit`
inputTreeFile_tracks=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask2_noGhostTail.root
bname_tracks=`basename $inputTreeFile_tracks`
pidFile=/p/lscratche/jerbundg/analysis/pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root

topdir=`pwd`/digitTrackPlotter_pidCut_`date +%d%b%y_%H%M.%S`/
mkdir $topdir
cd $topdir

for cuts in {fragCut_adc,fragCut_unlikely_adc,alphaCut_long_low_adc,alphaCut_long_adc,long_high_adc,alphaCut_short_higher_adc,alphaCut_short_low_adc,alphaCut_zCut_adc,blobs}
do

  cut_dir=$topdir/$cuts/
  mkdir $cut_dir
  cd $cut_dir
  cp ~/macros/digitTrackPlotter_pidCuts.{sh,C} $cut_dir
  launcher=$cut_dir/launch_${cuts}.sh
  echo "#!/bin/bash" $'\n' >> $launcher
  # echo sleep $[ ( $RANDOM % 20 )  + 1 ]s $'\n' >> $launcher


  echo time $cut_dir/digitTrackPlotter_pidCuts.sh \
  -preamp 2 \
  -actinide cf252 \
  -pidCut $cuts \
  -launch_dir $cut_dir \
  -pidFile $pidFile \
  -TPCSummaryDigitTree $inputTreeFile_digit \
  -TPCSummaryTree $inputTreeFile_tracks \
  $'\n\n'\
  rm -rf make_pidCut_trees.* \
  >> $launcher
  chmod 750 $launcher

  msub -l walltime=8:00:00:00 -q pbatch $launcher
  cd $topdir

done
