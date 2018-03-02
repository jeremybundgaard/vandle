#!/bin/bash

inputTreeFile_digit=/p/lscratche/jerbundg/data/thesisData/trees/digTrkTreeDir/100k_events/cf252.nEv100000.driftVel4.9.trk_dig_houghKalman.23Jun16_0357.58/noMask2.trk_dig.trees.root
inputTreeFile_tracks=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask2_noGhostTail.root
pidFile=/p/lscratche/jerbundg/analysis/pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root

topdir=`pwd`/digitTrackPlotter_pidCut_`date +%d%b%y_%H%M.%S`/
mkdir $topdir
cd $topdir

for cuts in {fragCut_adc,fragCut_unlikely_adc,alphaCut_long_low_adc,alphaCut_long_adc,long_high_adc,alphaCut_short_higher_adc,alphaCut_short_low_adc}
do
  cut_dir=$topdir/$cuts/
  mkdir $cut_dir
  cd $cut_dir
  launcher=$cut_dir/launch_${cuts}.sh

  echo "#!/bin/bash" \
  $'\n\n' \
  time ~/macros/digitTrackPlotter_pidCuts.sh \
  -preamp 2 \
  -actinide cf252 \
  -pidFile $pidFile \
  -pidCut $cuts \
  -TPCSummaryDigitTree $inputTreeFile_digit \
  -TPCSummaryTree $inputTreeFile_tracks \
  $'\n\n'\
  rm -rf make_pidCut_trees.* \
  >> $launcher
  chmod 750 $launcher

  # msub -q pbatch -l walltime=8:00:00:00 $launcher
  cd $topdir

done
