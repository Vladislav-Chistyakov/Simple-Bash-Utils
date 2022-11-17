GCC = gcc
GCC1 = gcc -c -Wall -Werror -Wextra

all : s21_grep

s21_grep : s21_grep.o
	$(GCC) s21_grep.o -o s21_grep
	rm -rf *.o

s21_grep.o : s21_grep.c s21_grep.h
	$(GCC) s21_grep.c -c

clean :
	rm -rf *.o s21_grep

rebuild : make clean
	make all