/**
 * @brief Provides an implementation of the game of konane
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

#include "game.h"
#include "game_node.h"
#include "move.h"
#include "state.h"
#include "konane.h"
#include "utility.h"

#define INPUT_SIZE  20 

/** Global timer */
time_t timer;

/**
 * Start playing a game of konane
 */
int game( char *file, char agent_color )
{
    char input[ INPUT_SIZE ];
    agent_color = toupper( agent_color );

    if( agent_color != 'B' && agent_color != 'W' )
    {
        /* human or computer player */
        printf( "Play konane:\n" );
        printf( " a) play computer\n" );
        printf( " b) computer vs computer\n" );
        char option[ INPUT_SIZE ];
        do
        {
            printf( "Please choose an option: " );
            fgets( option, INPUT_SIZE, stdin );
        }
        while( option[ 0 ] != 'a' && option[ 0 ] != 'b' );


        if( option[0] == 'a' )
        {
            /* choose player */
            do 
            {
                printf( "Please choose a player ('B' or 'W' ): " );
                fgets( input, INPUT_SIZE, stdin );
            }
            while( input[ 0 ] != 'W' && input[ 0 ] != 'B' );
            char player = input[0];
            human_vs_computer(file, opposite_player( player ));
        }
        else
        {
            computer_vs_computer(file, option[ 0]);
        }
    }
    else
    {
        human_vs_computer( file, agent_color );
    }

    return 1;
}

/**
 * Human vs compter
 *
 * Plays vs the computer
 * @param string for filename
 * @param agent_color the agent color
 * @return 1 if human player won, else return 0
 */
int human_vs_computer( char *file, char agent_color )
{
    char board[ SIZE ][ SIZE ];
    char player = toupper( agent_color );

    /* set up board */
    setup_board(file,board);


    /* create a new state */
    struct State * state = new_state( board, 'B' );

    print_state( state );

    /* START GAME */

    /* start game ( B first ) */
    struct State * temp_state;
    if( player == state->player )
    {
        temp_state = computer_player_first( state );
        Free( state, sizeof( struct State ) );
        state = temp_state;
    }
    else
    {
        temp_state = human_player_first( state );
        Free( state, sizeof( struct State ) );
        state = temp_state;
    }

    /* second move */
    printf( "\n" );
    print_state( state );
    if( player == state->player )
    {
        temp_state = computer_player_second( state );
        Free( state, sizeof( struct State ) );
        state = temp_state;
    }
    else
    {
        temp_state = human_player_second( state );
        Free( state, sizeof( struct State ) );
        state = temp_state;
    }

    /* regular game */
    for( ;; )
    {
        printf( "\n" );
        print_state( state );

        if( state->player == player )
        {
            temp_state = computer_player( state );
            Free( state, sizeof( struct State ) );
            state = temp_state;
        }
        else
        {
            temp_state = human_player( state );
            Free( state, sizeof( struct State ) );
            state = temp_state;
        }

        if( terminal_test( state ) == 1 )
        {
            printf( "\n\n%c wins!!!\n", opposite_player( state->player ) );
            break;
        }
    }

    return player == opposite_player( state->player );
}

/**
 * Computer vs computer
 *
 * The computer plays itself
 * @param string for filename
 * @param agent_color
 * @return 1 if black wins, else return 0
 */
int computer_vs_computer( char *file, char agent_color )
{
    char board[ SIZE ][ SIZE ];

    /* set up board */
    setup_board(file,board);

    /* create a new state */
    struct State * state = new_state( board, 'B' );
    struct State * temp_state;

    /* start game */
    print_state( state );
    temp_state = computer_player_first( state );
    Free( state, sizeof( struct State ) );
    state = temp_state;

    /* second move */
    printf( "\n" );
    print_state( state );
    temp_state = computer_player_second( state );
    Free( state, sizeof( struct State ) );
    state = temp_state;

    /* regular game */
    for( ;; )
    {
        printf( "\n" );
        print_state( state );

        //printf( "\n>> Mem usage before: %lu\n", memory_usage() );
        temp_state = computer_player( state );
        Free( state, sizeof( struct State ) );
        state = temp_state;
        //printf( "\n>> Mem usage after: %lu\n", memory_usage() );

        if( terminal_test( state ) == 1 )
        {
            printf( "\n" );
            print_state( state );
            printf( "\nNo moves left!... \n" );
            printf( "\n%c wins!!!\n", opposite_player( state->player ) );
            Free( state, sizeof( struct State ) );
            break;
        }
    }

    return opposite_player( state->player ) == 'B';
}

