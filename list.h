Linux Cross Reference
Free Electrons
Embedded Linux Experts

 • source navigation  • diff markup  • identifier search  • freetext search  • 

Version:  2.0.40 2.2.26 2.4.37 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 3.10 3.11 3.12 3.13 3.14 3.15 3.16 3.17 3.18
Linux/include/linux/list.h

  1 #ifndef _LINUX_LIST_H
  2 #define _LINUX_LIST_H
  3 
  4 #include <linux/types.h>
  5 #include <linux/stddef.h>
  6 #include <linux/poison.h>
  7 #include <linux/const.h>
  8 #include <linux/kernel.h>
  9 
 10 /*
 11  * Simple doubly linked list implementation.
 12  *
 13  * Some of the internal functions ("__xxx") are useful when
 14  * manipulating whole lists rather than single entries, as
 15  * sometimes we already know the next/prev entries and we can
 16  * generate better code by using them directly rather than
 17  * using the generic single-entry routines.
 18  */
 19 
 20 #define LIST_HEAD_INIT(name) { &(name), &(name) }
 21 
 22 #define LIST_HEAD(name) \
 23         struct list_head name = LIST_HEAD_INIT(name)
 24 
 25 static inline void INIT_LIST_HEAD(struct list_head *list)
 26 {
 27         list->next = list;
 28         list->prev = list;
 29 }
 30 
 31 /*
 32  * Insert a new entry between two known consecutive entries.
 33  *
 34  * This is only for internal list manipulation where we know
 35  * the prev/next entries already!
 36  */
 37 #ifndef CONFIG_DEBUG_LIST
 38 static inline void __list_add(struct list_head *new,
 39                               struct list_head *prev,
 40                               struct list_head *next)
 41 {
 42         next->prev = new;
 43         new->next = next;
 44         new->prev = prev;
 45         prev->next = new;
 46 }
 47 #else
 48 extern void __list_add(struct list_head *new,
 49                               struct list_head *prev,
 50                               struct list_head *next);
 51 #endif
 52 
 53 /**
 54  * list_add - add a new entry
 55  * @new: new entry to be added
 56  * @head: list head to add it after
 57  *
 58  * Insert a new entry after the specified head.
 59  * This is good for implementing stacks.
 60  */
 61 static inline void list_add(struct list_head *new, struct list_head *head)
 62 {
 63         __list_add(new, head, head->next);
 64 }
 65 
 66 
 67 /**
 68  * list_add_tail - add a new entry
 69  * @new: new entry to be added
 70  * @head: list head to add it before
 71  *
 72  * Insert a new entry before the specified head.
 73  * This is useful for implementing queues.
 74  */
 75 static inline void list_add_tail(struct list_head *new, struct list_head *head)
 76 {
 77         __list_add(new, head->prev, head);
 78 }
 79 
 80 /*
 81  * Delete a list entry by making the prev/next entries
 82  * point to each other.
 83  *
 84  * This is only for internal list manipulation where we know
 85  * the prev/next entries already!
 86  */
 87 static inline void __list_del(struct list_head * prev, struct list_head * next)
 88 {
 89         next->prev = prev;
 90         prev->next = next;
 91 }
 92 
 93 /**
 94  * list_del - deletes entry from list.
 95  * @entry: the element to delete from the list.
 96  * Note: list_empty() on entry does not return true after this, the entry is
 97  * in an undefined state.
 98  */
 99 #ifndef CONFIG_DEBUG_LIST
