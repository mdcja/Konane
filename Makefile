# Makefile for Konane
# 
# author: Eric Watkins, Julian Martinez del Campo, Michael Hnatiw
CC= gcc
CFLAGS= -Wall -g -pedantic -std=c99
LDFLAGS=

all: main

konane.o: konane.h state.h move.h list.h game_node.h
list.o: list.h utility.h
move.o: move.h utility.h
state.o: state.h konane.h utility.h move.h
game_node.o: game_node.h list.h state.h utility.h move.h
utility.o: utility.h

game.o: game.h game_node.h move.h state.h konane.h utility.h
game: game.o game_node.o move.o state.o konane.o utility.o list.o

main.o: game.h
main: main.o game.o game_node.o move.o state.o konane.o utility.o list.o

clean:
	$(RM) *.o *~ *#
	$(RM) main game 
