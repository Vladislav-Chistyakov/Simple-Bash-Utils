#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    int catOutput;
  };
  struct flags flagsCat = {0};
  struct flags *p_flags = &flagsCat;
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank)", 0, 0, 's'},
  };

  if (argc != 1) {
    while (test == 0 && (val = getopt_long(argc, argv, "ETtensbv", long_options,
                                           NULL)) != EOF) {
      switch (val) {
        case 'b':
          p_flags->flag_b++;
          p_flags->flag_b = (p_flags->flag_n = 0) + 1;
          p_flags->catOutput++;
          break;
        case 'e':
        case 'E':
          p_flags->flag_e++;
          p_flags->catOutput++;
          p_flags->flag_v++;
          break;
        case 'n':
          p_flags->flag_n++;
          p_flags->flag_n = p_flags->flag_b ? 0 : 1;
          p_flags->catOutput++;
          break;
        case 's':
          p_flags->flag_s++;
          p_flags->catOutput++;
          break;
        case 'v':
          p_flags->flag_v++;
          p_flags->catOutput++;
          break;
        case 'T':
        case 't':
          p_flags->flag_t++;
          p_flags->catOutput++;
          p_flags->flag_v++;
          break;
        default:
          exit(0);
          break;
      }
    }
  }
  // на каком арументе в массиве записан файл
  int argFile = optind;
  FILE *fp;

  int string_new = 0;
  // int string_old = 0;


  // Проверка на сущетсвование файла
  if ((fp = fopen(argv[argFile], "r")) == NULL) {
    perror("Error file");
    exit(0);
  }


  char simb;
  char oldSimb = '0';
  // int examination = 0;
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
      if (string_new == 1 && numberSimbol == 1) {
        string_new = 2;
        examS = 1;
      }
      if (string_new < 3) {
        examS = 1;
        if (simb != '\n') string_new = 0;
      } else if (simb != '\n') {
        string_new = 0;
        examS = 1;
      }
    }

    // отработка флага N
    if (flagsCat.flag_n && !flagsCat.flag_b) {
      if ((oldSimb == '\n' || numberSimbol == 1) && !flagsCat.flag_s) {
        printf("      %6.d   ", number);
        number++;
      } else if (flagsCat.flag_s && (oldSimb == '\n' || numberSimbol == 1)) {
        if (examS) {
          printf("      %6.d   ", number);
          number++;
        } 
      }
    }

    // отработка флага B
    if (flagsCat.flag_b) {
      if (!flagsCat.flag_s && oldSimb == '\n' && simb != '\n') {
        printf("      %6.d   ", number);
        number++;
      }
        if (flagsCat.flag_s && (oldSimb == '\n' || numberSimbol == 1)) {
          if (simb != '\n' && examS) {
            printf("      %6.d   ", number);
            number++;
          }
        }
    }

    // отработка флага E
    if (flagsCat.flag_e) {
      if (!flagsCat.flag_s && simb == '\n') {
        printf("$");
      } else if (examS && simb == '\n') {
        printf("$");
      }
    }
    
    // отработка флага t

    if (flagsCat.flag_t) {
      if (simb == 9) examT = 1;
    }

    // отработка флага V



    if (flagsCat.flag_v) {
      if (simb >= 0 && simb != 9 && simb != 10 && simb < 32) {
        printf("^%c", simb + 64);
        examV = 1;
      } else if (simb == 127) {
        printf("^?");
        examV = 1;
      }
    }

      // ВЫВОД В КОНСОЛЬ
      if (flagsCat.flag_s && examS) {
        if (examT) {
          printf("^I");
          examS = 0;
          examT = 0;
        } else if (examV) {
          examV = 0;
        } else {
          printf("%c", simb);
          examS = 0;
        }
      } else if (!flagsCat.flag_s) {
        if (examT) {
          printf("^");
          printf("I");
          examT = 0;
        } else if (examV){
          examV = 0;
        } else {
          printf("%c", simb);
        }
      }
    oldSimb = simb;
    numberSimbol++;
  }


  fclose(fp);
  return 0;


}



















// количество файлов

// while (argFile++ < argc && argc != 1) {
//   if ((fp = fopen(argv[argFile - 1], "r")) == NULL) {
//     perror("Error file");
//     exit(0);
//   }
//   for (char symbol = '0'; (symbol = getc(fp)) != EOF;) {
//     int strLine = 0;
//     if (p_flags->flag_s && line_Counter_C == 0 && symbol == '\n') {
//       line_Counter_C += 1;
//     } else if (line_Counter_C != 0 && symbol == '\n') {
//       line_Counter_C += 1;
//       strLine = 1;
//     } else if (line_Counter_C > 1 && symbol != '\n') {
//       line_Counter_C = 0;
//       p_flags->flag_e ? printf("$\n") : printf("\n");
//       if (p_flags->flag_n != 0) printf("%6d\t", p_flags->flag_n++);
//     } else {
//       line_Counter_C = 0;
//     }
//     if (p_flags->flag_n != 0 || p_flags->flag_b != 0) {
//       if (new_Line == 1 && !(new_Line = 0)) {
//         printf("%6d\t", p_flags->flag_n++);
//       }
//       if (p_flags->flag_n == 1) printf("%6d\t", p_flags->flag_n++);
//       if (p_flags->flag_b == 1) printf("%6d\t", p_flags->flag_b++);
//       if (symbol == '\n' && p_flags->flag_n != 0 && new_Line == 0) {
//         new_Line = 1;
//       }
//       if (symbol == '\n' && p_flags->flag_b != 0) line_Counter_B += 1;
//       if (symbol != '\n' && line_Counter_B != 0 && line_Counter_C == 0) {
//         if (!(line_Counter_B = 0)) {
//           printf("%6d\t", p_flags->flag_b++);
//         }
//       }
//     }
//     if (symbol == '\n' && p_flags->flag_e && strLine == 0) printf("$");
//     if (p_flags->flag_v) {
//       if (symbol < 32 && symbol != 9 && symbol != 10) {
//         if (symbol += 64) printf("^");
//       }
//       if (symbol == 127) {
//         if ((symbol = '?')) printf("^");
//       }
//     }
//     if (p_flags->flag_t && symbol == '\t') {
//       if ((symbol = 'I')) printf("^");
//     }
//     if (strLine == 0) printf("%c", symbol);
//   }
//   argFile++;
