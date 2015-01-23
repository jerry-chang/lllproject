#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct foo {
	char *word;
	struct list_head list;
};

void add_node(char *str, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    assert(fooptr != NULL);
    
    fooptr->word = strdup(str);
    
//    fooptr->word = malloc(strlen(str)+1);
//    strcpy(fooptr->word, str);
    INIT_LIST_HEAD(&fooptr->list);
    list_add_tail(&fooptr->list, head);
}
/*
struct foo *find_node(char *find, struct list_head *head)
{
	struct list_head *iter;
	struct foo *node;
	list_for_each(iter, head) {
		node = list_entry(iter, struct foo, list);
		if(strcmp(node->word,find) == 0) {
			printf("find it !\n");
			return iter;
		}
	}
	return NULL;
}
*/
int insert_node(char *find, char *insert, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *iter;
    struct foo *objPtr;
    assert(fooptr != NULL);
    fooptr->word = strdup(insert);
    list_for_each(iter, head) {
	objPtr = list_entry(iter, struct foo, list);
	if(strcmp(objPtr->word,find) == 0) {
		list_add(&fooptr->list,iter);
		return 0;
	}
    }
    return 1;
    
}

int append_node(char *find, char *append, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *iter;
    struct foo *objPtr;

    assert(fooptr != NULL);
    fooptr->word = strdup(append);
    list_for_each(iter, head) {
	objPtr = list_entry(iter, struct foo, list);
	if(strcmp(objPtr->word,find) == 0) {
		list_add_tail(&fooptr->list,iter);
		return 0;
	}
    }
    return 1;
}

int delete_node(char *delete, struct list_head *head)
{
    struct list_head *iter;
    struct list_head *n;
    struct foo *objPtr;

    list_for_each_safe(iter, n, head) {
	objPtr = list_entry(iter, struct foo, list);
	if(strcmp(objPtr->word,delete) == 0) {
		list_del(&objPtr->list);
		free(objPtr->word);
		free(objPtr);
		return 0;
	}
    }
    return 1;
}

void display(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each(iter, head) {
        objPtr = list_entry(iter, struct foo, list);
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
        objPtr = list_entry(iter, struct foo, list);
        list_del(&objPtr->list);
	free(objPtr->word);
        free(objPtr);
	goto redo;
    }
}


void main() 
{
	//char string[1024];
	char string[1024];
	char *s;
	char *put[100];
	int s_count = 0;
	int word_count = 0;
//	int x;
    	LIST_HEAD(fooHead);
	
	while(fgets(string,1024,stdin) != NULL) {
		if(string[strlen(string) - 1] == '\n') {
			string[strlen(string) - 1] = '\0';

			s = strtok(string," ");
			while(s != NULL) {
				put[s_count++] = s;
				s = strtok(NULL," ");
			}
			if(s_count > 20) {
				printf("String token is overflow. The following commands are not available.\n");
			}
			else {
				if(strcmp(put[0],"a") == 0) {
					//find_node(put[1],&fooHead);
					append_node(put[1],put[2],&fooHead);
					display(&fooHead);
				}
				else if(strcmp(put[0],"i") == 0) {
					insert_node(put[1],put[2],&fooHead);
                        		display(&fooHead);
				}
				else if(strcmp(put[0],"d") == 0) {
					delete_node(put[1],&fooHead);
					display(&fooHead);
				}
				else {
					delete_all(&fooHead);
					while(word_count != s_count) {
						add_node(put[word_count],&fooHead);
						word_count++;
					}
					display(&fooHead);
				}
				s_count = 0;
				word_count = 0;
			}
			s_count = 0;
		}
		else 
			printf("Your input string overflow. The following commands are not available.\n ");
	}
	//}                    
}

