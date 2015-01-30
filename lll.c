#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "lll.h"
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

int find_node(char *str, struct list_head *head, struct list_head *findptr)
{
	struct list_head *iter;
	struct list_head *n;
	struct dot *node;
	list_for_each_safe(iter, n, head) {
		node = list_entry(iter, struct dot, list);
		if(strcmp(node->word,str) == 0) {
			findptr->next = iter;
			return 0;
		}
	}
	return 1;
}
int insert_node(char *str, char *insert, struct list_head *head)
{
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	struct list_head *ptr = malloc(sizeof(struct list_head));
	assert(dotptr != NULL);
	dotptr->word = strdup(insert);
	if(find_node(str,head,ptr) == 0) {
		list_add_tail(&dotptr->list,ptr->next);
		free(ptr);
		return 0;
	} else {
		//printf("INVALID COMMAND\n");
		free(ptr);
		return -1;
	}
}    
int append_node(char *str, char *append, struct list_head *head)
{
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	struct list_head *ptr = malloc(sizeof(struct list_head));
	assert(dotptr != NULL);
	dotptr->word = strdup(append);
	if(find_node(str,head,ptr) == 0) {
		list_add(&dotptr->list,ptr->next);
		free(ptr);
		return 0;
	} else {
		//printf("INVALID COMMAND\n");
		free(ptr);
		return -1;
	}
}
int delete_node(char *str, struct list_head *head)
{
	struct list_head *ptr = malloc(sizeof(struct list_head));
	struct list_head *iter;
	struct dot *obj;
	if(find_node(str,head,ptr) == 0) {
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
    	struct list_head *n;
    	struct dot *obj;
    	list_for_each_safe(iter,n,head) {
		obj = list_entry(iter, struct dot, list);
		if(obj == NULL) {
			return 0; //finish delete
		}
		list_del(&obj->list);
		free(obj->word);
		free(obj);
    	}
	return 0;	
}
int main(int argc, char *argv[]) 
{
	char *string = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	char *token_arr[1];
	int ini_flag = 1;
    	LIST_HEAD(dothead);
	string = (char *)malloc(1024); // memory allocate
	while( fgets(string,1024,stdin) != NULL) { // gets string
		if(string[strlen(string) - 1] == '\n') { // find new line
			string[strlen(string) - 1] = '\0';
			token = strtok_r(string," ",&saveptr);
			if(token ==  NULL) { //null line. indicate the end of one pattern
				display(&dothead);
				delete_all(&dothead);
				ini_flag = 1;
				continue;
			}
			if( (strcmp(token,"a") == 0) && ini_flag == 0) { // appand command
				token_arr[0] = strtok_r(NULL," ",&saveptr);
				token_arr[1] = strtok_r(NULL," ",&saveptr);
				append_node(token_arr[0],token_arr[1],&dothead);
			} else if( (strcmp(token,"i") == 0) && ini_flag == 0) { // insert command
				token_arr[0] = strtok_r(NULL," ",&saveptr);
				token_arr[1] = strtok_r(NULL," ",&saveptr);
				insert_node(token_arr[0],token_arr[1],&dothead);
			} else if( (strcmp(token,"d") == 0) && ini_flag == 0) { // delete command
				token_arr[0] = strtok_r(NULL," ",&saveptr);
				delete_node(token_arr[0],&dothead);
			} else {  // initial input string
				delete_all(&dothead);
				while(token != NULL) {
					add_node(token,&dothead);
					token = strtok_r(NULL," ",&saveptr);
				}
				ini_flag = 0;
			}
		} else {
			string = (char *)realloc(string,sizeof(string) * 2);
		}
	}
	return 0;
}

