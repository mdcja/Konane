/**
 * @file utility.c
 * @brief this file implements utility.h
 * @author Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
 */
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

static unsigned long _memory_usage = 0;

/**
 * setup_board
 *
 * This funciton reads a file an places each character
 * read into the global board
 *
 * @param string for the filename
 * @return initialized global board state
 */
void setup_board(char *filename, char board[][SIZE])
{
  /* open file */
  FILE *fh = fopen(filename,"r");
  if(fh == NULL){
    printf("could not open file %s\n",filename);
    exit(EXIT_FAILURE);
  }

  /* iterate through file to fill initial board state */
  char c;
  int x = 0, y = 0;
  while((c = fgetc(fh)) != EOF){
    if(c == '\n'){
      x++;
      y = 0;
      continue;
    }
    board[x][y] = c;
    y++;
  }
  
  fclose(fh);
}


/**
 * Calloc
 *
 * This funciton is a wrapper to calloc. It checks that memory was
 *  properly allocated, and tracks memory usage
 *
 * @param the number of elements in the array
 * @param size the size of the element
 * @return a pointer to the allocated memory
 */
void * Calloc( size_t nmemb, size_t size )
{
  _memory_usage += (nmemb * size);

  return calloc( nmemb, size );
}

/**
 * Free memory
 *
 * Frees memory, and decreases the memory usage tracked accordingly
 *
 * @param ptr to memory to free
 * @param size the size of the memory to free
 */
void Free( void * ptr, size_t size )
{
    /* WARNING! UGLY HACK! TRACK DOWN ACTUAL PROBLEM
     * to stop negative sizes check if subtracting size
     * from memory would result in negative value
     */
    if( _memory_usage < size )
        _memory_usage = 0;
    else
        _memory_usage -= size;

  free( ptr );
}

/**
 * Get the memory used by the program
 *
 * @return the memory used, in bytes
 */
unsigned long memory_usage( void )
{
  return _memory_usage;
}
