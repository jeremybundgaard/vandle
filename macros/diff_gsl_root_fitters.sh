#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "required input: -dst_root_fitter </full/path/to/dst_root_fitter> \""
	echo "-dst_root_fitter </full/path/to/dst_root_fitter> \""
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
	if [[ "${array[$i-1]}" = "-dst_root_fitter" ]]; then
		dst_root_fitter="${array[$i]}"
		echo "-dst_root_fitter $dst_root_fitter"
	fi
	if [[ "${array[$i-1]}" = "-dst_gsl_fitter" ]]; then
		dst_gsl_fitter="${array[$i]}"
		echo "-dst_gsl_fitter $dst_gsl_fitter"
	fi
done

if [[ ! -f $dst_root_fitter ]]; then
	echo "-dst_root_fitter file doesn't exist"
	usage
fi

plotdir=`pwd`/VANDLE_diff_gsl_root_fitters_`date +%d%b%y_%H%M.%S`
mkdir $plotdir $plotdir/plots
cd $plotdir
tar -cf backups.tar.gz $macros/diff_gsl_root_fitters.{C,sh}

echo "root -l -q -b $macros/diff_gsl_root_fitters.C\+\+\(\"$dst_root_fitter\",\"$dst_gsl_fitter\",\"$cuts\")"
time  root -l -q -b $macros/diff_gsl_root_fitters.C\+\+\(\"$dst_root_fitter\",\"$dst_gsl_fitter\",\"$cuts\"\)

rm $macros/diff_gsl_root_fitters_*{.d,so,pcm}
