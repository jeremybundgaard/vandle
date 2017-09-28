#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "required input: -data_summary_tree </full/path/to/data_summary_tree> \""
	echo "-data_summary_tree </full/path/to/data_summary_tree> \""
	echo 'other inputs eg: -cuts \"vandle_QDC!=0\"'
	echo
	exit
}

cuts="1"

echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-h" ]]; then
		usage
	fi
	if [[ "${array[$i-1]}" = "--help" ]]; then
		usage
	fi
	if [[ "${array[$i-1]}" = "-data_summary_tree" ]]; then
		data_summary_tree="${array[$i]}"
		echo "-data_summary_tree $data_summary_tree"
	fi
done

if [[ ! -f $data_summary_tree ]]; then
	echo "-data_summary_tree file doesn't exist"
	usage
fi

plotdir=`pwd`/scan_QDC_`date +%d%b%y_%H%M.%S`
mkdir $plotdir $plotdir/plots
cd $plotdir
cp $macros/scan_QDC.{C,sh} .

echo "./scan_QDC.sh -data_summary_tree $data_summary_tree" >> rerun_this.sh
chmod 744 rerun_this.sh

echo "root -l -q -b scan_QDC.C\+\(\"$data_summary_tree\",\"$cuts\")"
time  root -l -q -b scan_QDC.C\+\(\"$data_summary_tree\",\"$cuts\"\)

gm convert -loop 0 -delay 10 `ls -t plots/QDC*.png` `ls -rt plots/QDC*.png` scanQDC_`date +%d%b%y_%H%M.%S`.gif

rm scan_QDC_*{.d,so,pcm} 
mkdir backups; mv scan_QDC.{C,sh} backups/.
