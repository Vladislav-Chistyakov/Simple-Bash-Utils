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
          if (argv[optind + i - 1][0] != '-' &&
              argv[optind + i - 1][1] != 'f') {
            numberOptind++;
          }
        }
      }
    }
    // присвоение паттерна с флагом -е и без
    if (flagsGrep.flag_e == 0) {
      if (flagsGrep.flag_f == 0) {
        pattern = argv[optind];
      }
    } else if (flagsGrep.flag_e) {
      pattern[strlen(pattern) - 1] = '\0';
    }

    // printf("pattern %s\n", pattern);

    // отработка grep без шалона <<  grep text file  >>
    if (flagsGrep.flag_e == 0) {
      grepNoFlags(optind, argc, pattern, argv);
    } else if (flagsGrep.flag_e) {
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
  int testingFiles = 0;
  int numberFiles = 0;
  int numberArg = 0;

  if (flagsGrep.flag_f && flagsGrep.flag_e == 0) {
    testingFiles = optind;
    numberArg = optind;
  } else if (flagsGrep.flag_f == 0 && flagsGrep.flag_e == 0) {
    testingFiles = optind + 1;
    numberArg = optind + 1;
  } else if (flagsGrep.flag_e) {
    testingFiles = optind;
    numberArg = optind;
  }

  // Считаем количество файлов после паттерна << GREP >>
  for (; testingFiles < argc; testingFiles++) {
    if (argv[testingFiles][0] != '-' && flagsGrep.flag_e == 0) numberFiles++;
  }


  for (; numberArg < argc; numberArg++) {
    // Выбеоаем тип флагов под флаг I и без него
    flagsGrep.flag_i > 0
        ? regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB | REG_ICASE)
        : regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    // printf("file = %s\n", argv[numberArg]);

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
        int regexecTrue = regexec(&re, buffer, 0, NULL, 0);
        // если находим потверждение то передаём данные в функции
        if (regexecTrue == 0 && flagsGrep.flag_v == 0 &&
            flagsGrep.flag_l == 0) {
          flagTraining(numberFiles, argv, buffer, numberStr, numberArg, pattern);
        } else if (regexecTrue != 0 && flagsGrep.flag_v > 0 &&
                   flagsGrep.flag_l == 0) {
          flagTraining(numberFiles, argv, buffer, numberStr, numberArg, pattern);
        }
        if (regexecTrue == 0 && flagsGrep.flag_c) {
          flagsGrep.flag_l ? flagTestC = 1 : flagTestC++;
        }
        if (regexecTrue == 0 && flagsGrep.flag_l && flagTestL) {
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
  // printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

//   отработка флага << F >>
void funcFlagF(char *fileName, char *pattern) {
  FILE *fp;
  char buffer[BUFFER_SIZE];
  if ((fp = fopen(fileName, "r")) == NULL) {
    if (flagsGrep.flag_s == 0) {
      perror("No FILE");
      exit(0);
    }
  }
  while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }
    textArg(buffer, pattern);
  }
  if (flagsGrep.flag_e) {
    pattern[strlen(pattern)] = '\0';
  } else if (!flagsGrep.flag_e) {
    pattern[strlen(pattern) - 1] = '\0';
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

// отработка флагов  <<  без >>  шаблона
void flagTraining(int numberFiles, char *argv[], char *buffer, int numberStr,
                  int numberArg, char *pattern) {
  if (flagsGrep.flag_c == 0) {
    if (numberFiles > 1 && flagsGrep.flag_h == 0)
      printf("%s:", argv[numberArg]);
    flagN(numberStr, flagsGrep.flag_n);

    if (flagsGrep.flag_o && flagsGrep.flag_v == 0) {
      flagO(buffer, pattern);
    } else {
      fputs(buffer, stdout);
    }


    if (buffer[strlen(buffer)] == '\0' && buffer[strlen(buffer) - 1] != '\n') {
      printf("\n");
    }
  }
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

void flagO(char *buffer, char *pattern) {
  regex_t re;
  // char buf[400];
  regmatch_t pmatch[100];
  int status = 1;
  char *s;
  s = buffer;

  if (flagsGrep.flag_i) {
    status = regcomp(&re, pattern, REG_EXTENDED | REG_ICASE);
  } else if (!flagsGrep.flag_i) {
    status = regcomp(&re, pattern, REG_EXTENDED);
  }
    
  // if (status != 0) {
  //   regerror(status, &re, buf, 120);
  // }
  if (status == 0 && flagsGrep.flag_v == 0) {
    for (int i = 0;; i++) {
      if (regexec(&re, s, 1, pmatch, 0) != 0) break;
      printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
             s + pmatch[0].rm_so);
      s += pmatch[0].rm_eo;
    }
  } 
  
  
  // else if (status != 0 && flagsGrep.flag_v > 0) {
  //   fputs(buffer, stdout);
  // }

  // if (status != 0) {
  //   regerror(status, &re, buf, 120);
  // }
  // for (int i = 0;; i++) {
  //   if (regexec(&re, s, 1, pmatch, 0) != 0) break;
  //   printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
  //           s + pmatch[0].rm_so);
  //   s += pmatch[0].rm_eo;
  // }
}
