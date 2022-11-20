#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

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

extern void output(int argc, char *argv[]);
extern void switchCase(int val, char *pattern);
extern void parser(char *pattern, char *argv[]);
extern void fileNumbers(char *argv[], int *amountFiles, int variant);
extern void textArg(char *pattern);
extern void walking(char *argv[], int variant, int *amountFiles, char *pattern);
extern void flagcL(char *nameFile);
extern void flagsC(int testFiles, char *nameFile, int numberStrC);
extern void flagN(int numberStr, int n);
extern void funcFlagF(char *fileName, char *pattern);
extern void flagO(char *buffer, char *pattern);
extern void flagTraining(int numberFiles, char *argv[], char *buffer,
                         int numberStr, int numberArg, char *pattern);
