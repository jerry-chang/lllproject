#ifndef _LINKLIST_H
#define _LINKLIST_H

struct dot {
	char *word;
	struct list_head list;
};

int find_str(char *str, struct list_head *head, struct list_head *findptr);

int insert_str(char *str, char *insert_str, struct list_head *head);

int append_str(char *str, char *append_str, struct list_head *head);

int delete_str(char *str, struct list_head *head);

int display(struct list_head *head);

int delete_all(struct list_head *head);

#endif
