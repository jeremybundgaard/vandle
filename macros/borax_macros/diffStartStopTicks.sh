
if [[ $# != 1 ]]; then
	echo "USAGE: $macros/diffStartStopTicks.sh <inputTreeFile.root>"
	exit
fi

inputTreeFile=$1
# inputTreeFileName=`basename $inputTreeFile .root`
# inputTreeFileName=${inputTreeFileName:10:9}

# plotdir=`pwd`/diffStartStopTicks.$inputTreeFileName
# mkdir $plotdir
# cd $plotdir
# cp $macros/diffStartStopTicks.{sh,C} $plotdir

echo root -l -q -b $macros/diffStartStopTicks.C\+\(\"$inputTreeFile\"\)
time root -l -q -b $macros/diffStartStopTicks.C\+\(\"$inputTreeFile\"\)
wait

rm diffStartStopTicks_C*

