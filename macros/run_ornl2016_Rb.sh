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
    config_fromCLI=${array[$i]}
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

# cp 	/home/jeremy/ornl2016_Rb/run_ornl2016_Rb.sh $debugDir/.
# tar zcvf $debugDir/paass_usedForThisScan.tar.gz $paassDir > /dev/null 2>&1
# echo "output folder = " $outdir

if [[ "$((timeCalflag))" = "1" ]] && [[ ! -f $timeCalPath ]]; then
	echo "timeCalPath $timeCalPath doesn't exist"
	exit 98
else
	if [[ ! -f $config_fromCLI ]]; then
		template_config=/SCRATCH/DRunScratch1/ornl2016/Rb/configs/Config-94rb_pre23March2016_forTIMECAL.XML
		timeCal_config=${debugDir}/${ldf}_timeCal.xml
		cp $template_config $timeCal_config
		cp $template_config $debugDir
		sed -i "s|timecalpath|$timeCalPath|g" "$timeCal_config"
		config=$timeCal_config
	else
		config=$config_fromCLI
	fi
fi

[[ ! -f $config ]] && (echo "config $config doesn't exist";exit 97)
[[ "1" = "$dryrun" ]] && (echo "nice -n 19 $paassDir/install/bin/utkscan -i $infile -c $config -o $ldf $batch >> stdout.log";exit)

mkdir -p /home/jeremy/.locks/
lockfile="/home/jeremy/.locks/utkscan.ldf_$ldf.random.${RANDOM}${RANDOM}";touch $lockfile; ls $lockfile
start=$SECONDS

nice -n 3 $paassDir/install/bin/utkscan -i $infile -c $config -o $ldf $batch >> stdout.log
echo -e "\n -------- finished `date +%d%b%Y_%H%M.%S` -------- " >> stdout.log
duration=$((SECONDS - start));echo -e "\n -------- scan duration `echo $( echo "scale=2;$duration/60" |bc -l) minutes ` -------- " >> stdout.log
cd - > /dev/null 2>&1
mv $outdir $final_dest
rm $lockfile
