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

plotdir=`pwd`/TOF_Peaks_`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/TOF_Peaks.{C,sh} .

echo "./TOF_Peaks.sh -data_summary_tree $data_summary_tree" >> rerun_this.sh
chmod 744 rerun_this.sh

echo "root -l -q -b TOF_Peaks.C\+\(\"$data_summary_tree\",\"$cuts\")"
time  root -l -q -b TOF_Peaks.C\+\(\"$data_summary_tree\",\"$cuts\"\)


rm TOF_Peaks_*{.d,so,pcm}
