#1/bin/bash

if [[ $# != "1" ]]; then
  echo "specify topdir"
  exit
fi
topdir=$1/
cd $topdir
echo topdir=$topdir

for theta in `seq -w 000 005 170`
do
  theta_dir=${topdir}ThetaLo$theta
  find $theta_dir -name "*grid*.png" | while read png_name
  do
    png_bname=${topdir}`basename $png_name .png`_thetalo${theta}.png
    mv $png_name $png_bname
  done
  echo
done

ls *thetalo000.png | while read gridPlots
do
  gm convert -loop 0 -delay 12 `ls ${topdir}${gridPlots%000.png}*.png|sort` `ls ${topdir}${gridPlots%000.png}*.png|sort -r` ${gridPlots%*thetalo000.png}.gif
  echo "created ${gridPlots%*thetalo000.png}.gif"
done

mkdir ${topdir}pngs ${topdir}backups ${topdir}gifs
mv ${topdir}*.png ${topdir}pngs/.
mv ${topdir}*.gif ${topdir}gifs/.
mv ${topdir}ThetaLo* ${topdir}backups/.
find $topdir -name "*.root" -exec rm {} \;
