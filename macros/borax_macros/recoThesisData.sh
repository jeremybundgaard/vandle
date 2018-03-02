#!/bin/bash

nEvents=-1
driftVel=4.4

echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-actinide" ]]; then
		actinide="${array[$i]}"
		echo "-actinide $actinide"
	fi
	if [[ "${array[$i-1]}" = "-c" ]]; then
		niffteConfigFile="${array[$i]}"
		echo "-c $niffteConfigFile"
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

if [[ ! -f "$niffteConfigFile" ]] || [[ -z "$actinide" ]]; then
	echo;echo "missing required input: ./recoThesisData.sh -actinide <cf252|cm244> -c <config.xml>"
	echo "other inputs: -driftVel <4.9> -n <nEvents>"
	echo
	exit
fi


if [[ "$nEvents" -eq "-1" ]]; then
	walltimeMin=11520:00
elif [[ "$nEvents" -ne "-1" ]]; then
	walltimeMin=`echo 3/1000*$nEvents | bc -l`;
	walltimeMin=${walltimeMin%.*}
	echo $walltimeMin
	if [[ "$walltimeMin" -lt "30" ]]; then
		walltimeMin=30:00
	else
		walltimeMin=$walltimeMin:00
	fi
fi

echo $walltimeMin

cd $NIFFTE;niffteRev=`svn info |  grep "Revision:"`; niffteRev=${niffteRev:10:4}; cd -
topDir=`pwd`/${actinide}.nEv${nEvents}.driftVel${driftVel}.`basename $niffteConfigFile .xml`.`date +%d%b%y_%H%M.%S`/
mkdir $topDir
cp $niffteConfigFile $topDir
niffteConfigFile=${topDir}/`basename $niffteConfigFile`
rawtopdir=/p/lscratche/jerbundg/data/thesisData/raw2013/${actinide:0:2}

for i in `find  $rawtopdir -type d`
do
	rundir=$topDir${i#$rawtopdir}
	if [[ -d $rundir ]]; then continue;	fi
	mkdir $rundir
	wait
done


set_nJobs ()
{
	nJobs=`showq -u jerbundg | grep "Total jobs:"`
	nJobs=${nJobs#Total jobs:}
	nJobs=${nJobs##*()}
}
set_nJobs
jobtest=$((190))

for rawfile in `find  $rawtopdir -type f -a -name "*.raw"`
do

	set_nJobs
	echo "nJobs:$nJobs jobtest:$jobtest"
	while [ "$nJobs" -gt "$jobtest" ]
	do
		echo "waiting for $((nJobs-jobtest)) jobs to finish as of `date`"
		sleep 60
		set_nJobs
		echo "nJobs:$nJobs"
	done

	runstring=`basename $rawfile .raw`
	tmp=$topDir${rawfile#$rawtopdir/}
	treedir=${tmp%.raw}
	treefile=$treedir.trees.root
	mkdir $treedir
	cd $treedir

	echo "#!/bin/bash" \
	$'\n\n' \
	$macros/launchNiffteJobs.sh \
	-c $niffteConfigFile \
	-i $rawfile \
	-tree $treefile \
	-n $nEvents \
	-driftVel $driftVel \
	>> ${treedir}/launchScript.sh
	chmod 750 ${treedir}/launchScript.sh
	msub -l walltime=8:00:00:00 -q pbatch ${treedir}/launchScript.sh
	echo "msub -l walltime=8:00:00:00 -q pbatch ${treedir}/launchScript.sh"

done
