#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 512

struct flags {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
  int flag_error;
};
struct flags flag = {0};
struct flags *p_flags = &flag;

void output(int argc, char *argv[]);
void switchCase(int val, char *pattern);
void parser(char *pattern, char *argv[]);
void textArg(char *pattern);

int main(int argc, char *argv[]) {
  output(argc, argv); 
  return 0;
}

void output(int argc, char *argv[]) {
  int status = 0;
  int testArg = 1;
  char *params = "e:ivclnhsf:o?";
  char pattern[BUFFER_SIZE] = {0};
  if (argc > 1) {
    while ((status = getopt_long(argc, argv, params, 0, NULL)) != -1) {
      switchCase(status, pattern);
      if (flag.flag_e && argc == 2) {
        testArg = 0;
        break;
      }
    }
    if (testArg) {
      parser(pattern, argv);
    }
  }
}

void parser(char *pattern, char *argv[]) {
  if (!flag.flag_e & !flag.flag_f) {
    pattern = argv[optind];
  }
  printf("%s", pattern);
}

// Создаём строку pattern
void textArg(char *pattern) {

  if (pattern[0] == 0) {
    strcat(pattern, optarg);
  } else {
    strcat(pattern, "|");
    strcat(pattern, optarg);
  }
}

// Свич Кейс
void switchCase(int val, char *pattern) {
  switch (val) {
    case 'e':
      p_flags->flag_e = 1;
      textArg(pattern);
      break;
    case 'i':
      p_flags->flag_i = 1;
      break;
    case 'v':
      p_flags->flag_v = 1;
      break;
    case 'c':
      p_flags->flag_c = 1;
      break;
    case 'l':
      p_flags->flag_l = 1;
      break;
    case 'n':
      p_flags->flag_n = 1;
      break;
    case 'h':
      p_flags->flag_h = 1;
      break;
    case 's':
      p_flags->flag_s = 1;
      break;
    case 'f':
      p_flags->flag_f = 1;
      // funcFlagF(optarg, pattern);
      printf("SFFFFF\n");
      break;
    case 'o':
      p_flags->flag_o = 1;
      break;
    case '?':
    default:
      fprintf(stderr, "Флаг не найден\n");
      exit(1);
  }
}