import pandas as pd
import matplotlib.pyplot as plt
import argparse


def plot(input, output):
  df = pd.read_csv(input)
  df.plot()
  plt.title(input)
  plt.xlabel('Ping [#]')
  plt.ylabel('Delay [ms]')
  plt.show()
  # plt.savefig(output, bbox_inches='tight', transparent=True)


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument(
      'input', help='The input path', metavar='INPUT')
  parser.add_argument(
      'output', help='The output path', metavar='OUTPUT')

  args = parser.parse_args()
  plot(args.input, args.output)


if __name__ == '__main__':
  main()
