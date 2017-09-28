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

plotdir=`pwd`/traceShape_`date +%d%b%y_%H%M.%S`
mkdir $plotdir $plotdir/plots
cd $plotdir
cp $macros/traceShape.{C,sh} .
tar cf backups.tar.gz $macros/traceShape.{C,sh}

echo "./traceShape.sh -data_summary_tree $data_summary_tree" >> rerun_this.sh
chmod 744 rerun_this.sh

echo "root -l -q -b traceShape.C\+\+\(\"$data_summary_tree\",\"$cuts\")"
time  root -l -q -b traceShape.C\+\+\(\"$data_summary_tree\",\"$cuts\"\)

rm traceShape_*{.d,so,pcm}
