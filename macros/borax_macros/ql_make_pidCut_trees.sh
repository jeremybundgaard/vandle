#!/bin/bash


topdir=`pwd`/pidCut_trees_`date +%d%b%y_%H%M.%S`/
mkdir $topdir
cd $topdir

pidFile=/p/lscratche/jerbundg/analysis/pid/pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.9.root
inputTreeFile=/p/lscratche/jerbundg/data/thesisData/trees/drift/cf252.nEv-1.driftVel4.9.houghKalman/cf252.nEv-1.driftVel4.9.houghKalman_hadd_trees/noMask2_noGhostTail.root

for pidCut in {fragCut_adc,fragCut_unlikely_adc,alphaCut_zCut_adc,alphaCut_long_low_adc,alphaCut_long_adc,long_high_adc,alphaCut_short_higher_adc,alphaCut_short_low_adc};
do
	bname=`basename $inputTreeFile`
	outputTreeFile=${topdir}${pidCut}${bname}
	launcher=launch_${pidCut}.sh
	echo '#!/bin/bash' >> $launcher
	echo "" >> $launcher
  echo "sh ~/macros/make_pidCut_trees.sh \\" >> $launcher
  echo "-pidFile $pidFile \\">> $launcher
  echo "-inputTreeFile $inputTreeFile \\">> $launcher
  echo "-outputTreeFile $outputTreeFile \\">> $launcher
  echo "-cuts $pidCut">> $launcher
  chmod 750 $launcher
  # msub -q pbatch -l walltime=44444 $launcher
  $topdir/$launcher
  # sleep 1
done
