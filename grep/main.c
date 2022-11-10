#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

// структура для записи найденных флагов

// FLAGI
// -e  ШАБЛОН
// –i : не будет различать верхний и нижний
//          регистр.
// –v : выбирает строки,
//     которые не совпадают.
// –c - Показывает только одно количество выбранных строк для каждого файла.
// –l : показывает только имена файлов с выделенными строками.
// –n : показывает номер строки с запрошенными
//           словами.
// –h : Выводит совпадающие строки, не предваряя их именами файлов.
// - s Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.

// - f file Получает регулярные выражения из файла.
// - o Печатает только совпадающие(непустые)
// части совпавшей строки.
void textArg(char *optarg, char *pattern);
void grepNoFlags(int optind, int argc, char *pattern, char *argv[], int flag_i);
void funcFlagE(int flag_i, char *argv[], int optind, int testFiles,
               char *pattern);

// void smallCase(char *argv[], char *arrayString[], int flag_i);
// void fileArg(char *optind);

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
        if (argv[optind+i] == NULL) {
          break;
        }
        if (argv[optind + i][0] != '-' && argv[optind][0] != '-') {
          numberOptind++;
        }
      }
    }
    // printf("\t %d\t", numberOptind);

    // присвоение паттерна с флагом -е и без

    ////////////////////////////////////////////
    if (flagsGrep.flag_e == 0) {
      pattern = argv[optind];
    } else {
      pattern[strlen(pattern) - 1] = '\0';
    }
    

    // отработка grep без шалона <<  grep text file  >>
    if (flagsGrep.flag_e == 0) {
      grepNoFlags(optind, argc, pattern, argv, flagsGrep.flag_i);
    }

    // отработка grep с шаблоно   <<  -е 'шаблон' >>
    if (flagsGrep.flag_e > 0) {
      funcFlagE(flagsGrep.flag_i, argv, optind, numberOptind, pattern);
      printf("%s\n", pattern);
    }
  }
  return 0;
}

void grepNoFlags(int optind, int argc, char *pattern, char *argv[],
                 int flag_i) {
  regex_t re;
  char buffer[BUFFER_SIZE];
  int test = 0;
  if (optind + 1 != argc - 1) test = 1;
  for (int numberArg = optind + 1; numberArg < argc; numberArg++) {
    if (flag_i > 0) {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
    } else {
      regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    }
    FILE *fp;
    if ((fp = fopen(argv[numberArg], "r")) == NULL) {
      perror("No FILE");
      exit(0);
    } else {
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (regexec(&re, buffer, 0, NULL, 0) == 0) {
          if (test == 1) printf("%s:", argv[numberArg]);
          fputs(buffer, stdout);
        }
      }
    }
    regfree(&re);
    fclose(fp);
  }
}

void textArg(char *optarg, char *pattern) {
  if (pattern == NULL) {
    strcat(pattern, optarg);
  } else {
    strcat(pattern, optarg);
    strcat(pattern, "|");
  }
  // regcomp(reg, optarg, REG_NOSUB);
}

void funcFlagE(int flag_i, char *argv[], int optind, int testFiles,
               char *pattern) {
  regex_t re;
  char buffer[BUFFER_SIZE];

  if (flag_i) {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE);
  } else {
    regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
  }
  while (argv[optind] != NULL) {
    FILE *fp;

    fp = fopen(argv[optind], "r");
    if (fp != NULL) {
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (regexec(&re, buffer, 0, NULL, 0) == 0) {
          if (testFiles > 1) printf("%s:", argv[optind]);

          fputs(buffer, stdout);
        }
      }
      fclose(fp);
    } else {
      printf("No such file\n");
    }
    optind++;
  }
  regfree(&re);
}