
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "validate.h"
#include "map.h"
#include "utils.h"
#include "display.h"
#include "update.h"
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
	printf("Finished reading map....\n");
	display_map((char *)map.map, map.map_size);
	printf("\n\n");

	for (int iter = 0; iter < config.duration; iter++) {
		map_update(&map, &config);
		display_map((char *)map.map, map.map_size);
	}


	return 0;
}




