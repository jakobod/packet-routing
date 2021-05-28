#!/bin/zsh

# TODO: 
# [ ] scale the number of nodes and transitions!

output_path=${1%/}

for d in $(seq 0 0.1 1); do
  for e in $(seq 0 0.1 1); do 
    # Until we find the problem with the segfaults, this has to suffice..
    until build/main -n 30 -t 100 -s12345 -d${d} -e${e} -m10000 -o "${output_path}/ant_${d}_${e}.csv" > "${output_path}/ant_${d}_${e}.log"; do
      echo "!! benchmark with d=${d} and e=${e} failed. Rerunning..."
    done;
    echo ">> done with benchmark d=${d} and e=${e}"
  done;
done;


until build/main -n 30 -t 100 -r -s12345 -m10000 -o "${output_path}/random.csv" > "${output_path}/random.log"; do
  echo "random benchmark failed. Rerunning..."
done;