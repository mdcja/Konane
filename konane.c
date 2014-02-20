/**
 * @file konane.c
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * Provides an implementation of the game of konane
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

#include "konane.h"
#include "state.h"
#include "move.h"
#include "list.h"
#include "game_node.h"
#include "utility.h"

#define MAX_DEPTH 15
#define MEMORYSIZE 1000000
#define THINKING_TIME 10

extern time_t timer;

static int max( int a, int b );
static int min( int a, int b );
static int min_value( struct GameNode * game_state, int depth, int alpha, int beta );
static int max_value( struct GameNode * game_state, int depth, int alpha, int beta );

/**
 * Find possible actions right on a row
 *
 * @param state a game state
 * @param row the row to check
 * @return a list of actions
 */
static struct List * actions_right( const struct State * state, int row )
{
    int idx = 0;
    struct List * actions = new_list();
    
    for(idx = 0; (idx < SIZE) && ((idx + 2) < SIZE); idx++){
      if( (state->board[ row ][idx] == state->player )
	    && (state->board[ row ][ idx + 1 ] == opposite_player( state->player ))
	    && (state->board[ row ][ idx + 2 ] == 'O')){
	   /* check for moves  - increment to optimize move*/
	   int end_col = idx + 2;	   
	   /* add move to action list for single jump  */
	   add_front( &actions, create_move( row, idx, row, end_col) );
	   while( ((end_col + 2) < SIZE )
	        && (state->board[ row ][ end_col ] == 'O' )
	        && (state->board[ row ][ end_col + 1 ] == opposite_player( state->player ))
	        && (state->board[ row ][ end_col + 2 ] == 'O') ){
	     end_col += 2;
	     /* add optimized moves to action list */
	     add_front( &actions, create_move( row, idx, row, end_col) );
	   }
      }
    }
    
    return actions;
}

/**
 * Find possible actions left on a row
 *
 * @param state a game state
 * @param row the row to check
 * @return a list of actions
 */
static struct List * actions_left( const struct State * state, int row )
{
    int idx = SIZE-1;
    struct List * actions = new_list();

     for(idx = SIZE - 1; (idx > 0) && ((idx - 2) >= 0); idx--){
       if( (state->board[ row ][idx] == state->player )
	    && (state->board[ row ][ idx - 1 ] == opposite_player( state->player ))
	    && (state->board[ row ][ idx - 2 ] == 'O')){
	   /* check for moves  - increment to optimize move*/
	   int end_col = idx - 2;
	   /* add move to action list for single jump  */
	   add_front( &actions, create_move( row, idx, row, end_col) );
	   while( ((end_col - 2) > 0 )
	        && (state->board[ row ][ end_col ] == 'O' )
	        && (state->board[ row ][ end_col - 1 ] == opposite_player( state->player ))
	        && (state->board[ row ][ end_col - 2 ] == 'O') ){
	     end_col -= 2;
	     /* add optimized moves to action list */ 
	     add_front( &actions, create_move( row, idx, row, end_col) );
	   }
       }
     }

    return actions;
}

/**
 * Find possible actions down on a column
 *
 * @param state a game state
 * @param col the column to check
 * @return a list of actions
 */
static struct List * actions_down( const struct State * state, int col )
{
    int idx = 0;
    struct List * actions = new_list();

    for(idx = 0; (idx < SIZE) && ((idx + 2) < SIZE); idx++){
      if(  (state->board[ idx ][col] == state->player )
	    && (state->board[ idx + 1 ][ col ] == opposite_player( state->player ))
	    && (state->board[ idx + 2 ][ col ] == 'O')){
	   /* check for moves  - increment to optimize move*/
	   int end_row = idx + 2;
	   /* add move to action list for single jump  */
	   add_front( &actions, create_move( idx, col, end_row, col) );
	   while( ((end_row + 2) < SIZE )
	        && (state->board[ end_row ][ col ] == 'O' )
	        && (state->board[ end_row + 1 ][ col ] == opposite_player( state->player ))
	        && (state->board[ end_row + 2 ][ col ] == 'O') ){
	     end_row += 2;
	     /* add optimized moves to action list */
	     add_front( &actions, create_move( idx, col, end_row, col) );
	   }
      }
    }
    
    return actions;
}

/**
 * Find possible actions up on a column
 *
 * @param state a game state
 * @param col the column to check
 * @return a list of actions
 */
static struct List * actions_up( const struct State * state, int col )
{
    int idx = SIZE - 1;
    struct List * actions = new_list();

