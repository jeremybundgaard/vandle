#!/bin/bash

inputTreeFile_digit=/p/lscratche/jerbundg/data/thesisData/trees/digTrkTreeDir/run_trees/noMask2.driftVel4.9_trk_dig.trees.root
bname_digit=`basename $inputTreeFile_digit`
inputTreeFile_tracks=/p/lscratche/jerbundg/data/thesisData/trees/noGhostTails/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask2_noGhostTail.root
bname_tracks=`basename $inputTreeFile_tracks`
pidFile=/p/lscratche/jerbundg/analysis/pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root

topdir=`pwd`/digitTrackPlotter_pidCut_`date +%d%b%y_%H%M.%S`/
mkdir $topdir
cd $topdir

cuts="blobs"
cut_dir=$topdir/$cuts/
mkdir $cut_dir
cd $cut_dir
cp ~/macros/digitTrackPlotter_pidCuts.{sh,C} $cut_dir
launcher=$cut_dir/launch_${cuts}.sh
echo "#!/bin/bash" $'\n' >> $launcher
echo sleep $[ ( $RANDOM % 20 )  + 1 ]s $'\n' >> $launcher

outputTreeFile_tracks=${cut_dir}${cuts}_${bname_tracks}
echo time ~/macros/make_pidCut_trees.sh \
-pidFile $pidFile \
-inputTreeFile $inputTreeFile_tracks \
-outputTreeFile $outputTreeFile_tracks \
-dataType "tracks"  \
-cuts $cuts $'\n' \
>> $launcher

outputTreeFile_digit=${cut_dir}${cuts}_${bname_digit}
echo time ~/macros/make_pidCut_trees.sh \
-pidFile $pidFile \
-inputTreeFile $inputTreeFile_digit \
-outputTreeFile $outputTreeFile_digit \
-dataType "digits"  \
-cuts $cuts $'\n' \
>> $launcher

echo time $cut_dir/digitTrackPlotter_pidCuts.sh \
-preamp 2 \
-actinide cf252 \
-pidCut $cuts \
-launch_dir $cut_dir \
-pidFile $pidFile \
-TPCSummaryDigitTree $outputTreeFile_digit \
-TPCSummaryTree $outputTreeFile_tracks \
$'\n\n'\
rm -rf make_pidCut_trees.* \
>> $launcher
chmod 750 $launcher

msub -l walltime=8:00:00:00 -q pbatch $launcher
cd $topdir
