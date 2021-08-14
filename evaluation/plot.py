#!/Users/boss/code/packet-routing/evaluation/envs/bin/python

import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
import numpy as np


# File name convention
# ${name}_${num_nodes}_${num_transitions}_${a}_${b}_${d}_${e}

# TODO
# [ ] Plot path length
# [ ] Plot duration
# [ ] Plot success


# take all files in folder and sort by avg duration.
# Print some kind of scale + file names for easier evaluation afterward


plt.rc('figure', figsize=(16, 9))


# -- helper functions ----------------------------------------------------------

def concatenate(input, remove_dropped, output):
  inputs = [f'{input}_{x}.csv' for x in range(0, 10)]
  dataframes = [pd.read_csv(input,
                            usecols=['duration', 'arrived', 'path_length'])
                for input in inputs]
  if remove_dropped:
    dataframes = [df[df.arrived] for df in dataframes]
    dataframes = [df[df.path_length != 0] for df in dataframes]
  dataframes = [df.drop(['arrived', 'path_length'], axis=1)
                for df in dataframes]
  res = pd.concat(dataframes, axis=1)
  if output:
    res.to_csv(output, index=False)
  else:
    print(res)


def movingaverage(interval, window_size):
  window = np.ones(int(window_size)) / float(window_size)
  res = np.convolve(interval, window, 'same')
  return res, np.mean(res), np.median(res)


def save_or_show(output):
  if output:
    plt.savefig(f'{output}.png', bbox_inches='tight', transparent=False)
  else:
    plt.show()


# -- plot functions ------------------------------------------------------------


def plot_single(input, output, title):
  print(f'plotting {input}')
  df = pd.read_csv(input)
  df['mean'] = df.mean(axis=1)
  df.reset_index().plot.scatter(x='index', y='mean', zorder=1, s=5,
                                alpha=0.1)
  y_av, mean, median = movingaverage(df['mean'], 250)
  plt.plot(y_av, color='r', linestyle='-', zorder=2, label='moving average')
  ax = plt.gca()
  ax.legend()
  if title:
    plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)
  plt.close('all')
  return mean, median


def plot_multiple(inputs, output_dir):
  plots = []
  print('plotting multiple')
  if output_dir:
    output_dir = os.path.normpath(output_dir)
  for input in inputs:
    filename = os.path.basename(input)
    if output_dir:
      output_path = f'{output_dir}/{os.path.splitext(filename)[0]}'
    else:
      output_path = None
    mean, median = plot_single(input, output_path, None)
    plots.append((mean, median, input))
  with open(f'{output_dir}/ranking.txt', "w") as f:
    for p in sorted(plots, key=lambda tup: tup[0]):
      f.write(str(p) + '\n')


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument(
      '--title', '-t', help='The title of the plot', metavar='TITLE')
  parser.add_argument('--remove-dropped', '-r',
                      help='Remove the dropped packets values',
                      action='store_true')
  parser.add_argument('--concatenate', '-c',
                      help='Concatenate multiple runs and ',
                      action='store_true')

  args = parser.parse_args()
  if not args.input:
    print('Please specify an input to read')
  elif args.concatenate:
    concatenate(args.input[0], args.remove_dropped, args.output)
  elif len(args.input) == 1:
    plot_single(args.input[0], args.output, args.title)
  else:
    if args.output:
      plot_multiple(args.input, args.output)
    else:
      print('Output HAS to be specified for multiple inputs')


if __name__ == '__main__':
  main()
