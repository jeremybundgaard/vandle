topdir=`pwd`/$1

preamp=1
cd ${topdir}/linear/preamp$preamp
rm slurm* launchScript_*.sh
find . -name "*png" -exec cp {} . \;
ls *.pidStep11.*png | while read i
do
	pngname=${i#cf252.preamp1.pidStep11.}
	gm convert -loop 0 -delay 20 \
	`ls *$pngname |sort` \
	`ls *$pngname|sort -r` \
	preamp$preamp.${pngname%png}gif
done
# mkdir plots
# mv cftAnaPlots.* plots/.
# tar zcf plots.tar.gz plots
rm -rf *png plots


preamp=2
cd ${topdir}/linear/preamp$preamp
rm slurm* launchScript_*.sh
find . -name "*png" -exec cp {} . \;
ls *.pidStep11.*png | while read i
do
	pngname=${i#cf252.preamp2.pidStep11.}
	gm convert -loop 0 -delay 20 \
	`ls *$pngname |sort` \
	`ls *$pngname|sort -r` \
	preamp$preamp.${pngname%png}gif
done

# mkdir plots
# mv cftAnaPlots.* plots/.
# tar zcf plots.tar.gz plots
rm -rf *png plots

cd $topdir

find $topdir -name "*.gif" -exec mv {} $topdir \;

# tar zcf linear.tar.gz linear
rm -rf linear
