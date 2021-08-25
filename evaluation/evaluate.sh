#!/bin/zsh

output_dir=${1%/}
fig_dir=${2%/}

message_log_dir=${output_dir}/message
output_dir=${output_dir}/message_evaluated

mkdir -p ${message_log_dir}
mkdir -p ${output_dir}

num_nodes=30

function run_ant_benchmark() {
  num_transitions=$1
  a=$2
  b=$3
  change=$4
  for d in $(seq 1 2 10); do
    for e in 0.0 0.2 0.4 0.6 0.8 1.0; do
      file_name=ant_${num_transitions}_${a}_${b}_${d}_${e}_${change}
      input=${message_log_dir}/${file_name}
      output_file_name=ant_${d}_${e}_${change}
      output=${output_dir}/${output_file_name}.csv
      ./evaluation/plot.py -c -r -i ${input} -o ${output}
    done;
  done;

}

function random_benchmark() {
  num_transitions=$1
  change=$2  
  file_name=random_${num_transitions}_${change}
  input=${message_log_dir}/${file_name}
  file_name=random_${change}
  output=${output_dir}/${output_file_name}.csv
  python evaluation/plot.py -c -r -i ${input} -o ${output} &
}


# for num_transitions in 435; do
#   for a in 1.0; do
#     for b in 1.0; do
#       for change in 0ms 100ms 1000ms; do
#         run_ant_benchmark ${num_transitions} ${a} ${b} ${change}
#         random_benchmark ${num_transitions} ${change}
#       done;
#     done;
#   done;
# done;

# ant_<deposition>_<evaporation>_<change>

for change in 0ms 100ms 1000ms; do
  for dep in $(seq 1 2 10); do
    files=( ${output_dir}/ant_${dep}_*_${change}* )
    echo "python evaluation/plot.py -a -i ${files} -o ${fig_dir}/ant_dep${dep}_change${change}_all.pdf"
    python evaluation/plot.py -a -i ${files} -o ${fig_dir}/ant_dep${dep}_change${change}_all.pdf
  done;

  for evap in 0.0 0.2 0.4 0.6 0.8 1.0; do
    files=( ${output_dir}/ant_*_${evap}_${change}* )
    echo "python evaluation/plot.py -a -i ${files} -o ${fig_dir}/ant_evap${evap}_change${change}_all.pdf"
    python evaluation/plot.py -a -i ${files} -o ${fig_dir}/ant_evap${evap}_change${change}_all.pdf
  done;
done;
