#ifndef _LINKLIST_H
#define _LINKLIST_H

struct dot {
	char *word;
	struct list_head list;
};

struct dot *new_node(char *);

int del_node(struct dot *);

struct list_head *find_str(char *,struct list_head *);

int insert_str(char *, char *, struct list_head *);

int append_str(char *, char *, struct list_head *);

int delete_str(char *, struct list_head *);

int delete_all(struct list_head *);

int display_all(struct list_head *);
#endif
