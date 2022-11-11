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
void funcFlagE(char *argv[], int optind, int testFiles, char *pattern);
void flagsC(int testFiles, char *argv[], int optind, int *numberStrC);
void flagcL(int optind, char *argv[]);
void flagN(int numberStr, int n);
void flagTraining(int test, char *argv[], char *buffer, int numberStr,
                  int numberArg, int *numberStrC, char *pattern, int *numberO);
void flagPracticeTemplate(char *argv[], int optind, int numberStr,
                          int *numberStrC, int testFiles, char *buffer,
                          char *pattern, int *numberO);
void funcFlagF(char *fileName, char *pattern);
void funcFlagO(char *pattern, int *numberO, char *buffer);

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

int main(int argc, char *argv[]) {
  int val;
  int test = 0;
  int numberOptind = 0;
  size_t size = 0;
  char *pattern = malloc(size);

  if (argc != 1) {
    // нажождение в аргументах файлов и флагов
    while (test == 0 &&
           (val = getopt_long(argc, argv, "e:ivclnhsf:o", 0, NULL)) != EOF) {
      switchCase(val, pattern);
      // подсчитываем количество файлов
      for (int i = 0; i < argc; i++) {
        if (argv[optind + i] == NULL) {
          break;
        }
        if (argv[optind + i][0] != '-' && argv[optind][0] != '-') {
          numberOptind++;
        }
      }
    }
    // присвоение паттерна с флагом -е и без
    if (flagsGrep.flag_e == 0) {
      pattern = argv[optind];
    } else {
      pattern[strlen(pattern) - 1] = '\0';
    }
    // отработка grep без шалона <<  grep text file  >>
    if (flagsGrep.flag_e == 0) {
      grepNoFlags(optind, argc, pattern, argv);
    }
    // отработка grep с шаблоно   <<  -е 'шаблон' >>
    if (flagsGrep.flag_e > 0) {
      funcFlagE(argv, optind, numberOptind, pattern);
      // printf("%s\n", pattern);
    }
  }
  return 0;
}

// Свитч кейс
void switchCase(int val, char *pattern) {
  switch (val) {
    case 'e':
      p_flags->flag_e++;
      textArg(optarg, pattern);
      break;
    case 'i':
      p_flags->flag_i++;
      break;
    case 'v':
      p_flags->flag_v++;
      break;
    case 'c':
      p_flags->flag_c++;
      break;
    case 'l':
      p_flags->flag_l++;
      break;
    case 'n':
      p_flags->flag_n++;
      break;
    case 'h':
      p_flags->flag_h++;
      break;
    case 's':
      p_flags->flag_s++;
      break;
    case 'f':
      p_flags->flag_f++;
      funcFlagF(optarg, pattern);
      break;
    case 'o':
      p_flags->flag_o++;
      break;
    default:
      exit(0);
      break;
  }
}

//  Действие << без >> флага -E
void grepNoFlags(int optind, int argc, char *pattern, char *argv[]) {
  int numberStrC = 0;
  regex_t re;
  char buffer[BUFFER_SIZE];
  int test = 0;
  int numberO = 0;
  if (optind + 1 != argc - 1) test = 1;
  for (int numberArg = optind + 1; numberArg < argc; numberArg++) {
    if (flagsGrep.flag_i > 0) {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
    } else {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    }
    FILE *fp;
    int numberStr = 1;

    if ((fp = fopen(argv[numberArg], "r")) == NULL) {
      if (flagsGrep.flag_s == 0) {
        perror("No FILE");
      }
    } else {
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flagsGrep.flag_v == 0 &&
            flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg, &numberStrC,
                       pattern, &numberO);
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 &&
                   flagsGrep.flag_v > 0 && flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg, &numberStrC,
                       pattern, &numberO);
        }
        numberStr++;
      }
    }
    if (flagsGrep.flag_c > 0) {
      flagsC(test, argv, optind, &numberStrC);
    }
    if (flagsGrep.flag_l) {
      flagcL(numberArg, argv);
    }
    regfree(&re);
    fclose(fp);
  }
}

//  Действие << с >> флагом -E
void funcFlagE(char *argv[], int optind, int testFiles, char *pattern) {
  int numberStrC = 0;
  regex_t re;
  char buffer[BUFFER_SIZE];
  int numberO = 0;
  if (flagsGrep.flag_i) {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
  } else {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
  }
  while (argv[optind] != NULL) {
    int numberStr = 1;
    FILE *fp;
    if ((fp = fopen(argv[optind], "r")) == NULL) {
      if (flagsGrep.flag_s == 0) {
        perror("No FILE");
      }
    } else {
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flagsGrep.flag_v == 0 &&
            flagsGrep.flag_l == 0) {
          flagPracticeTemplate(argv, optind, numberStr, &numberStrC, testFiles,
                               buffer, pattern, &numberO);
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 &&
                   flagsGrep.flag_v > 0 && flagsGrep.flag_l == 0) {
          flagPracticeTemplate(argv, optind, numberStr, &numberStrC, testFiles,
                               buffer, pattern, &numberO);
        }
        numberStr++;
      }
      if (flagsGrep.flag_c > 0) {
        flagsC(testFiles, argv, optind, &numberStrC);
      }
      if (flagsGrep.flag_l) {
        flagcL(optind, argv);
      }
      fclose(fp);
    }
    optind++;
  }
  regfree(&re);
}

