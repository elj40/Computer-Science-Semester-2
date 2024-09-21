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

void display_map(char *map, int s);
void print_horz_border(int s);
void print_horz_row(char *map, int r, int s);
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

void display_map(char *map, int s) {
	// Display the map in a temrinal board fashion, with numbers so it is easy to read
	// This is the format:
	//
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


	//Print first line that displays columns
	printf("   "); //3 empty spaces for top left corner
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
		printf("| %c ", *(map + r*s + i));
	}
	printf("|");
}
