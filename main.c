//PART 1
//
//Generate a read_map functions that will read inputs and then construct objects based on those inputs
//Idea for testing: dont implement api yet, just have a counter that checks if it is behaving as expected
//	
//Make a test_compass function that gives the next location of an object by passing in speed and direction

/* Board objects¶ */
/* Each line after the first line will begin to describe the configuration of a single map object, which can be a flower, or a hive. Your program must validate the setup of each board object, and report the following error message if there are any violations: */


/* ERROR: Invalid object setup on line <line> */
/* Where <line> is the line number in the configuration file where the setup is invalid. If the setup is invalid due to abruptly reaching the end of the file, the last line number of the file should be reported using the error message above. */

/* If an attempt is made to place an object (a flower or a hive) on a position that is already occupied, the appropriate error message from the following must be reported: */

/* ERROR: Cannot place flower at already occupied location (<row>, <col>) */

/* ERROR: Cannot place hive at already occupied location (<row>, <col>) */

/* Where <row> and <col> are the row and column of the location that is already occupied. */

/* Flower¶ */
/* A flower is a map element that contains pollen which bees can pick up. The input for a flower consists of a line describing the flowers configuration, followed by n lines, describing each pollen granule. The flower configuration line consists of the following, separated by a single space character: */

/* The character 'F', denoting a flower placement. */

/* The x coordinate of the flower (integer). */

/* The y coordinate of the flower (integer). */

/* The number (n) of pollen granules the flower contains (integer). The line describing the flower will be followed by n lines containing the information for each pollen granule in the flower. */

/* An example of a flower input is as follows: */


/* F 20 50 2 */
/* Hello, */
/* world! */
/* which describes a flower at x=20, y=50, with 2 pollen granules, one containing the string ‘Hello,‘ and the other containing the string ‘world!’ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "validate.h"
#include "utils.h"
#include "display.h"

#define CONFIG_LINE_LEN 50
#define CONFIG_OPTIONS_LEN 4
#define POLLEN_MAX_CHARS 50

void clear_map(char *map, int s);
void read_map(char *map, int pollen_type, int beehive_action);
void invalid_object_setup(int l);

int main(int argc, char* argv[]) {

	int gui_mode;
	if (!validate_gui_mode(argc, argv, &gui_mode))
		return 0;

	char config_line[CONFIG_LINE_LEN];
	if (fgets(config_line, CONFIG_LINE_LEN, stdin) == NULL)
		return 0;


	//TODO: convert pollen_type and beehive_action to enums
	int map_size, duration, pollen_type, beehive_action;


	if (!validate_config_line(config_line, CONFIG_LINE_LEN, &map_size, &duration, &pollen_type, &beehive_action)) {
		printf("ERROR: Invalid configuration line\n");
		return 0;
	}


	char map[map_size][map_size];

	clear_map(&map[0][0], map_size);
	read_map(&map[0][0], pollen_type, beehive_action);

	/* display_map(&map[0][0], map_size); */
	return 0;
}

void read_map(char *map, int pollen_type, int beehive_action) {
	int current_line = 2;
	char line[CONFIG_LINE_LEN];

	while (fgets(line, CONFIG_LINE_LEN, stdin) != NULL) {
		char *tokens[CONFIG_OPTIONS_LEN];
		split_string(tokens, CONFIG_LINE_LEN, line, " \n");

		char *object = tokens[0];

		if (strcmp(object, "F") == 0) {
			
			int x, y, n;
			if (!string_to_int(&x, tokens[1])) invalid_object_setup(current_line);
			if (!string_to_int(&y, tokens[2])) invalid_object_setup(current_line);
			if (!string_to_int(&n, tokens[3])) invalid_object_setup(current_line);

			printf("Making flower at %d, %d with %d pollen\n", x, y, n);

			for (int i = 0; i < n; i++) {

				current_line++;

				char pollen_value[POLLEN_MAX_CHARS];
				if (fgets(pollen_value, POLLEN_MAX_CHARS, stdin) == NULL) {
					invalid_object_setup(current_line);
				}
				
				if 	(pollen_type == 0) printf("Making float pollen: %s\n", pollen_value);
				else if (pollen_type == 1) printf("Making string pollen: %s\n", pollen_value);
				
			}


		}
		else if (strcmp(object, "B") == 0) {}
		else invalid_object_setup(current_line);

		printf("# %s\n", tokens[0]);
		current_line++;
	}

}

void invalid_object_setup(int l) {
	printf("ERROR: Invalid object setup on line %d", l);
	exit(0);
}

/* bool validate_flower(char *map, char *tokens[], int cl, int pt) { */
/* 	int x, y, n; */
/* 	if (!string_to_int(&x, tokens[1])) return false; */
/* 	if (!string_to_int(&y, tokens[2])) return false; */
/* 	if (!string_to_int(&n, tokens[3])) return false; */

/* 	for (int i = 0; i < n; i++) { */
/* 		char *pollen_value; */
/* 		if (fgets(pollen_value, POLLEN_MAX_CHARS, stdin) == NULL) return false; */
/* 	} */
/* } */

void clear_map(char *map, int s) {
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			int index = i*s + j;
			*(map + index) = ' ';
		}
	}
}
