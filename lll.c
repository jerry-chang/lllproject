#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "lll.h"

struct dot *new_node(char *new_str)
{
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	assert(dotptr != NULL);
	dotptr->word = strdup(new_str);
	return dotptr;
}

int del_node(struct dot *delete)
{
	list_del(&delete->list);
	free(delete->word);
	free(delete);
	return 0; //success
}

struct list_head *find_str(char *str, struct list_head *head)
{
	struct list_head *iter;
	list_for_each(iter, head) {
		struct dot *node = list_entry(iter, struct dot, list);
		if(strcmp(node->word,str) == 0) {
			return iter; // find the string
		}
	}
	return NULL; // string not found, but ok
}

int insert_str(char *str, char *insert_str, struct list_head *head)
{
	if(str == NULL) { // add string
		INIT_LIST_HEAD(&(new_node(insert_str))->list);
		list_add_tail(&(new_node(insert_str))->list,head);	
		return 0; //success
	}
	
	if(find_str(str,head) == NULL) {
		return -1; // string not found, but ok
	}

	list_add_tail(&(new_node(insert_str))->list,find_str(str,head));
	return 0; // insert the string
}    
int append_str(char *str, char *append_str, struct list_head *head)
{
	if(find_str(str,head) == NULL) {
		return -1; //string not found, but ok
	}

	list_add(&(new_node(append_str))->list,find_str(str,head));
	return 0; // append the string
}
int delete_str(char *str, struct list_head *head)
{
	if(find_str(str,head) == NULL) {
		return -1; // string not found, nut ok
	}

	struct dot *obj = list_entry(find_str(str,head), struct dot, list);
	del_node(obj);
	return 0; //delete the string
}
int display_all(struct list_head *head)
{
	struct list_head *iter;
	list_for_each(iter, head) {
    		struct dot *obj = list_entry(iter, struct dot, list);
    		printf("%s ", obj->word);
	}
	printf("\n");
	return 0; //success
}
int delete_all(struct list_head *head)
{
	struct list_head *iter;
    	struct list_head *n;
    	list_for_each_safe(iter,n,head) {
		struct dot *obj = list_entry(iter, struct dot, list);
		del_node(obj);
    	}
	return 0; //success
}
enum
{
	ini_mode,com_mode
};
int main(int argc, char *argv[]) 
{
	char *string = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	int flag = ini_mode; //set the flag to initial mode
	size_t len = 0;
	ssize_t read;
    	LIST_HEAD(dothead);
	while((read = getline(&string,&len,stdin)) != -1) {
		char *newline = strchr(string,'\n'); // find new line
		strncpy(newline,"\0",1);
		token = strtok_r(string," ",&saveptr);
		if(token ==  NULL) { //null line. indicate the end of one pattern
			display_all(&dothead);
			delete_all(&dothead);
			flag = ini_mode;
			continue;
		}
		if(flag == com_mode) { // command mode
			if(strcmp(token,"a") == 0) { // appand command
				char *app_find = strtok_r(NULL," ",&saveptr);
				char *app_str = strtok_r(NULL," ",&saveptr);
				append_str(app_find,app_str,&dothead);
			} else if(strcmp(token,"i") == 0) { // insert command
				char *ins_find = strtok_r(NULL," ",&saveptr);
				char *ins_str= strtok_r(NULL," ",&saveptr);
				insert_str(ins_find,ins_str,&dothead);
			} else if(strcmp(token,"d") == 0) { // delete command
				char *del_str = strtok_r(NULL," ",&saveptr);
				delete_str(del_str,&dothead);
			} else {
				return -1; // invalid command
			}
		} else { //initial mode
			delete_all(&dothead);
			while(token != NULL) {
				insert_str(NULL,token,&dothead);
				token = strtok_r(NULL," ",&saveptr);
			}
			flag = com_mode;
		}
	}
	free(string);
	return 0; //success
}
