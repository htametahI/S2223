#!/bin/bash

#ADIR=AnalysisTrees
#ADIR=/mnt/g/Backups/2024_TRIUMF_S2223/S2223
ADIR=/home/mikeqiu/S2223/AnalysisTrees/UsingCalibrationFilePostExperiment

#HISTDIR=HistFiles
HISTDIR=HistFiles

DLEN=${#ADIR}
SORTCODE=/home/mikeqiu/S2223/SortTigress2
#SORTCODE=SortCode/Clock

#CFILE=Calibrations/EnCal/Calibration_Quadratic.cal
#CFILE=alphacalibrations/CalibrationFile.cal
#CFILE=backups/20241118_CalibrationFile.cal

#CFILE=Calibrations/CalibrationFile.cal
#CFILE=Calibrations/CalibrationFile_cam.cal
#CFILE=Calibrations/20241203_calibrationfile.cal
#CFILE=Calibrations/20241204_calibrationfile.cal
#CFILE=Calibrations/20241205_calibrationfile.cal
#CFILE=Calibrations/CalibrationFile20250307.cal
CFILE=CalibrationFilePostExperiment.cal

TARGET=three
m=60647 #Only Sort after this run
n=1000000 #Only Sort until this run 

RUNLIST=runs_good.txt # File containing run numbers

# Check if run list exists
if [ ! -f "$RUNLIST" ]; then
  echo "Run list file '$RUNLIST' not found. Exiting."
  exit 1
fi

if [ ! -d $HISTDIR ]; then
  mkdir $HISTDIR
fi

# Open runlist on FD 3 to avoid conflict with stdin
exec 3< "$RUNLIST"

while read -r runnum <&3; do

  if [[ -z "$runnum" || "$runnum" =~ ^# ]]; then # skip if empty or comment line
    continue
  fi

  if [ "$runnum" -gt "$m" ] && [ "$n" -gt "$runnum" ]; then
    for f in $ADIR/analysis${runnum}_*.root; do 
      if [ -f "$f" ]; then
        g=${f:DLEN+9}
        h=${g:0:${#g}-5} 
        #i=${g:0:${#g}-9}
        #echo "$i"

        HFILE=$HISTDIR/Hist_$h.root
        echo "$SORTCODE $f $CFILE $HFILE"
        $SORTCODE "$f" "$CFILE" "$HFILE" 
      fi
    done
  fi
done 

exec 3<&-  # Close FD 3
echo "Finished looping through run list"