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
void fileNumbers(char *argv[], int *amountFiles, int variant);
void textArg(char *pattern);
void walking(char *argv[], int variant, int *amountFiles,
             char *pattern);
void flagcL(char *nameFile);
void flagsC(int testFiles, char *nameFile, int numberStrC);
void flagTraining(int numberFiles, char *argv[], char *buffer, int numberStr,
                  int numberArg);
void flagN(int numberStr, int n);

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
  // выбераем вариант сборки, от него будет зависеть прохождение по файлам
  int variant = 0;
  // количество файлов
  int amountFiles = 0;
  // если нет шаблона или файла для паттерна, то выбераем стандартную сборку
  if (!flag.flag_e & !flag.flag_f) {
    pattern = argv[optind];
    variant = 1;
    // посчитываем количество файлов
    fileNumbers(argv, &amountFiles, variant);
  }
  // иначе выбераем сборку по готовому паттерну
  if (!variant) {
    // посчитываем количество файлов
    fileNumbers(argv, &amountFiles, variant);
  }

  walking(argv, variant, &amountFiles, pattern);

  // printf("%s", pattern);
}

// Функция отработки по файлам
void walking(char *argv[], int variant, int *amountFiles,
             char *pattern) {
  for (int i = 0; i < *amountFiles; i++) {
    FILE *fp;
    regex_t re;
    // Проверка для L
    int testL = 0;
    // Подсчёт количества совпадений для С
    int numberStrFlagC = 0;
    // Подсчёт количества строк
    int numberStr = 1;
    // Создаём буффер для записи строки
    char buffer[BUFFER_SIZE] = {0};

    if (flag.flag_i) {
      regcomp(&re, pattern, REG_EXTENDED | REG_ICASE);
    } else {
      regcomp(&re, pattern, REG_EXTENDED);
    }


    if ((fp = fopen(argv[optind + variant + i], "r")) != NULL)  {
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flag.flag_v == 0) {
          if (flag.flag_c && flag.flag_l) {
            numberStrFlagC = 1;
          } else if (flag.flag_c && !flag.flag_l) {
            numberStrFlagC++;
          }
          if (!flag.flag_l) {
              flagTraining(*amountFiles, argv, buffer, numberStr, optind + variant + i);
          } else {
            testL = 1;
          }
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 && flag.flag_v) {
          if (flag.flag_c && flag.flag_l) {
            numberStrFlagC = 1;
          } else if (flag.flag_c && !flag.flag_l) {
            numberStrFlagC++;
          }
          if (!flag.flag_l) {
              flagTraining(*amountFiles, argv, buffer, numberStr, optind + variant + i);
          } else {
            testL = 1;
          }
        }
        numberStr++;
      }
      if (flag.flag_c) {
        flagsC(*amountFiles, argv[optind + variant + i], numberStrFlagC);
      }
      if (flag.flag_l && testL) {
        flagcL(argv[optind + variant + i]);
      }
    }
    fclose(fp);
    regfree(&re);
  }
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

// Счётчик файлов
void fileNumbers(char *argv[], int *amountFiles, int variant) {
  if (variant) {
    for (int i = optind + 1; argv[i] != NULL; i++) {
      if (argv[i][0] != '-' ||
          (argv[i - 1][0] != '-' && argv[i - 1][1] != 'f')) {
        *amountFiles = *amountFiles + 1;
      }
    }
  } else if (!variant) {
    for (int i = optind; argv[i] != NULL; i++) {
      if (argv[i][0] != '-' ||
          (argv[i - 1][0] != '-' && argv[i - 1][1] != 'f')) {
        *amountFiles = *amountFiles + 1;
      }
    }
  }
}

void flagTraining(int numberFiles, char *argv[], char *buffer, int numberStr,
                  int numberArg) {
  if (flag.flag_c == 0) {
    if (numberFiles > 1 && flag.flag_h == 0) {
      printf("%s:", argv[numberArg]);
    }
    flagN(numberStr, flag.flag_n);

    if (flag.flag_o && flag.flag_v == 0) {
      // flagO(buffer, pattern);
      printf("flag O\n");
    } else {
      fputs(buffer, stdout);
    }

    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n') {
      printf("\n");
    }
  }
}

// отработка флага N
void flagN(int numberStr, int n) {
  if (n) {
    printf("%d:", numberStr);
  }
}

// отработка флага << C >>
void flagsC(int testFiles, char *nameFile, int numberStrC) {
  if (testFiles > 1 && flag.flag_h == 0) {
    printf("%s:%d\n", nameFile, numberStrC);
  } else {
    printf("%d\n", numberStrC);
  }
}

// отработка флага << L >>
void flagcL(char *nameFile) { printf("%s\n", nameFile); }

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