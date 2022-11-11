#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

// структура для записи найденных флагов
// FLAGI
// - s Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.
// - f file Получает регулярные выражения из файла.
// - o Печатает только совпадающие(непустые)
// части совпавшей строки.
// void stringBuffer(char *string);
void textArg(char *optarg, char *pattern);
void grepNoFlags(int optind, int argc, char *pattern, char *argv[]);
void funcFlagE(char *argv[], int optind, int testFiles, char *pattern);
void flagsC(int testFiles, char *argv[], int optind, int *numberStrC,
            int flag_l);
void flagcL(int optind, char *argv[]);
void flagN(int numberStr, int n);
void flagTraining(int test, char *argv[], char *buffer, int numberStr,
                  int numberArg, int *numberStrC);
void flagPracticeTemplate(char *argv[], int optind, int numberStr,
                          int *numberStrC, int testFiles, char *buffer);

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
  int flagsNumber;
};
struct flags flagsGrep = {0};
struct flags *p_flags = &flagsGrep;
static struct option long_options[] = {
    {"number-nonblank", 0, 0, 'b'},
    {"number", 0, 0, 'n'},
    {"squeeze-blank)", 0, 0, 's'},
};

void switchCase(int val, char *pattern);

int main(int argc, char *argv[]) {
  int val;
  int test = 0;
  int numberOptind = 0;
  size_t size = 0;
  char *pattern = malloc(size);

  if (argc != 1) {
    // нажождение в аргументах файлов и флагов
    while (test == 0 && (val = getopt_long(argc, argv, "e:ivclnhsfo",
                                           long_options, NULL)) != EOF) {
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
      p_flags->flagsNumber++;
      // printf("\te\t%d\n", p_flags->flag_e);
      textArg(optarg, pattern);
      // fileArg(argv[optind]);-

      break;
    case 'i':
      p_flags->flag_i++;
      p_flags->flagsNumber++;
      // printf("\ti\t%d\n", p_flags->flag_i);

      break;
    case 'v':
      p_flags->flag_v++;
      p_flags->flagsNumber++;
      // printf("\tv\t%d\n", p_flags->flag_v);

      break;
    case 'c':
      p_flags->flag_c++;
      p_flags->flagsNumber++;
      // printf("\tc\t%d\n", p_flags->flag_c);

      break;
    case 'l':
      p_flags->flag_l++;
      p_flags->flagsNumber++;
      // printf("\tl\t%d\n", p_flags->flag_l);

      break;
    case 'n':
      p_flags->flag_n++;
      p_flags->flagsNumber++;
      // printf("\tn\t%d\n", p_flags->flag_n);

      break;
    case 'h':
      p_flags->flag_h++;
      p_flags->flagsNumber++;
      // printf("\th\t%d\n", p_flags->flag_h);

      break;
    case 's':
      p_flags->flag_s++;
      p_flags->flagsNumber++;
      // printf("\ts\t%d\n", p_flags->flag_s);

      break;
    case 'f':
      p_flags->flag_f++;
      p_flags->flagsNumber++;
      // printf("\t\ft%d\n", p_flags->flag_f);

      break;
    case 'o':
      p_flags->flag_o++;
      p_flags->flagsNumber++;
      // printf("\to\t%d\n", p_flags->flag_o);

      break;
    default:
      exit(0);
      break;
  }
}

//  Действие << без >> флага -E
void grepNoFlags(int optind, int argc, char *pattern, char *argv[]) {
  //  задаём изначальное значение для вывода количества совпадений
  int numberStrC = 0;
  // созда>м regex_t
  regex_t re;
  char buffer[BUFFER_SIZE];
  int test = 0;
  if (optind + 1 != argc - 1) test = 1;
  // проходимся по списку файлов
  for (int numberArg = optind + 1; numberArg < argc; numberArg++) {
    // отработка с флагом  << i >>
    if (flagsGrep.flag_i > 0) {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
      // отработка << без >> флага << i >>
    } else {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    }
    // открываем файл
    FILE *fp;
    int numberStr = 1;

    if ((fp = fopen(argv[numberArg], "r")) == NULL) {
      if (flagsGrep.flag_s == 0) {
        perror("No FILE");
      }
    } else {
      // проходимся по строчно в поиска совпадения
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        // если флага << V >>  нет, то обрабатываем по стандарту
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flagsGrep.flag_v == 0 &&
            flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg, &numberStrC);
          // Если флаг << V >>  присутствует, обрабатываем по нему
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 &&
                   flagsGrep.flag_v > 0 && flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg, &numberStrC);
        }
        numberStr++;
      }
    }
    // вывод данных, если есть флаг << C >>
    if (flagsGrep.flag_c > 0) {
      flagsC(test, argv, optind, &numberStrC, flagsGrep.flag_l);
    }
    // вывод флага << L >>
    if (flagsGrep.flag_l) {
      flagcL(numberArg, argv);
    }
    regfree(&re);
    fclose(fp);
  }
}

