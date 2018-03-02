
if [[ $# != 3 ]]; then
	echo "USAGE: $macros/makeNewTree.sh <inputTreeFile.root> <outputTreeFile.root>  <TTree cut eg fitType==5>"
	exit
fi

inputTreeFile=$1
outputTreeFile=$2
treeCut=$3
plotdir=`pwd`/makeNewTree.`basename $inputTreeFile .root`.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
chmod -R 750 $macros
cp $macros/makeNewTree.{sh,C} $plotdir

cd $plotdir
echo root -l -q -b makeNewTree.C\+\(\"$inputTreeFile\",\"$outputTreeFile\",\"$treeCut\"\)
time root -l -q -b makeNewTree.C\+\(\"$inputTreeFile\",\"$outputTreeFile\",\"$treeCut\"\)
wait

rm makeNewTree_C*
