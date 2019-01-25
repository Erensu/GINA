#!/bin/bash
#shell script to download code ionex file, draw tec maps and create animate gif

#local path
lpath="/home/bence/kutat/ems/"
cd $lpath

#the number of days back in time, since ionex files may have some latency
n=5

#get day of year and year from the PC date
doy=$(date -d "-$n day" +"%j")
year=$(date -d "-$n day" +"%Y")
year2=$(date -d "-$n day" +"%y")

#download ionex file from AIUB
wget -N "ftp://ftp.aiub.unibe.ch/CODE/$year/CODG"$doy"0."$year2"I.Z"

#uncompress Z file
uncompress "CODG"$doy"0."$year2"I.Z"

#read ionex file and draw tec maps
octave read_ionex.m "CODG"$doy"0."$year2"I"

#convert images into animated gif
convert -delay 100 -loop 0 iono*.jpg iono.gif

#delete tec maps images
rm iono*.jpg
rm "CODG"$doy"0."$year2"I"

#upload to the server
scp iono.gif 152.66.5.8:public_html/ems/.
cd
