#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list_modify.h"

struct foo {
	char *word;
	struct list_head list_member;
};

void add_node(char *str, struct list_head *head)
{
    struct foo *fooPtr = (struct foo *)malloc(sizeof(struct foo));
    assert(fooPtr != NULL);
    
    fooPtr->word = str;
    INIT_LIST_HEAD(&fooPtr->list_member);
    list_add_tail(&fooPtr->list_member, head);
}

void display(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each(iter, head) {
        objPtr = list_entry(iter, struct foo, list_member);
        printf("%s ", objPtr->word);
    }
    printf("\n");
}

void delete_all(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;
    
    redo:
    list_for_each(iter, head) {
        objPtr = list_entry(iter, struct foo, list_member);
        list_del(&objPtr->list_member);
        free(objPtr);
	goto redo;
    }
}


main() 
{
	char string[1024];
	char *s;
	char *put[100];
	int s_count = 0;
	int word_count = 0;
    	LIST_HEAD(fooHead);
	while(gets(string) != NULL) {
		s = strtok(string," ");
		while(s != NULL) {
			put[s_count++] = s;
			s = strtok(NULL," ");
		}	
		while(word_count != s_count) {
			add_node(put[word_count],&fooHead);
			word_count++;
		}
		display(&fooHead);
		delete_all(&fooHead);
		s_count = 0;
		word_count = 0;
	}
}

