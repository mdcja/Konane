/**
 * @file move.c
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 *
 * Provides an implementation of moves
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "move.h"
#include "utility.h"

#define STR_LEN 12

/**
 * Create a new move
 *
 * @param start_row the starting row
 * @param start_col the starting column
 * @param end_row the ending row
 * @param end_col the ending column
 * @return a new move containing the start row, start col, end row, and end col
 */
struct Move * create_move( short start_row, short start_col, short end_row, short end_col )
{
    struct Move * move = Calloc( 1, sizeof( struct Move ) );
    assert( move );

    move->start_row = start_row;
    move->start_col = start_col;
    move->end_row = end_row;
    move->end_col = end_col;

    return move;
}

/**
 * Convert a row number to a string
 *
 * @param i the row number
 * @return the row as a letter, if row is invalid return X
 */
char num2letter( int i )
{
    static char letters[ 8 ] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    if( i < 0 || i > 8 )
        return 'X';

    return letters[i];
}

/**
 * Translate a move into a string
 *
 * @post string returned will be null terminated
 * @post output as: [letter][number] - [letter][number], e.g. a1 - a4
 *
 * @param move a move to convert to string
 * @return a string in human readable form
 */
char * move2str( const struct Move * move )
{
    char * human_readable = Calloc( STR_LEN, sizeof( char ) );
    assert( human_readable );
    
    /* format string */
    snprintf( human_readable, 
            STR_LEN, 
            "%c%d - %c%d",
            num2letter( move->start_col ),
            SIZE - move->start_row,
            num2letter( move->end_col ),
            SIZE - move->end_row );

    return human_readable;
}

/**
 * Translate a move into a string
 *
 * @param move a move to convert to string
 * @return a string in human readable form
 */
char * first_move2str( const struct Move * first_move )
{
    char * human_readable = Calloc( STR_LEN, sizeof( char ) );
    assert( human_readable );
    
    /* format string */
    snprintf( human_readable, STR_LEN, "%c%d",
                num2letter( first_move->start_col ),
                SIZE - first_move->start_row );

    return human_readable;
}

/**
 * Convert a letter to a row
 *
 * @param letter a letter to convert a row number
 * @return the row number
 */
int letter2num( char letter )
{
    char l = toupper( letter );
    return (int) l - 65;
}

/**
 * Translate an input move
 *
 * @param move a move input by the user
 * @return a move
 */
struct Move * str2move( const char * move )
{
    int length = strnlen( move, 20 );
    int i;
    int valid = 0;
    char start_row[2];
    char start_col[2];
    char end_row[2];
    char end_col[2];

    /* find first letter */
    for( i = 0; i < length; i++ )
        if( isalpha( move[ i ] ) )
        {
            start_col[0] = move[ i ];
            start_col[1] = '\0';
            valid++;
            break;
        }

    /* find next number */
    for( ; i< length; i++ )
        if( isdigit( move[i] ) )
        {
            start_row[0] = move[ i ];
            start_row[1] = '\0';
            valid++;
            break;
        }

    /* find next letter */
    for( ; i < length; i++ )
        if( isalpha( move[ i ] ) )
        {
            end_col[0] = move[ i ];
            end_col[1] = '\0';
            valid++;
            break;
        }

    /* find next number */
    for( ; i < length; i++ )
        if( isdigit( move[ i ] ) )
        {
            end_row[0] = move[ i ];
            end_row[1] = '\0';
            valid++;
            break;
        }

    if( valid != 4 )
        return NULL;

    /*
    struct Move * translated_move = new_move( 
            letter2row( start_row ), 
            atoi( start_col ) - 1,
            letter2row( end_row ),
            atoi( end_col ) - 1 );
            */
    struct Move * translated_move = create_move( 
            SIZE - atoi( start_row ),
            letter2num( start_col[0] ),
            SIZE - atoi( end_row ),
            letter2num( end_col[0] ) );

    return translated_move;
}

/**
 * Translate a beginning move to coordinates
 *
 * @param move a string consisting of the human input (will only read one piece)
 * @return a move consisting of one piece
 */
struct Move * first_str2move( const char * move )
{
    int length = strnlen( move, 20 );
    int i;
    int valid = 0;
    char start_row[2];
    char start_col[2];

    /* find first letter */
    for( i = 0; i < length; i++ )
        if( isalpha( move[ i ] ) )
        {
            start_col[0] = move[ i ];
            start_col[1] = '\0';
            valid++;
            break;
        }

    /* find next number */
    for( ; i< length; i++ )
        if( isdigit( move[i] ) )
        {
            start_row[0] = move[ i ];
            start_row[1] = '\0';
            valid++;
            break;
        }

    /* check input parameters */
    if( valid != 2 )
        return NULL;

    struct Move * translated_move = create_move( 
            SIZE - atoi( start_row ),
            letter2num( start_col[0] ),
            0,
            0 );

    return translated_move;
}

/**
 * Compare a move
 *
 * @param a a move
 * @param b a move
 * @return 1 if moves are the same, else return 0
 */
int compare_move( const struct Move * a, const struct Move * b )
{
    return ( a->start_row == b->start_row &&
             a->start_col == b->start_col &&
             a->end_row == b->end_row &&
             a->end_col == b->end_col );
}

/**
 * Print a move
 *
 * @param move a move to print
 */
void print_move( const struct Move * move )
{
    //printf( "(%d,%d),(%d,%d)", 
    //            move->start_row,
    //            move->start_col,
    //            move->end_row,
    //            move->end_col );
    
    char * human_readable = move2str( move );

    printf( ": %s", human_readable );

    Free( human_readable, sizeof( char ) * STR_LEN );
}

/**
 * Print first/second move
 *
 * @param move a move to print
 */
void print_single_move( const struct Move * move )
{
    char * human_readable = first_move2str( move );

    //printf( "(%d,%d)", 
    //            move->start_row,
    //            move->start_col );

    printf( ": %s", human_readable );

    Free( human_readable, sizeof( char ) * STR_LEN );
}

/**
 * Clone a move
 *
 * @param move a move to clone
 * @return a duplicate move
 */
struct Move * clone_move( const struct Move * move )
{
    return create_move( 
            move->start_row, 
            move->start_col, 
            move->end_row, 
            move->end_col );
}
