#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "required input: -gamma_tree </full/path/to/gamma_tree> \""
	echo "-gamma_tree </full/path/to/gamma_tree> \""
	echo 'other inputs eg: -cuts \"vandle_QDC!=0\"'
	echo
	exit
}

cuts=""

array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-h" ]] || [[ "${array[$i-1]}" = "--help" ]]; then
		usage
	fi
  if [[ "${array[$i-1]}" = "-gamma_tree" ]]; then
		gamma_tree="${array[$i]}"
		echo "-gamma_tree $gamma_tree"
	fi
  if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
done

if [[ ! -f $gamma_tree ]]; then
	echo "-gamma_tree file doesn't exist"
	usage
fi
isotope=`basename $gamma_tree Rb_gamma.root`
topDir=`pwd`/${isotope}_ornl2016_gammaPlots_`date +%d%b%y_%H%M.%S`
runDir=$topDir/runDir
plotDir=$topDir/plots
mkdir $topDir $runDir $plotDir $plotDir/individual_runs
cd $runDir
cp $macros/ornl2016_gammaPlots.{C,sh} $runDir
ln -s $gamma_tree tree_link

lockfile=`dirname $topDir`/${isotope}Rb_ornl2016_gammaPlots_`date +%d%b%y_%H%M.%S`.LOCK
touch $lockfile

echo "root -l -q -b ornl2016_gammaPlots.C\(\"$gamma_tree\",\"$cuts\",\"${isotope}Rb\"\")"
time  root -l -q -b ornl2016_gammaPlots.C\(\"$gamma_tree\",\"$cuts\",\"${isotope}Rb\"\)

cd $topDir
tar cf runDir.backup.tar.gz $runDir > /dev/null 2>&1
# rm -rf $runDir

rm $lockfile
