#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "validate.h"
#include "utils.h"

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

		


bool validate_config_line(char *config_line, int config_len, int *s, int *d, int *t, int *a) {
	// Format of config line (seperatde by spaces):
	//n 		(size of map: int)
	//duration	(number of iterations: int)
	//pollen type [s or f] (denotes if pollen is strings or floats: char)
	//pollen action [max,min,sum,sort] (function to summarise pollen in beehive, onyl sort for strings: str)
	//e.g. 100 50 f sum
	//if not matching format: ERROR: Invalid configuration line

	char *config_tokens[config_len];
	const char *delimiter = " \n";

	split_string(config_tokens, config_len, config_line, delimiter);

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
