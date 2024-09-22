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
#include "utils.h"
#include "display.h"
#include "types.h"

#define CONFIG_LINE_LEN 50
#define CONFIG_OPTIONS_LEN 4
#define POLLEN_MAX_CHARS 50
#define BEE_MAX_CHARS 50

void clear_map(char *map, int s);
void read_map(char *map, int map_size, enum PollenType pollen_type, enum BeehiveAction beehive_action);
void invalid_object_setup(int l);

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

	if (!validate_config_line(config_line, CONFIG_LINE_LEN, &map_size, &duration, &pollen_type, &beehive_action)) {
		printf("ERROR: Invalid configuration line\n");
		return 0;
	}

	printf("pollen type %d\n", pollen_type);

	char map[map_size][map_size];

	clear_map(&map[0][0], map_size);
	read_map(&map[0][0], map_size, pollen_type, beehive_action);

	/* display_map(&map[0][0], map_size); */
	return 0;
}

void read_map(char *map, int map_size, enum PollenType pollen_type, enum BeehiveAction beehive_action) {
	int current_line = 2;
	char line[CONFIG_LINE_LEN];

	while (fgets(line, CONFIG_LINE_LEN, stdin) != NULL) {

		char *tokens[CONFIG_OPTIONS_LEN];
		split_string(tokens, CONFIG_LINE_LEN, line, " \n");

		char *object_tok = tokens[0];
		int object_len = strlen(object_tok);
		printf("Size of object string: %d, %s\n", object_len, object_tok);
		if (object_len != 1) invalid_object_setup(current_line);

		char object = object_tok[0];

		int x, y, n; 
		if (!string_to_int(&x, tokens[1])) invalid_object_setup(current_line);
		printf("x parsed correctly\n");
		if (!string_to_int(&y, tokens[2])) invalid_object_setup(current_line);
		printf("y parsed correctly\n");
		if (!string_to_int(&n, tokens[3])) invalid_object_setup(current_line);
		printf("n parsed correctly\n");

		if (*(map + y*map_size + x) != ' ') {
			printf("ERROR: Cannot place flower at already occupied location (%d, %d)\n", y, x);
			exit(0);
		}

		int speed;
		int perception;
		switch(object) 	 {
			case 'F':
				printf("Making flower at %d, %d with %d pollen\n", x, y, n);

				for (int i = 0; i < n; i++) {
					current_line++;

					char pollen_value[POLLEN_MAX_CHARS];
					if (fgets(pollen_value, POLLEN_MAX_CHARS, stdin) == NULL) {
						invalid_object_setup(current_line);
					}

					//Get rid of newline at end of pollen_value
					char *pollen_v = strtok(pollen_value, "\n");
					
					if 	(pollen_type == 0) printf("Making float pollen: %s\n", pollen_v);
					else if (pollen_type == 1) printf("Making string pollen: %s\n", pollen_v);
					
				}
				break;
			case 'B':
			case 'D':
			case 'H':
				printf("Making hive at %d, %d with %d bees\n", x, y, n);
				char bee_line[BEE_MAX_CHARS];
				if (fgets(bee_line, BEE_MAX_CHARS, stdin) == NULL) {
					invalid_object_setup(current_line);
				}

				current_line++;

				char *bee_tokens[2];
				split_string(bee_tokens, 2, bee_line, " \n");

				if (!string_to_int(&speed, bee_tokens[0])) invalid_object_setup(current_line);
				if (!string_to_int(&perception, bee_tokens[1])) invalid_object_setup(current_line);

				for (int i = 0; i < n; i++) {
					 printf("Making bee-> speed:%d, percpeption:%d\n", speed, perception);
				}
				break;
			case 'W':
				printf("Making wasp hive at %d, %d with %d wasps\n", x, y, n);
				char wasp_line[BEE_MAX_CHARS];
				if (fgets(wasp_line, BEE_MAX_CHARS, stdin) == NULL) {
					invalid_object_setup(current_line);
				}

				current_line++;

				char *wasp_tokens[2];
				split_string(wasp_tokens, 2, wasp_line, " \n");

				if (!string_to_int(&speed, wasp_tokens[0])) invalid_object_setup(current_line);

				for (int i = 0; i < n; i++) {
					 printf("Making wasp-> speed:%d\n", speed);
				}
				break;
			default:
				invalid_object_setup(current_line);
		}



		printf("# %s\n", tokens[0]);
		current_line++;
	}

}

void invalid_object_setup(int l) {
	printf("ERROR: Invalid object setup on line %d", l);
	exit(0);
}


void clear_map(char *map, int s) {
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			int index = i*s + j;
			*(map + index) = ' ';
		}
	}
}
