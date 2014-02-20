/**
 * @file list.h
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * This file provides a linked list
 */
#ifndef _LIST_H_
#define _LIST_H_

/** a linked list */
struct List {
  struct ListNode * head; /**< pointer to head */
  struct ListNode * tail; /**< pointer to tail */
  int count;              /**< number of items in list */
};

/** a linked list node */
struct ListNode {
  void * data;            /**< data item */
  struct ListNode * next; /**< next node */
};

struct List * new_list( void );
void delete_list( struct List ** list );

void add_front( struct List ** list, void * data );

#endif /* _LIST_H_ */
