#!/bin/bash
#shell script to download hourly ems files for one day and concatenate them
#the first argument is the year
#the second one is the day of year
#the third one is the geostaionary satellite id
#typical call_ ./get_ems.sh 2019 46 136 

#check number of arguments
if [ "$#" != 3 ]
then
	echo "typical call ./get_ems.sh 2019 46 136"
	exit
fi

year=$1
printf -v doy "%03d" $2
prn=$3

#get files from EMS server and concatenate hourly files into a daily one
output="y"$year"_d"$doy"_"$prn".ems"
if [ ! -f $year"/ems/"$output ]
then
	> $output

	for i in {0..23}
	do
		printf -v hour "%02d" $i
		wget -N "ftp://131.176.49.48/pub/PRN$prn/y$year/d$doy/h$hour.ems"
		cat "h$hour.ems" >> $output
	done
	rm h??.ems
	if [ -s $output ]
	then
		mv $output $year"/ems/."
	else
		rm $output
	fi
fi
