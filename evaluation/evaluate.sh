#!/bin/zsh

# num_nodes           100,    1000
# num_transitions    1000,  10.000
# deposition            0,       1
# evaporation           0,       1
# alpha                 0,       1
# beta                  0,       1

output_dir=${1%/}
message_log_dir=${output_dir}/message
output_dir=${output_dir}/message_evaluated
mkdir -p ${output_dir}

function concatenate() {
  num_nodes=100
  num_transitions=2000
  a=1
  b=1
  for d in 1 2 3 4 5 6 7 8 9 10; do
    for e in 0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0; do
        file_name=ant_${num_nodes}_${num_transitions}_${d}_${e}
        input_path=${message_log_dir}/${file_name}
        output_file_name=${output_dir}/${file_name}
        echo "Evaluating ${input_path}"
        ./evaluation/plot.py -c -r -i ${input_path} -o ${output_file_name}.csv
    done;
  done;
}

concatenate