//  Действие << с >> флагом -E
void funcFlagE(char *argv[], int optind, int testFiles, char *pattern) {
  //  задаём изначальное значение для вывода количества совпадений
  int numberStrC = 0;
  // задаём regex_t
  regex_t re;
  char buffer[BUFFER_SIZE];
  // проверка на флаг << -i >>
  if (flagsGrep.flag_i) {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
  } else {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
  }
  // начинаем проходится по файлам
  while (argv[optind] != NULL) {
    int numberStr = 1;

    FILE *fp;
    if ((fp = fopen(argv[optind], "r")) == NULL) {
      if (flagsGrep.flag_s == 0) {
        perror("No FILE");
      }
    } else {
      // проходимся по каждой строчке
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        //  если флага << V >> не обнаруживаем, то выполняем стандартные функции
        // для вывода данных
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flagsGrep.flag_v == 0 &&
            flagsGrep.flag_l == 0) {
          // отображаем файлы, если их заданного больше одного
          flagPracticeTemplate(argv, optind, numberStr, &numberStrC, testFiles,
                               buffer);
          // если обнаруживается флаг << V >> то выводим несоответсвия
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 &&
                   flagsGrep.flag_v > 0 && flagsGrep.flag_l == 0) {
          flagPracticeTemplate(argv, optind, numberStr, &numberStrC, testFiles,
                               buffer);
        }
        numberStr++;
      }
      // вывод данных, если есть флаг << C >>
      if (flagsGrep.flag_c > 0) {
        flagsC(testFiles, argv, optind, &numberStrC, flagsGrep.flag_l);
      }
      // вывод флага << L >>
      if (flagsGrep.flag_l) {
        flagcL(optind, argv);
      }

      fclose(fp);
    }
    optind++;
  }
  regfree(&re);
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

// отработка флага << C >>
void flagsC(int testFiles, char *argv[], int optind, int *numberStrC,
            int flag_l) {
  if (flag_l == 0) {
    if (testFiles > 1) printf("%s:", argv[optind]);
    printf("%d\n", *numberStrC);
    *numberStrC = 0;
  } else if (flag_l) {
    if (testFiles > 1) printf("%s:", argv[optind]);
    printf("%d\n", 1);
  }
}
// отработка флага << L >>
void flagcL(int optind, char *argv[]) { printf("%s\n", argv[optind]); }
// отработка флага << N >>
void flagN(int numberStr, int n) {
  if (n) printf("%d:", numberStr);
}

// отработка флагов  <<  без >> шаблона
void flagTraining(int test, char *argv[], char *buffer, int numberStr,
                  int numberArg, int *numberStrC) {
  if (flagsGrep.flag_c == 0) {
    if (test == 1 && flagsGrep.flag_h == 0) printf("%s:", argv[numberArg]);
    flagN(numberStr, flagsGrep.flag_n);
    // вывод строки с результатом нахождения
    fputs(buffer, stdout);

  } else {
    *numberStrC = *numberStrC + 1;
  }
}

// отработка флагов  <<  с шаблоном  >>
void flagPracticeTemplate(char *argv[], int optind, int numberStr,
                          int *numberStrC, int testFiles, char *buffer) {
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
    fputs(buffer, stdout);
  }
}