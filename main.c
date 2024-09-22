//PART 1
//
//Generate a read_map functions that will read inputs and then construct objects based on those inputs
//Idea for testing: dont implement api yet, just have a counter that checks if it is behaving as expected
//	
//Make a test_compass function that gives the next location of an object by passing in speed and direction

/* Board objects¶ */
/* Each line after the first line will begin to describe the configuration of a single map object, which can be a flower, or a hive. Your program must validate the setup of each board object, and report the following error message if there are any violations: */

/* If an attempt is made to place an object (a flower or a hive) on a position that is already occupied, the appropriate error message from the following must be reported: */

/* ERROR: Cannot place flower at already occupied location (<row>, <col>) */

/* ERROR: Cannot place hive at already occupied location (<row>, <col>) */

/* Where <row> and <col> are the row and column of the location that is already occupied. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "validate.h"
#include "map.h"
#include "utils.h"
#include "display.h"
#include "types.h"


int main(int argc, char* argv[]) {

	int gui_mode;
	if (!validate_gui_mode(argc, argv, &gui_mode))
		return 0;

	char config_line[CONFIG_LINE_LEN];
	if (fgets(config_line, CONFIG_LINE_LEN, stdin) == NULL)
		return 0;

	int map_size, duration; 
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;

	struct Map map;
	struct Config config;


	if (!validate_config_line(config_line, CONFIG_LINE_LEN, &map.map_size, &config)) {
		printf("ERROR: Invalid configuration line\n");
		return 0;
	}


	clear_map(&map);
	read_map(&map, config);
	display_map((char *)map.map, map.map_size);


	return 0;
}




