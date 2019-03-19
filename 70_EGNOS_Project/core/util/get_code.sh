#!/bin/bash
#shell script to download CODE ionosphere file

#the first argument is the number of the days before the current day
#the default values is 5
if [ "$1" != "" ]; then
	n=$1
else
	n=5
fi

#get day of year and year from the PC date
doy=$(date -d "-$n day" +"%j")
year=$(date -d "-$n day" +"%Y")
year2=$(date -d "-$n day" +"%y")

#get files from AIUB ftp server
wget -N "ftp://ftp.aiub.unibe.ch/CODE/$year/CODG"$doy"0."$year2"I.Z"