    for(idx = SIZE - 1; (idx > 0) && ((idx - 2) >= 0); idx--){
      if(  (state->board[ idx ][col] == state->player )
	    && (state->board[ idx - 1 ][ col ] == opposite_player( state->player ))
	    && (state->board[ idx - 2 ][ col ] == 'O')){
	   /* check for moves  - increment to optimize move*/
	   int end_row = idx - 2;
	   /* add move to action list for single jump  */
	   add_front( &actions, create_move( idx, col, end_row, col) );
	   while( ((end_row - 2) > 0 )
	        && (state->board[ end_row ][ col ] == 'O' )
	        && (state->board[ end_row - 1 ][ col ] == opposite_player( state->player ))
	        && (state->board[ end_row - 2 ][ col ] == 'O') ){
	     end_row -= 2;
	     /* add optimized moves to action list */
	     add_front( &actions, create_move( idx, col, end_row, col) );
	   }
      }
    }

    return actions;
}

/**
 * Find all possible actions/moves in a state
 *
 * @param state a state to check for moves
 * @return a list of actions/moves
 */
struct List * actions( const struct State * state )
{

    struct List * moves = new_list();
    struct List * temp_moves;
    struct ListNode * current;

    /* combine all actions */
    for( int i = 0; i < SIZE; i++ )
    {
        /* actions right */
        temp_moves = actions_right( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions left */
        temp_moves = actions_left( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions up */
        temp_moves = actions_up( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions down */
        temp_moves = actions_down( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );
    }

    return moves;
}

/**
 * Transition model
 *
 * Find the result of applying a move to a state
 *
 * @param state a state
 * @param action an action
 * @return a new state, which is the result of applying an action to state, if
 *  action is invalid or no move is possible return null.
 */
struct State * result( const struct State * state, const struct Move * action )
{
    char new_board[SIZE][SIZE];
    
    /* copy old board */
    for( int i = 0; i < SIZE; i++ )
        for( int j = 0; j < SIZE; j++ )
            new_board[ i ][ j ] = state->board[ i ][ j ];

    /* validate move */ 
    if( validate_action( state, action ) )
    {
        /* get resulting board state 
         * by removing all pieces between start and end action 
         */
        if( action->start_row == action->end_row )
        {
            /* blank column */
            if( action->start_col < action->end_col )
                for( int i = action->start_col; i < action->end_col; i++ )
                    new_board[ action->start_row ][ i ] = 'O';
            else
                for( int i = action->start_col; i > action->end_col; i-- )
                    new_board[ action->start_row ][ i ] = 'O';
        }
        else
        {
            /* blank row */
            if( action->start_row < action->end_row )
                for( int i = action->start_row; i < action->end_row; i++ )
                    new_board[ i ][ action->start_col ] = 'O';
            else
                for( int i = action->start_row; i > action->end_row; i-- )
                    new_board[ i ][ action->start_col ] = 'O';
        }

        /* set current piece */
        new_board[ action->end_row ][ action->end_col ] = state->player;

        /* return changed board */
        return new_state( new_board, opposite_player( state->player ) );
    }

    return NULL;
}

/**
 * Validate an action
 *
 * @param state a state to check
 * @param action a move to perform
 * @return 1 if action is valid, else return 0
 */
int validate_action( const struct State * state, const struct Move * action )
{
    struct List * moves = new_list();
    struct List * temp_moves;
    struct ListNode * current;
    int is_valid = 0;

    /* combine all actions */
    for( int i = 0; i < SIZE; i++ )
    {
        /* actions right */
        temp_moves = actions_right( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions left */
        temp_moves = actions_left( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions up */
        temp_moves = actions_up( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );

        /* actions down */
        temp_moves = actions_down( state, i );

        current = temp_moves->head; 
        while( current != NULL )
        {
            add_front( &moves, current->data );
            current = current->next;
        }

        delete_list( &temp_moves );
    }

    /* check if moves is in possible actions */
    struct Move * move;
    current = moves->head;
    while( current != NULL )
    {
        move = current->data;
        if( compare_move( move, action ) == 1 )
        {
            is_valid = 1;
        }
        current = current->next;

        Free( move, sizeof( struct Move ) );
    }

    delete_list( &moves );

    return is_valid;
}

/**
 * Check if the game is over
 *
 * @param state a state to check
 * @return 1 if game is over, else return 0
 */
int terminal_test( const struct State * state )
{
    /* if there are no more moves for other player, game is done */
    struct List * moves = actions( state );

    /* check if state is terminal */
    int has_move = ( moves->count == 0 );

    struct ListNode * current = moves->head;
    struct Move * move;

    while( current != NULL )
    {
        move = current->data;
        Free( move, sizeof( struct Move ) );

        current = current->next;
    }
    delete_list( &moves );

    return has_move;
}

//will evaluate the board and return a number based on the
//number of moves an enemy has
static int evaluation (char fcolor, char ecolor, char board[][BOARD_SIZE]) {
  int count = 0, counter = 0;
  int size=BOARD_SIZE;
  int tc=0;
   
 //goes through the board horizontally
 while(count<size){
     while(counter<size){
       if(counter+2<size){
	 if((ecolor==board[count][counter])
	    && (fcolor==board[count][counter+1])
	    && (board[count][counter+2]=='O'))
	   { 
	     tc=tc+1; 
	   }
       }
       if(counter-2>=0){
	 if((ecolor==board[count][counter])
	    && (fcolor==board[count][counter-1])
	    && (board[count][counter-2]=='O')) 
	   {
	     tc=tc+1;
	   }
       }
       //printf("count: %d counter: %d\n",count,counter);
       counter++;
     }
     counter=0;
    count++;
 }

 count=0;
 counter=0;
 //goes through the board vertically
 while(counter<size){
   while(count<size){
     if(count+2<size){
       if((ecolor==board[count][counter])
	  && (fcolor==board[count+1][counter])
	  && (board[count+2][counter]=='O'))
	 {
	   tc=tc+1;
	 }
     }
     if(count-2>=0){
       if((ecolor==board[count][counter])
	  && (fcolor==board[count-1][counter])
	  && (board[count-2][counter]=='O'))
	 {
	   tc=tc+1;
	 }
     }

     //printf("count: %d counter: %d\n",count,counter);
     count++;
   }
   count=0;
   counter++;
 }
 
 return tc;
}

/**
 * Call the evaluation function
 *
 * @param state a state to evaluate
 * @return the utility value of the state
 */
int eval( struct State * state )
{
    return evaluation( opposite_player( state->player), state->player, state->board ) - 
           evaluation( state->player, opposite_player( state->player ), state->board );
}

/**
 * Get the opposite player
 *
 * @param player the current player
 * @return the opposite player
 */
char opposite_player( char player )
{
    if( player == 'B' )
        return 'W';
    return 'B';
}

/**
 * Validate the first move
 *
 * @param state a state to validate
 * @param action an action
 * @return 1 if move is valid, else return 0
 */
int validate_first_in_move( const struct State * state, const struct Move * action )
{
    if( ((action->start_row == 3) && (action->start_col == 3)) ||
        ((action->start_row == 3) && (action->start_col == 4)) ||
        ((action->start_row == 4) && (action->start_col == 3)) ||
        ((action->start_row == 4) && (action->start_col == 4)) ||
        ((action->start_row == 0) && (action->start_col == 0)) ||
        ((action->start_row == 0) && (action->start_col == 7)) ||
        ((action->start_row == 7) && (action->start_col == 0)) ||
        ((action->start_row == 7) && (action->start_col == 7)) ) 
    {
        if( state->board[ action->start_row ][ action->start_col ] == 'B' )
            return 1;
    }

    return 0;
}

/**
 * Validate second move
 *
 * @param state a state to validate
 * @param action an action
 * @return 1 if move is valid, else return 0
 */
int validate_second_in_move( const struct State * state, const struct Move * action )
{
    int i, j;
    int row, col;

    /* find empty space on board */
    for( row = 0; row < SIZE; row++ )
        for( col = 0; col < SIZE; col++ )
            if( state->board[ row ][ col ] == 'O' )
            {
                i = row;
                j = col;
            }

    /* valid moves are on either side of empty space */
    if( i == 0 && j == 0 )
    {
        if( ( state->board[ i ][ j + 1 ] == 'W' ) ||
            ( state->board[ i + 1][ j ] == 'W' ) )
            return 1;
    }
    else if( i == 0 && j == 7 )
    {
        if( (state->board[ i ][ j - 1 ] == 'W' ) ||
            (state->board[ i + 1 ][ j ] == 'W' ) )
            return 1;
    }
    else if( i == 7 && j == 0 )
    {
        if( (state->board[ i ][ j + 1 ] == 'W' ) ||
            (state->board[ i - 1 ][ j ] == 'W' ) )
            return 1;
    }
    else if( i == 7 && j == 7 )
    {
        if( (state->board[ i ][ j - 1 ] == 'W' ) ||
            (state->board[ i - 1 ][ j ] == 'W' ) )
            return 1;
    }
    else
    {
        if( (state->board[ i + 1 ][ j ] == 'W' ) ||
            (state->board[ i - 1 ][ j ] == 'W' ) ||
            (state->board[ i ][ j + 1 ] == 'W' ) ||
            (state->board[ i ][ j - 1 ] == 'W' ) )
            return 1;
    }

    return 0;
}

/**
 * Get max of a or b
 *
 * @param a
 * @param b
 * @return the max value
 */
static int max( int a, int b )
{
    return a > b ? a : b;
}

/**
 * Get min of a or b
 *
 * @param a
 * @param b
 * @return the min value
 */
static int min( int a, int b )
{
    return a < b ? a : b;
}

/**
 * Max of alpha beta
 *
 * @param game_state the current game tree state
 * @param depth the depth of the tree
 * @param alpha
 * @param beta
 * @return a utility value
 */
static int max_value( struct GameNode * game_state, int depth, int alpha, int beta )
{
    time_t current_time;
    time( &current_time );

    if( ( (current_time - timer) > THINKING_TIME - 1 ) ||
           cutoff_test( game_state->state, depth ) ||
           memory_usage() > MEMORYSIZE )
    {
        return eval( game_state->state );
    }
    struct ListNode * current_b;

    ++depth;
    int v = INT_MIN;
    int min_val;

    struct List * a = actions( game_state->state ); /* get possible actions */
    /* iterate over all moves */
    struct ListNode * current = a->head;
    while( current != NULL )
    {
        /* create new game node */
        struct State * state = result( game_state->state, current->data );
        struct GameNode * node = new_game_node( state, game_state );

        add_child_game_node( game_state, node );

        /* find max value */
        min_val = min_value( node, depth, alpha, beta );

        if( min_val > v )
        {
            game_state->best_util_val = v;
            if( game_state->best_move != NULL )
                Free( game_state->best_move, sizeof( struct Move ) );
            game_state->best_move = clone_move( current->data );
        }

        v = max( v, min_val );

        if( v >= beta )
        {
            if( game_state->best_move != NULL )
                Free( game_state->best_move, sizeof( struct Move ) );
            game_state->best_move = clone_move( current->data );

            /* free list of actions except for best move */
            current_b = a->head;
            while( current_b != NULL )
            {
                Free( current_b->data, sizeof( struct Move ) );
                current_b = current_b->next;
            }

            delete_list( &a );
            return v;
        }

        alpha = max( alpha, v );
        
        current = current->next;
    }

    /* free list of actions */
    current_b = a->head;
    while( current_b != NULL )
    {
        Free( current_b->data, sizeof( struct Move ) );
        current_b = current_b->next;
    }

    delete_list( &a );
    return v;
}

/**
 * Min of alpha beta
 *
 * @param game_state the current game tree state
 * @param depth the depth of the tree
 * @param alpha
 * @param beta
 * @return a utility value
 */
static int min_value( struct GameNode * game_state, int depth, int alpha, int beta )
{
    time_t current_time;
    time( &current_time );

    if( ( (current_time - timer) > THINKING_TIME - 1 ) ||
           cutoff_test( game_state->state, depth ) ||
           memory_usage() > MEMORYSIZE )
        return eval( game_state->state );

    ++depth;
    int v = INT_MAX;
    int max_val;
    struct ListNode * current_b;

    struct List * a = actions( game_state->state ); /* get possible actions */
    /* iterate over all actions */
    struct ListNode * current = a->head;
    while( current != NULL )
    {
        /* create new game node */
        struct State * state = result( game_state->state, current->data );
        struct GameNode * node = new_game_node( state, game_state );
        add_child_game_node( game_state, node );    /* add child node */

        max_val = max_value( node, depth, alpha, beta );
        if( max_val < v )
        {
            game_state->best_util_val = v;
            if( game_state->best_move != NULL )
                Free( game_state->best_move, sizeof( struct Move ) );
            game_state->best_move = clone_move( current->data );
        }

        v = min( v, max_val );

        if( v <= alpha )
        {
            if( game_state->best_move != NULL )
                Free( game_state->best_move, sizeof( struct Move ) );
            game_state->best_move = clone_move( current->data );
            /* free list of actions except for chosen action */
            current_b = a->head;
            while( current_b != NULL )
            {
                Free( current_b->data, sizeof( struct Move ) );
                current_b = current_b->next;
            }
            delete_list( &a );
            return v;
        }

        beta = min( beta, v );

        current = current->next;
    }

    /* free list of actions */
    current_b = a->head;
    while( current_b != NULL )
    {
        Free( current_b->data, sizeof( struct Move ) );
        current_b = current_b->next;
    }
    delete_list( &a );
    return v;
}

/**
 * Perform a cutoff test
 *
 * @param state the state of the game
 * @param depth the current depth
 * @return 1 if depth has exceeded max depth, or if terminal state has been reached, else return 0
 */
int cutoff_test( const struct State * state, int depth )
{
    if( depth > MAX_DEPTH )
        return 1;

    return terminal_test( state );
}

/**
 * Alpha beta search with time, memory, and depth cutoff
 *
 * @param game_state a game tree root
 * @return a move
 */
struct Move * alpha_beta_search( struct GameNode * game_state )
{
    int v = max_value( game_state, 0, INT_MIN, INT_MAX );

    //printf( "Best util val: %d\n", game_state->best_util_val );
    //printf( "Max val : %d\n", v );
    //printf( "Best move: " );
    //print_move( game_state->best_move );
    
    return game_state->best_move;
}