/**
 * Human player goes first
 *
 * @param game_state the current state of the game
 * @return a new game state
 */
struct State * human_player_first( struct State * game_state )
{
    char input[ INPUT_SIZE ];
    struct State * state;
    struct Move * move;

    printf( "To begin game, please remove one piece from the board.\n" );
    printf( "Valid moves are from the 4 center squares, or one of the corners.\n" );
    printf( "Piece removed must be a piece of your color!\n" );

    /* get input */
    do 
    {
        printf( "Please enter a move: " );
        fgets( input, INPUT_SIZE, stdin );

        move = first_str2move( input );

        /* make sure piece is of players color */
        if( game_state->board[ move->start_row ][ move->start_col ] != 'B' )
        {
            /* invalid move */
            Free( move, sizeof( struct Move ) );
        }
    }
    while( move == NULL );

    /* create a new state */
    state = new_state( game_state->board, opposite_player( game_state->player ) );

    /* apply move */
    state->board[ move->start_row ][ move->start_col ] = 'O';
    print_single_move( move );

    return state;
}

/**
 * Human player goes second
 *
 * @param game_state the current state of the game
 * @return a new game state
 */
struct State * human_player_second( struct State * game_state )
{
    char input[ INPUT_SIZE ];
    struct State * state;
    struct Move * move;

    /* must remove piece orthoganally adjacent */

    printf( "Please remove a piece of your color adjacent to the piece removed\n" );

    /* get input */
    do 
    {
        printf( "Please enter a move: " );
        fgets( input, INPUT_SIZE, stdin );

        move = first_str2move( input );

        /* make sure piece is valid */
        if( move != NULL )
        {
            if( validate_second_in_move( game_state, move ) == 0 )
            {
                /* invalid move */
                Free( move, sizeof( struct Move ) );
            }
            else
                break;
        }
    }
    while( 1 );

    printf( "Move chosen: " );
    print_single_move( move );

    /* create a new state */
    state = new_state( game_state->board, opposite_player( game_state->player ) );

    /* apply move */
    state->board[ move->start_row ][ move->start_col ] = 'O';

    return state;
    
}

/**
 * Computer player goes first
 *
 * @param game_state the current state of the game
 * @return a new game state
 */
struct State * computer_player_first( struct State * game_state )
{
    struct State * state;
    struct Move * move;

    srand( time( NULL ) );

    do
    {
        /* determine a move randomly */
        int random_move = rand() % 8;

        switch( random_move )
        {
        case 0:
            /* center top left */
            move = create_move( 3, 3, 0, 0 );
            break;
        case 1:
            /* center top right */
            move = create_move( 3, 4, 0, 0 );
            break;
        case 2:
            /* center botom left */
            move = create_move( 4, 3, 0, 0 );
            break;
        case 3:
            /* center bottom right */
            move = create_move( 4, 4, 0, 0 );
            break;
#if 1
        case 4:
            /* corner top left */
            move = create_move( 0, 0, 0, 0 );
            break;
        case 5:
            /* corner top right */
            move = create_move( 0, 7, 0, 0 );
            break;
        case 6:
            /* corner bottom left */
            move = create_move( 7, 0, 0, 0 );
            break;
        case 7:
            /* corner bottom right */
            move = create_move( 7, 7, 0, 0 );
            break;
#endif
        }

        /* check if move is valid */
        if( game_state->board[ move->start_row ][ move->start_col ] == 'B' )
        {
            break;
        }
        else
        {
            Free( move, sizeof( struct Move ) );
        }
    }
    while( 1 );

    /* print move */
    printf( "Move chosen: " );
    print_single_move( move );

    /* create new state */
    state = new_state( game_state->board, opposite_player( game_state->player ) );

    /* apply move */
    state->board[ move->start_row ][ move->start_col ] = 'O';

    Free( move, sizeof( struct Move ) );

    return state;

}

/**
 * Computer player goes second
 *
 * @param game_state the current game state
 * @return a new state
 */
struct State * computer_player_second( struct State * game_state )
{
    struct State * state;
    struct Move * move;
    int random_move = 0;
    int row, col, i, j;

    /* find blank spot */
    for( row = 0; row < SIZE; row++ )
        for( col = 0; col < SIZE; col++ )
            if( game_state->board[row][col] == 'O' )
            {
                i = row;
                j = col;
                break;
            }

    srand( time( NULL ) );

