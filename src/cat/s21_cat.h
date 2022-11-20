#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions_cat.c"
#include "functions_cat.h"
#include "output.c"
#include "output.h"

// структура для записи найденных флагов
struct flags {
  int flag_b;
  int flag_s;
  int flag_n;
  int flag_e;
  int flag_t;
  int flag_v;
};
struct flags flagsCat = {0};
struct flags *p_flags = &flagsCat;
static struct option long_options[] = {
    {"number-nonblank", 0, 0, 'b'},
    {"number", 0, 0, 'n'},
    {"squeeze-blank)", 0, 0, 's'},
};

extern void catFunc(int argc, char *argv[]);
