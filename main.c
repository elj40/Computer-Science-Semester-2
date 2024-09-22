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

void clear_map(struct Map *map);
void read_map(struct Map *map, struct Config c);
void invalid_object_setup(int l);
void add_pollen(struct Flower *f, union Pollen p);

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


	/* char map_chars[map_size][map_size]; */

	clear_map(&map);
	printf("Reading map\n");
	read_map(&map, config);

	display_map((char *)map.map, map.map_size);

	return 0;
}

void add_hive(struct Map *m, struct Hive h) {
	printf("Adding hive-> row: %3d, col: %3d\n", h.row, h.col);
	m->map[h.row][h.col] = h.type;
	
	if (m->hive_len < MAX_MAP_SIZE*MAX_MAP_SIZE) {
		m->hives[m->hive_len] = h;
		m->hive_len++;
	}else {
		printf("ERROR: overflow of hives, could not add latest hive\n");
	}
}


void add_flower(struct Map *m, struct Flower f) {
	printf("Adding flower-> row: %3d, col: %3d, p_type: %d\n", f.row, f.col, f.pollen_type);
	m->map[f.row][f.col] = 'F';
	if (m->flower_len < MAX_MAP_SIZE*MAX_MAP_SIZE) {
		m->flowers[m->flower_len] = f;
		m->flower_len++;
	}else {
		printf("ERROR: overflow of flowers, could not add latest flower\n");
	}
}

void add_bee(struct Hive *h, struct Bee b) {
	printf("Adding bee-> row: %3d, col: %3d, speed: %3d, percep: %3d\n", b.row, b.col, b.speed, b.perception);
	union Pollinator p = { .bee = b };
	if (h->pollinator_len < MAX_HIVE_POLLINATORS) {
		h->pollinators[h->pollinator_len] = p;
		h->pollinator_len++;
	}else {
		printf("ERROR: overflow of pollinators, could not add latest pollinator\n");
	}
}


void add_wasp(struct Hive *h, struct Wasp w) {
	printf("Adding wasp-> row: %3d, col: %3d, speed: %3d\n", w.row, w.col, w.speed);
	union Pollinator p = { .wasp = w };
	if (h->pollinator_len < MAX_HIVE_POLLINATORS) {
		h->pollinators[h->pollinator_len] = p;
		h->pollinator_len++;
	}else {
		printf("ERROR: overflow of pollinators, could not add latest pollinator\n");
	}
}

void add_pollen(struct Flower *f, union Pollen p) {
	if (f->pollen_type == 0) 
		printf("Adding pollen-> info_f: %f\n", p.float_info);
	else if (f->pollen_type == 1)
		printf("Adding pollen-> info_s: %s\n", p.string_info);
	if (f->pollen_len < MAX_HIVE_POLLINATORS) {
		f->pollen[f->pollen_len] = p;
		f->pollen_len++;
	}else {
		printf("ERROR: overflow of pollens, could not add latest pollen\n");
	}
}


void read_map(struct Map *map, struct Config c) {
	int current_line = 2;
	char line[CONFIG_LINE_LEN];

	while (fgets(line, CONFIG_LINE_LEN, stdin) != NULL) {

		char *tokens[CONFIG_OPTIONS_LEN];
		split_string(tokens, CONFIG_LINE_LEN, line, " \n");

		char *object_tok = tokens[0];
		int object_len = strlen(object_tok);
		if (object_len != 1) invalid_object_setup(current_line);

		char object = object_tok[0];

		int x, y, n; 
		if (!string_to_int(&x, tokens[1])) invalid_object_setup(current_line);
		if (!string_to_int(&y, tokens[2])) invalid_object_setup(current_line);
		if (!string_to_int(&n, tokens[3])) invalid_object_setup(current_line);

		if (x >= map->map_size) invalid_object_setup(current_line);
		if (y >= map->map_size) invalid_object_setup(current_line);

		if (map->map[y][x] != ' ') {
			if (object == 'F') {
				printf("ERROR: Cannot place flower at already occupied location (%d, %d)\n", y, x);
				exit(0); 
			} else if (object == 'B' || object == 'H' || object == 'D' || object == 'W') {
				printf("ERROR: Cannot place hive at already occupied location (%d, %d)\n", y, x);
				exit(0); 
			}
		}

		int speed;
		int perception;
		if (object == 'F') {
			// Do some weird semicolons because of the switch case
			struct Flower flower = { .row = y, .col = x, .pollen_len = 0, .pollen_type = c.pollen_type };

			for (int i = 0; i < n; i++) {
				current_line++;

				char pollen_value[POLLEN_MAX_CHARS];
				if (fgets(pollen_value, POLLEN_MAX_CHARS, stdin) == NULL) {
					invalid_object_setup(current_line);
				}

				//Get rid of newline at end of pollen_value
				char *pollen_v = strtok(pollen_value, "\n");

				if (c.pollen_type == FLOAT) {
					union Pollen pollen;
					if (!string_to_float(&pollen.float_info, pollen_v)) 
						invalid_object_setup(current_line);
					add_pollen(&flower, pollen);
				}
				else if (c.pollen_type == STRING) {
					union Pollen pollen = { .string_info = pollen_v };
					add_pollen(&flower, pollen);
				}


			}
			add_flower(map, flower);
		}
		else if (object == 'B' || object == 'H' || object == 'D') {
			struct Hive hive = { .row = y, .col = x, .type=object, .pollinator_len = 0};

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
				struct Bee bee;
				bee.row = hive.row;
				bee.col = hive.col;
				bee.speed = speed;
				bee.perception = perception;
				add_bee(&hive, bee);
			}
			add_hive(map, hive);
		}
		else if (object == 'W') {
			struct Hive hive = { .row = y, .col = x, .type=object, .pollinator_len = 0};
			char wasp_line[BEE_MAX_CHARS];
			if (fgets(wasp_line, BEE_MAX_CHARS, stdin) == NULL) {
				invalid_object_setup(current_line);
			}

			current_line++;

			char *wasp_tokens[2];
			split_string(wasp_tokens, 2, wasp_line, " \n");

			if (!string_to_int(&speed, wasp_tokens[0])) invalid_object_setup(current_line);

			for (int i = 0; i < n; i++) {
				struct Wasp wasp = { .row = hive.row, .col = hive.col, .speed = speed };
				add_wasp(&hive, wasp);
			}
			add_hive(map, hive);
		}
		else {
			invalid_object_setup(current_line);
		}



		current_line++;
	}

}

void invalid_object_setup(int l) {
	printf("ERROR: Invalid object setup on line %d", l);
	exit(0);
}


void clear_map(struct Map *map) {
	int s = map->map_size;
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			map->map[i][j] = ' ';
		}
	}
}
