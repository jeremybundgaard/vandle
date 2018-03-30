#!/bin/bash

usage ()
{
	echo
	echo '**********  USAGE  **********'
	echo
	echo "/full/path/to/gammaGatedNeutronTOF.sh -vandle_tree </full/path/to/vandle_tree> -gamma_tree </full/path/to/gamma_tree> \""
	echo
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
	if [[ "${array[$i-1]}" = "-gamma_tree" ]]; then
		gamma_tree="${array[$i]}"
		echo "-gamma_tree $gamma_tree"
	fi
	if [[ "${array[$i-1]}" = "-isotope" ]]; then
		isotope="${array[$i]}"
		echo "-isotope $isotope"
	fi
  if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
done

if [[ ! -f $vandle_tree ]] || [[ ! -f $gamma_tree ]] ; then
	echo "-vandle_tree file doesn't exist"
	usage
fi

topDir=`pwd`/gammaGatedNeutronTOF_${isotope}_`date +%d%b%y_%H%M.%S`
runDir=$topDir/runDir
plotDir=$topDir/plots
mkdir $topDir $runDir $plotDir $plotDir/individual_runs
cd $runDir
cp $macros/gammaGatedNeutronTOF.{C,sh} $runDir
ln -s $vandle_tree tree_link

lockfile=`dirname $topDir`/${isotope}Rb_ornl2016_gammaGatedNeutronTOF_`date +%d%b%y_%H%M.%S`.LOCK
touch $lockfile

echo "root -l -q -b gammaGatedNeutronTOF.C\(\"$gamma_tree\",\"$vandle_tree\",\"$cuts\",\"${isotope}\"\")"
time  root -l -q -b gammaGatedNeutronTOF.C\(\"$gamma_tree\",\"$vandle_tree\",\"$cuts\",\"${isotope}\"\)

cd $topDir
find $topDir -name "*png" -exec mv {} $topDir/plots/. \; > /dev/null 2>&1
tar cf runDir.backup.tar.gz $runDir > /dev/null 2>&1
gm montage -mode concatenate -tile 3x3 $topDir/plots/*png grid.png
rm -rf $runDir $lockfile
