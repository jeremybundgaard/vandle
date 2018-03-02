usage ()
{
	echo "required input: -pidFile </full/path/to/inputTreeFile.trees.root> -inputTreeFile </full/path/to/inputTreeFile.trees.root> "
	echo
	exit
}


echo

echo root -l -q -b $macros/read_event_csv.C\+
time root -l -q -b $macros/read_event_csv.C\+
wait
