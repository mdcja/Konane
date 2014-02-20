/**
 * @brief Play a game of konane
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 */
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main( int argc, char * argv[] )
{
    if( argc != 3 )
    {
        printf( "%s usage: <input file> <player color>\n", argv[ 0 ] );
        printf( "   input file - a text file consisting of a konane board\n" );
        printf( "   player color - a single character B, W which indicates the \n" );
        printf( "       role the agent assumes. If player color is not equal \n" );
        printf( "       to b or w, then game enters interactive mode\n" );
        return EXIT_FAILURE;
    }

    char * str = argv[ 2 ];
    game( argv[ 1 ], str[ 0 ] );
    
  return EXIT_SUCCESS;
}