void funcFlagF(char *fileName, char *pattern) {
  FILE *fp;
  char buffer[BUFFER_SIZE];
  if ((fp = fopen(fileName, "r")) == NULL) {
    if (flagsGrep.flag_s == 0) {
      perror("No FILE");
    }
  }
  while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }
    textArg(buffer, pattern);
  }
  fclose(fp);
}

// отработка флага << C >>
void flagsC(int testFiles, char *argv[], int optind, int *numberStrC) {
  if (flagsGrep.flag_l == 0) {
    if (flagsGrep.flag_h == 0) {
      if (testFiles > 1) printf("%s:", argv[optind]);
    }
    printf("%d\n", *numberStrC);
    *numberStrC = 0;
  } else if (flagsGrep.flag_l) {
    if (flagsGrep.flag_h == 0) {
      if (testFiles > 1) printf("%s:", argv[optind]);
    }
    printf("%d\n", 1);
  }
}
// отработка флага << L >>
void flagcL(int optind, char *argv[]) { printf("%s\n", argv[optind]); }
// отработка флага << N >>
void flagN(int numberStr, int n) {
  if (n) printf("%d:", numberStr);
}

// соединяем паттерны
void textArg(char *optarg, char *pattern) {
  if (pattern == NULL) {
    strcat(pattern, optarg);
  } else {
    strcat(pattern, optarg);
    strcat(pattern, "|");
  }
}

// отработка флагов  <<  без >> шаблона
void flagTraining(int test, char *argv[], char *buffer, int numberStr,
                  int numberArg, int *numberStrC, char *pattern, int *numberO) {
  if (flagsGrep.flag_o) funcFlagO(pattern, numberO, buffer);
  if (flagsGrep.flag_c == 0) {
    if (test == 1 && flagsGrep.flag_h == 0) printf("%s:", argv[numberArg]);
    flagN(numberStr, flagsGrep.flag_n);
    // вывод строки с результатом нахождения
    if (flagsGrep.flag_o) {
      funcFlagO(pattern, numberO, buffer);
    } else {
      fputs(buffer, stdout);
      if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n')
        printf("\n");
    }
  } else {
    *numberStrC = *numberStrC + 1;
  }
}

// отработка флагов  <<  с шаблоном  >>
void flagPracticeTemplate(char *argv[], int optind, int numberStr,
                          int *numberStrC, int testFiles, char *buffer,
                          char *pattern, int *numberO) {
  // отображаем файлы, если их заданного больше одного
  if (flagsGrep.flag_c == 0) {
    if (testFiles > 1 && flagsGrep.flag_h == 0) printf("%s:", argv[optind]);
    flagN(numberStr, flagsGrep.flag_n);
  }
  // если находим флаг << C >>  то заносим данные о совпадениях
  if (flagsGrep.flag_c > 0) {
    *numberStrC = *numberStrC + 1;
  } else {
    // Иначе вывод совпадения в консоль
    if (flagsGrep.flag_o) {
      funcFlagO(pattern, numberO, buffer);
    } else {
      fputs(buffer, stdout);
      if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n')
        printf("\n");
    }
  }
}

void funcFlagO(char *pattern, int *numberO, char *buffer) {
  int i = 0;
  int j = 0;
  char stringArg[100][BUFFER_SIZE];
  for (*numberO = 0; pattern[*numberO] != '\0'; *numberO = *numberO + 1) {
    if (pattern[*numberO] == '|') {
      i++;
      j = 0;
    } else {
      stringArg[i][j] = pattern[*numberO];
      j++;
    }
  }
  *numberO = i;
  for (int i = 0; i <= *numberO; i++) {
    int j = 0;
    int k = 0;
    while (buffer[j] != '\0') {
      if (flagsGrep.flag_i &&
          (buffer[j] == stringArg[i][k] || buffer[j] == stringArg[i][k] - 32 ||
           buffer[j] == stringArg[i][k] + 32)) {
        printf("%c", buffer[j]);
        k++;
      } else if (buffer[j] == stringArg[i][k]) {
        printf("%c", buffer[j]);
        k++;
      }
      if (flagsGrep.flag_i && stringArg[i][k] == '\0' &&
          (buffer[j] == stringArg[i][k - 1] ||
           buffer[j] == stringArg[i][k - 1] - 32 ||
           buffer[j] == stringArg[i][k - 1] + 32)) {
        printf("\n");
        k = 0;
      } else if (stringArg[i][k] == '\0' && buffer[j] == stringArg[i][k - 1]) {
        printf("\n");
        k = 0;
      }
      j++;
    }
  }
}
