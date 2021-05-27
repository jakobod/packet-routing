#!/bin/zsh

output_path="../output"



for d in $(seq 0 0.2 1); do
  for e in $(seq 0 0.2 1); do 
    build/main -n 30 -t 100 -s12345 -d${d} -e${e} -m100 -o "${output_path}/results_${d}_${e}.csv" > /dev/null
  done;
done;





