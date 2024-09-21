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
#include "display.h"

#define CONFIG_LINE_LEN 50
#define CONFIG_OPTIONS_LEN 4
#define MAP_SIZE_DIGITS 5
#define DURATION_DIGITS 5

void clear_map(char *map, int s);
bool validate_gui_mode(int argc,char* argv[], int *gm);
bool validate_config_line(char *config_line, int *s, int *d, int *t, int *a);
void split_string(char *buffer[], int buf_len, char *string, const char *delims);
bool string_to_int(int *n, char *s);

int main(int argc, char* argv[]) {

	int gui_mode;

	if (!validate_gui_mode(argc, argv, &gui_mode))
		return 0;

	char config_line[CONFIG_LINE_LEN];
	fgets(config_line, CONFIG_LINE_LEN, stdin);


	//TODO: convert pollen_type and beehive_action to enums
	int map_size, duration, pollen_type, beehive_action;


	if (!validate_config_line(config_line, &map_size, &duration, &pollen_type, &beehive_action)) {
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

bool validate_gui_mode(int argc,char* argv[],int *gm) {
	int correct_num_args = 2;
	if (argc < correct_num_args) {
		printf("ERROR: Too few arguments\n");
		return false;
	}
	if (argc > correct_num_args) {
		printf("ERROR: Too many arguments\n");
		return false;
	}

	char* gui_mode = argv[1];
	// For some reason strcmp checks if two string are not the same
	if (strcmp(gui_mode,"0") && strcmp(gui_mode, "1")) {
		printf("ERROR: Invalid argument: %s\n", gui_mode);
		return false;
	}

	*gm = argv[1][0] - '0';
	return true;
}

void split_string(char *buffer[], int buf_len, char *string, const char *delims) {
	char* tok = strtok(string, delims);
	int tc = 0;

	while (tok != NULL && tc < buf_len) {
		buffer[tc] = tok;
		tc++;
		tok = strtok(NULL, delims);
	}
}

bool string_to_int(int *n, char *s) {
	int i = 0;
	*n = 0;
	char c;
	while (1) {
		c = *(s+i);	
		if (c == '\0') break;

		bool is_digit = '0' <= c && c <= '9';
		if (!is_digit) return false;

		int digit = c - '0';
		*n *= 10;
		*n += digit;
		i++;
	}
	return true;
}
		


bool validate_config_line(char *config_line, int *s, int *d, int *t, int *a) {
	// Format of config line (seperatde by spaces):
	//n 		(size of map: int)
	//duration	(number of iterations: int)
	//pollen type [s or f] (denotes if pollen is strings or floats: char)
	//pollen action [max,min,sum,sort] (function to summarise pollen in beehive, onyl sort for strings: str)
	//e.g. 100 50 f sum
	//if not matching format: ERROR: Invalid configuration line

	char *config_tokens[CONFIG_OPTIONS_LEN];
	const char *delimiter = " \n";

	split_string(config_tokens, CONFIG_OPTIONS_LEN, config_line, delimiter);

	if (!string_to_int(s, config_tokens[0])) return false;
	if (!string_to_int(d, config_tokens[1])) return false;

	if 	(!strcmp("f", config_tokens[2])) *t = 0;
	else if (!strcmp("s", config_tokens[2])) *t = 1;
	else return false;

	bool is_max = strcmp("max", config_tokens[3]) == 0;
	bool is_min = strcmp("min", config_tokens[3]) == 0;
	bool is_sum = strcmp("sum", config_tokens[3]) == 0;
	bool is_sort = strcmp("sort", config_tokens[3]) == 0;

	if 	(is_max) *a = 0;
	else if (is_min) *a = 1;
	else if (is_sum ) *a = 2;
	else if (is_sort) *a = 3;
	else return false;
	
	return true;
}
