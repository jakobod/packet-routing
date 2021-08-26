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

def concatenate(inputs, remove_dropped, column, output):
  print(f'concatenate {inputs}')
  dataframes = [pd.read_csv(input,
                            usecols=[column, 'arrived', 'path_length'])
                for input in inputs]
  if remove_dropped:
    dataframes = [df[df.arrived] for df in dataframes]
    dataframes = [df[df.path_length != 0] for df in dataframes]
  if column == 'arrived':
    dataframes = [df.drop(['path_length'], axis=1)
                  for df in dataframes]
  elif column == 'path_length':
    dataframes = [df.drop(['arrived'], axis=1)
                  for df in dataframes]
  else:
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
    plt.savefig(f'{output}', bbox_inches='tight', transparent=False)
  else:
    plt.show()


# -- plot functions ------------------------------------------------------------


def plot_single(input, output):
  df = pd.read_csv(input)
  df['mean'] = df.mean(axis=1)
  df.reset_index().plot.scatter(x='index', y='mean', zorder=1, s=5,
                                alpha=0.1)
  y_av, mean, median = movingaverage(df['mean'], 250)
  plt.plot(y_av, color='r', linestyle='-', zorder=2, label='moving average')
  ax = plt.gca()
  ax.legend()
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)
  plt.close('all')
  return mean, median


def plot(inputs, output, title, xlabel, ylabel, window=250):
  _, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    df['mean'] = df.mean(axis=1)
    y_av, _, _ = movingaverage(df['mean'], window)
    ax.plot(y_av, linestyle='-', label=os.path.basename(input))
  ax.legend()
  plt.title(title)
  plt.xlabel(xlabel)
  plt.ylabel(ylabel)
  save_or_show(output)


def plot_duration(inputs, output):
  plot(inputs, output, 'Duration', 'Message [#]', 'Duration [ms]')


def plot_success(inputs, output):
  plot(inputs, output, 'Success', 'Message [#]', 'Succes [%]')


def plot_path_length(inputs, output):
  plot(inputs, output, 'Path lengths', 'Message [#]', 'Path length [#]')


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument('--concatenate', '-c',
                      help='Concatenate multiple runs and ')
  parser.add_argument('--duration', '-d',
                      help='Combine all to single plot',
                      action='store_true')
  parser.add_argument('--path-length', '-p',
                      help='Combine all to single plot',
                      action='store_true')
  parser.add_argument('--success', '-s',
                      help='Combine all to single plot',
                      action='store_true')
  parser.add_argument('--remove-dropped', '-r',
                      help='Remove the dropped packets values',
                      action='store_true')

  args = parser.parse_args()
  if not args.input:
    print('Please specify an input to read')
  elif args.concatenate:
    concatenate(args.input, args.remove_dropped,
                args.concatenate, args.output)
  elif len(args.input) == 1:
    plot_single(args.input[0], args.output)
  else:
    if args.duration:
      plot_duration(args.input, args.output)
    elif args.path_length:
      plot_path_length(args.input, args.output)
    elif args.success:
      plot_success(args.input, args.output)
    else:
      print('Output HAS to be specified for multiple inputs')


if __name__ == '__main__':
  main()
