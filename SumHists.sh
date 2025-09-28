#!/bin/bash


declare -a FILE_LIST # array to hold file names
HIST_DIR=./HistFiles

firstrun=0
lastrun=1000000

if [ $# -eq 0 ]; then 
  echo "usage $0 <first run> <last run>"
  exit
fi

if [ $# -eq 1 ]; then 
  firstrun=$1
  lastrun=$1
fi

if [ $# -eq 2 ]; then
  firstrun=$1
  lastrun=$2
fi

# summing on a run by run basis
for run in `seq $firstrun $lastrun` ; do
  SUM_FILE="$HIST_DIR/Sum_$run.root"

  if [[ -e $SUM_FILE ]] ; then 
    read -p "Sum_$run.root already exists! Overwrite? (y/N): " confirm
    if [[ $confirm != "y" && $confirm != "Y" ]]; then
      #echo "Sum_$run.root already exists!"
      echo "Skipping run $run..."
      continue 
    fi
  fi 

  echo "::: Summing subruns for $run ... "
  echo ""

  FILE_LIST=() # clear the array before use
  for file in "$HIST_DIR"/Hist_"$run"_* ; do 
    FILE_LIST+=("$file")
  done

  # sum all subruns into single .root file
  if [ ${#FILE_LIST[@]} -ne 0 ]; then
    #echo "${#FILE_LIST[@]}   "
    echo "hadd -f $SUM_FILE ${FILE_LIST[@]}"
    hadd -f "$SUM_FILE" ${FILE_LIST[@]}
  else
    if [ -e ${FILE_LIST[0]} ] ; then
      echo "cp ${FILE_LIST[0]} $SUM_FILE"
      cp "${FILE_LIST[0]}" "$SUM_FILE"
    fi
  fi

  echo ""
  echo "::: Summing subruns for $run ... [Done]"

done

# sum all individual runs together
echo "::: Summing All Runs :::"
  ./SumAllHist.sh "$HIST_DIR" "$firstrun" "$lastrun"
echo "::: [DONE] :::"