
if [[ $# != 1 ]]; then
	echo "USAGE: $macros/niffteTreePlots.sh <inputTreeFile.root>"
	exit
fi

inputTreeFile=$1
plotdir=`pwd`/niffteTreePlots.`basename $inputTreeFile .root`.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
cp $macros/niffteTreePlots.{sh,C} $plotdir

echo root -l -q -b niffteTreePlots.C\+\(\"$inputTreeFile\"\)
time root -l -q -b niffteTreePlots.C\+\(\"$inputTreeFile\"\)
wait

rm niffteTreePlots_C*

