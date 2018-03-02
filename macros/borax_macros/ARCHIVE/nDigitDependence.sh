topdir=`pwd`/nDigPlots.$1.`date +%d%b%y_%H%M.%S`
mkdir $topdir
cd $topdir

preamp=1
plotdir=`pwd`/${preamp}preamp
mkdir $plotdir
cd $plotdir
cp $macros/nDigitDependence.{sh,C} $plotdir

echo root -l -q -b nDigitDependence.C\+\($preamp\)
time root -l -q -b nDigitDependence.C\+\($preamp\) &

cd $topdir

preamp=2
plotdir=`pwd`/${preamp}preamp
mkdir $plotdir
cd $plotdir
cp $macros/nDigitDependence.{sh,C} $plotdir

echo root -l -q -b nDigitDependence.C\+\($preamp\)
time root -l -q -b nDigitDependence.C\+\($preamp\) &

find $topdir -name "nDigitDependence_C*" -exec rm {} \;

