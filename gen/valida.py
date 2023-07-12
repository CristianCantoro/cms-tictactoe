#!/usr/bin/env python3

import os
import sys


def usage():
    print("Usage:\n  {} file_input.txt"
          .format(os.path.basename(sys.argv[0])),
          file=sys.stderr)
    exit(1)


def run(f):

    assert (len(f) == 1), \
       "Error: unexpected number of lines."

    data = f[0].strip().split()

    player = int(data[0])
    seed = int(data[1])

    assert (player in [1, 2]), "Error: 'player' not 1 or 2."
    assert (seed > 0), "Error: Invalid seed."

    # valid input
    return 0


if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()

    f = open(sys.argv[1]).readlines()

    exit(run(f))
