#include "s21_grep.h"

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
  regex_t re;
  char buffer[BUFFER_SIZE];
  int test = 0;

  if (optind + 1 != argc - 1) test = 1;

  int numberArg = 0;
  int numberFiles = 0;

  for (numberArg = optind + 1; numberArg < argc; numberArg++) {
    // Считаем количество файлов после паттерна << GREP >>
    if (argv[numberArg][0] != '-' && flagsGrep.flag_e == 0) numberFiles++;
  }

  for (int numberArg = optind + 1; numberArg < argc; numberArg++) {
    // Выбеоаем тип флагов под флаг I и без него
    flagsGrep.flag_i > 0
        ? regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE)
        : regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);

    FILE *fp;
    int numberStr = 1;
    int flagTestL = 1;
    int flagTestC = 0;

    if ((fp = fopen(argv[numberArg], "r")) == NULL) {
      if (flagsGrep.flag_s == 0) {
        perror("No File");
      }
    } else {
      // Проходимся по каждой строке в файле
      while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        int regTru = regexec(&re, buffer, 0, NULL, 0);

        // если находим потверждение то передаём данные в функции
        if (regTru == 0 && flagsGrep.flag_v == 0 && flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg);
        } else if (regTru != 0 && flagsGrep.flag_v > 0 &&
                   flagsGrep.flag_l == 0) {
          flagTraining(test, argv, buffer, numberStr, numberArg);
        }
        if (regTru == 0 && flagsGrep.flag_c) {
          flagsGrep.flag_l ? flagTestC = 1 : flagTestC++;
        }
        if (regTru == 0 && flagsGrep.flag_l && flagTestL) {
          flagTestL = 0;
        }
        numberStr++;
      }
      if (flagsGrep.flag_c) {
        flagsC(numberFiles, argv[numberArg], flagTestC);
      }
      if (flagsGrep.flag_l && flagTestL == 0) {
        flagcL(argv[numberArg]);
      }
    }
    regfree(&re);
    fclose(fp);
  }
}

//   отработка флага << F >>
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

void flagsC(int testFiles, char *nameFile, int numberStrC) {
  if (testFiles > 1 && flagsGrep.flag_h == 0) {
    printf("%s:%d\n", nameFile, numberStrC);
  } else {
    printf("%d\n", numberStrC);
  }
}

// отработка флага << L >>
void flagcL(char *nameFile) { printf("%s\n", nameFile); }

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

// отработка флагов  <<  без >>  шаблона
void flagTraining(int test, char *argv[], char *buffer, int numberStr,
                  int numberArg) {
  // if (flagsGrep.flag_o) funcFlagO(pattern, numberO, buffer);
  if (flagsGrep.flag_c == 0) {
    if (test == 1 && flagsGrep.flag_h == 0) printf("%s:", argv[numberArg]);
    flagN(numberStr, flagsGrep.flag_n);
    // вывод строки с результатом нахождения
    // if (flagsGrep.flag_o) {
    //   funcFlagO(pattern, numberO, buffer);
    // }
    fputs(buffer, stdout);
    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n') {
      printf("\n");
    }
  }
}

// void funcFlagO(char *pattern, int *numberO, char *buffer) {
//   int i = 0;
//   int j = 0;
//   char stringArg[100][BUFFER_SIZE];
//   for (*numberO = 0; pattern[*numberO] != '\0'; *numberO = *numberO + 1) {
//     if (pattern[*numberO] == '|') {
//       i++;
//       j = 0;
//     } else {
//       stringArg[i][j] = pattern[*numberO];
//       j++;
//     }
//   }
//   *numberO = i;
//   for (int i = 0; i <= *numberO; i++) {
//     int j = 0;
//     int k = 0;
//     while (buffer[j] != '\0') {
//       if (flagsGrep.flag_i &&
//           (buffer[j] == stringArg[i][k] || buffer[j] == stringArg[i][k] - 32
//           ||
//            buffer[j] == stringArg[i][k] + 32)) {
//         printf("%c", buffer[j]);
//         k++;
//       } else if (buffer[j] == stringArg[i][k]) {
//         printf("%c", buffer[j]);
//         k++;
//       }
//       if (flagsGrep.flag_i && stringArg[i][k] == '\0' &&
//           (buffer[j] == stringArg[i][k - 1] ||
//            buffer[j] == stringArg[i][k - 1] - 32 ||
//            buffer[j] == stringArg[i][k - 1] + 32)) {
//         printf("\n");
//         k = 0;
//       } else if (stringArg[i][k] == '\0' && buffer[j] == stringArg[i][k - 1])
//       {
//         printf("\n");
//         k = 0;
//       }
//       j++;
//     }
//   }
// }
