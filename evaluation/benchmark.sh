#!/bin/zsh

# 
# num_nodes           100,    1000
# num_transitions    1000,  10.000
# deposition            0,       1
# evaporation           0,       1
# alpha                 0,       1
# beta                  0,       1

output_path=${1%/}
log_path=${output_path}/logs
mkdir -p ${log_path}

num_packets=1000000
seed=123456

for num_nodes in 100 1000 do
  for num_transitions in 1000 10000 do
    for d in 0 1 do
      for e in 0 1 do
        for a in 0 1 do
          for b in 0 1; do
            message_log_path=${output_path}/ant_message_${a}_${b}_${d}_${e}.csv
            load_log_path=${output_path}/ant_load_${a}_${b}_${d}_${e}.csv
            log_path=${log_path}/ant_${a}_${b}_${d}_${e}.log
            # Until we find the problem with the segfaults, this has to suffice..
            echo "*** Running ant benchmark with: num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e}..."
            until build/main -n${num_nodes} -t${num_transitions} -s${seed} -a${a} -b${b} -d${d} -e${e} -m${num_packets} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${log_path}; do
              echo "!!! benchmark with num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e} failed. Rerunning..."
            done;
            echo "*** Done with benchmark num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e}!"
          done;
        done;
      done;
    done;
  done;
done;


function random_benchmark() {
  num_nodes=${1}
  num_transitions=${2}
  echo "*** Running random benchmark num_nodes=${num_nodes}, num_transitions=${num_transitions}..."
  until build/main -n${num_nodes} -t${num_transitions} -r -s${seed} -m${num_packets} --logging.message-log-path=${output_path}/random_message_${num_nodes}_${num_transitions}.csv --logging.load-log-path=${output_path}/random_load_${num_nodes}_${num_transitions}.csv > ${log_path}/random_${num_nodes}_${num_transitions}.log; do
    echo "!!! Random benchmark failed. Rerunning..."
  done;
  echo "*** Done with random benchmark!"
}

for num_nodes in $(seq 100 100 1000); do
  for num_transitions in $(seq 1000 1000 10000); do
    random_benchmark $num_nodes $num_transitions
  done;
done;
