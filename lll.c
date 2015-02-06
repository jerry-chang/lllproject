#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "lll.h"

struct dot *new_node(char *new_str)
{
	if(new_str == NULL) {
		return NULL; // no input
	}
	struct dot *dotptr = (struct dot *)malloc(sizeof(struct dot));
	assert(dotptr != NULL);
	dotptr->word = strdup(new_str);
	assert(dotptr->word != NULL);
	return dotptr;
}
int del_node(struct dot *delete)
{
	if(delete == NULL) {
		return -2; // no input
	}
	free(delete->word);
	free(delete);
	return 0; //success
}
struct list_head *find_str(char *str, struct list_head *head)
{
	if(str == NULL) {
		return NULL; // no input
	}
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
	if(insert_str == NULL) {
		return -2; // no insert_str
	}
	struct list_head *position = head;
	if(str != NULL) {
		// str is not NULL
		position = find_str(str,head);
	}

	if(position == NULL) {
		return -1; // no valid position to put string
	}

	struct dot *newnode = new_node(insert_str);
	if(newnode == NULL) {
		return -2; // no input
	}
	list_add_tail(&newnode->list,position);
	return 0; //success
}    
int append_str(char *str, char *append_str, struct list_head *head)
{
	if(append_str == NULL) {
		return -2; // no append_str;
	}
	struct list_head *position = head;
	if(str != NULL) {
		// str is not NULL
		position = find_str(str,head);
	}
	
	if(position == NULL) {
		return -1; //no valid position to put string
	}

	struct dot *newnode = new_node(append_str);
	if(newnode == NULL) {
		return -2; // malloc fail
	}
	list_add(&newnode->list,position);
	return 0;
}
int delete_str(char *str, struct list_head *head)
{
	struct list_head *del_fs_list = find_str(str,head);
	if(del_fs_list == NULL) {
		return -1; // string not found, nut ok
	}

	struct dot *obj = list_entry(del_fs_list, struct dot, list);
	list_del(&obj->list);
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
		list_del(&obj->list);
		del_node(obj);
    	}
	return 0; //success
}
enum
{
	init_mode, //initial the input string
	cmd_mode, //command line
};
int main(int argc, char *argv[]) 
{
	char *string = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	int flag = init_mode; //set the flag to initial mode
	size_t len = 0;
	ssize_t read;
    	LIST_HEAD(dothead);
	while((read = getline(&string,&len,stdin)) != -1) {
		char *newline = strchr(string,'\n'); // find new line
		if(newline != NULL){
			*newline = '\0';
		}	
		//strncpy(newline,"\0",1);
		token = strtok_r(string," ",&saveptr);
		if(token ==  NULL) {
			//null line. indicate the end of one pattern
			display_all(&dothead);
			delete_all(&dothead);
			flag = init_mode;
			continue;
		}
		if(flag == cmd_mode) { 
			// command mode
			if(strcmp(token,"a") == 0) { 
				// appand command
				char *app_find = strtok_r(NULL," ",&saveptr);
				char *app_str = strtok_r(NULL," ",&saveptr);
				append_str(app_find,app_str,&dothead);
			} else if(strcmp(token,"i") == 0) { 
				// insert command
				char *ins_find = strtok_r(NULL," ",&saveptr);
				char *ins_str= strtok_r(NULL," ",&saveptr);
				insert_str(ins_find,ins_str,&dothead);
			} else if(strcmp(token,"d") == 0) { 
				// delete command
				char *del_str = strtok_r(NULL," ",&saveptr);
				delete_str(del_str,&dothead);
			} else {
				return -1; // invalid command
			}
		} else { 
			//initial mode
			while(token != NULL) {
				insert_str(NULL,token,&dothead);
				token = strtok_r(NULL," ",&saveptr);
			}
			flag = cmd_mode;
		}
	}
	free(string);
	return 0; //success
}
