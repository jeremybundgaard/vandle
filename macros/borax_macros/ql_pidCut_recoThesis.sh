#!/bin/bash
#
# export DB_FAIL_OK=1
# niffte \
# -c `pwd`/fragCut_adc_events.csv_digTrees_HDKK.xml \
# -i /p/lscratche/jerbundg/data/thesisData/raw2013/cf/noMask/2ndPreamp/niffte-run100002055_03Jul2013_13-28.raw \
# -o dummy.root \
# -a trees.root \
# -n 1234 \

csv_pidCut_zCuts=/p/lscratche/jerbundg/analysis/pid/csv_pidCut_zCuts
ls $csv_pidCut_zCuts | while read csvfile
do

  pidCutName=${csvfile%_adc_events.csv}
  pidCut_config=`pwd`/${pidCutName}_digTrees_HDKK.xml
  cp /p/lscratche/jerbundg/data/thesisData/trees/digTrees/template/TEMPLATE_digTrees_RawToHDKK.xml $pidCut_config

  find=pidCut_fileName_REPLACE_ME_SED
  replace=${csv_pidCut_zCuts}/${csvfile}
  sed -i "s#"$find"#"$replace"#g" $pidCut_config

  launchScript=launch_${pidCutName}.sh
  echo ~/macros/recoThesisData.sh \
  -actinide cf252 \
  -c $pidCut_config \
  -driftVel 4.9 >> $launchScript
  chmod 700 $launchScript
  msub -l walltime=8:00:00:00 -q pbatch $launchScript
  echo $pidCutName

done
