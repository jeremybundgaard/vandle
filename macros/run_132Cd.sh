#!/bin/bash

batch=""
infile=/data/cern/isoldeData/oct/is600/IS600Oct_a132_01.ldf
config=~/links/configs/132Cd/132Cd_Config.xml
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

echo "~/paass/install/bin/utkscan -i $infile -c $config -o $ldf --frequency 250 -f R34300 $batch"
~/paass/install/bin/utkscan -i $infile -c $config -o $ldf --frequency 250 -f R34300 $batch

cd -
