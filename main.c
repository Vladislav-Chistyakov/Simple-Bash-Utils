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

int main(int argc, char *argv[]) {
  int val;
  int test = 0;

  if (argc != 1) {
    // нажождение в аргументах файлов и флагов
    while (test == 0 && (val = getopt_long(argc, argv, "+ETtensbv",
                                           long_options, NULL)) != EOF) {
      switch (val) {
        case 'b':
          p_flags->flag_b++;
          p_flags->flag_b = (p_flags->flag_n = 0) + 1;
          break;
        case 'e':
        case 'E':
          p_flags->flag_e++;
          p_flags->flag_v++;
          break;
        case 'n':
          p_flags->flag_n++;
          p_flags->flag_n = p_flags->flag_b ? 0 : 1;
          break;
        case 's':
          p_flags->flag_s++;
          break;
        case 'v':
          p_flags->flag_v++;
          break;
        case 'T':
        case 't':
          p_flags->flag_t++;
          p_flags->flag_v++;
          break;
        default:
          exit(0);
          break;
      }
    }
  }
  // цикл для возможности реализации CAT для нескольких файлов
  while (optind < argc) {
    int argFile = optind;
    FILE *fp;

    // Открытие и проверка файла
    if ((fp = fopen(argv[argFile], "r")) == NULL) {
      perror("Error file");
      exit(0);
    }

    char simb;
    char oldSimb = '0';
    int string_new = 0;
    int numberSimbol = 1;
    int examS = 0;
    int examT = 0;
    int examV = 0;
    int number = 1;

    //Прохождение по каждому символу файла
    while ((simb = getc(fp)) != EOF) {
      // Смотрим, есть ли смвол переноса строки
      if (simb == '\n') string_new = string_new + 1;
      // отработка флага S
      if (flagsCat.flag_s) funcFlagS(&examS, &string_new, simb, numberSimbol);
      // отработка флага N
      if (flagsCat.flag_n && !flagsCat.flag_b)
        funcFlagN(numberSimbol, flagsCat.flag_s, &number, examS, oldSimb);
      // отработка флага B
      if (flagsCat.flag_b)
        funcFlagB(numberSimbol, flagsCat.flag_s, &number, examS, oldSimb, simb);
      // отработка флага E
      if (flagsCat.flag_e) funcFlagE(flagsCat.flag_s, simb, examS);
      // отработка флага t
      if (flagsCat.flag_t) funcFlagT(simb, &examT);
      // отработка флага V
      if (flagsCat.flag_v) funcFlagV(simb, &examV);
      // ВЫВОД В КОНСОЛЬ
      outputFuncCat(flagsCat.flag_s, &examS, &examV, &examT, simb);
      // взаимодействие с символами и строками
      oldSimb = simb;
      numberSimbol++;
    }
    fclose(fp);
    optind++;
  }
  return 0;
}
