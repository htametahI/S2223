#!/bin/bash

RUNLIST=runs_good.txt

declare -a FILE_LIST # array to hold file names
HIST_DIR=$1

if [ $# -lt 1 ]; then 
  echo "Sums all run histograms into single file using a run list"
  echo "usage $0 HistDir"
  exit
fi

if [ ! -f "$RUNLIST" ]; then
  echo "Run list file '$RUNLIST' not found. Exiting."
  exit 1
fi

# Read run numbers from the file, skipping empty/comment lines
mapfile -t RUNS < <(grep -E '^[0-9]+' "$RUNLIST" | sort -n)

# Get firstrun and lastrun from the list
firstrun=${RUNS[0]}
lastrun=${RUNS[-1]}

echo "Using run range: $firstrun – $lastrun"

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
for run in "${RUNS[@]}"; do
  SUM_FILE="$HIST_DIR/Sum_${run}.root"
  if [ -e "$SUM_FILE" ]; then
    FILE_LIST+=("$SUM_FILE")
  fi
done

# sum all runs into single .root file
if [ ${#FILE_LIST[@]} -ne 0 ]; then
  echo "::: Summing ${#FILE_LIST[@]} runs from $firstrun to $lastrun ..."
  echo ""
  echo "hadd -f \"$SUM_ALL_FILE\" \"${FILE_LIST[@]}\""
  hadd -f "$SUM_ALL_FILE" "${FILE_LIST[@]}"
  echo ""
  echo "::: Summing runs [Done]"
else
  echo "No valid Sum_*.root files found for the given range. Exiting."
fi