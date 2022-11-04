#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 100

int main(int argc, char *argv[]) {
  int val;
  int test = 0;
  int flag_b = 0;
  int flag_s = 0;
  int flag_n = 0;
  int flag_e = 0;
  int flag_t = 0;

  int flagNumberOutput = 0;
  int flagBufferOne = 0;

  char bufferOne[BUFFER] = {0};
  char bufferTwo[BUFFER] = {0};
  char emptyArray[2] = {""};

  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank)", 0, 0, 's'},
  };

  if (argc != 1) {
    while (test == 0 && (val = getopt_long(argc, argv, "ETtensb", long_options,
                                           NULL)) != EOF) {
      switch (val) {
        case 'b':
          flag_b++;
          break;
        case 'e':
        case 'E':
          flag_e++;
          break;
        case 'n':
          flag_n++;
          break;
        case 's':
          flag_s++;
          break;
        case 'T':
        case 't':
          flag_t++;
          break;
        default:
          exit(0);
          break;
      }
    }

    printf("%d", argc);
    for (int i = 0; i < argc; i++) {
      printf("%s\n", argv[i]);
    }

    // на каком арументе в массиве записан файл
    int argFile = optind;
    FILE *fp;

    if ((fp = fopen(argv[argFile], "r")) == NULL) {
      perror("Error file");
      exit(0);
    } else {
    }

    // flag S
    if (flag_s) {
      int index = 0;
      int flagTest = 0;
      char simb;

      while ((simb = getc(fp)) != EOF) {
        if (simb != '\0') {
          if (simb != '\n') {
            flagTest = 0;
            bufferOne[index] = simb;
            index++;
          } else if (simb == '\n' && flagTest < 2) {
            bufferOne[index] = simb;
            index++;
            flagTest = flagTest + 1;
          } else {
            flagTest = flagTest + 1;
          }
        }
      }
      flagBufferOne = 1;
    }

    // flag E
    if (flag_e && flagBufferOne) {
      int indexTwo = 0;

      for (int i = 0; bufferOne[i] != '\0'; i++) {
        bufferTwo[indexTwo] = bufferOne[i];
        if (bufferTwo[indexTwo] == '\n') {
          bufferTwo[indexTwo] = '$';
          indexTwo++;
          bufferTwo[indexTwo] = '\n';
        }
        indexTwo++;
      }

      strcpy(bufferOne, bufferTwo);
      strcpy(bufferTwo, emptyArray);

    } else if (flag_e && flagBufferOne == 0) {
      int indexOne = 0;
      int indexTwo = 0;
      char simb;

      while ((simb = getc(fp)) != EOF) {
        bufferOne[indexOne] = simb;
        indexOne++;
      }

      for (int i = 0; bufferOne[i] != '\0'; i++) {
        bufferTwo[indexTwo] = bufferOne[i];
        if (bufferTwo[indexTwo] == '\n') {
          bufferTwo[indexTwo] = '$';
          indexTwo++;
          bufferTwo[indexTwo] = '\n';
        }
        indexTwo++;
      }
      flagBufferOne = 1;
      strcpy(bufferOne, bufferTwo);
      strcpy(bufferTwo, emptyArray);
    }

    // flag T

    if (flag_t && flagBufferOne) {
      int indexTwo = 0;

      for (int i = 0; bufferOne[i] != '\0'; i++) {
        bufferTwo[indexTwo] = bufferOne[i];
        if (bufferTwo[indexTwo] == '\t') {
          bufferTwo[indexTwo] = '^';
          indexTwo++;
          bufferTwo[indexTwo] = 'I';
        }
        indexTwo++;
      }

      strcpy(bufferOne, bufferTwo);
      strcpy(bufferTwo, emptyArray);

    } else if (flag_t && flagBufferOne == 0) {

      int indexOne = 0;
      int indexTwo = 0;
      char simb;

      while ((simb = getc(fp)) != EOF) {
        bufferOne[indexOne] = simb;
        indexOne++;
        }

        for (int i = 0; bufferOne[i] != '\0'; i++) {
        bufferTwo[indexTwo] = bufferOne[i];
        if (bufferTwo[indexTwo] == '\t') {
            bufferTwo[indexTwo] = '^';
            indexTwo++;
            bufferTwo[indexTwo] = 'I';
        }
        indexTwo++;
        }
        flagBufferOne = 1;
        strcpy(bufferOne, bufferTwo);
        strcpy(bufferTwo, emptyArray);
    }

    // flag B
    if (flag_b) {
      flagNumberOutput = 1;
      int sc = 1;
      if (flagBufferOne) {
        for (int i = 0; bufferOne[i] != '\0'; i++) {
          char simbol;
          if (bufferOne[i] != '\0') { simbol = bufferOne[i]; }
          if ((bufferOne[i - 1] == '\n' || i == 0) && simbol != '\n') {
            if (simbol != '$') {
              printf("     %d  %c", sc, simbol);
              sc++;
            } else {
              printf("%c", simbol);
            }
          } else if (bufferOne[i+1] != '\0') {
            printf("%c", simbol);
          }
        }
      } else {

        char simbol;
        int sc = 1;
        char simbTest = 't';

        while ((simbol = getc(fp)) != EOF) {
          if (simbTest == 't' && simbol != '\n') {
            printf("     %d  %c", sc, simbol);
            sc++;
            // printf("%c", simbol);
            simbTest = 'p';
          } else if (simbol == '\n') {
            simbTest = '\n';
            printf("%c", simbol);
          } else if (simbTest == '\n') {
            printf("     %d  %c", sc, simbol);
            sc++;
            simbTest = 'p';
          } else {
            printf("%c", simbol);
          }
        }
      }
    }
    //flag N
    else if (flag_n) {     
      flagNumberOutput = 1;
      int sc = 1;
      if (flagBufferOne) {
        for (int i = 0; bufferOne[i] != '\0'; i++) {
          char simbol = bufferOne[i];
          if ((bufferOne[i - 1] == '\n' || i == 0)) {
              printf("     %d  %c", sc, simbol);
              sc++;
          } else {
            printf("%c", simbol);
          }
        }
      } else {
        int sc = 1;
        char simbol;
        char simbTest;
        while ((simbol = getc(fp)) != EOF) {
          if ((simbTest == '\n' || sc == 1)) {
            printf("     %d  %c", sc, simbol);
            sc++;
          } else {
            printf("%c", simbol);
          }
          simbTest = simbol;
        }
      }


    }
    if (flagNumberOutput == 0) {
      printf("%s", bufferOne);
      // printf("%d", flagBufferOne);
    }

    fclose(fp);
  }
  return 0;
}
