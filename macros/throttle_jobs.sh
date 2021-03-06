#!/bin/bash

# pass this script a single path to a file full of many paths of ldf files
# for example: find /full/path/to/ornl2016/ldf/Rb/ -name *.ldf >> path_to_many_ldfs.txt
# then one could run -->    ./throttle_jobs.sh -ldf_paths path_to_many_ldfs.txt -final_dest /path/to/final/dest/folder

run_script=/home/jeremy/vandle/macros/batchProcess_utkscan.sh
config=/SCRATCH/DRunScratch1/ornl2016/Rb/configs/Config-rbTest.xml
Ncores=50
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
  if [[ "${array[$i-1]}" = "-run_script" ]]; then
    run_script=${array[$i]}
	fi
  if [[ "${array[$i-1]}" = "-ldf_paths" ]]; then
    ldf_paths=${array[$i]}
    tmp=`basename $ldf_paths .txt `
    final_dest=/SCRATCH/DScratch3/jeremy/utkscan_output/${tmp}_`date +%d%b%Y_%H%M_%S`
	fi
  if [[ "${array[$i-1]}" = "-final_dest" ]]; then
    final_dest=${array[$i]}
	fi
  if [[ "${array[$i-1]}" = "-Ncores" ]]; then
    Ncores=${array[$i]}
	fi
done

if [[ ! -f $ldf_paths ]]; then
  echo "pass this script a single path to a file full of many paths of ldf files"
  echo "for example: find /full/path/to/ornl2016/ldf/Rb/ -name *.ldf >> path_to_many_ldfs.txt"
  echo "then one could run -->  ./throttle_jobs.sh -ldf_paths path_to_many_ldfs.txt -final_dest /path/to/final/dest/folder"
  exit
fi

if [[ ! -d $final_dest ]]; then
  mkdir -p $final_dest
fi

runDir=throttle_jobs_`basename $run_script`_`basename $ldf_paths .txt`
mkdir $runDir
cd $runDir

nJobsToDo=`cat $ldf_paths | wc -l`
nJobsCompleted=0
timeStarted=$SECONDS
cat $ldf_paths | while read ldf
do
  echo $final_dest
  if [[  ! -f $ldf ]]; then
    echo "$ldf doesn't exist"
    continue
  fi
  nLOCKS=`ls /home/jeremy/.locks/ |wc -l`
  # echo "nLOCKS: $nLOCKS"
  ### each job should utilize one core (donda:56 cores, kqxhc:24 cores)...so be neighborly ###
  while [ $nLOCKS -gt $Ncores ]
  do
    waittime=60
  	echo "nLOCKS: $nLOCKS will try and scan ldf:$ldf in $waittime seconds"
  	sleep $waittime
  	nLOCKS=`ls /home/jeremy/.locks/ |wc -l`
  done
  # echo "$run_script -i $ldf -c $config -b "
  $run_script -i $ldf -c $config -b -final_dest $final_dest &
  sleep 11
  ((++nJobsCompleted))
  percentComplete=$(echo "scale=2;$nJobsCompleted/$nJobsToDo*100" | bc)
  echo "${percentComplete%.*}% of jobs launched --> running for $(((SECONDS-timeStarted)/60)) minutes"
done

[ "$(ls -A $runDir)" ] && rm -rf $runDir
cd -
