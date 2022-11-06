void funcFlagS(int *testingFlags, int *stringNew, char simb, int numberSimbol) {
  if (*stringNew == 1 && numberSimbol == 1) {
    *stringNew = 2;
    *testingFlags = 1;
  }
  if (*stringNew < 3) {
    *testingFlags = 1;
    if (simb != '\n') *stringNew = 0;
  } else if (simb != '\n') {
    *stringNew = 0;
    *testingFlags = 1;
  }
}

void funcFlagN(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb) {
  if ((oldSimb == '\n' || numberSimbol == 1) && !flagS) {
    printf("%6.d\t", *number);
    *number = *number + 1;
  } else if (flagS && (oldSimb == '\n' || numberSimbol == 1)) {
    if (examS) {
      printf("%6.d\t", *number);
      *number = *number + 1;
    }
  }
}

void funcFlagB(int numberSimbol, int flagS, int *number, int examS,
               char oldSimb, char simb) {
  if (!flagS && (numberSimbol == 1 || oldSimb == '\n') && simb != '\n') {
    printf("%6.d\t", *number);
    *number = *number + 1;
  }
  if (flagS && (oldSimb == '\n' || numberSimbol == 1)) {
    if (simb != '\n' && examS) {
      printf("%6.d\t", *number);
      *number = *number + 1;
    }
  }
}

void funcFlagE(int flagS, char simb, int examS) {
  if (!flagS && simb == '\n') {
    printf("$");
  } else if (examS && simb == '\n') {
    printf("$");
  }
}

void funcFlagT(char simb, int *examT) {
  if (simb == 9) *examT = 1;
}

void funcFlagV(char simb, int *examV) {
  if (simb >= 0 && simb != 9 && simb != 10 && simb < 32) {
    printf("^%c", simb + 64);
    *examV = 1;
  } else if (simb == 127) {
    printf("^?");
    *examV = 1;
  }
}