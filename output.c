void outputFuncCat(int flagS, int *examS, int *examV, int *examT, char simb) {
  if (flagS && *examS) {
    if (*examT) {
      printf("^I");
      *examS = 0;
      *examT = 0;
    } else if (*examV) {
      *examV = 0;
    } else {
      printf("%c", simb);
      *examS = 0;
    }
  } else if (!flagS) {
    if (*examT) {
      printf("^I");
      *examT = 0;
    } else if (*examV) {
      *examV = 0;
    } else {
      printf("%c", simb);
    }
  }
}