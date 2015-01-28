#ifndef _LINKLIST_H
#define _LINKLIST_H

struct dot {
	char *word;
	struct list_head list;
};

int add_node(char *str, struct list_head *head);

int find_node(char *str, struct list_head *head, struct list_head *findptr);

int insert_node(char *str, char *insert, struct list_head *head);

int append_node(char *str, char *append, struct list_head *head);

int delete_node(char *str, struct list_head *head);

int display(struct list_head *head);

int delete_all(struct list_head *head);

#endif
