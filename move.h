/**
 * @file move.h
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * This file provides a move in konane
 */
#ifndef _MOVE_H_
#define _MOVE_H_

/** A move */
struct Move {
    short start_row;
    short start_col;
    short end_row;
    short end_col;
};

#if 1

struct Move *   create_move( short start_row, short start_col, short end_row, short end_col );
struct Move *   clone_move( const struct Move * move );
char *          move2str( const struct Move * move );
struct Move *   str2move( const char * move );
char *          first_move2str( const struct Move * first_move );
struct Move *   str2first_move( const char * move );
struct Move *   first_str2move( const char * move );
int             compare_move( const struct Move * a, const struct Move * b );
char            num2letter( int i );
int             letter2num( char letter );
void            print_move( const struct Move * move );
void            print_single_move( const struct Move * move );

#endif

#if 0
struct Move * new_move( short start_row, short start_col, short end_row, short end_col );
char * translate_move( const struct Move * move );
struct Move * clone_move( const struct Move * move );
struct Move * translate_in_move( const char * move );
struct Move * translate_first_in_move( const char * move );
char * translate_first_move( const struct Move * move );
int compare_move( const struct Move * a, const struct Move * b );
void print_move( const struct Move * move );
void print_single_move( const struct Move * move );
char row2letter( int i );
#endif

#endif /* _MOVE_H_ */
