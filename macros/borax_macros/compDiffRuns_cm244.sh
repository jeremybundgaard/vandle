
if [[ $# != 3 ]]; then
	echo "USAGE: $macros/compDiffRuns_cm244.sh <cmCFT1|cmCFT2|cmNoMask1|cmNoMask2> <hk|fp> <inputTreeFile.root>"
	exit
fi

runType=$1
recoType=$2
inputTreeFile=$3
plotdir=`pwd`/compDiffRuns_cm244.${runType}${recoType}.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/compDiffRuns_cm244.{sh,C} $plotdir

echo root -l -q -b compDiffRuns_cm244.C\+\(\"$runType\",\"$recoType\",\"$inputTreeFile\"\)
time root -l -q -b compDiffRuns_cm244.C\+\(\"$runType\",\"$recoType\",\"$inputTreeFile\"\)
wait

rm compDiffRuns_cm244_C*

