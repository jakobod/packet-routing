import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np


plt.rc('figure', figsize=(16, 9))


def movingaverage(interval, window_size):
  window = np.ones(int(window_size))/float(window_size)
  return np.convolve(interval, window, 'same')


def save_or_show(output):
  if output:
    plt.savefig(output, bbox_inches='tight', transparent=False)
  else:
    plt.show()


def plot_single(input, output, title):
  print('plotting single')
  df = pd.read_csv(input)
  print(df)
  df.plot(y='duration', zorder=1)
  y_av = movingaverage(df['duration'], 100)
  plt.plot(y_av, color='r', linestyle='-', zorder=2, label='moving average')
  mean_duration = df['duration'].mean()
  print(f'mean_duration={mean_duration}')
  plt.hlines(mean_duration, xmin=0, xmax=len(
      df), color='orange', zorder=3, label='mean runtime')

  ax = plt.gca()
  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')

  save_or_show(output)


def plot_multiple(inputs, output, title):
  print('plotting multiple')
  fig, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    y_av = movingaverage(df['duration'], 100)
    ax.plot(y_av, linestyle='-', label=input)

  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)


def throughput(inputs, output, title):
  fig, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    print(df)
    begin = df['time_created'][0]
    end = df['time_received'][len(df['time_received'])-1]
    print(f'begin = {begin}, end = {end}')
    runtime = (end - begin) / 1000
    throughput = len(df) / runtime
    print(
        f'runtime = {runtime}s, # messages = {len(df)}, throughput = {throughput}')
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


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument(
      '--title', '-t', help='The title of the plot', metavar='TITLE')
  parser.add_argument(
      '--mode', '-m', help='The mode in which to run the script', default='plot')

  args = parser.parse_args()
  if args.mode == 'success':
    plot_success(args.input, args.output, args.title)
  elif args.mode == 'throughput':
    throughput(args.input, args.output, args.title)
  elif args.mode == 'plot':
    if len(args.input) == 1:
      plot_single(args.input[0], args.output, args.title)
    else:
      plot_multiple(args.input, args.output, args.title)


if __name__ == '__main__':
  main()
