#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include "tictactoe.h"

using namespace std;

int main(int argc, char **argv) {
  signal(SIGPIPE, SIG_IGN);

  FILE *fin, *fout, *fifo_in, *fifo_out;

  fin = fopen("input.txt", "r");
  fout = fopen("output.txt", "w");
  fifo_in = fopen(argv[1], "w");
  fifo_out = fopen(argv[2], "r");

  int player;
  fscanf(fin, "%d", &player);
  fprintf(fifo_in, "%d\n", player);
  fflush(fifo_in);


  while (true) {
    fscanf(fifo_out, "%d", &res);
    if (res < 0) {
      fprintf(fifo_in, "%d\n", c);
      fflush(fifo_in);
    } else {
      fprintf(fout, "%d\n", res);
      fflush(fout);
      break;
    }
  }


  if (a+b == res) {
    fprintf(stderr, "A-ha, you're the best adding program I've ever met!\n");
    printf("1.0\n");
  } else {
    fprintf(stderr, "How dreadful, never met anyone as dumb as you...\n");
    printf("0.0\n");
  }

  fclose(fin);
  fclose(fout);
  fclose(fifo_in);
  fclose(fifo_out);

}

