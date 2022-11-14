
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

// структура для записи найденных флагов
// FLAGI
// - o Печатает только совпадающие(непустые)
// части совпавшей строки.
// void stringBuffer(char *string);
void textArg(char *optarg, char *pattern);
void grepNoFlags(int optind, int argc, char *pattern, char *argv[]);
void flagsC(int testFiles, char *nameFile, int numberStrC);
void flagcL(char *nameFile);
void flagN(int numberStr, int n);
void funcFlagF(char *fileName, char *pattern);
void flagTraining(int numberFiles, char *argv[], char *buffer, int numberStr,
                  int numberArg);

// void funcFlagO(char *pattern, int *numberO, char *buffer);
// void funcFlagE(char *argv[], int optind, int testFiles, char *pattern);
// void flagPracticeTemplate(char *argv[], int optind, int numberStr,
//                           int *numberStrC, int testFiles, char *buffer,
//                           char *pattern, int *numberO);

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
};
struct flags flagsGrep = {0};
struct flags *p_flags = &flagsGrep;

void switchCase(int val, char *pattern);