#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "map.h"
#include "types.h"
#include "update.h"
#include "utils.h"

#define INPUT_LINE_LEN 50
#define POLLEN_MAX_CHARS 50
#define BEE_MAX_CHARS 50

void clear_map_cells(Cell *map, int map_size) {
	int s = map_size;
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			
			Cell c = {
				.display_char = ' ', //32 in ascii
				.bee_head_ptr = NULL,
				.wasp_head_ptr = NULL
			};
			
			*(map + i*MAX_MAP_SIZE + j) = c;
		}
	}
}

void copy_map_cells(Cell *dest, Cell *src, int map_size) {
	int s = map_size;
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			*(dest + i*MAX_MAP_SIZE + j) = *(src + i*MAX_MAP_SIZE + j);
		}
	}
}
void copy_map_enitites(Cell *dest, Cell *src, int map_size) {
	int s = map_size;
	Cell *dest_cell;
	Cell *src_cell;
	for (int i = 0 ; i < s; i++) {
		for (int j = 0 ; j < s; j++) {
			dest_cell = dest + i*MAX_MAP_SIZE + j;
			src_cell = src + i*MAX_MAP_SIZE + j;

			dest_cell->bee_head_ptr = src_cell->bee_head_ptr;
			dest_cell->wasp_head_ptr = src_cell->wasp_head_ptr;
		}
	}
}

void bee_free_linked_list(BeeNode *head) {
	BeeNode * current = head;

	while (current != NULL) {
		BeeNode *next_bee_ptr = current->next_ptr;
		free(current);
		current = next_bee_ptr;
	}	
}

void wasp_free_linked_list(WaspNode *head) {
	WaspNode *current = head;
	WaspNode *next_wasp_ptr;

	while (current != NULL) {
		next_wasp_ptr = current->next_ptr;
		free(current);
		current = next_wasp_ptr;
	}	
}

void free_map(Map *map) {
	int ms = map->map_size;

	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			Cell c = map->map[i][j];
			free(c.flower_ptr);
			free(c.hive_ptr);
			bee_free_linked_list(c.bee_head_ptr);
			wasp_free_linked_list(c.wasp_head_ptr);
		}

	}
}


void add_hive(Map *m,  Hive h) {
	printf("Adding hive-> row: %3d, col: %3d, type:%c\n", h.row, h.col, h.type);
	Cell *c = &m->map[h.row][h.col];

	if (c->display_char != ' ') {
		printf("Already something here: %c %d\n", c->display_char, c->display_char);
		printf("ERROR: Cannot place hive at already occupied location (%d, %d)\n", h.row, h.col);
		exit(0); 
	}

	c->display_char = h.type;
	c->hive_ptr = &h;
}


void add_flower(Map *m, Flower f) {
	printf("Adding flower-> row: %3d, col: %3d, p_type: %d\n", f.row, f.col, f.pollen_type);
	Cell *c = &m->map[f.row][f.col];

	// The real flower gets deallocated outside of read map, and so becomes gibberish
	// Allocating memory and storing it like this might work
	// This works but only as a shallow copy
	Flower *f_ptr = (Flower *) malloc(sizeof(Flower));
	f_ptr->pollen_len = f.pollen_len;
	f_ptr->pollen_type = f.pollen_type;
	f_ptr->fight = false;

	memcpy(f_ptr->pollen, f.pollen, sizeof(f.pollen));


	if (c->display_char != ' ') {
		printf("ERROR: Cannot place flower at already occupied location (%d, %d)\n", f.row, f.col);
		exit(0); 
	}

	c->display_char = 'F';
	c->flower_ptr = f_ptr;

	/* print_flower(*c->flower_ptr); */
}

void add_bee(Map *m, Bee b) {
	static int id = 0;
	printf("Adding bee-> row: %3d, col: %3d, speed: %3d, percep: %3d, id:%3d\n", b.row, b.col, b.speed, b.perception, id);
	Cell *c = &m->map[b.row][b.col];
	/* c->bee_head_ptr = NULL; */

	b.id = id;
	add_bee_to_cell(&c->bee_head_ptr, b);
	/* bee_print_list(c->bee_head_ptr); */
	id++;
}


void add_wasp(Map *m,  Wasp w) {
	static int id = 0;
	printf("Adding wasp-> row: %3d, col: %3d, speed: %3d\n", w.row, w.col, w.speed);
	Cell *c = &m->map[w.row][w.col];

	w.id = id;
	add_wasp_to_cell(&c->wasp_head_ptr, w);

	id++;
}

void add_pollen(Flower *f, union Pollen p) {
	// TODO: consider making pollen a linked list too
	if (f->pollen_type == 0) 
		printf("Adding pollen-> info_f: %f\n", p.float_info);
	else if (f->pollen_type == 1)
		printf("Adding pollen-> info_s: %s\n", p.string_info);

	if (f->pollen_len < MAX_FLOWER_POLLEN) {
		f->pollen[f->pollen_len] = p;
		f->pollen_len++;
	}else {
		printf("ERROR: overflow of pollens, could not add latest pollen\n");
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

		int speed;
		int perception;
		union Pollen pollen;
		if (object == 'F') {
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
					if (!string_to_float(&pollen.float_info, pollen_v)) 
						invalid_object_setup(current_line);
					add_pollen(&flower, pollen);
				}
				else if (c.pollen_type == STRING) {
					printf("Pollen value: %s\n", pollen_v);
					strncpy(pollen.string_info, pollen_v, MAX_POLLEN_CHARS);
					add_pollen(&flower, pollen);
				}


			}
			/* print_flower(flower); */
			add_flower(map, flower);
		}
		else if (object == 'B' || object == 'H' || object == 'D') {
			Hive hive = { 
				.row = y,
				.col = x, 
				.type=object, 
			};

			if (n > 0) {
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
				add_bee(map, bee);
			}
			}
			add_hive(map, hive);
		}
		else if (object == 'W') {
			 Hive hive = { 
				 .row = y, 
				 .col = x, 
				 .type= object 
			 };
			if (n > 0) {
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
					add_wasp(map, wasp);
				}
			}
			add_hive(map, hive);
		}
		else invalid_object_setup(current_line);
		
		current_line++;
	}

}

