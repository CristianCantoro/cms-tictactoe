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

    player = data[0]
    start_move = int(data[1])
    seed = int(data[2])

    assert (player in ['X', 'O']), "Error: 'player' not 'X' or 'O'."
    assert (start_move in [1, 2]), "Error: 'start_move' not 1 or 2."

    # valid input
    return 0


if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()

    f = open(sys.argv[1]).readlines()

    exit(run(f))
