import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np


plt.rc('figure', figsize=(16, 9))


def movingaverage(interval, window_size):
  window = np.ones(int(window_size))/float(window_size)
  return np.convolve(interval, window, 'same')


def plot(input, output, title):
  df = pd.read_csv(input)
  df.plot()
  y_av = movingaverage(df[' duration'], 50)
  plt.plot(range(0, len(df)), y_av, color='r', linestyle='-')

  plt.title(title)
  plt.xlabel('Message number [#]')
  plt.ylabel('Duration [ms]')

  plt.savefig(output, bbox_inches='tight', transparent=True)


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument(
      'input', help='The input path', metavar='INPUT')
  parser.add_argument(
      'output', help='The output path', metavar='OUTPUT')
  parser.add_argument(
      'title', help='The title of the plot', metavar='TITLE')

  args = parser.parse_args()
  plot(args.input, args.output, args.title)


if __name__ == '__main__':
  main()
