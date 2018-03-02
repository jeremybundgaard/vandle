if [[ $# != "1" ]]; then
  echo "specify topdir"
  exit
fi
topdir=${1}/
cd $topdir
echo topdir=$topdir

for digMin_loop in `seq -w 000 005 150`
do
  dig_dir=${topdir}dig_min$digMin_loop
  find $dig_dir -name "*grid*.png" | while read png_name
  do
    png_bname=${topdir}`basename $png_name .png`_dig_min${digMin_loop}.png
    mv $png_name $png_bname
  done
  echo $digMin_loop
done

ls *dig_min000.png | while read gridPlots
do
  gm convert -loop 0 -delay 12 `ls ${topdir}${gridPlots%000.png}*.png|sort` `ls ${topdir}${gridPlots%000.png}*.png|sort -r` ${gridPlots%*dig_min000.png}.gif
  echo "created ${gridPlots%*dig_min000.png}.gif"
done

mkdir ${topdir}pngs ${topdir}backups ${topdir}gifs
mv ${topdir}*.png ${topdir}pngs/.
mv ${topdir}*.gif ${topdir}gifs/.
mv ${topdir}dig_min* ${topdir}backups/.
find $topdir -name "*.root" -exec rm {} \;
