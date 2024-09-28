#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "utils.h"

#define INPUT_LINE_LEN 50
#define POLLEN_MAX_CHARS 50
#define BEE_MAX_CHARS 50

void clear_map( Map *map) {
	int s = map->map_size;
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			map->map[i][j] = ' ';
		}
	}
}

void add_hive( Map *m,  Hive h) {
	printf("Adding hive-> row: %3d, col: %3d\n", h.row, h.col);
	m->map[h.row][h.col] = h.type;
	
	if (m->hive_len < MAX_MAP_SIZE*MAX_MAP_SIZE) {
		m->hives[m->hive_len] = h;
		m->hive_len++;
	}else {
		printf("ERROR: overflow of hives, could not add latest hive\n");
	}
}


void add_flower( Map *m,  Flower f) {
	printf("Adding flower-> row: %3d, col: %3d, p_type: %d\n", f.row, f.col, f.pollen_type);
	m->map[f.row][f.col] = 'F';
	if (m->flower_len < MAX_MAP_SIZE*MAX_MAP_SIZE) {
		m->flowers[m->flower_len] = f;
		m->flower_len++;
	}else {
		printf("ERROR: overflow of flowers, could not add latest flower\n");
	}
}

void add_bee( Hive *h,  Bee b) {
	printf("Adding bee-> row: %3d, col: %3d, speed: %3d, percep: %3d\n", b.row, b.col, b.speed, b.perception);
	union Pollinator p = { .bee = b };
	if (h->pollinator_len < MAX_HIVE_POLLINATORS) {
		h->pollinators[h->pollinator_len] = p;
		h->pollinator_len++;
	}else {
		printf("ERROR: overflow of pollinators, could not add latest pollinator\n");
	}
	printf("Moving bee-> row: %3d, col: %3d, speed: %3d, percep: %3d\n",p.bee.row,p.bee.col,p.bee.speed,p.bee.perception );
}


void add_wasp( Hive *h,  Wasp w) {
	printf("Adding wasp-> row: %3d, col: %3d, speed: %3d\n", w.row, w.col, w.speed);
	union Pollinator p = { .wasp = w };
	if (h->pollinator_len < MAX_HIVE_POLLINATORS) {
		h->pollinators[h->pollinator_len] = p;
		h->pollinator_len++;
	}else {
		printf("ERROR: overflow of pollinators, could not add latest pollinator\n");
	}
}

void add_pollen( Flower *f, union Pollen p) {
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

void get_pollinators_at_position(int r, int c,  Map *map, union Pollinator *polls[], int poll_len ) {
	int count = 0;
	for (int i = 0; i < map->hive_len; i++) {
		for (int j = 0; j < map->hives[i].pollinator_len; j++) {
			union Pollinator *p = &map->hives[i].pollinators[j];

			int poll_r;
			int poll_c;


			if (map->hives[i].type == 'B' || map->hives[i].type == 'H' || map->hives[i].type == 'D') {
				if (p->bee.row == r && p->bee.col == c && count < poll_len) {
					polls[count] = p;
					count++;
				} else if (count >= poll_len) {
					printf("ERROR: ran out of space, cant add more pollinators to list from get_pollinators_at_position\n");
				}
			}
			else if (map->hives[i].type == 'W') {
				if (p->wasp.row == r && p->wasp.col == c && count < poll_len) {
					polls[count] = p;
					count++;
				} else if (count >= poll_len) {
					printf("ERROR: ran out of space, cant add more pollinators to list from get_pollinators_at_position\n");
				}
			}
		}
	}
}
void read_map(Map *map, Config c) {
	int current_line = 2;
	char line[INPUT_LINE_LEN];


	while (fgets(line, INPUT_LINE_LEN, stdin) != NULL) {

		char *tokens[INPUT_LINE_LEN];
		split_string(tokens, INPUT_LINE_LEN, line, " \n");

		if (tokens[4][0] != '\0') invalid_object_setup(current_line);

		char *object_tok = tokens[0];
		int object_len = strlen(object_tok);
		if (object_len != 1) invalid_object_setup(current_line);

		char object = object_tok[0];

		int x, y, n; 
		if (!string_to_int(&x, tokens[2])) invalid_object_setup(current_line);
		if (!string_to_int(&y, tokens[1])) invalid_object_setup(current_line);
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
			Flower flower = { 
				.row = y, 
				.col = x, 
				.pollen_len = 0,
				.pollen_type = c.pollen_type
			};

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
			Hive hive = { 
				.row = y,
				.col = x, 
				.type=object, 
				.pollinator_len = 0
			};

			char bee_line[BEE_MAX_CHARS];
			if (fgets(bee_line, BEE_MAX_CHARS, stdin) == NULL) {
				invalid_object_setup(current_line);
			}

			current_line++;

			char *bee_tokens[3];
			split_string(bee_tokens, 3, bee_line, " \n");

			if (!string_to_int(&speed, bee_tokens[0])) invalid_object_setup(current_line);
			if (!string_to_int(&perception, bee_tokens[1])) invalid_object_setup(current_line);
			if (bee_tokens[2][0] != '\0') invalid_object_setup(current_line);

			for (int i = 0; i < n; i++) {
				Bee bee = {
					.row = hive.row,
					.col = hive.col,
					.speed = speed,
					.perception = perception,
					.state = WANDER,
					.hive_ptr = &hive
				};
				add_bee(&hive, bee);
			}
			add_hive(map, hive);
		}
		else if (object == 'W') {
			 Hive hive = { .row = y, .col = x, .type=object, .pollinator_len = 0};
			char wasp_line[BEE_MAX_CHARS];
			if (fgets(wasp_line, BEE_MAX_CHARS, stdin) == NULL) {
				invalid_object_setup(current_line);
			}

			current_line++;

			char *wasp_tokens[2];
			split_string(wasp_tokens, 2, wasp_line, " \n");

			if (!string_to_int(&speed, wasp_tokens[0])) invalid_object_setup(current_line);

			if (wasp_tokens[1][0] != '\0') invalid_object_setup(current_line);


			for (int i = 0; i < n; i++) {
			 	Wasp wasp = { 
					.row = hive.row, 
					.col = hive.col, 
					.speed = speed,
					.hive_ptr = &hive
				};
				add_wasp(&hive, wasp);
			}
			add_hive(map, hive);
		}
		else invalid_object_setup(current_line);
		
		current_line++;
	}

}

