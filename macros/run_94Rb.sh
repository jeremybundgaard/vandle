#!/bin/bash

array=( "$@" )
arraylength=${#array[@]}

infile=/scratch3/ornl2016_vandle/micronet3/pixie16/ornl2016/ldf/rb/094rb_14-48.ldf
config=~/links/configs/Config-94rb.xml

ldf=`basename $infile .ldf`
outdir=${ldf}_date_`date +%d%b%y_%H%M.%S`
mkdir $outdir
cd $outdir

echo "~/paass/install/bin/utkscan -i $infile -c $config -o $ldf"
~/paass/install/bin/utkscan -i $infile -c $config -o $ldf

cd -
