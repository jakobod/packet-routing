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
timeout=30s

function run_ant_benchmark() {
  change=$1
  for d in $(seq 1 2 10); do
    for e in 0.0 0.2 0.4 0.6 0.8 1.0; do
      for run in $(seq 0 1 9); do
        file_name=ant_${num_transitions}_${d}_${e}_${change}
        run_file_name=${file_name}_${run}
        message_log_path=${message_log_dir}/${run_file_name}.csv
        load_log_path=${load_log_dir}/${run_file_name}.csv
        program_log_path=${program_log_dir}/${run_file_name}.log

        Run the benchmark
        echo "build/main -n${num_nodes} -t${num_transitions} -s${seed} -d${d} -e${e} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}"
        until build/main -n${num_nodes} -t${num_transitions} -s${seed} -d${d} -e${e} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}; do
          echo "!!! Run failed. Rerunning!"
        done;
      done;

      # Concatenate runtimes of all runs
      for column in arrived duration path_length; do
        mkdir -p ${message_log_dir}/${column} > /dev/null
        files=( ${message_log_dir}/${file_name}* )
        echo "python evaluation/plot.py -c ${column} -r -i ${files} -o ${message_log_dir}/${column}/${file_name}.csv"
        python evaluation/plot.py -c ${column} -r -i ${files} -o ${message_log_dir}/${column}/${file_name}.csv
      done;
      mkdir -p ${message_log_dir}/arrived > /dev/null
      files=( ${message_log_dir}/${file_name}* )
      echo "python evaluation/plot.py -c arrived -i ${files} -o ${message_log_dir}/arrived/${file_name}.csv"
      python evaluation/plot.py -c arrived -i ${files} -o ${message_log_dir}/arrived/${file_name}.csv
    done;
  done;
}

function random_benchmark() {
  change=$1
  for run in $(seq 0 1 9); do
    file_name=random_${num_transitions}_${change}
    run_file_name=${file_name}_${run}
    message_log_path=${message_log_dir}/${run_file_name}.csv
    load_log_path=${load_log_dir}/${run_file_name}.csv
    program_log_path=${program_log_dir}/${run_file_name}.log
    evaluated_log_path=${message_log_dir}/evaluated/${file_name}.csv

    # Run the benchmark
    echo "build/main -n${num_nodes} -t${num_transitions} -r -s${seed} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}"
    until build/main -n${num_nodes} -t${num_transitions} -r -s${seed} -m${num_packets} -c${change} --drop-timeout=${timeout} --logging.message-log-path=${message_log_path} --logging.load-log-path=${load_log_path} > ${program_log_path}; do
      echo "!!! Run failed. Rerunning!"
    done;
  done;

  # Concatenate runtimes of all runs
  for column in duration path_length; do
    mkdir -p ${message_log_dir}/${column} > /dev/null
    files=( ${message_log_dir}/${file_name}* )
    echo "python evaluation/plot.py -c ${column} -r -i ${files} -o ${message_log_dir}/${column}/${file_name}.csv"
    python evaluation/plot.py -c ${column} -r -i ${files} -o ${message_log_dir}/${column}/${file_name}.csv
  done;
  mkdir -p ${message_log_dir}/arrived > /dev/null
  files=( ${message_log_dir}/${file_name}* )
  echo "python evaluation/plot.py -c arrived -i ${files} -o ${message_log_dir}/arrived/${file_name}.csv"
  python evaluation/plot.py -c arrived -i ${files} -o ${message_log_dir}/arrived/${file_name}.csv
}

for change in 0ms 100ms 1000ms; do
  run_ant_benchmark ${change}
  random_benchmark ${change}
done;
