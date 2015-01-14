#This is a makefile for linux link list.

all:verify_llll.c
	gcc verify_llll.c -o verify_llll
clean:
	rm verify_llll
