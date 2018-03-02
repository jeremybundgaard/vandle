#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "USAGE: $macros/createRunTimeLogsPerRun.sh </p/lscratche/jerbundg/analysis/ratio/runTime/cm244.runLogs>"
	echo "USAGE: $macros/createRunTimeLogsPerRun.sh </p/lscratche/jerbundg/analysis/ratio/runTime/cf252.runLogs>"
fi

find $1 -name "*nifftePreamble.log" | while read runlog;
do


	grep -q "NiffteRawIO::RunNumber() =" $runlog
	greptest1=$?
	runNumber=`grep  "NiffteRawIO::RunNumber() =" $runlog`
	runNumber=${runNumber:27:9}
	runtimelog=${runlog%$runNumber.nifftePreamble.log}/$runNumber.runtimes

	grep -q "Run Start Time is:" $runlog
	greptest2=$?
	runStartString=`grep  "Run Start Time is:" $runlog`
	runStart_HMS=${runStartString:109:114}
	runStart_Hr=${runStart_HMS:0:2}
	runStart_Hr=${runStart_Hr#0}
	runStart_Min=${runStart_HMS:3:2}
	runStart_Min=${runStart_Min#0}
	runStart_Sec=${runStart_HMS:6:2}
	runStart_Sec=${runStart_Sec#0}
	runStartTime=$((runStart_Hr*3600 + runStart_Min*60 + runStart_Sec))

	grep -q "Run Stop Time is:" $runlog
	greptest2=$?
	runStopString=`grep  "Run Stop Time is:" $runlog`
	runStop_HMS=${runStopString:109:114}
	runStop_Hr=${runStop_HMS:0:2}
	runStop_Hr=${runStop_Hr#0}
	runStop_Min=${runStop_HMS:3:2}
	runStop_Min=${runStop_Min#0}
	runStop_Sec=${runStop_HMS:6:2}
	runStop_Sec=${runStop_Sec#0}
	runStopTime=$((runStop_Hr*3600 + runStop_Min*60 + runStop_Sec))

	if (( "$runStop_Hr" < "$runStart_Hr")); then
		runStopTime=$(( (runStart_Hr + 1)*3600 + runStop_Min*60 + runStop_Sec))
		# echo runStopTime=$runStopTime "              hhhhhhhhhhh"
	fi
	runTime=$((runStopTime - runStartTime))


	echo "$runlog" >> $runtimelog
	echo "runNumber=$runNumber" >> $runtimelog
	echo "runStartTime=$runStartTime =$runStart_HMS" >> $runtimelog
	echo "runStopTime=$runStopTime =$runStop_HMS" >> $runtimelog
	echo "runTime=$runTime" >> $runtimelog

	if [[ "$greptest1" = "1" ]]||[[ "$greptest2" = "1" ]]||[[ "$greptest3" = "1" ]]; then
		echo $runlog
		echo "grep failed"
		echo $greptest1
		echo $greptest2
		echo $greptest3
		# exit
	fi


done 

