#This is a makefile for linux link list.

verify: verify_llll.c
	gcc verify_llll.c -o verify_llll
lll: lll.c
	gcc lll.c -o lll
test:
	cat append.txt | ./lll | ./verify_llll append_ans.txt
	cat empty.txt | ./lll | ./verify_llll empty_ans.txt
	cat input.txt | ./lll | ./verify_llll input_ans.txt
	cat insert.txt | ./lll | ./verify_llll insert_ans.txt
	cat missing_word.txt | ./lll | ./verify_llll missing_word_ans.txt
	cat over_del.txt | ./lll | ./verify_llll over_del_ans.txt
	cat revert.txt | ./lll | ./verify_llll revert_ans.txt
lll_test:
	cat input.txt | ./lll
clean:
	rm -f verify_llll
	rm -f lll
