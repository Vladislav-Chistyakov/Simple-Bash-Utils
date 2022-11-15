#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 256

int main() {

  char *pattern = "ab";
  char *string = "acdcscsdab  Привеabтasdvxsddfvcxzsdwqfwaфисыфиифыифиbabababababbababab";
  regex_t re;
  char buf[BUFSIZE];
  regmatch_t pmatch[100];
  int status;
  char *s;
  /* Компиляция шаблона */

  if ((status = regcomp(&re, pattern, REG_EXTENDED | REG_ICASE)) != 0) {
    regerror(status, &re, buf, 120);
    exit(2);
  }


  // while ((status = regexec(&re, s, 1, pmatch, 0)) == 0) {
  //   int f = pmatch[0].rm_eo - pmatch[0].rm_so;
  //   for (int i = 0; i < f; i++) {
  //     printf("%c", s[pmatch[0].rm_so + i]);
  //   }
  //   printf("\n");
  //   s += pmatch[0].rm_eo;
  // }

  s = string;
  for (int i = 0;; i++) {
    if (regexec(&re, s, 1, pmatch, 0) != 0) break;
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           s + pmatch[0].rm_so);
    s += pmatch[0].rm_eo;
  }











regfree(re);




}

// void flagO(int numberFiles, char *argv[], char *buffer, int numberStr, int numberArg);




