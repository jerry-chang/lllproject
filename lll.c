#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "lll.h"
int find_str(char *str, struct list_head *head, struct list_head *findptr)
{
	struct list_head *iter;
	struct dot *node;
	list_for_each(iter, head) {
		node = list_entry(iter, struct dot, list);
		if(strcmp(node->word,str) == 0) {
			findptr->next = iter;
			return 0; // find the string
		}
	}
	return 1; // string no found, but ok
}
int insert_str(char *str, char *insert_str, struct list_head *head)
{
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	struct list_head *ptr = malloc(sizeof(struct list_head));
	assert(dotptr != NULL);
	dotptr->word = strdup(insert_str);
	if(str != NULL) {
		if(find_str(str,head,ptr) == 0) {
			list_add_tail(&dotptr->list,ptr->next);
			free(ptr);
			return 0; //insert the string
		} else {
			//printf("INVALID COMMAND\n");
			free(ptr);
			return 1; // string no found, but ok
		}
	} else { // add string 
		INIT_LIST_HEAD(&dotptr->list); // initial the head pointer in link list
		list_add_tail(&dotptr->list,head);
		return 0; // success
	}
}    
int append_str(char *str, char *append_str, struct list_head *head)
{
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	struct list_head *ptr = malloc(sizeof(struct list_head));
	assert(dotptr != NULL);
	dotptr->word = strdup(append_str);
	if(find_str(str,head,ptr) == 0) {
		list_add(&dotptr->list,ptr->next);
		free(ptr);
		return 0; //append the string
	} else {
		//printf("INVALID COMMAND\n");
		free(ptr);
		return 1; // string no found, but ok
	}
}
int delete_str(char *str, struct list_head *head)
{
	struct list_head *ptr = malloc(sizeof(struct list_head));
	struct list_head *iter;
	struct dot *obj;
	if(find_str(str,head,ptr) == 0) {
		obj = list_entry(ptr->next, struct dot, list);
		list_del(&obj->list);
		free(ptr);
		free(obj->word);
		free(obj);
		return 0; //delete the string
	} else {
		//printf("INVALID COMMAND\n");
		free(ptr);
		return 1; //string no found, but ok
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
	return 0; //success
}
int delete_all(struct list_head *head)
{
	struct list_head *iter;
    	struct list_head *n;
    	struct dot *obj;
    	list_for_each_safe(iter,n,head) {
		obj = list_entry(iter, struct dot, list);
		if(obj == NULL) {
			return 0; //link list is empty
		}
		list_del(&obj->list);
		free(obj->word);
		free(obj);
    	}
}
int main(int argc, char *argv[]) 
{
	char *string = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	char *token_arr[1];
	int ini_flag = 1; //recognize the input string is initial string or command. 1 indicates initial string, 0 indicate command
	size_t len = 0;
	ssize_t read;
    	LIST_HEAD(dothead);
	while((read = getline(&string,&len,stdin)) != -1) {
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
			append_str(token_arr[0],token_arr[1],&dothead);
		} else if( (strcmp(token,"i") == 0) && ini_flag == 0) { // insert command
			token_arr[0] = strtok_r(NULL," ",&saveptr);
			token_arr[1] = strtok_r(NULL," ",&saveptr);
			insert_str(token_arr[0],token_arr[1],&dothead);
		} else if( (strcmp(token,"d") == 0) && ini_flag == 0) { // delete command
			token_arr[0] = strtok_r(NULL," ",&saveptr);
			delete_str(token_arr[0],&dothead);
		} else {  // initial input string
			delete_all(&dothead);
			while(token != NULL) {
				//add_node(token,&dothead);
				insert_str(NULL,token,&dothead);
				token = strtok_r(NULL," ",&saveptr);
			}
			ini_flag = 0;
		}
	}
	free(string);
	return 0; //success
}
