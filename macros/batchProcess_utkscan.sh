#!/bin/bash

batch=""
ldf=`basename $infile .ldf`
paassDir=~/paass
final_dest=/SCRATCH/DScratch3/jeremy/utkscan_output/

array=( "$@" )
arraylength=${#array[@]}
for (( i=1; i<${arraylength}+1; i++ ));
do
	if [[ "${array[$i-1]}" = "-b" ]]; then
    batch="-b"
	fi
	if [[ "${array[$i-1]}" = "-dryrun" ]]; then
    dryrun=1
	fi
	if [[ "${array[$i-1]}" = "-c" ]]; then
    config=${array[$i]}
	fi
	if [[ "${array[$i-1]}" = "-final_dest" ]]; then
	final_dest=${array[$i]}
	fi
	if [[ "${array[$i-1]}" = "-i" ]]; then
		infile=${array[$i]}
		bname_infile=`basename $infile .ldf`;
		isotope=${bname_infile%rb_*}${bname_infile#*rb_};
		ldf=${isotope%-*}${isotope#*-}; #echo $ldf
	fi
	if [[ "${array[$i-1]}" = "-timeCal" ]]; then
		timeCalflag=1
		timeCalPath="/SCRATCH/DRunScratch1/ornl2016/Rb/configs/timeCalibratedConfigs/${ldf}/mediumConfig.xml"
	fi
	if [[ "${array[$i-1]}" = "-paassDir" ]]; then
		paassDir=${array[$i]}
	fi
	if [[ "${array[$i-1]}" = "-rebuild-paass" ]]; then
		##### install any changes to paass just incase
		cd $paassdir/build/ > /dev/null 2>&1
		make install -j12 > /dev/null 2>&1
		cd - > /dev/null 2>&1;
	fi
done

if [[ ! -f $infile ]]; then
	echo "infile $infile doesn't exist"
	exit 99
fi

outdir=$PWD/${ldf}_`date +%d%b%Y_%H%M.%S`
debugDir=$outdir/debug
mkdir $outdir $debugDir
cd $outdir

let ldf="10#$ldf" ### allows handling leading zeros for integer comparison
micronet4_timeCals=/SCRATCH/DRunScratch1/ornl2016/Rb/configs/micronet4_timeCals
micronet3_timeCals=/SCRATCH/DRunScratch1/ornl2016/Rb/configs/micronet3_timeCals
(( "970000"<="$ldf")) && (("$ldf"<="970209" )) && config=${micronet3_timeCals}/0970203_Rb_timeCal_config.xml
(( "975000"<="$ldf")) && (("$ldf"<="975001" )) && config=${micronet4_timeCals}/0975000_Rb_timeCal_config.xml
(( "975101"<="$ldf")) && (("$ldf"<="975123" )) && config=${micronet4_timeCals}/0975103_Rb_timeCal_config.xml
(( "975200"<="$ldf")) && (("$ldf"<="975203" )) && config=${micronet4_timeCals}/0975200_Rb_timeCal_config.xml
(( "975300"<="$ldf")) && (("$ldf"<="975328" )) && config=${micronet4_timeCals}/0975300_Rb_timeCal_config.xml
(( "985000"<="$ldf")) && (("$ldf"<="985000" )) && config=${micronet4_timeCals}/0985000_Rb_timeCal_config.xml
(( "985100"<="$ldf")) && (("$ldf"<="985116" )) && config=${micronet4_timeCals}/0985101_Rb_timeCal_config.xml
(( "985200"<="$ldf")) && (("$ldf"<="985204" )) && config=${micronet4_timeCals}/0985200_Rb_timeCal_config.xml
(( "985300"<="$ldf")) && (("$ldf"<="985303" )) && config=${micronet4_timeCals}/0985302_Rb_timeCal_config.xml
(( "994000"<="$ldf")) && (("$ldf"<="994016" )) && config=${micronet4_timeCals}/0994007_Rb_timeCal_config.xml
(( "995000"<="$ldf")) && (("$ldf"<="995002" )) && config=${micronet4_timeCals}/0995000_Rb_timeCal_config.xml
(( "995003"<="$ldf")) && (("$ldf"<="995010" )) && config=${micronet4_timeCals}/0995010_Rb_timeCal_config.xml
(( "995100"<="$ldf")) && (("$ldf"<="995616" )) && config=${micronet4_timeCals}/0995130_Rb_timeCal_config.xml
(("1000000"<="$ldf")) && (("$ldf"<="1000647")) && config=${micronet4_timeCals}/1000400_Rb_timeCal_config.xml

[[ ! -f $config ]] && (echo "config $config doesn't exist";exit 97)
[[ "1" = "$dryrun" ]] && (echo "nice -n 19 $paassDir/install/bin/utkscan -i $infile -c $config -o $ldf $batch >> stdout.log";exit)

mkdir -p /home/jeremy/.locks/
lockfile="/home/jeremy/.locks/utkscan.ldf_$ldf.random.${RANDOM}${RANDOM}";touch $lockfile; ls $lockfile
start=$SECONDS

local_infile=${ldf}.ldf
cp $infile $outdir/$local_infile
cp $config $outdir/.

echo "nice -n 3 $paassDir/install/bin/utkscan -i $local_infile -c $config -o $ldf $batch"
nice -n 3 $paassDir/install/bin/utkscan -i $local_infile -c $config -o $ldf $batch >> stdout.log
echo -e "\n -------- finished `date +%d%b%Y_%H%M.%S` -------- " >> stdout.log
duration=$((SECONDS - start));echo -e "\n -------- scan duration `echo $( echo "scale=2;$duration/60" |bc -l) minutes ` -------- " >> stdout.log
cd - > /dev/null 2>&1
rm $outdir/$local_infile
mv $outdir $final_dest
rm $lockfile
