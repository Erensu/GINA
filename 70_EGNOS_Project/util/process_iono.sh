#!/bin/bash

#the first argument is the year
#the second one is the day of year
#the third argument is the station name
#the fourth one is the EGNOS geo satellite id
#typical call ./process_iono.sh 2018 7 bute 136

#check number of arguments
if [ "$#" != 4 ]; then
	echo "typical call ./process_iono.sh 2018 7 bute 136"
	exit
fi

#input
year=$1
doy2=$2
printf -v doy "%03d" $doy2
year2=`echo $year | cut -c 3-4`
stat=$3
prn=$4

#download data
./get_obs.sh $year $doy2 $stat
./get_code.sh $year $doy2
./get_ems.sh $year $doy2 $prn

#single poit positioning
/usr/local/bin/rnx2rtkp -k util/abs.conf -o "pos/"$stat"_"$doy"_"$year"_abs.pos" $year"/obs/"$stat$doy"0."$year2"d" $year"/obs/brdc"$doy"0."$year2"n"
octave pos_stat.m "pos/"$stat"_"$doy"_"$year"_abs.pos"

#code iono map
sed -i '/file-ionofile/d' util/ionomap.conf 
echo "file-ionofile      ="$year"/code/CODG"$doy"0."$year2"I" >> util/ionomap.conf 
/usr/local/bin/rnx2rtkp -k util/ionomap.conf -o "pos/"$stat"_"$doy"_"$year"_ionomap.pos" $year"/obs/"$stat$doy"0."$year2"d" $year"/obs/brdc"$doy"0."$year2"n" $year"/code/CODG"$doy"0."$year2"I"
octave pos_stat.m "pos/"$stat"_"$doy"_"$year"_ionomap.pos"

#sbas iono corrections
if [ -s $year"/ems/y"$year"_d"$doy"_"$prn".ems" ]
then
	/usr/local/bin/rnx2rtkp -k util/sbas.conf -o "pos/"$stat"_"$doy"_"$year"_sbas.pos" $year"/obs/"$stat$doy"0."$year2"d" $year"/obs/brdc"$doy"0."$year2"n" $year"/ems/y"$year"_d"$doy"_"$prn".ems"
	octave pos_stat.m "pos/"$stat"_"$doy"_"$year"_sbas.pos"
fi
