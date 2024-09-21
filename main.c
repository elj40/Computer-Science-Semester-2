//PART 1
//
//Generate a read_map functions that will read inputs and then construct objects based on those inputs
//Idea for testing: dont implement api yet, just have a counter that checks if it is behaving as expected
//	
// Format of config line (seperatde by spaces):
//n 		(size of map: int)
//duration	(number of iterations: int)
//pollen type [s or f] (denotes if pollen is strings or floats: char)
//pollen action [max,min,sum,sort] (function to summarise pollen in beehive, onyl sort for strings: str)
//e.g. 100 50 f sum
//if not matching format: ERROR: Invalid configuration line
//
//
//Make a test_compass function that gives the next location of an object by passing in speed and direction
//
//
//


#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "validate.h"
#include "display.h"

#define CONFIG_LINE_LEN 50
#define CONFIG_OPTIONS_LEN 4
#define MAP_SIZE_DIGITS 5
#define DURATION_DIGITS 5

void clear_map(char *map, int s);

int main(int argc, char* argv[]) {

	int gui_mode;

	if (!validate_gui_mode(argc, argv, &gui_mode))
		return 0;

	char config_line[CONFIG_LINE_LEN];
	fgets(config_line, CONFIG_LINE_LEN, stdin);


	//TODO: convert pollen_type and beehive_action to enums
	int map_size, duration, pollen_type, beehive_action;


	if (!validate_config_line(config_line, CONFIG_LINE_LEN, &map_size, &duration, &pollen_type, &beehive_action)) {
		printf("ERROR: Invalid configuration line\n");
		return 0;
	}


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
