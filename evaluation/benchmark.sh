#!/bin/zsh

output_dir=${1%/}

message_log_dir=${output_dir}/message
load_log_dir=${output_dir}/load
program_log_dir=${output_dir}/logs

mkdir -p ${message_log_dir}
mkdir -p ${load_log_dir}
mkdir -p ${program_log_dir}

num_nodes=30
num_transitions=435
num_packets=250000
seed=12345
a=1
b=1
timeout=10s

function run_ant_benchmark() {
  for change in 0ms 100ms 1000ms; do
    for d in $(seq 1 1 10); do
      for e in $(seq 0 0.1 1); do
        for run in $(seq 0 1 9); do
          file_name=ant_${num_nodes}_${num_transitions}_${d}_${e}_${change}_${run}
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
  done;
}

function random_benchmark() {
  for change in 100ms 1000ms; do
    for run in $(seq 0 1 9); do
      file_name=random_${change}_${run}
      message_log_path=${message_log_dir}/${file_name}.csv
      load_log_path=${load_log_dir}/${file_name}.csv
      program_log_path=${program_log_dir}/${file_name}.log
      echo "*** Running random benchmark num_nodes=${num_nodes}, num_transitions=${num_transitions}, change=${change}..."
      until build/main -n${num_nodes} -t${num_transitions} -r -s${seed} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}; do
        echo "!!! Random benchmark failed. Rerunning..."
      done;
    done;
  done;
  echo "*** Done with random benchmark!"
}

run_ant_benchmark

random_benchmark