100 static inline void __list_del_entry(struct list_head *entry)
101 {
102         __list_del(entry->prev, entry->next);
103 }
104 
105 static inline void list_del(struct list_head *entry)
106 {
107         __list_del(entry->prev, entry->next);
108         entry->next = LIST_POISON1;
109         entry->prev = LIST_POISON2;
110 }
111 #else
112 extern void __list_del_entry(struct list_head *entry);
113 extern void list_del(struct list_head *entry);
114 #endif
115 
116 /**
117  * list_replace - replace old entry by new one
118  * @old : the element to be replaced
119  * @new : the new element to insert
120  *
121  * If @old was empty, it will be overwritten.
122  */
123 static inline void list_replace(struct list_head *old,
124                                 struct list_head *new)
125 {
126         new->next = old->next;
127         new->next->prev = new;
128         new->prev = old->prev;
129         new->prev->next = new;
130 }
131 
132 static inline void list_replace_init(struct list_head *old,
133                                         struct list_head *new)
134 {
135         list_replace(old, new);
136         INIT_LIST_HEAD(old);
137 }
138 
139 /**
140  * list_del_init - deletes entry from list and reinitialize it.
141  * @entry: the element to delete from the list.
142  */
143 static inline void list_del_init(struct list_head *entry)
144 {
145         __list_del_entry(entry);
146         INIT_LIST_HEAD(entry);
147 }
148 
149 /**
150  * list_move - delete from one list and add as another's head
151  * @list: the entry to move
152  * @head: the head that will precede our entry
153  */
154 static inline void list_move(struct list_head *list, struct list_head *head)
155 {
156         __list_del_entry(list);
157         list_add(list, head);
158 }
159 
160 /**
161  * list_move_tail - delete from one list and add as another's tail
162  * @list: the entry to move
163  * @head: the head that will follow our entry
164  */
165 static inline void list_move_tail(struct list_head *list,
166                                   struct list_head *head)
167 {
168         __list_del_entry(list);
169         list_add_tail(list, head);
170 }
171 
172 /**
173  * list_is_last - tests whether @list is the last entry in list @head
174  * @list: the entry to test
175  * @head: the head of the list
176  */
177 static inline int list_is_last(const struct list_head *list,
178                                 const struct list_head *head)
179 {
180         return list->next == head;
181 }
182 
183 /**
184  * list_empty - tests whether a list is empty
185  * @head: the list to test.
186  */
187 static inline int list_empty(const struct list_head *head)
188 {
189         return head->next == head;
190 }
191 
192 /**
193  * list_empty_careful - tests whether a list is empty and not being modified
194  * @head: the list to test
195  *
196  * Description:
197  * tests whether a list is empty _and_ checks that no other CPU might be
198  * in the process of modifying either member (next or prev)
199  *
200  * NOTE: using list_empty_careful() without synchronization
201  * can only be safe if the only activity that can happen
202  * to the list entry is list_del_init(). Eg. it cannot be used
203  * if another CPU could re-list_add() it.
204  */
205 static inline int list_empty_careful(const struct list_head *head)
206 {
207         struct list_head *next = head->next;
208         return (next == head) && (next == head->prev);
209 }
210 
211 /**
212  * list_rotate_left - rotate the list to the left
213  * @head: the head of the list
214  */
215 static inline void list_rotate_left(struct list_head *head)
216 {
217         struct list_head *first;
218 
219         if (!list_empty(head)) {
220                 first = head->next;
221                 list_move_tail(first, head);
222         }
223 }
224 
225 /**
226  * list_is_singular - tests whether a list has just one entry.
227  * @head: the list to test.
228  */
229 static inline int list_is_singular(const struct list_head *head)
230 {
231         return !list_empty(head) && (head->next == head->prev);
232 }
233 
234 static inline void __list_cut_position(struct list_head *list,
235                 struct list_head *head, struct list_head *entry)
236 {
237         struct list_head *new_first = entry->next;
238         list->next = head->next;
239         list->next->prev = list;
240         list->prev = entry;
241         entry->next = list;
242         head->next = new_first;
243         new_first->prev = head;
244 }
245 
246 /**
247  * list_cut_position - cut a list into two
248  * @list: a new list to add all removed entries
249  * @head: a list with entries
250  * @entry: an entry within head, could be the head itself
251  *      and if so we won't cut the list
252  *
253  * This helper moves the initial part of @head, up to and
254  * including @entry, from @head to @list. You should
255  * pass on @entry an element you know is on @head. @list
256  * should be an empty list or a list you do not care about
257  * losing its data.
258  *
259  */
260 static inline void list_cut_position(struct list_head *list,
261                 struct list_head *head, struct list_head *entry)
262 {
263         if (list_empty(head))
264                 return;
265         if (list_is_singular(head) &&
266                 (head->next != entry && head != entry))
267                 return;
268         if (entry == head)
269                 INIT_LIST_HEAD(list);
270         else
271                 __list_cut_position(list, head, entry);
272 }
273 
274 static inline void __list_splice(const struct list_head *list,
275                                  struct list_head *prev,
276                                  struct list_head *next)
277 {
278         struct list_head *first = list->next;
279         struct list_head *last = list->prev;
280 
281         first->prev = prev;
282         prev->next = first;
283 
284         last->next = next;
285         next->prev = last;
286 }
287 
288 /**
289  * list_splice - join two lists, this is designed for stacks
290  * @list: the new list to add.
291  * @head: the place to add it in the first list.
292  */
293 static inline void list_splice(const struct list_head *list,
294                                 struct list_head *head)
295 {
296         if (!list_empty(list))
297                 __list_splice(list, head, head->next);
298 }
299 
300 /**
301  * list_splice_tail - join two lists, each list being a queue
302  * @list: the new list to add.
303  * @head: the place to add it in the first list.
304  */
305 static inline void list_splice_tail(struct list_head *list,
306                                 struct list_head *head)
307 {
308         if (!list_empty(list))
309                 __list_splice(list, head->prev, head);
310 }
311 
312 /**
313  * list_splice_init - join two lists and reinitialise the emptied list.
314  * @list: the new list to add.
315  * @head: the place to add it in the first list.
316  *
317  * The list at @list is reinitialised
318  */
319 static inline void list_splice_init(struct list_head *list,
320                                     struct list_head *head)
321 {
322         if (!list_empty(list)) {
323                 __list_splice(list, head, head->next);
324                 INIT_LIST_HEAD(list);
325         }
326 }
327 
328 /**
329  * list_splice_tail_init - join two lists and reinitialise the emptied list
330  * @list: the new list to add.
331  * @head: the place to add it in the first list.
332  *
333  * Each of the lists is a queue.
334  * The list at @list is reinitialised
335  */
336 static inline void list_splice_tail_init(struct list_head *list,
337                                          struct list_head *head)
338 {
339         if (!list_empty(list)) {
340                 __list_splice(list, head->prev, head);
341                 INIT_LIST_HEAD(list);
342         }
343 }
344 
345 /**
346  * list_entry - get the struct for this entry
347  * @ptr:        the &struct list_head pointer.
348  * @type:       the type of the struct this is embedded in.
349  * @member:     the name of the list_struct within the struct.
350  */
351 #define list_entry(ptr, type, member) \
352         container_of(ptr, type, member)
353 
354 /**
355  * list_first_entry - get the first element from a list
356  * @ptr:        the list head to take the element from.
357  * @type:       the type of the struct this is embedded in.
358  * @member:     the name of the list_struct within the struct.
359  *
360  * Note, that list is expected to be not empty.
361  */
362 #define list_first_entry(ptr, type, member) \
363         list_entry((ptr)->next, type, member)
364 
365 /**
366  * list_last_entry - get the last element from a list
367  * @ptr:        the list head to take the element from.
368  * @type:       the type of the struct this is embedded in.
369  * @member:     the name of the list_struct within the struct.
370  *
371  * Note, that list is expected to be not empty.
372  */
373 #define list_last_entry(ptr, type, member) \
374         list_entry((ptr)->prev, type, member)
375 
376 /**
377  * list_first_entry_or_null - get the first element from a list
378  * @ptr:        the list head to take the element from.
379  * @type:       the type of the struct this is embedded in.
380  * @member:     the name of the list_struct within the struct.
381  *
382  * Note that if the list is empty, it returns NULL.
383  */
384 #define list_first_entry_or_null(ptr, type, member) \
385         (!list_empty(ptr) ? list_first_entry(ptr, type, member) : NULL)
386 
387 /**
388  * list_next_entry - get the next element in list
389  * @pos:        the type * to cursor
390  * @member:     the name of the list_struct within the struct.
391  */
392 #define list_next_entry(pos, member) \
393         list_entry((pos)->member.next, typeof(*(pos)), member)
394 
395 /**
396  * list_prev_entry - get the prev element in list
397  * @pos:        the type * to cursor
398  * @member:     the name of the list_struct within the struct.
399  */
400 #define list_prev_entry(pos, member) \
401         list_entry((pos)->member.prev, typeof(*(pos)), member)
402 
403 /**
404  * list_for_each        -       iterate over a list
405  * @pos:        the &struct list_head to use as a loop cursor.
406  * @head:       the head for your list.
407  */
408 #define list_for_each(pos, head) \
409         for (pos = (head)->next; pos != (head); pos = pos->next)
410 
411 /**
412  * list_for_each_prev   -       iterate over a list backwards
413  * @pos:        the &struct list_head to use as a loop cursor.
414  * @head:       the head for your list.
415  */
416 #define list_for_each_prev(pos, head) \
417         for (pos = (head)->prev; pos != (head); pos = pos->prev)
418 
419 /**
420  * list_for_each_safe - iterate over a list safe against removal of list entry
421  * @pos:        the &struct list_head to use as a loop cursor.
422  * @n:          another &struct list_head to use as temporary storage
423  * @head:       the head for your list.
424  */
425 #define list_for_each_safe(pos, n, head) \
426         for (pos = (head)->next, n = pos->next; pos != (head); \
427                 pos = n, n = pos->next)
428 
429 /**
430  * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
431  * @pos:        the &struct list_head to use as a loop cursor.
432  * @n:          another &struct list_head to use as temporary storage
433  * @head:       the head for your list.
434  */
435 #define list_for_each_prev_safe(pos, n, head) \
436         for (pos = (head)->prev, n = pos->prev; \
437              pos != (head); \
438              pos = n, n = pos->prev)
439 
440 /**
441  * list_for_each_entry  -       iterate over list of given type
442  * @pos:        the type * to use as a loop cursor.
443  * @head:       the head for your list.
444  * @member:     the name of the list_struct within the struct.
445  */
446 #define list_for_each_entry(pos, head, member)                          \
447         for (pos = list_first_entry(head, typeof(*pos), member);        \
448              &pos->member != (head);                                    \
449              pos = list_next_entry(pos, member))
450 
451 /**
452  * list_for_each_entry_reverse - iterate backwards over list of given type.
453  * @pos:        the type * to use as a loop cursor.
454  * @head:       the head for your list.
455  * @member:     the name of the list_struct within the struct.
456  */
457 #define list_for_each_entry_reverse(pos, head, member)                  \
458         for (pos = list_last_entry(head, typeof(*pos), member);         \
459              &pos->member != (head);                                    \
460              pos = list_prev_entry(pos, member))
461 
462 /**
463  * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
464  * @pos:        the type * to use as a start point
465  * @head:       the head of the list
466  * @member:     the name of the list_struct within the struct.
467  *
468  * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
469  */
470 #define list_prepare_entry(pos, head, member) \
471         ((pos) ? : list_entry(head, typeof(*pos), member))
472 
473 /**
474  * list_for_each_entry_continue - continue iteration over list of given type
475  * @pos:        the type * to use as a loop cursor.
476  * @head:       the head for your list.
477  * @member:     the name of the list_struct within the struct.
478  *
479  * Continue to iterate over list of given type, continuing after
480  * the current position.
481  */
482 #define list_for_each_entry_continue(pos, head, member)                 \
483         for (pos = list_next_entry(pos, member);                        \
484              &pos->member != (head);                                    \
485              pos = list_next_entry(pos, member))
486 
487 /**
488  * list_for_each_entry_continue_reverse - iterate backwards from the given point
489  * @pos:        the type * to use as a loop cursor.
490  * @head:       the head for your list.
491  * @member:     the name of the list_struct within the struct.
492  *
493  * Start to iterate over list of given type backwards, continuing after
494  * the current position.
495  */
496 #define list_for_each_entry_continue_reverse(pos, head, member)         \
497         for (pos = list_prev_entry(pos, member);                        \
498              &pos->member != (head);                                    \
499              pos = list_prev_entry(pos, member))
500 
501 /**
502  * list_for_each_entry_from - iterate over list of given type from the current point
503  * @pos:        the type * to use as a loop cursor.
504  * @head:       the head for your list.
505  * @member:     the name of the list_struct within the struct.
506  *
507  * Iterate over list of given type, continuing from current position.
508  */
509 #define list_for_each_entry_from(pos, head, member)                     \
510         for (; &pos->member != (head);                                  \
511              pos = list_next_entry(pos, member))
512 
513 /**
514  * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
515  * @pos:        the type * to use as a loop cursor.
516  * @n:          another type * to use as temporary storage
517  * @head:       the head for your list.
518  * @member:     the name of the list_struct within the struct.
519  */
520 #define list_for_each_entry_safe(pos, n, head, member)                  \
521         for (pos = list_first_entry(head, typeof(*pos), member),        \
522                 n = list_next_entry(pos, member);                       \
523              &pos->member != (head);                                    \
524              pos = n, n = list_next_entry(n, member))
525 
526 /**
527  * list_for_each_entry_safe_continue - continue list iteration safe against removal
528  * @pos:        the type * to use as a loop cursor.
529  * @n:          another type * to use as temporary storage
530  * @head:       the head for your list.
531  * @member:     the name of the list_struct within the struct.
532  *
533  * Iterate over list of given type, continuing after current point,
534  * safe against removal of list entry.
535  */
536 #define list_for_each_entry_safe_continue(pos, n, head, member)                 \
537         for (pos = list_next_entry(pos, member),                                \
538                 n = list_next_entry(pos, member);                               \
539              &pos->member != (head);                                            \
540              pos = n, n = list_next_entry(n, member))
541 
542 /**
543  * list_for_each_entry_safe_from - iterate over list from current point safe against removal
544  * @pos:        the type * to use as a loop cursor.
545  * @n:          another type * to use as temporary storage
546  * @head:       the head for your list.
547  * @member:     the name of the list_struct within the struct.
548  *
549  * Iterate over list of given type from current point, safe against
550  * removal of list entry.
551  */
552 #define list_for_each_entry_safe_from(pos, n, head, member)                     \
553         for (n = list_next_entry(pos, member);                                  \
554              &pos->member != (head);                                            \
555              pos = n, n = list_next_entry(n, member))
556 
557 /**
558  * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
559  * @pos:        the type * to use as a loop cursor.
560  * @n:          another type * to use as temporary storage
561  * @head:       the head for your list.
562  * @member:     the name of the list_struct within the struct.
563  *
564  * Iterate backwards over list of given type, safe against removal
565  * of list entry.
566  */
567 #define list_for_each_entry_safe_reverse(pos, n, head, member)          \
568         for (pos = list_last_entry(head, typeof(*pos), member),         \
569                 n = list_prev_entry(pos, member);                       \
570              &pos->member != (head);                                    \
571              pos = n, n = list_prev_entry(n, member))
572 
573 /**
574  * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
575  * @pos:        the loop cursor used in the list_for_each_entry_safe loop
576  * @n:          temporary storage used in list_for_each_entry_safe
577  * @member:     the name of the list_struct within the struct.
578  *
579  * list_safe_reset_next is not safe to use in general if the list may be
580  * modified concurrently (eg. the lock is dropped in the loop body). An
581  * exception to this is if the cursor element (pos) is pinned in the list,
582  * and list_safe_reset_next is called after re-taking the lock and before
583  * completing the current iteration of the loop body.
584  */
585 #define list_safe_reset_next(pos, n, member)                            \
586         n = list_next_entry(pos, member)
587 
588 /*
589  * Double linked lists with a single pointer list head.
590  * Mostly useful for hash tables where the two pointer list head is
591  * too wasteful.
592  * You lose the ability to access the tail in O(1).
593  */
594 
595 #define HLIST_HEAD_INIT { .first = NULL }
596 #define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
597 #define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
598 static inline void INIT_HLIST_NODE(struct hlist_node *h)
599 {
600         h->next = NULL;
601         h->pprev = NULL;
602 }
603 
604 static inline int hlist_unhashed(const struct hlist_node *h)
605 {
606         return !h->pprev;
607 }
608 
609 static inline int hlist_empty(const struct hlist_head *h)
610 {
611         return !h->first;
612 }
613 
614 static inline void __hlist_del(struct hlist_node *n)
615 {
616         struct hlist_node *next = n->next;
617         struct hlist_node **pprev = n->pprev;
618         *pprev = next;
619         if (next)
620                 next->pprev = pprev;
621 }
622 
623 static inline void hlist_del(struct hlist_node *n)
624 {
625         __hlist_del(n);
626         n->next = LIST_POISON1;
627         n->pprev = LIST_POISON2;
628 }
629 
630 static inline void hlist_del_init(struct hlist_node *n)
631 {
632         if (!hlist_unhashed(n)) {
633                 __hlist_del(n);
634                 INIT_HLIST_NODE(n);
635         }
636 }
637 
638 static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
639 {
640         struct hlist_node *first = h->first;
641         n->next = first;
642         if (first)
643                 first->pprev = &n->next;
644         h->first = n;
645         n->pprev = &h->first;
646 }
647 
648 /* next must be != NULL */
649 static inline void hlist_add_before(struct hlist_node *n,
650                                         struct hlist_node *next)
651 {
652         n->pprev = next->pprev;
653         n->next = next;
654         next->pprev = &n->next;
655         *(n->pprev) = n;
656 }
657 
658 static inline void hlist_add_behind(struct hlist_node *n,
659                                     struct hlist_node *prev)
660 {
661         n->next = prev->next;
662         prev->next = n;
663         n->pprev = &prev->next;
664 
665         if (n->next)
666                 n->next->pprev  = &n->next;
667 }
668 
669 /* after that we'll appear to be on some hlist and hlist_del will work */
670 static inline void hlist_add_fake(struct hlist_node *n)
671 {
672         n->pprev = &n->next;
673 }
674 
675 /*
676  * Move a list from one list head to another. Fixup the pprev
677  * reference of the first entry if it exists.
678  */
679 static inline void hlist_move_list(struct hlist_head *old,
680                                    struct hlist_head *new)
681 {
682         new->first = old->first;
683         if (new->first)
684                 new->first->pprev = &new->first;
685         old->first = NULL;
686 }
687 
688 #define hlist_entry(ptr, type, member) container_of(ptr,type,member)
689 
690 #define hlist_for_each(pos, head) \
691         for (pos = (head)->first; pos ; pos = pos->next)
692 
693 #define hlist_for_each_safe(pos, n, head) \
694         for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
695              pos = n)
696 
697 #define hlist_entry_safe(ptr, type, member) \
698         ({ typeof(ptr) ____ptr = (ptr); \
699            ____ptr ? hlist_entry(____ptr, type, member) : NULL; \
700         })
701 
702 /**
703  * hlist_for_each_entry - iterate over list of given type
704  * @pos:        the type * to use as a loop cursor.
705  * @head:       the head for your list.
706  * @member:     the name of the hlist_node within the struct.
707  */
708 #define hlist_for_each_entry(pos, head, member)                         \
709         for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member);\
710              pos;                                                       \
711              pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
712 
713 /**
714  * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
715  * @pos:        the type * to use as a loop cursor.
716  * @member:     the name of the hlist_node within the struct.
717  */
718 #define hlist_for_each_entry_continue(pos, member)                      \
719         for (pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member);\
720              pos;                                                       \
721              pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
722 
723 /**
724  * hlist_for_each_entry_from - iterate over a hlist continuing from current point
725  * @pos:        the type * to use as a loop cursor.
726  * @member:     the name of the hlist_node within the struct.
727  */
728 #define hlist_for_each_entry_from(pos, member)                          \
729         for (; pos;                                                     \
730              pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
731 
732 /**
733  * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
734  * @pos:        the type * to use as a loop cursor.
735  * @n:          another &struct hlist_node to use as temporary storage
736  * @head:       the head for your list.
737  * @member:     the name of the hlist_node within the struct.
738  */
739 #define hlist_for_each_entry_safe(pos, n, head, member)                 \
740         for (pos = hlist_entry_safe((head)->first, typeof(*pos), member);\
741              pos && ({ n = pos->member.next; 1; });                     \
742              pos = hlist_entry_safe(n, typeof(*pos), member))
743 
744 #endif
745 

This page was automatically generated by LXR 0.3.1 (source).  •  Linux is a registered trademark of Linus Torvalds  •  Contact us

    Home
    Development
    Services
    Training
    Docs
    Community
    Company
    Blog


