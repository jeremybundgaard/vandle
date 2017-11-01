#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "required input: -sim_tree </full/path/to/sim_tree> \""
	echo "-sim_tree </full/path/to/sim_tree> \""
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
	if [[ "${array[$i-1]}" = "-sim_tree" ]]; then
		sim_tree="${array[$i]}"
		echo "-sim_tree $sim_tree"
	fi
done

if [[ ! -f $sim_tree ]]; then
	echo "-sim_tree file doesn't exist"
	usage
fi

plotdir=`pwd`/sim_scinton_pathlength_`date +%d%b%y_%H%M.%S`
mkdir $plotdir $plotdir/plots
cd $plotdir
cp $macros/sim_scinton_pathlength.{C,sh} .

echo "root -l -q -b sim_scinton_pathlength.C\+\+\(\"$sim_tree\",\"$cuts\")"
time  root -l -q -b sim_scinton_pathlength.C\+\+\(\"$sim_tree\",\"$cuts\"\)

rm sim_scinton_pathlength_*{.d,so,pcm}
