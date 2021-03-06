#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo "required input: -vandle_tree </full/path/to/vandle_tree> \""
	echo "-vandle_tree </full/path/to/vandle_tree> \""
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
  if [[ "${array[$i-1]}" = "-vandle_tree" ]]; then
		vandle_tree="${array[$i]}"
		echo "-vandle_tree $vandle_tree"
	fi
  if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
done

if [[ ! -f $vandle_tree ]]; then
	echo "-vandle_tree file doesn't exist"
	usage
fi
isotope=`basename $vandle_tree Rb_vandle.root`
topDir=`pwd`/${isotope}_vandlePlots_varyNeutron_TCut_`date +%d%b%y_%H%M.%S`
runDir=$topDir/runDir
plotDir=$topDir/plots
mkdir $topDir $runDir $plotDir $plotDir/individual_runs
cd $runDir
cp $macros/vandlePlots_varyNeutron_TCut.{C,sh} $runDir
ln -s $vandle_tree tree_link

lockfile=`dirname $topDir`/${isotope}Rb_ornl2016_vandlePlots_varyNeutron_TCut_`date +%d%b%y_%H%M.%S`.LOCK
touch $lockfile

echo "root -l -q -b vandlePlots_varyNeutron_TCut.C\(\"$vandle_tree\",\"$cuts\",\"${isotope}Rb\"\")"
time  root -l -q -b vandlePlots_varyNeutron_TCut.C\(\"$vandle_tree\",\"$cuts\",\"${isotope}Rb\"\)

cd $topDir
find $topDir -name "*png" -exec mv {} $plotDir/. \; > /dev/null 2>&1
tar cf runDir.backup.tar.gz $runDir > /dev/null 2>&1
rm -rf $runDir $lockfile
