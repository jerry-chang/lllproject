#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct foo {
	char *word;
	struct list_head list;
};

int add_node(char *str, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    assert(fooptr != NULL);
    
    fooptr->word = strdup(str);
    
//    fooptr->word = malloc(strlen(str)+1);
//    strcpy(fooptr->word, str);
    INIT_LIST_HEAD(&fooptr->list);
    list_add_tail(&fooptr->list, head);
    return 0;
}

int find_node(char *find, struct list_head *head, struct list_head *findptr)
{
	struct list_head *iter;
	struct list_head *n;
	struct foo *node;
	list_for_each_safe(iter, n, head) {
		node = list_entry(iter, struct foo, list);
		if(strcmp(node->word,find) == 0) {
			findptr->next = iter;
			return 0;
		}
	}
	return 1;
}
int insert_node(char *find, char *insert, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *ptr = malloc(sizeof(struct list_head));
    assert(fooptr != NULL);
    fooptr->word = strdup(insert);
    if(find_node(find,head,ptr) == 0) {
    	list_add_tail(&fooptr->list,ptr->next);
   	free(ptr);
    	return 0;
    } else {
        printf("INVALID COMMAND\n");
    	return 1;
    }
}    
int append_node(char *find, char *append, struct list_head *head)
{
    struct foo *fooptr = (struct foo *)malloc(sizeof(struct foo));
    struct list_head *ptr = malloc(sizeof(struct list_head));
    assert(fooptr != NULL);
    fooptr->word = strdup(append);
    if(find_node(find,head,ptr) == 0) {
    	list_add(&fooptr->list,ptr->next);
    	free(ptr);
    	return 0;
    } else {
	printf("INVALID COMMAND\n");
	return 1;
    }
}

int delete_node(char *delete, struct list_head *head)
{
    struct list_head *ptr = malloc(sizeof(struct list_head));
    struct list_head *iter;
    struct foo *objptr;
    if(find_node(delete,head,ptr) == 0) {
    	objptr = list_entry(ptr->next, struct foo, list);
    	list_del(&objptr->list);
    	free(ptr);
    	free(objptr->word);
    	free(objptr);
    	return 0;
    } else {
	printf("INVALID COMMAND\n");
	return 1;
    }
}

int display(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objptr;

    list_for_each(iter, head) {
        objptr = list_entry(iter, struct foo, list);
        printf("%s ", objptr->word);
    }
    printf("\n");
    return 0;
}

int delete_all(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objptr;
    redo: 
    list_for_each(iter, head) {
        objptr = list_entry(iter, struct foo, list);
        list_del(&objptr->list);
	free(objptr->word);
        free(objptr);
	goto redo;
    }
    return 0;
}


int main() 
{
	//char string[1024];
	char *string = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	char *put[100];
	int token_count = 0;
	int word_count = 0;
	int size = 1;
	int endexe = 0;
//	int x;
    	LIST_HEAD(fooHead);
	string = (char *)malloc(size * 1024);
	while( fgets(string,size * 1024,stdin) != NULL) {
		if(string[strlen(string) - 1] == '\n') {
			string[strlen(string) - 1] = '\0';
			token = strtok_r(string," ",&saveptr);
			if(token ==  NULL) {
				display(&fooHead);
				delete_all(&fooHead);
			} else {
				while(token != NULL) {
					put[token_count++] = token;
					token = strtok_r(NULL," ",&saveptr);
				}
				//display(&fooHead);
				if(token_count > 100) {
					printf("String token is overflow. The following commands are not available.\n");
				}
				else {
					if(strcmp(put[0],"a") == 0) {
						if(append_node(put[1],put[2],&fooHead)) {
							return 1;
							break;
						}
						//display(&fooHead);
					}
					else if(strcmp(put[0],"i") == 0) {
						if(insert_node(put[1],put[2],&fooHead)) {
							return 1;
							break;
						}
                        			//display(&fooHead);
					}
					else if(strcmp(put[0],"d") == 0) {
						if(delete_node(put[1],&fooHead)) {
							return 1;
							break;
						}
						//display(&fooHead);
					}
					else {
						delete_all(&fooHead);
						while(word_count != token_count) {
							add_node(put[word_count],&fooHead);
							word_count++;
						}
						//display(&fooHead);
					}
					token_count = 0;
					word_count = 0;
				}
				token_count = 0;
			}
		}
		else {
 			size++;
			string = (char *)malloc(size *1024);
		}
	}
	display(&fooHead);
	return 0;
}

