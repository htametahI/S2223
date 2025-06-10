#!/bin/bash

ADIR=AnalysisTrees/UsingCalibrationFilePostExperiment
HISTDIR=HistFiles
DLEN=${#ADIR}
SORTCODE=/home/mikeqiu/S2223/SortTigress2
#SORTCODE=SortCode/Clock
#CFILE=Calibrations/EnCal/CalibrationFile.cal
#CFILE=Calibrations/EnCal/Calibration_Quadratic.cal
#CFILE=alphacalibrations/CalibrationFile.cal
CFILE=CalibrationFilePostExperiment.cal
#CFILE=CalibrationFile.cal
#CFILE=backups/20241118_CalibrationFile.cal
TARGET=three
m=60647 #Only Sort after this run
n=1000000 #Only Sort until this run 

if [ ! -d $HISTDIR ]; then
mkdir $HISTDIR
fi


if [ $# -eq 0 ]
then

for f in $ADIR/*.root

		do
# echo "$f"
g=${f:DLEN+9}
h=${g:0:${#g}-5} 
i=${g:0:${#g}-9}
echo "$i"

HFILE=$HISTDIR/Hist_$h.root

if [ $i -gt $m ] && [ $n -gt $i ];
then
#if [ ! -f $HFILE ]; then 
echo "$SORTCODE $f $CFILE $HFILE"
$SORTCODE $f $CFILE $HFILE 
#fi
fi
done

else 

for f in $ADIR/analysis"$@"_*.root;

do
echo "$f"
g=${f:DLEN+9}
h=${g:0:${#g}-5} 
i=${g:0:${#g}-9}

HFILE=$HISTDIR/Hist_$h.root

if [ $i -gt $m ] && [ $n -gt $i ];
then
#if [ ! -f $HFILE ]; then 
echo "$SORTCODE $f $CFILE $HFILE"
$SORTCODE $f $CFILE $HFILE
#fi
fi
done

fi
