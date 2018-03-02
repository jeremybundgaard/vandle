#!/bin/bash
if [[ $# != 1 ]]; then
  echo '$# = '$#
  echo usage: inFileName
  exit
fi

infile=$1
infile_bname=`basename $infile .trees.root`
outfile_dir=`dirname $infile`
for cuts in {fragCut_adc,fragCut_unlikely_adc,alphaCut_long_low_adc,alphaCut_long_adc,long_high_adc,alphaCut_short_higher_adc,alphaCut_short_low_adc,alphaCut_zCut_adc}
do
  outfile=${outfile_dir}/$cuts.${infile_bname}.trees.root
  ~/macros/makeNewTree.sh $infile $outfile $cuts
done
