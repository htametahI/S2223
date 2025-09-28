#!/bin/bash

declare -a FILE_LIST # array to hold file names
HIST_DIR=$1

firstrun=0
lastrun=1000000

if [ $# -lt 3 ]; then 
  echo "Sums all run histograms into single file"
  echo "usage $0 HistDir <first run> <last run>"
  exit
fi

firstrun=$2
lastrun=$3

SUM_ALL_FILE="$HIST_DIR/Runs_${firstrun}-${lastrun}.root"

# Check if the summed file already exists
if [[ -e $SUM_ALL_FILE ]]; then 
  read -p "$SUM_ALL_FILE already exists! Overwrite? (y/N): " confirm
  if [[ $confirm != "y" && $confirm != "Y" ]]; then
    echo "Skipping sum of all runs."
    exit
  fi
fi 

# loading all runs in FILE_LIST
FILE_LIST=() # Clear the array before use
for run in $(seq $firstrun $lastrun) ; do
  SUM_FILE="$HIST_DIR/Sum_${run}.root"
  if [ -e "$SUM_FILE" ]; then
    FILE_LIST+=("$SUM_FILE")
  fi
done

# sum all runs into single .root file
if [ ${#FILE_LIST[@]} -ne 0 ]; then
  echo "::: Summing runs for $firstrun to $lastrun ..."
  echo ""
  echo "hadd -f \"$SUM_ALL_FILE\" \"${FILE_LIST[@]}\""
  hadd -f "$SUM_ALL_FILE" "${FILE_LIST[@]}"
  echo ""
  echo "::: Summing runs for $firstrun to $lastrun ... [Done]"
else
  echo "No valid Sum_*.root files found for the given range. Exiting."
fi