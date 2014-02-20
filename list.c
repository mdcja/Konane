/**
 * @file list.c
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * This file provides an implementation of a list
 */
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "utility.h"

/**
 * Create a new list
 *
 * @return a new empty list
 */
struct List * new_list( void )
{
  struct List * list = Calloc( 1, sizeof( struct List ) );

  list->head = NULL;
  list->tail = NULL;
  list->count = 0;

  return list;
}

/**
 * Delete a list
 *
 * @param list a list to destroy
 */
void delete_list( struct List ** list )
{
  struct ListNode * current, * temp;

  current = (*list)->head;
  while( current != NULL )
    {
      temp = current;
      current = current->next;

      Free( temp, sizeof( struct ListNode ) );
    }

  Free( (*list ), sizeof( struct List ) );
  *list = NULL;
}

/**
 * Add an item to the front of the list
 *
 * @param list a list to add to
 * @param data the data to add
 */
void add_front( struct List ** list, void * data )
{
  struct ListNode * node = Calloc( 1, sizeof( struct ListNode ) );
  assert( node );

  node->data = data;
  node->next = (*list)->head;

  if( (*list)->count == 0 )
    {
      (*list)->tail = node;
      (*list)->head = node;
    }
  else
    {
      (*list)->head = node;
    }
  (*list)->count++;
}
