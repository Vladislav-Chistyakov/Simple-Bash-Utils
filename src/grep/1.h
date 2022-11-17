#ifndef SRC_S21_CAT_H_
#define SRC_S21_CAT_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define size_l 1000
#define size_patt 9000
#define const_char 4
#define line_size 1000

typedef struct {
  int flag_e; // Шаблон
  int flag_i; // Игнорирует различия регистра.
  int flag_v; // Инвертирует смысл поиска соответствий.
  int flag_c; // Выводит только количество совпадающих строк.
  int flag_l; // Выводит только совпадающие файлы.
  int flag_n; // Предваряет каждую строку вывода номером строки из файла ввода.
  int flag_h; // Выводит совпадающие строки, не предваряя их именами файлов.
  int flag_s; // Подавляет сообщения об ошибках о несуществующих или нечитаемых
              // файлах.
  int flag_f; // Получает регулярные выражения из файла.
  int flag_o; // Печатает только совпадающие (непустые) части совпавшей строки.
} options;

struct line_const { // tom
  int reg_c;
  int re_ez;
  int c_line;
  int complete;
  int complete_c;
};

char *t_line = NULL;
regex_t regex;
size_t line = line_size;
size_t n_mat = const_char;
regmatch_t p_mat[const_char];
char patt[size_patt] = {0};

const char *short_options = "e:ivclnhsf:o?";
static struct option long_options[] = {
    {NULL, 0, NULL, 0},
};

void Read_file(char **argv, int argc, int optind, options *flags,
               struct line_const); //чтение файла
void Flags(char f, char **argv, options *flags,
           struct line_const); // выбор флага
void pattern(char **argv, options *flags, struct line_const);
void kolizz(FILE *fileop, int argc, char *p, options *flags,
            struct line_const); //функция отработки флагов
void Flag_V(int argc, char *p, options *flags, struct line_const);
void Flag_O(int argc, char *p, options *flags, struct line_const);
void No_Flag_H(int argc, char *p, options *flags);
void Reg_memory(options *flags, struct line_const);

#endif // SRC_GREP_H_
