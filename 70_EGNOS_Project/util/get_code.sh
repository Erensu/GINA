#!/bin/bash
#shell script to download CODE ionosphere file

#the first argument is the year
#the second one is the day of year
#typical call_ ./get_code.sh 2019 46

#check number of arguments
if [ "$#" != 2 ]
then
	echo "typical call ./get_code.sh 2019 46"
	exit
fi

year=$1
printf -v doy "%03d" $2
year2=`echo $year | cut -c 3-4`

#get files from AIUB ftp server
if [ ! -f $year"/code/CODG"$doy"0."$year2"I" ]
then
	wget -N "ftp://ftp.aiub.unibe.ch/CODE/$year/CODG"$doy"0."$year2"I.Z"
	uncompress "CODG"$doy"0."$year2"I.Z"
	mv "CODG"$doy"0."$year2"I" $year"/code/."
fi
