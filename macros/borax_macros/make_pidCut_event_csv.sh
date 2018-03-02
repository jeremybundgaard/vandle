usage ()
{
	echo "required input: -pidFile </full/path/to/inputTreeFile.trees.root> -inputTreeFile </full/path/to/inputTreeFile.trees.root> "
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

echo root -l -q -b $macros/make_pidCut_event_csv.C\+\(\"$inputTreeFile\",\"$pidFile\"\)
time root -l -q -b $macros/make_pidCut_event_csv.C\+\(\"$inputTreeFile\",\"$pidFile\"\)
wait

rm make_pidCut_event_csv_C*
