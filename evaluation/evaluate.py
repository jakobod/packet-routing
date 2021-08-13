#!/Users/boss/code/packet-routing/evaluation/envs/bin/python

import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np


def evaluate(inputs, remove_dropped, output):
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


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--input', '-i', help='The input path',
                      nargs='+', default=[], metavar='INPUT')
  parser.add_argument(
      '--output', '-o', help='The output path', metavar='OUTPUT')
  parser.add_argument('--remove-dropped', '-r',
                      help='Remove the dropped packets values',
                      action='store_true')

  args = parser.parse_args()
  if not args.input:
    print('Please specify an input to read')
  else:
    evaluate(args.input, args.remove_dropped, args.output)


if __name__ == '__main__':
  main()
