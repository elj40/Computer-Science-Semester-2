/*   Filename:       display.c
*    Date:           2024/09/21
*    Name:           Joubert E.L.
*    Student number: 28178564
*    ------------------------------------------------------
*    File that displays the map that is stored in a 2D array of chars
*    ------------------------------------------------------
*
*/

// Display the map in a temrinal board fashion, with numbers so it is easy to read
// This is the format:
//
//    000 001 002 003 004 005 006 007 008 009
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 009|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 008|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 007|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 006|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 005|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 004|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 003|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 002|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 001|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */
/* 000|   |   |   |   |   |   |   |   |   |   | */
/*    +---+---+---+---+---+---+---+---+---+---+ */

#include <stdio.h>
#include "display.h"
#include "types.h"
#include "map.h"

void display_map(Map *map) {
	char char_map[map->map_size][map->map_size];
	
	for (int i = 0 ; i < map->map_size; i++) {
		for (int j = 0 ; j < map->map_size; j++) {
			char_map[i][j] = map->map[i][j].display_char;
		}
	}

	display_char_map(&char_map[0][0], map->map_size);
}

void display_char_map(char *map, int s) {


	//Print first line that displays columns
	printf("    "); //3 empty spaces for top left corner
	for (int i = 0 ; i < s; i++) {
		printf("%03d ", i);
	}
	printf("\n");
	

	for (int i = s-1; i >= 0; i--) {
		print_horz_border(s);
		printf("\n");
		print_horz_row(map, i,s);
		printf("\n");
	}
	
	//Print horizontal border once before
	print_horz_border(s);
	printf("\n");

}

void print_horz_border(int s) {
	//Prints the a horizontal border between cells
	//Input: size of map
	//Output: none
	printf("   "); //3 left padding
	for (int i = 0; i < s; i++) {
		printf("+---");
	}
	printf("+");
}

void print_horz_row(char *map, int r, int s) {
	//Prints the a horizontal border between cells
	//Input: 
	//	r: current row
	//	s: size of map
	//Output: none
	printf("%03d", r); 
	for (int i = 0; i < s; i++) {
		int index = r*MAX_MAP_SIZE + i;
		char c = *(map + index);
		printf("| %c ", c);
	}
	printf("|");
}
