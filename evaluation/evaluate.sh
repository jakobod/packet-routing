#!/bin/zsh

input_dir=${1%/}
fig_dir=${2%/}

duration_dir=${fig_dir}/duration
path_length_dir=${fig_dir}/path_length
success_dir=${fig_dir}/success

mkdir -p  ${duration_dir}
mkdir -p  ${path_length_dir}
mkdir -p  ${success_dir}

# ant_435_<deposition>_<evaporation>_<change>
# '--duration', '-d'
# '--path-length', '-p'
# '--success', '-s'

for change in 0ms 100ms 1000ms; do
  mkdir -p ${duration_dir}/${change}
  mkdir -p ${path_length_dir}/${change}
  mkdir -p ${success_dir}/${change}

  for dep in $(seq 1 2 10); do
    # success
    files=( ${input_dir}/arrived/ant_435_${dep}_*_${change}* ${input_dir}/arrived/random_435_${change}* )
    echo "python evaluation/plot.py -s --deposition-const -i ${files} -o ${success_dir}/${change}/ant_dep${dep}_change${change}.pdf"
    python evaluation/plot.py -s --deposition-const -i ${files} -o ${success_dir}/${change}/ant_dep${dep}_change${change}.pdf
    
    files=( ${input_dir}/duration/ant_435_${dep}_*_${change}* ${input_dir}/duration/random_435_${change}* )
    echo "python evaluation/plot.py -d --deposition-const -i ${files} -o ${duration_dir}/${change}/ant_dep${dep}_change${change}.pdf"
    python evaluation/plot.py -d --deposition-const -i ${files} -o ${duration_dir}/${change}/ant_dep${dep}_change${change}.pdf
    
    # path-length
    files=( ${input_dir}/path_length/ant_435_${dep}_*_${change}* ${input_dir}/path_length/random_435_${change}* )
    echo "python evaluation/plot.py -p --deposition-const -i ${files} -o ${path_length_dir}/${change}/ant_dep${dep}_change${change}.pdf"
    python evaluation/plot.py -p --deposition-const -i ${files} -o ${path_length_dir}/${change}/ant_dep${dep}_change${change}.pdf
  done;

  for evap in 0.0 0.2 0.4 0.6 0.8 1.0; do
    # success
    files=( ${input_dir}/arrived/ant_435_*_${evap}_${change}* ${input_dir}/arrived/random_435_${change}* )
    echo "python evaluation/plot.py -s -i ${files} -o ${success_dir}/${change}/ant_evap${evap}_change${change}.pdf"
    python evaluation/plot.py -s -i ${files} -o ${success_dir}/${change}/ant_evap${evap}_change${change}.pdf
    
    files=( ${input_dir}/duration/ant_435_*_${evap}_${change}* ${input_dir}/duration/random_435_${change}* )
    echo "python evaluation/plot.py -d -i ${files} -o ${duration_dir}/${change}/ant_evap${evap}_change${change}.pdf"
    python evaluation/plot.py -d -i ${files} -o ${duration_dir}/${change}/ant_evap${evap}_change${change}.pdf
    
    # path-length
    files=( ${input_dir}/path_length/ant_435_*_${evap}_${change}* ${input_dir}/path_length/random_435_${change}* )
    echo "python evaluation/plot.py -p -i ${files} -o ${path_length_dir}/${change}/ant_evap${evap}_change${change}.pdf"
    python evaluation/plot.py -p -i ${files} -o ${path_length_dir}/${change}/ant_evap${evap}_change${change}.pdf
  done;

  # success
  files=( ${input_dir}/arrived/random_435_${change}* )
  echo "python evaluation/plot.py -s -i ${files} -o ${success_dir}/${change}/random_change${change}.pdf"
  python evaluation/plot.py -s -i ${files} -o ${success_dir}/${change}/random_change${change}.pdf
  
  files=( ${input_dir}/duration/random_435_${change}* )
  echo "python evaluation/plot.py -d -i ${files} -o ${duration_dir}/${change}/random_change${change}.pdf"
  python evaluation/plot.py -d -i ${files} -o ${duration_dir}/${change}/random_change${change}.pdf
  
  # path-length
  files=( ${input_dir}/path_length/random_435_${change}* )
  echo "python evaluation/plot.py -p -i ${files} -o ${path_length_dir}/${change}/random_change${change}.pdf"
  python evaluation/plot.py -p -i ${files} -o ${path_length_dir}/${change}/random_change${change}.pdf


done;
