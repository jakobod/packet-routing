#!/bin/zsh

output_dir=${1%/}

message_log_dir=${output_dir}/message
load_log_dir=${output_dir}/load
program_log_dir=${output_dir}/logs

mkdir -p ${message_log_dir}
mkdir -p ${load_log_dir}
mkdir -p ${program_log_dir}

num_nodes=30
num_packets=250000
seed=12345
timeout=10s
runs=0

run_ant_benchmark num_transitions a b change
function run_ant_benchmark() {
  num_transitions=$1
  a=$2
  b=$3
  change=$4
  for d in $(seq 1 2 10); do
    for e in $(seq 0 0.2 1); do
      for run in $(seq 0 1 9); do
        file_name=ant_${num_transitions}_${a}_${b}_${d}_${e}_${change}_${run}
        message_log_path=${message_log_dir}/${file_name}.csv
        load_log_path=${load_log_dir}/${file_name}.csv
        program_log_path=${program_log_dir}/${file_name}.log
        # Until we find the problem with the segfaults, this has to suffice..
        echo "*** Running ant benchmark with: num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e}..."
        until build/main -n${num_nodes} -t${num_transitions} -s${seed} -a${a} -b${b} -d${d} -e${e} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}; do
          echo "!!! benchmark with num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e} failed. Rerunning..."
        done;
        echo "*** Done with benchmark num_nodes=${num_nodes}, num_transitions=${num_transitions}, a=${a}, b=${b}, d=${d}, and e=${e}!"
      done;
    done;
  done;
}

function random_benchmark() {
  num_transitions=$1
  change=$2  
  for run in $(seq 0 1 9); do
    file_name=random_${num_transitions}_${change}_${run}
    message_log_path=${message_log_dir}/${file_name}.csv
    load_log_path=${load_log_dir}/${file_name}.csv
    program_log_path=${program_log_dir}/${file_name}.log
    echo "*** Running random benchmark num_nodes=${num_nodes}, num_transitions=${num_transitions}, change=${change}..."
    until build/main -n${num_nodes} -t${num_transitions} -r -s${seed} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}; do
      echo "!!! Random benchmark failed. Rerunning..."
    done;
  done;
  echo "*** Done with random benchmark!"
}


for num_transitions in 435; do
  for a in 1.0; do
    for b in 1.0; do
      for change in 0ms 100ms 1000ms; do
        run_ant_benchmark ${num_transitions} ${a} ${b} ${change}
        random_benchmark ${num_transitions} ${change}
      done;
    done;
  done;
done;

echo "$runs will be run"
