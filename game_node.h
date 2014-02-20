/**
 * @file game_node.h
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * A game node 
 */ 
#ifndef _GAME_NODE_ 
#define _GAME_NODE_

#define TREE_DEPTH 3
#define BOARD_SIZE 8

#include "state.h"
#include "move.h"

typedef struct GameNode {
    short int utility_val;
    short int child_count;
    struct GameNode * parent;       /**< parent node */
    struct State * state;           /**< current state */
    struct List * children;
    struct Move * best_move;        /**< best move so far */
    short int best_util_val;
} TNode;

struct GameNode * new_game_node( struct State * state, struct GameNode * parent );
void add_child_game_node( struct GameNode * parent, struct GameNode * child );
void delete_game_node( struct GameNode ** root );

#endif /* _GAME_NODE_ */
