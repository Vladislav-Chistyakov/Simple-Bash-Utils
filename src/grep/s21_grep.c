#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void output(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  output(argc, argv); 
  return 0;
}

void output(int argc, char *argv[]) {
  printf("n %d  %s", argc, argv[0]);
}