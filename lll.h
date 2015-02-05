#ifndef _LINUXLINKLIST_H
#define _LINUXLINKLIST_H

struct dot {
	char *word;
	struct list_head list;
};

struct dot *new_node(char *new_str);
int del_node(struct dot *delete);

struct list_head *find_str(char *str,struct list_head *head);
int insert_str(char *str, char *insert_str, struct list_head *head);
int append_str(char *str, char *append_str, struct list_head *head);
int delete_str(char *str, struct list_head *head);

int delete_all(struct list_head *head);
int display_all(struct list_head *head);

#endif
