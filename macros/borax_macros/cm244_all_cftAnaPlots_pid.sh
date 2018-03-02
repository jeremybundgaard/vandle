#!/bin/bash

cuts="1"
driftVel="4.42"
launch="local"
n=100000
echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
	if [[ "${array[$i-1]}" = "-launch" ]]; then
		launch="${array[$i]}"
		echo "-launch $launch"
	fi
	if [[ "${array[$i-1]}" = "-n" ]]; then
		n="${array[$i]}"
		echo "-n $n"
	fi
	if [[ "${array[$i-1]}" = "-driftVel" ]]; then
		driftVel="${array[$i]}"
		echo "-driftVel $driftVel"
	fi
done

driftVel=driftVel${driftVel}

topdir=`pwd`/cm244_all_cftAnaPlots_`date +%d%b%y_%H%M.%S`
echo $topdir
mkdir $topdir
cd $topdir


noMaskTree1=$trees/cm244.nEv-1.driftVel4.42.houghKalman.24Jun16_1144.39/noMask1.trees.root
cftTree1=$trees/cm244.nEv-1.driftVel4.42.houghKalman.24Jun16_1144.39/all_cft.trees.root
preamp=1
pidFile1=/p/lscratche/jerbundg/analysis/pid/cm244_pidCut_preamp1_zCut10_fragThresh1.2.driftVel4.42.root
launchScript=launchScript_preamp${preamp}.sh
echo "#!/bin/bash" \
$'\n\n' \
$macros/cm244_cftAnaPlots.sh \
-preamp $preamp \
-actinide cm244 \
-noMaskTree $noMaskTree1 \
-cftTree $cftTree1 \
-driftVel $driftVel \
-pidFile $pidFile1 \
>> $launchScript
chmod 750 $launchScript
if [[ "$launch" = "msub" ]]; then
	msub -q pbatch -l walltime=3600 $launchScript
else
	time ./$launchScript
	mv $launchScript cftAnaPlots.*/backups/.
fi

# preamp=2
# pidFile2="$pid/cm244_pidCut_preamp2_zCut30_fragThresh2.0.driftVel4.42.root"
# launchScript=launchScript_preamp${preamp}.sh
# echo "#!/bin/bash" \
# $'\n\n' \
# $macros/cm244_cftAnaPlots.sh \
# -preamp $preamp \
# -actinide cf252 \
# -noMaskTree $noMaskTree2 \
# -cftTree $cftTree2 \
# -driftVel $driftVel \
# -pidFile $pidFile2 \
# >> $launchScript
# chmod 750 $launchScript
# if [[ "$launch" = "msub" ]]; then
# 	msub -q pbatch -l walltime=3600 $launchScript
# else
# 	time ./$launchScript
# 	mv $launchScript cftAnaPlots.*/backups/.
# fi

find $topdir >/dev/null &

exit
