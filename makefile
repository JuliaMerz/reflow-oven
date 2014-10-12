all:
	gcc tc.c -g -o test -lbcm2835 -std=c99
