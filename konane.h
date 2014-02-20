/**
 * @file konane.h
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 */
#ifndef _KONANE_H_
#define _KONANE_H_

#include "state.h"
#include "move.h"
#include "list.h"
#include "game_node.h"

struct List * actions( const struct State * state );
struct State * result( const struct State * state, const struct Move * action );
int validate_action( const struct State * state, const struct Move * action );
int terminal_test( const struct State * state );
char opposite_player( char player );

int validate_first_in_move( const struct State * state, const struct Move * action );
int validate_second_in_move( const struct State * state, const struct Move * action );

int cutoff_test( const struct State * state, int depth );
int eval( struct State * state );

struct Move * alpha_beta_search( struct GameNode * game_state );

#endif /* _KONANE_H_ */
