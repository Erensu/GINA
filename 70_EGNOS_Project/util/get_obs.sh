#!/bin/bash
#shell script to download observation and navigation files from igs

#the first argument is the year
#the second one is the day of year
#the third argument is the station name
#typical call_ ./get_obs.sh 2019 46 bute

#check number of arguments
if [ "$#" != 3 ]; then
	echo "typical call ./get_obs.sh 2019 46 bute"
	exit
fi

year=$1
printf -v doy "%03d" $2
year2=`echo $year | cut -c 3-4`
stat=$3

#get files from IGS
if [ ! -f $year"/obs/"$stat$doy"0."$year2"d" ]
then
	wget -N "ftp://igs.bkg.bund.de/EUREF/BRDC/"$year"/"$doy"/brdc"$doy"0."$year2"n.Z"
	uncompress "brdc"$doy"0."$year2"n.Z"
	mv $stat$doy"0."$year2"d" $year"/obs/."
fi

if [ ! -f $year"/obs/brdc"$doy"0."$year2"n" ]
then
	wget -N "ftp://igs.bkg.bund.de/EUREF/obs/"$year"/"$doy"/"$stat$doy"0."$year2"d.Z"
	uncompress $stat$doy"0."$year2"d.Z"
	mv "brdc"$doy"0."$year2"n" $year"/obs/."
fi


