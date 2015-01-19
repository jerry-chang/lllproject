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

void insert_node(char *find, char *insert, struct list_head *head)
{
    struct foo *fooPtr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *iter;
    struct foo *objPtr;

    assert(fooPtr != NULL);
    fooPtr->word = insert;
    list_for_each(iter, head) {
	objPtr = list_entry(iter, struct foo, list_member);
	if(strcmp(objPtr->word,find) == 0) {
		list_add(&fooPtr->list_member,iter);
	}
    }
}

void append_node(char *find, char *append, struct list_head *head)
{
    struct foo *fooPtr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *iter;
    struct foo *objPtr;

    assert(fooPtr != NULL);
    fooPtr->word = append;
    list_for_each(iter, head) {
	objPtr = list_entry(iter, struct foo, list_member);
	if(strcmp(objPtr->word,find) == 0) {
		list_add_tail(&fooPtr->list_member,iter);
	}
    }
}

int delete_node(char *delete, struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each(iter, head) {
	objPtr = list_entry(iter, struct foo, list_member);
	if(strcmp(objPtr->word,delete) == 0) {
		list_del(&objPtr->list_member);
		free(objPtr);
		return 1;
	}
    }
    return 0;
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
	/*
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
	*/
	add_node("hello",&fooHead);
	add_node("world",&fooHead);
	display(&fooHead);
	append_node("world","my",&fooHead);
	insert_node("world","dogs",&fooHead);
	delete_node("world",&fooHead);
	append_node("dogs","and",&fooHead);
	append_node("and","cats",&fooHead);
	display(&fooHead);
}

