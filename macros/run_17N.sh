#!/bin/bash

batch=""
infile=~/links/data/17N/17N_cern/a17_m31_08-4.ldf
config=~/links/configs/cern/Config-17N-lowgain.xml
ldf=`basename $infile .ldf`

array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-b" ]]; then
    batch="-b"
	fi
	if [[ "${array[$i-1]}" = "-c" ]]; then
    config=${array[$i]}
	fi
	if [[ "${array[$i-1]}" = "-i" ]]; then
    infile=${array[$i]}
		ldf=`basename $infile .ldf`
	fi
done

outdir=${ldf}_date_`date +%d%b%y_%H%M.%S`
mkdir $outdir
cd $outdir

# echo "~/paass/install/bin/utkscan -i $infile --frequency 250 -f R34300 -c $config -o $ldf"
# ~/paass/install/bin/utkscan -i $infile --frequency 250 -f R34300 -c $config -o $ldf

echo "~/paass/install/bin/utkscan -i $infile -c $config -o $ldf $batch"
~/paass/install/bin/utkscan -i $infile -c $config -o $ldf $batch

cd -
