#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct dot {
	char *word;
	struct list_head list;
};

int add_node(char *str, struct list_head *head)
{
    struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
    assert(dotptr != NULL);
    
    dotptr->word = strdup(str);
    
//    dotptr->word = malloc(strlen(str)+1);
//    strcpy(dotptr->word, str);
    INIT_LIST_HEAD(&dotptr->list);
    list_add_tail(&dotptr->list, head);
    return 0;
}

int find_node(char *find, struct list_head *head, struct list_head *findptr)
{
	struct list_head *iter;
	struct list_head *n;
	struct dot *node;
	list_for_each_safe(iter, n, head) {
		node = list_entry(iter, struct dot, list);
		if(strcmp(node->word,find) == 0) {
			findptr->next = iter;
			return 0;
		}
	}
	return 1;
}
int insert_node(char *find, char *insert, struct list_head *head)
{
    struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
    struct list_head *ptr = malloc(sizeof(struct list_head));
    assert(dotptr != NULL);
    dotptr->word = strdup(insert);
    if(find_node(find,head,ptr) == 0) {
    	list_add_tail(&dotptr->list,ptr->next);
   	free(ptr);
    	return 0;
    } else {
        //printf("INVALID COMMAND\n");
	free(ptr);
    	return -1;
    }
}    
int append_node(char *find, char *append, struct list_head *head)
{
    struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
    struct list_head *ptr = malloc(sizeof(struct list_head));
    assert(dotptr != NULL);
    dotptr->word = strdup(append);
    if(find_node(find,head,ptr) == 0) {
    	list_add(&dotptr->list,ptr->next);
    	free(ptr);
    	return 0;
    } else {
	//printf("INVALID COMMAND\n");
	free(ptr);
	return -1;
    }
}

int delete_node(char *delete, struct list_head *head)
{
    struct list_head *ptr = malloc(sizeof(struct list_head));
    struct list_head *iter;
    struct dot *obj;
    if(find_node(delete,head,ptr) == 0) {
    	obj = list_entry(ptr->next, struct dot, list);
    	list_del(&obj->list);
    	free(ptr);
    	free(obj->word);
    	free(obj);
    	return 0;
    } else {
	//printf("INVALID COMMAND\n");
	free(ptr);
	return -1;
    }
}

int display(struct list_head *head)
{
    struct list_head *iter;
    struct dot *obj;

    list_for_each(iter, head) {
        obj = list_entry(iter, struct dot, list);
        printf("%s ", obj->word);
    }
    printf("\n");
    return 0;
}

int delete_all(struct list_head *head)
{
    struct list_head *iter;
    struct dot *obj;
    redo: 
    list_for_each(iter, head) {
        obj = list_entry(iter, struct dot, list);
        list_del(&obj->list);
	free(obj->word);
        free(obj);
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
//	int size = 1;
	int endexe = 0;
//	int x;
    	LIST_HEAD(dothead);
	string = (char *)malloc(1024);
	while( fgets(string,1024,stdin) != NULL) {
		if(string[strlen(string) - 1] == '\n') {
			token_count = 0;
			word_count = 0;
			string[strlen(string) - 1] = '\0';
			token = strtok_r(string," ",&saveptr);
			if(token ==  NULL) { 
				display(&dothead);
				delete_all(&dothead);
				continue;
			}
			while(token != NULL) {
				put[token_count++] = token;
				token = strtok_r(NULL," ",&saveptr);
			}
			//display(&dothead);
			if(token_count >= 100) {
				printf("Token overflow. The following commands are not available.\n");
				return 1;
				break;
			}
			if(strcmp(put[0],"a") == 0) {
				append_node(put[1],put[2],&dothead);
			} else if(strcmp(put[0],"i") == 0) {
				insert_node(put[1],put[2],&dothead);
			} else if(strcmp(put[0],"d") == 0) {
				delete_node(put[1],&dothead);
			} else {
				delete_all(&dothead);
				while(word_count != token_count) {
					add_node(put[word_count],&dothead);
					word_count++;
				}
			}
		} else {
			string = (char *)realloc(string,sizeof(string) * 2);
		}
	}
//	display(&dothead);
	return 0;
}

