#!/bin/bash

# pass this script a single path to a file full of many paths of ldf files
# for example: find /full/path/to/ornl2016/ldf/Rb/ -name *.ldf >> path_to_many_ldfs.txt
# then one could run -->    ./throttle_jobs.sh path_to_many_ldfs.txt

if [[ "$#" != "1" ]]; then
 echo "pass this script a single path to a file full of many paths of ldf files"
 exit
fi


nJobsToDo=`cat $1 | wc -l`
nJobsCompleted=0
timeStarted=$SECONDS
cat $1 | while read ldf
do

  if [[  ! -f $ldf ]]; then
    echo "$ldf doesn't exist"
    continue
  fi

  nLOCKS=`ls /home/jeremy/.locks/ |wc -l`
  # echo "nLOCKS: $nLOCKS"

  ### each job will dominate a single core (donda:56 cores, kqxhc:24 cores)...so be neighborly ###
  nJobs_allowed_at_a_Time=49
  while [ $nLOCKS -gt $nJobs_allowed_at_a_Time ]
  do
    waittime=60
  	echo "nLOCKS: $nLOCKS will try and scan ldf:$ldf in $waittime seconds"
  	sleep $waittime
  	nLOCKS=`ls /home/jeremy/.locks/ |wc -l`
  done

  # echo '/SCRATCH/DRunScratch1/ornl2016/Rb/macros/run_ornl2016_Rb.sh -i $ldf -c /SCRATCH/DRunScratch1/ornl2016/Rb/Config-rbTest.xml -b &'
  /SCRATCH/DRunScratch1/ornl2016/Rb/macros/run_ornl2016_Rb.sh -i $ldf -c /SCRATCH/DRunScratch1/ornl2016/Rb/Config-rbTest.xml -b &
  # sleep .5
  ((++nJobsCompleted))
  percentComplete=$(echo "scale=2;$nJobsCompleted/$nJobsToDo*100" | bc)
  # echo "${percentComplete%.*}% of jobs launched      been running $(((SECONDS-timeStarted)/60)) minutes"

done