    /* select piece */
    /* valid moves are on either side of empty space */
    if( i == 0 && j == 0 )
    {
        if( ( game_state->board[ i ][ j + 1 ] == 'W' ) ||
            ( game_state->board[ i + 1][ j ] == 'W' ) )
        {
            random_move = rand() % 2;

            switch( random_move )
            {
            case 0:
                move = create_move( i, j+1, 0, 0 );
                break;
            case 1:
                move = create_move( i+1, j, 0, 0 );
                break;
            }
        }

    }
    else if( i == 0 && j == 7 )
    {
        if( (game_state->board[ i ][ j - 1 ] == 'W' ) ||
            (game_state->board[ i + 1 ][ j ] == 'W' ) )
        {
            random_move = rand() % 2;

            switch( random_move )
            {
            case 0:
                move = create_move( i, j-1, 0, 0 );
                break;
            case 1:
                move = create_move( i+1, j, 0, 0 );
                break;
            }
        }
    }
    else if( i == 7 && j == 0 )
    {
        if( (game_state->board[ i ][ j + 1 ] == 'W' ) ||
            (game_state->board[ i - 1 ][ j ] == 'W' ) )
        {
            random_move = rand() % 2;

            switch( random_move )
            {
            case 0:
                move = create_move( i, j+1, 0, 0 );
                break;
            case 1:
                move = create_move( i-1, j, 0, 0 );
                break;
            }
        }
    }
    else if( i == 7 && j == 7 )
    {
        if( (game_state->board[ i ][ j - 1 ] == 'W' ) ||
            (game_state->board[ i - 1 ][ j ] == 'W' ) )
        {
            random_move = rand() % 2;

            switch( random_move )
            {
            case 0:
                move = create_move( i, j-1, 0, 0 );
                break;
            case 1:
                move = create_move( i-1, j, 0, 0 );
                break;
            }
        }
    }
    else
    {
        if( (game_state->board[ i + 1 ][ j ] == 'W' ) ||
            (game_state->board[ i - 1 ][ j ] == 'W' ) ||
            (game_state->board[ i ][ j + 1 ] == 'W' ) ||
            (game_state->board[ i ][ j - 1 ] == 'W' ) )
        {
            random_move = rand() % 4;

            switch( random_move )
            {
            case 0:
                move = create_move( i+1, j, 0, 0 );
                break;
            case 1:
                move = create_move( i-1, j, 0, 0 );
                break;
            case 2:
                move = create_move( i, j+1, 0, 0 );
                break;
            case 3:
                move = create_move( i, j-1, 0, 0 );
                break;
            }
        }
    }

    /* print move */
    printf( "Move chosen: " );
    print_single_move( move );

    /* create new state */
    state = new_state( game_state->board, opposite_player( game_state->player ) );

    /* apply move */
    state->board[ move->start_row ][ move->start_col ] = 'O';
    Free( move, sizeof( struct Move ) );

    return state;
}

/**
 * Human player's turn
 *
 * @param game_state a game state
 * @return a new game state
 */
struct State * human_player( struct State * game_state )
{
    char input[ INPUT_SIZE ];
    struct State * state;
    struct Move * move;

    /* get input */
    do 
    {
        printf( "Please enter a move: " );
        fgets( input, INPUT_SIZE, stdin );

        move = str2move( input );

        if( move != NULL )
        {
            if( validate_action( game_state, move ) == 1 )
                break;
            else
            {
                Free( move, sizeof( struct Move ) );
                move = NULL;
            }
        }
    }
    while( move == NULL );

    /* print move */
    printf( "Move chosen: " );
    print_move( move );

    /* create a new state & apply move */
    state = result( game_state, move );
    Free( move, sizeof( struct Move ) );
    state->player = opposite_player( game_state->player );

    return state;

}

/**
 * Computer player moves
 *
 * @param game_state the game state
 * @return a new game state
 */
struct State * computer_player( struct State * game_state )
{
    struct State * state;
    struct Move * move;
    struct GameNode * root;
    time_t stop;

    /* create a new game node */
    root = new_game_node( game_state, NULL );

    /* computer player */
    time( &timer );
    move = alpha_beta_search( root );
    time( &stop );

    /* print time */
    printf( "Time taken: %d\n", stop - timer );
    printf( "Memory used: %lu\n", memory_usage() );

    /* print move */
    printf( "Move chosen: " );
    print_move( move );

    /* create a new state and apply move */
    state = result( game_state, move );
    Free( move, sizeof( struct Move ) );
    state->player = opposite_player( game_state->player );

    delete_game_node( &root );

    return state;
}
