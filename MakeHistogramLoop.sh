#!/usr/bin/env bash
set -euo pipefail
shopt -s nullglob

ADIR="/home/mikeqiu/S2223/AnalysisTrees/UsingCalibrationFilePostExperiment"
HISTDIR="HistFiles"

# Use absolute path to the sorter
SORTCODE="/home/mikeqiu/S2223/SortTigress2"
CFILE="CalibrationFilePostExperiment.cal"

m=60647
n=1000000
RUNLIST="runs_good.txt"

# Checks
[[ -f "$RUNLIST" ]] || { echo "Run list '$RUNLIST' not found." >&2; exit 1; }
mkdir -p "$HISTDIR"
command -v hadd >/dev/null || { echo "ERROR: 'hadd' not found in PATH (ROOT not set up?)." >&2; exit 1; }
[[ -x "$SORTCODE" ]] || { echo "ERROR: SORTCODE not executable: $SORTCODE" >&2; exit 1; }
[[ -f "$CFILE" ]] || { echo "ERROR: calibration file not found: $CFILE" >&2; exit 1; }

all_run_outputs=()  # to optionally merge across runs at the end

while IFS= read -r line || [[ -n "$line" ]]; do
  runnum="${line#"${line%%[![:space:]]*}"}"
  runnum="${runnum%"${runnum##*[![:space:]]}"}"
  [[ -z "$runnum" || "$runnum" =~ ^# ]] && continue
  [[ "$runnum" =~ ^[0-9]+$ ]] || { echo "Skipping non-numeric run: '$runnum'"; continue; }

  if (( runnum > m && runnum < n )); then
    echo "== Processing run $runnum =="

    sub_outputs=()
    for f in "$ADIR"/analysis"${runnum}"_*.root; do
      [[ -f "$f" ]] || continue

      base="$(basename "$f" .root)"             # e.g., analysis60648_003
      out="$HISTDIR/Hist_${base}.root"          # per-subrun output

      echo "$SORTCODE" "$f" "$CFILE" "$out"
      "$SORTCODE" "$f" "$CFILE" "$out"
      sub_outputs+=("$out")
    done

    if ((${#sub_outputs[@]} == 0)); then
      echo "No subruns found for run $runnum"
      continue
    fi

    merged_run="$HISTDIR/Hist_run${runnum}.root"
    if ((${#sub_outputs[@]} == 1)); then
      # Single subrun -> just copy to the per-run merged name
      cp -f "${sub_outputs[0]}" "$merged_run"
    else
      # Merge (sum) histograms across subruns for this run
      echo "Merging ${#sub_outputs[@]} files -> $merged_run"
      hadd -f "$merged_run" "${sub_outputs[@]}"
    fi

    all_run_outputs+=("$merged_run")
  fi
done < "$RUNLIST"

# Optional: merge across all runs into one big file
if ((${#all_run_outputs[@]} > 1)); then
  mega="$HISTDIR/Hist_all_runs.root"
  echo "Merging all runs (${#all_run_outputs[@]}) -> $mega"
  hadd -f "$mega" "${all_run_outputs[@]}"
fi

echo "Finished."
