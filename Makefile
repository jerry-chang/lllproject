#This is a makefile for linux link list.

verify: verify_llll.c
	gcc verify_llll.c -o verify_llll
lll: lll.c
	gcc lll.c -o lll
test:
	cat input.txt | ./lll
clean:
	rm -f verify_llll
	rm -f lll
