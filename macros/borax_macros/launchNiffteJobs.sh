#!/bin/bash

nEvents=-1
driftVel=4.4

echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-c" ]]; then
		config="${array[$i]}"
		echo "-c $config"
	fi
	if [[ "${array[$i-1]}" = "-i" ]]; then
		infile="${array[$i]}"
		echo "-i $infile"
	fi
	if [[ "${array[$i-1]}" = "-tree" ]]; then
		treefile="${array[$i]}"
		echo "-tree $treefile"
	fi
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		echo "-driftVel $driftVel"
	fi
	if [[ "${array[$i-1]}" = "-n" ]]; then
		nEvents="${array[$i]}"
		echo "-n $nEvents"
	fi
done

if [[ -z "$infile" ]] || [[ -z "$treefile" ]] || [[ -z "$config" ]]; then
	echo "required inputs =>   -c <config.xml> -i <infile.raw> -tree <treefile>"
	echo "USAGE: launchNiffteJobs.sh -c <config.xml> -i <infile.raw> -tree <treefile> -driftVel <4.4> -n <nEvents>"
	exit
fi

####### compile any changes to NIFFTE code #######
# cd $NIFFTE_BUILD
# cmake -DCMAKE_INSTALL_PREFIX=$NIFFTE_INSTALL $NIFFTE
# make install
# cd -
export DB_FAIL_OK=1;

driftSpeedFile=driftSpeed_${driftVel}.txt
echo "$driftVel, $driftVel, 50.0" > $driftSpeedFile
export READ_CALIB_FROM_FILE=1
export CALIB_FILE_PATH=`pwd`/
export DRIFT_CALIB_FILE_NAME=$driftSpeedFile

echo niffte -c $config -i $infile -a $treefile -n $nEvents
echo "niffte -c $config -i $infile -a $treefile -n $nEvents -o dummy.out.`date +%d%b%y_%H%M.%S`.root" > launchScript.sh
chmod 700 launchScript.sh
./launchScript.sh
# msub -l walltime=1:00:00:00 -q pbatch launchScript.sh
echo
