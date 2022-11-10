#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

// структура для записи найденных флагов

// FLAGI
// –n : показывает номер строки с запрошенными
//           словами.
// –h : Выводит совпадающие строки, не предваряя их именами файлов.
// - s Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.

// - f file Получает регулярные выражения из файла.
// - o Печатает только совпадающие(непустые)
// части совпавшей строки.
void textArg(char *optarg, char *pattern);
void grepNoFlags(int optind, int argc, char *pattern, char *argv[], int flag_i,
                 int flag_v, int flag_c, int flag_l);
void funcFlagE(int flag_i, char *argv[], int optind, int testFiles,
               char *pattern, int flag_v, int flag_c, int flag_l);
void flagsC(int testFiles, char *argv[], int optind, int *numberStrC,
            int flag_l);

void flagcL(int optind, char *argv[]);

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
      grepNoFlags(optind, argc, pattern, argv, flagsGrep.flag_i,
                  flagsGrep.flag_v, flagsGrep.flag_c, flagsGrep.flag_l);
    }

    // отработка grep с шаблоно   <<  -е 'шаблон' >>
    if (flagsGrep.flag_e > 0) {
      funcFlagE(flagsGrep.flag_i, argv, optind, numberOptind, pattern,
                flagsGrep.flag_v, flagsGrep.flag_c, flagsGrep.flag_l);
      // printf("%s\n", pattern);
    }
  }
  return 0;
}

//  Действие << без >> флага -E
void grepNoFlags(int optind, int argc, char *pattern, char *argv[], int flag_i,
                 int flag_v, int flag_c, int flag_l) {
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
    if (flag_i > 0) {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
      // отработка << без >> флага << i >>
    } else {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    }
    // открываем файл
    FILE *fp;
    if ((fp = fopen(argv[numberArg], "r")) == NULL) {
      perror("No FILE");
      exit(0);
    } else {
      // проходимся по строчно в поиска совпадения
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        // если флага << V >>  нет, то обрабатываем по стандарту
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flag_v == 0 && flag_l == 0) {
          // вывод названия файла
          // если файлов больше 1, то выводим текст
          if (flag_c == 0) {
            if (test == 1) printf("%s:", argv[numberArg]);
            // вывод строки с результатом нахождения
            fputs(buffer, stdout);
          } else {
            numberStrC++;
          }
          // Если флаг << V >>  присутствует, обрабатываем по нему
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 && flag_v > 0 && flag_l == 0) {
          // вывод названия файла
          // если файлов больше 1, то выводим текст
          if (flag_c == 0) {
            if (test == 1) printf("%s:", argv[numberArg]);
            // вывод строки с результатом нахождения
            fputs(buffer, stdout);
          } else {
            numberStrC++;
          }
        }
      }
    }
    // вывод данных, если есть флаг << C >>
    if (flag_c > 0) {
      flagsC(test, argv, optind, &numberStrC, flag_l);
    }
    // вывод флага << L >>
    if (flag_l) {
      flagcL(numberArg, argv);
    }
    regfree(&re);
    fclose(fp);
  }
}

//  Действие << с >> флагом -E
void funcFlagE(int flag_i, char *argv[], int optind, int testFiles,
               char *pattern, int flag_v, int flag_c, int flag_l) {
  //  задаём изначальное значение для вывода количества совпадений
  int numberStrC = 0;
  // задаём regex_t
  regex_t re;
  char buffer[BUFFER_SIZE];
  // проверка на флаг << -i >>
  if (flag_i) {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
  } else {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
  }
  // начинаем проходится по файлам
  while (argv[optind] != NULL) {
    FILE *fp;
    fp = fopen(argv[optind], "r");
    // если файл открывается, то заходим и ищем совпадения
    if (fp != NULL) {
      // проходимся по каждой строчке
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        //  если флага << V >> не обнаруживаем, то выполняем стандартные функции
        // для вывода данных
        if (regexec(&re, buffer, 0, NULL, 0) == 0 && flag_v == 0 &&
            flag_l == 0) {
          // отображаем файлы, если их заданного больше одного
          if (flag_c == 0) {
            if (testFiles > 1) printf("%s:", argv[optind]);
          }
          // если находим флаг << C >>  то заносим данные о совпадениях
          if (flag_c > 0) {
            numberStrC++;
          } else {
            // Иначе вывод совпадения в консоль
            fputs(buffer, stdout);
          }
          // если обнаруживается флаг << V >> то выводим несоответсвия
        } else if (regexec(&re, buffer, 0, NULL, 0) != 0 && flag_v > 0 &&
                   flag_l == 0) {
          // отображаем файлы, если их заданного больше одного
          if (flag_c == 0) {
            if (testFiles > 1) printf("%s:", argv[optind]);
          }
          // если находим флаг << C >>  то заносим данные о совпадениях
          if (flag_c > 0) {
            numberStrC++;
          } else {
            // Иначе вывод совпадения в консоль
            fputs(buffer, stdout);
          }
        }
      }
      // вывод данных, если есть флаг << C >>
      if (flag_c > 0) {
        flagsC(testFiles, argv, optind, &numberStrC, flag_l);
      }
      // вывод флага << L >>
      if (flag_l) {
        flagcL(optind, argv);
      }

      fclose(fp);
    } else {
      printf("No such file\n");
    }
    optind++;
  }
  regfree(&re);
}

void textArg(char *optarg, char *pattern) {
  if (pattern == NULL) {
    strcat(pattern, optarg);
  } else {
    strcat(pattern, optarg);
    strcat(pattern, "|");
  }
}

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

void flagcL(int optind, char *argv[]) { printf("%s\n", argv[optind]); }