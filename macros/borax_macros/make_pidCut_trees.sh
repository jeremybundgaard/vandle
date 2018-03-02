usage ()
{
	echo "required input: -pidFile </full/path/to/inputTreeFile.trees.root> -inputTreeFile </full/path/to/inputTreeFile.trees.root> -outputTreeFile </full/path/to/outputTreeFile.trees.root> "
	echo
	echo "other inputs eg: -cuts \"&&0.2<direction.CosTheta()&&direction.CosTheta()<0.8\" "
	echo
	exit
}


echo
array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-pidFile" ]]; then
		pidFile="${array[$i]}"
		echo "-pidFile $pidFile"
	fi
	if [[ "${array[$i-1]}" = "-inputTreeFile" ]]; then
		inputTreeFile="${array[$i]}"
		echo "-inputTreeFile $inputTreeFile"
	fi
	if [[ "${array[$i-1]}" = "-outputTreeFile" ]]; then
		outputTreeFile="${array[$i]}"
		echo "-outputTreeFile $outputTreeFile"
	fi
	if [[ "${array[$i-1]}" = "-cuts" ]]; then
		cuts="${array[$i]}"
		echo "-cuts $cuts"
	fi
	if [[ "${array[$i-1]}" = "-dataType" ]]; then
		dataType="${array[$i]}"
		echo "-dataType $dataType"
	fi
done
echo

if [[ ! -f $pidFile ]]; then
	echo "pidFile doesn't exist: $pidFile"
	usage
fi
if [[ ! -f $inputTreeFile ]]; then
	echo "inputTreeFile doesn't exist: $inputTreeFile"
	usage
fi
if [[ -z $outputTreeFile ]]; then
	echo "outputTreeFile not set: $outputTreeFile"
	usage
fi
if [[ -z $cuts ]]; then
	echo "cuts not set: $cuts"
	usage
fi
if [[ -z $dataType ]]; then
	echo "dataType not set: $dataType"
	usage
fi

plotdir=`pwd`/make_pidCut_trees.`basename $inputTreeFile .root`.`date +%d%b%y_%H%M.%S`
mkdir $plotdir
cd $plotdir
chmod -R 750 $macros
cp $macros/make_pidCut_trees.{sh,C} $plotdir

cd $plotdir
echo root -l -q -b make_pidCut_trees.C\+\(\"$inputTreeFile\",\"$outputTreeFile\",\"$pidFile\",\"$cuts\",\"$dataType\"\)
time root -l -q -b make_pidCut_trees.C\+\(\"$inputTreeFile\",\"$outputTreeFile\",\"$pidFile\",\"$cuts\",\"$dataType\"\)
wait

rm make_pidCut_trees_C*
