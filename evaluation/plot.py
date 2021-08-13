#!/Users/boss/code/packet-routing/evaluation/envs/bin/python

import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np


# File name convention
# ${name}_${num_nodes}_${num_transitions}_${a}_${b}_${d}_${e}

# TODO
# [ ] Plot path length
# [ ] Plot duration
# [ ] Plot success


plt.rc('figure', figsize=(16, 9))


def movingaverage(interval, window_size):
  window = np.ones(int(window_size)) / float(window_size)
  res = np.convolve(interval, window, 'same')
  return res, np.mean(res)


def save_or_show(output):
  if output:
    plt.savefig(output, bbox_inches='tight', transparent=False)
  else:
    plt.show()


def plot_single(input, output, title):
  print('plotting single')
  df = pd.read_csv(input)
  df.dropna()
  df['mean'] = df.mean(axis=1)
  print(df)
  df.reset_index().plot.scatter(x='index', y='mean', zorder=1, s=5,
                                alpha=0.1)
  y_av, _ = movingaverage(df['mean'], 100)
  plt.plot(y_av, color='r', linestyle='-', zorder=2, label='moving average')

  ax = plt.gca()
  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')

  save_or_show(output)


def plot_multiple(inputs, output, title):
  print('plotting multiple runs')
  _, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    y_av, _ = movingaverage(df['duration'], 100)
    ax.plot(y_av, linestyle='-', label=input)
  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)


def plot_multiple(inputs, output, title):
  print('plotting multiple')
  _, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    y_av, _ = movingaverage(df['duration'], 100)
    ax.plot(y_av, linestyle='-', label=input)
  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)


def plot_multiple_selection(inputs, output, title):
  print('plotting multiple')
  avgs = []
  _, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    avg, mean = movingaverage(df['duration'], 1000)
    avgs.append((avg, mean, input))
  for tup in sorted(avgs, key=lambda tup: tup[1])[:4]:
    ax.plot(tup[0], linestyle='-', label=tup[2])
  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)


def throughput(inputs, output, title):
  _, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    print(df)
    begin = df['time_created'][0]
    end = df['time_received'][len(df['time_received']) - 1]
    print(f'begin = {begin}, end = {end}')
    runtime = (end - begin) / 1000
    throughput = len(df) / runtime
    print(
        f'runtime = {runtime}s, # messages={len(df)}, throughput={throughput}')
    ax.bar(x=input, height=throughput, label=input)
  plt.title(title)
  plt.xticks(rotation=45)
  plt.xlabel('Configuration')
  plt.ylabel('Throughput [#/s]')
  save_or_show(output)


def plot_success(inputs, output, title, window=100):
  for input in inputs:
    df = pd.read_csv(input)
    print(df)
    counts = df.rolling(window=window).success.sum()
    print(counts)
    counts = counts.divide(window)
    counts = counts.multiply(100)
    counts.plot(label=input)
  ax = plt.gca()
  ax.set_ylim([0, 105])
  if len(inputs) > 1:
    ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Success Rate per 100 messages[%/100 msg]')
  save_or_show(output)


def plot_duration(input):
  df = pd.read_csv(input, nrows=10000)
  print(df)
  df.plot(x='msg_id', y='duration')


def plot_path_length(input):
  df = pd.read_csv(input, nrows=10000)
  print(df)
  df.plot(x='msg_id', y='path_length')


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument(
      '--title', '-t', help='The title of the plot', metavar='TITLE')

  args = parser.parse_args()
  if not args.input:
    print('Please specify an input to read')
  elif len(args.input) == 1:
    plot_single(args.input[0], args.output, args.title)
  else:
    # plot_success(args.input, args.output, args.title)
    # throughput(args.input, args.output, args.title)
    plot_multiple_runs(args.input, args.output, args.title)
    # plot_multiple(args.input, args.output, args.title)
    # plot_multiple_selection(args.input, args.output, args.title)


if __name__ == '__main__':
  main()
