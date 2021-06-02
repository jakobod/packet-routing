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
    plt.savefig(output, bbox_inches='tight', transparent=True)
  else:
    plt.show()


def plot_single(input, output, title):
  print('plotting single')
  df = pd.read_csv(input)
  df.plot(y='duration')
  y_av = movingaverage(df['duration'], 10)
  plt.plot(y_av, color='r', linestyle='-')

  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')

  save_or_show(output)


def plot_multiple(inputs, output, title, windowsize=50):
  print('plotting multiple')
  fig, ax = plt.subplots()
  for input in inputs:
    df = pd.read_csv(input)
    y_av = movingaverage(df['duration'], windowsize)
    ax.plot(y_av, linestyle='-', label=input)

  ax.legend()
  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')
  save_or_show(output)


def evaluate(input):
  df = pd.read_csv(input[0])
  print(df)


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument(
      '--title', '-t', help='The title of the plot', metavar='TITLE')
  parser.add_argument(
      '--evaluate', '-e', help='Evaluate the given csvs', action='store_true')

  args = parser.parse_args()
  if args.evaluate:
    evaluate(args.input)
  elif len(args.input) == 1:
    plot_single(args.input[0], args.output, args.title)
  else:
    plot_multiple(args.input, args.output, args.title)


if __name__ == '__main__':
  main()
