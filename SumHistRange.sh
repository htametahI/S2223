#!/usr/bin/env bash
# Usage: ./SumRange.sh <HistDir> <start_run> <end_run>
# Example: ./SumRange.sh ./HistFiles/UsingCalibrationFilePostExperiment 61000 61025

set -e

RUNLIST=runs_good.txt

if [ $# -lt 3 ]; then
  echo "Sums histograms for runs in a numeric range (including subruns)."
  echo "Usage: $0 <HistDir> <start_run> <end_run>"
  exit 1
fi

HIST_DIR="$1"
START="$2"
END="$3"

# Basic validation
[[ "$START" =~ ^[0-9]+$ ]] || { echo "start_run must be an integer"; exit 1; }
[[ "$END"   =~ ^[0-9]+$ ]] || { echo "end_run must be an integer";   exit 1; }
if [ "$START" -gt "$END" ]; then
  tmp="$START"; START="$END"; END="$tmp"
fi

# Make globbing behave nicely: empty matches -> empty array (not literal pattern)
shopt -s nullglob

# Build the list of runs to process
declare -a RUNS=()

if [ -f "$RUNLIST" ]; then
  # Use only runs present in runs_good.txt AND within [START,END]
  mapfile -t RUNS < <(grep -E '^[0-9]+' "$RUNLIST" | awk -v s="$START" -v e="$END" '$1>=s && $1<=e {print $1}' | sort -n)
  if [ ${#RUNS[@]} -eq 0 ]; then
    echo "No runs in [$START,$END] were found in $RUNLIST."
    exit 1
  fi
else
  # Fall back to the raw numeric range
  for (( r=START; r<=END; r++ )); do RUNS+=("$r"); done
fi

echo "Run range: ${RUNS[0]} – ${RUNS[-1]}"
echo "Hist dir : $HIST_DIR"
echo

# Ensure hadd is available
command -v hadd >/dev/null 2>&1 || { echo "ERROR: 'hadd' not found in PATH (source your ROOT setup)."; exit 1; }

# 1) For each run: sum subruns -> Sum_<run>.root (create/overwrite with prompt)
declare -a PER_RUN_SUMS=()

for run in "${RUNS[@]}"; do
  run_sum="$HIST_DIR/Sum_${run}.root"

  # Collect subrun files
  subruns=( "$HIST_DIR"/Hist_"$run"_*.root )

  if [ ${#subruns[@]} -eq 0 ] && [ ! -e "$run_sum" ]; then
    echo "Run $run: no subruns found and no $run_sum exists. Skipping."
    continue
  fi

  if [ ${#subruns[@]} -gt 0 ]; then
    # Ask before overwriting existing per-run sum
    if [ -e "$run_sum" ]; then
      read -p "Run $run: $run_sum exists. Overwrite? (y/N): " ans
      [[ "$ans" == "y" || "$ans" == "Y" ]] || { echo "Skipping re-sum for $run."; PER_RUN_SUMS+=("$run_sum"); continue; }
    fi
    echo "Run $run: hadd -> $run_sum (${#subruns[@]} subruns)"
    hadd -f "$run_sum" "${subruns[@]}"
  else
    echo "Run $run: using existing $run_sum"
  fi

  PER_RUN_SUMS+=("$run_sum")
done

if [ ${#PER_RUN_SUMS[@]} -eq 0 ]; then
  echo "No per-run sums available to merge. Exiting."
  exit 0
fi

# 2) Merge all per-run sums into one master file
master_out="$HIST_DIR/Runs_${RUNS[0]}-${RUNS[-1]}.root"

if [ -e "$master_out" ]; then
  read -p "$master_out already exists. Overwrite? (y/N): " confirm
  if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo "Skipping master merge."
    exit 0
  fi
fi

echo
echo "::: Summing ${#PER_RUN_SUMS[@]} runs -> $master_out"
hadd -f "$master_out" "${PER_RUN_SUMS[@]}"
echo "::: Done"
