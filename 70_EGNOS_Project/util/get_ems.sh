#!/bin/bash
#shell script to download hourly ems files for one day and concatenate them

#the first argument is the number of the days before the current day
#if no any arguments are given, it downloads the data of the previous day
if [ "$1" != "" ]; then
	n=$1
else
	n=1
fi

#second argument is the geostationary satellite id, 136 by default
if [ "$2" != "" ]; then
	prn=$2
else
	prn=136
fi

#get day of year and year from the PC date
doy=$(date -d "-$n day" +"%j")
year=$(date -d "-$n day" +"%Y")
year2=$(date -d "-$n day" +"%y")

#create output directory
outputdir="../files/ems/"
if [ ! -e $outputdir ]
then
    mkdir $outputdir
fi

#create output file
output=$outputdir"y"$year"_d"$doy"_"$prn".ems"
>$output

#get files from EMS server and concatenate hourly files into a daily one
for i in {0..9}
do
	wget -N -q --show-progress "ftp://131.176.49.48/pub/PRN$prn/y$year/d$doy/h0$i.ems"
	cat "h0$i.ems" >> $output
done
for i in {10..23}
do
	wget -N -q --show-progress "ftp://131.176.49.48/pub/PRN$prn/y$year/d$doy/h$i.ems"
	cat "h$i.ems" >> $output
done
rm h??.ems

#show output file properties
echo "output file details:"
ls -lh $output


