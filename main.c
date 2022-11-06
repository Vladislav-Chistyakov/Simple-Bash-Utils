#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void funcFlagS(int *testingFlags, int *stringNew, char simb, int numberSimbol);
void funcFlagN(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb);
void funcFlagB(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb, char simb);

void funcFlagE(int flagS, char simb, int examS);

void funcFlagT(char simb, int *examT);

void funcFlagV(char simb, int *examV);

void outputFuncCat(int flagS, int *examS, int *examV, int *examT, char simb);

int main(int argc, char *argv[]) {
  int val;
  int test = 0;

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

  if (argc != 1) {
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
  // на каком арументе в массиве записан файл
  while (optind < argc) {
    int argFile = optind;
    FILE *fp;

    // Проверка на сущетсвование файла
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

    while ((simb = getc(fp)) != EOF) {
      // Смотрим, есть ли смвол переноса строки
      if (simb == '\n') string_new = string_new + 1;

      // отработка флага S
      if (flagsCat.flag_s) {
        funcFlagS(&examS, &string_new, simb, numberSimbol);
      }
      // отработка флага N
      if (flagsCat.flag_n && !flagsCat.flag_b) {
        funcFlagN(numberSimbol, flagsCat.flag_s, &number, examS, oldSimb);
      }
      // отработка флага B
      if (flagsCat.flag_b) {
        funcFlagB(numberSimbol, flagsCat.flag_s, &number, examS, oldSimb, simb);
      }
      // отработка флага E
      if (flagsCat.flag_e) {
        funcFlagE(flagsCat.flag_s, simb, examS);
      }
      // отработка флага t
      if (flagsCat.flag_t) {
        funcFlagT(simb, &examT);
      }
      // отработка флага V
      if (flagsCat.flag_v) {
        funcFlagV(simb, &examV);
      }
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

void funcFlagS(int *testingFlags, int *stringNew, char simb, int numberSimbol) {
  if (*stringNew == 1 && numberSimbol == 1) {
    *stringNew = 2;
    *testingFlags = 1;
  }
  if (*stringNew < 3) {
    *testingFlags = 1;
    if (simb != '\n') *stringNew = 0;
  } else if (simb != '\n') {
    *stringNew = 0;
    *testingFlags = 1;
  }
}

void funcFlagN(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb) {
  if ((oldSimb == '\n' || numberSimbol == 1) && !flagS) {
    printf("%6.d\t", *number);
    *number = *number + 1;
  } else if (flagS && (oldSimb == '\n' || numberSimbol == 1)) {
    if (examS) {
      printf("%6.d\t", *number);
      *number = *number + 1;
    }
  }
}

void funcFlagB(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb, char simb) {
  if (!flagS && (numberSimbol == 1 || oldSimb == '\n') && simb != '\n') {
    printf("%6.d\t", *number);
    *number = *number + 1;
  }
  if (flagS && (oldSimb == '\n' || numberSimbol == 1)) {
    if (simb != '\n' && examS) {
      printf("%6.d\t", *number);
      *number = *number + 1;
    }
  }
}

void funcFlagE(int flagS, char simb, int examS) {
  if (!flagS && simb == '\n') {
    printf("$");
  } else if (examS && simb == '\n') {
    printf("$");
  }
}

void funcFlagT(char simb, int *examT) {
  if (simb == 9) *examT = 1;
}

void funcFlagV(char simb, int *examV) {
  if (simb >= 0 && simb != 9 && simb != 10 && simb < 32) {
    printf("^%c", simb + 64);
    *examV = 1;
  } else if (simb == 127) {
    printf("^?");
    *examV = 1;
  }
}

void outputFuncCat(int flagS, int *examS, int *examV, int *examT, char simb) {
  if (flagS && *examS) {
    if (*examT) {
      printf("^I");
      *examS = 0;
      *examT = 0;
    } else if (*examV) {
      *examV = 0;
    } else {
      printf("%c", simb);
      *examS = 0;
    }
  } else if (!flagS) {
    if (*examT) {
      printf("^I");
      *examT = 0;
    } else if (*examV) {
      *examV = 0;
    } else {
      printf("%c", simb);
    }
  }
}