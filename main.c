//PART 1
//
//Generate a read_map functions that will read inputs and then construct objects based on those inputs
//Idea for testing: dont implement api yet, just have a counter that checks if it is behaving as expected
//
//Make a test_compass function that gives the next location of an object by passing in speed and direction
//
//Print board using their format:
//    000 001 002 003 004 005 006 007 008 009
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 009|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 008|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 007|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 006|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 005|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 004|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 003|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 002|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 001|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
/* 000|   |   |   |   |   |   |   |   |   |   | */
   /* +---+---+---+---+---+---+---+---+---+---+ */
#include <stdio.h>

#include "display.h"

void clear_map(char *map, int s);
int main() {
	printf("Hello world\n");

	int map_size = 10;
	char map_chars[map_size][map_size];

	clear_map(&map_chars[0][0], map_size);
	display_map(&map_chars[0][0], map_size);
	return 0;
}

void clear_map(char *map, int s) {
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			int index = i*s + j;
			*(map + index) = ' ';
		}
	}
}

