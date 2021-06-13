#!/bin/zsh

# TODO: 
# [ ] scale the number of nodes and transitions!

output_path=${1%/}
log_path=${output_path}/logs
mkdir -p ${log_path}

for d in $(seq 0 0.1 1); do
  for e in $(seq 0 0.1 1); do 
    # Until we find the problem with the segfaults, this has to suffice..
    echo "*** Running benchmark d=${d} and e=${e}..."
    until build/main -n 30 -t 100 -s12345 -d${d} -e${e} -m10000 --logging.message-log-path=${output_path}/ant_message_${d}_${e}.csv --logging.load-log-path=${output_path}/ant_load_${d}_${e}.csv > ${log_path}/ant_${d}_${e}.log; do
      echo "!!! benchmark with d=${d} and e=${e} failed. Rerunning..."
    done;
    echo "*** Done with benchmark d=${d} and e=${e}!"
  done;
done;

echo "*** Running random benchmark..."
until build/main -n 30 -t 100 -r -s12345 -m10000 --logging.message-log-path=${output_path}/ant_message_${d}_${e}.csv --logging.load-log-path=${output_path}/ant_load_${d}_${e}.csv > ${log_path}/random.log; do
  echo "!!! Random benchmark failed. Rerunning..."
done;
echo "*** Done with random benchmark!"