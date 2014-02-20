/**
 * @brief A konane game node
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 */
#include <stdlib.h>
#include <assert.h>

#include "game_node.h"
#include "state.h"
#include "list.h"
#include "utility.h"

/**
 * Create a new game node
 *
 * @param state a game state
 * @param parent the parent node
 * @return a new game node with state and parent set
 */
struct GameNode * new_game_node( struct State * state, struct GameNode * parent )
{
    struct GameNode * node = Calloc( 1, sizeof( struct GameNode ) );
    assert( node );

    node->state = state;
    node->parent = parent;
    node->children = new_list();

    return node;
}

/**
 * Add a child to a game node
 *
 * @param parent a parent node
 * @param child the child node to add to parent
 */
void add_child_game_node( struct GameNode * parent, struct GameNode * child )
{
    child->parent = parent;
    add_front( &parent->children, child );
}

/**
 * Delete a game node
 *
 * @param root a game tree root
 */
void delete_game_node( struct GameNode ** root )
{
    if( *root == NULL )
        return;
    
    /* for each child */
    if( (*root)->children != NULL )
    {
        struct ListNode * current = (*root)->children->head;
        struct ListNode * temp;
        while( current != NULL )
        {
            struct GameNode * temp_node = current->data;
            Free( temp_node->state, sizeof( struct State ) );
            Free( temp_node->best_move, sizeof( struct Move ) );

            delete_game_node( &temp_node );

            current = current->next;
        }
        delete_list( &(*root)->children );
    }
    
    Free( *root, sizeof( struct GameNode ) );
    *root = NULL;
}
