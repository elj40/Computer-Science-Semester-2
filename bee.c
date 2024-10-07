#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "map.h"
#include "utils.h"
#include "update.h"
#include "compass.h"
#include "bee.h"

void honey_bee_action(Bee *bee, Map *map);
void bee_land_on_flower(Bee *bee, Cell *cell);
void print_bee(Bee b) {
	printf("Bee:\tr:%d c:%d p:%d id:%d type:%d state:%d\n", b.row, b.col, b.perception, b.id, b.type, b.state);
}
void bee_print_list(BeeNode * head) {
	BeeNode * current = head;

	while (current != NULL) {
		print_bee(current->bee);
		current = current->next_ptr;
	}
}

void bee_free_flower_path(Trajectory ** flower_path, int len) {
	for (int i = 0; i < len; i++) {
		free(flower_path[i]);
	}
	free(flower_path);
}
void bee_action(Bee * bee, Map *map) {
	Cell *cell;
	Cell *next_cell;
	BeeNode *bee_current_head;

	int r = bee->row;
	int c = bee->col;
	int pollen_len;
	int total_bees_count;
	cell = &map->map[r][c];
	next_cell = &map->next_map[r][c];

	/* printf("Doing bee action: %c %d %d\n", cell->display_char, r, c ); */

	//Special behaviours
	// Dying on wasp hive is the same for all bees
	if (cell->display_char == 'W') {
		remove_bee_from_cell(&next_cell->bee_head_ptr, *bee);
		return;
	}

	// This behaviour is general, except for desert bee, who now needs to remember the path
	// and scout bee, who will only be here if it has perception of 0, who must leave without pollen;
	if (cell->display_char == 'F' 
		&& cell->flower_ptr->pollen_len > 0 
		&& (bee->state == SEEK || bee->state == WANDER)) 
	{

		if (bee->role == SCOUT) {
			bee->flower_location.row = bee->row;
			bee->flower_location.col = bee->col;
			bee->state = RETURN;
			return;
		}

		int this_bee_pos = bee_linked_list_get_node_pos(cell->bee_head_ptr, *bee);
		total_bees_count = bee_linked_list_len(cell->bee_head_ptr);

		pollen_len = cell->flower_ptr->pollen_len;

		if (bee->type == DESERT && pollen_len <= 0) {
			// Cant actually easliy reset array, so will just try overwrite stuff
			/* bee->flower_path = {0}; */
			bee->flower_path_len = 0;
			bee->flower_path_index = -1;
			bee->state = RETURN;
			return;
		}

		if (total_bees_count > pollen_len && this_bee_pos == 0) {
			cell->flower_ptr->fight = true;
		}

		if (cell->flower_ptr->fight) {
			bee->state = WANDER;
			printf("Plant fighting, leaving now\n");
		}
		else {
			bee_land_on_flower(bee,cell);
			bee->state = RETURN;
		}

		return;
	}
	
	//Usual behaviour
	if (bee->type == NORMAL) normal_bee_action(bee, map);
	else if (bee->type == HONEY) honey_bee_action(bee, map);
}

void hive_add_pollen(Hive *hive, union Pollen pollen) {
	printf("Adding pollen to hive\n");
	int l = hive->pollen_len;
	if (hive->pollen_len >= MAX_FLOWER_POLLEN) {
		printf("ERROR: could not add latest pollen to hive, overflow\n");
	}
	if (hive->pollen_type == FLOAT) hive->pollen[l] = pollen;
	else if (hive->pollen_type == STRING) 
		strncpy(hive->pollen[l].string_info, pollen.string_info, MAX_POLLEN_CHARS);

	hive->pollen_len++;
}

void normal_bee_action(Bee *bee, Map *map) {
	// This behaviour is specific to general bee
	bool flower_close = bee_check_for_flowers(bee, map);
	if (bee->state == RETURN) {
		if (bee->row == bee->hive_ptr->row && bee->col == bee->hive_ptr->col) {
			bool has_pollen = bee->pollen.string_info[0] != '\0';
			bee->pollen.string_info[0] != '\0';
			// Add pollen to hive
			if (has_pollen) hive_add_pollen(bee->hive_ptr, bee->pollen);
			bee->state = WANDER;
		}
		return;
	}
	if (bee->state == SEEK && !flower_close) bee->state = WANDER;
	if (flower_close) bee->state = SEEK;
	else bee->state = WANDER;
}


void honey_bee_action(Bee *bee, Map *map) {
	/* - Honey: */
	/* 	- Two roles: scouts of forager */
	/* 	- Scout: */ 
	/* 		- double perception */
	/* 		- 2 states: wander and return*/
	/*         - Stores location */
	/*     - Forager: */ 
	/*         - dormant until hearing about location */
	/*         - 3 states: dormant (stay at hive), go to target, search (look around empty flower for 5 moves or pollen) */
	/*         - Stores location and pollen */


	int r = bee->row;
	int c = bee->col;
	Cell cell = map->map[r][c];
	if (bee->role == SCOUT) {
		if (bee->state == RETURN) {
			if (bee->row == bee->hive_ptr->row && bee->col == bee->hive_ptr->col) {
				printf("Scout %d reported flower\n", bee->id);
				wake_up_honey_bees_in_cell(&cell, bee->flower_location.row, bee->flower_location.col);
				bee->state = WANDER;
				return;
			}
		}
		bool flower_close = bee_check_for_flowers(bee, map);
		if (flower_close) {
			printf("Scout %d spotted a flower\n", bee->id);
			int *fr = &bee->flower_location.row;
			int *fc = &bee->flower_location.col;
			bee_locate_flowers(bee, map, fr, fc);
			bee->state = RETURN;
			return;
		}
		else bee->state = WANDER;
	}

	if (bee->role == FORAGER) {
		if (bee->state == RETURN) {
			if (r == bee->hive_ptr->row && c == bee->hive_ptr->col) {

				bool has_pollen = bee->pollen.string_info[0] != '\0';
				bee->pollen.string_info[0] != '\0';
				if (has_pollen) hive_add_pollen(bee->hive_ptr, bee->pollen);

				bee->flower_location.row = MAX_MAP_SIZE+1;
				bee->flower_location.col = MAX_MAP_SIZE+1;
				bee->state = DORMANT;
			}
		}
	}
}

void desert_bee_action(Bee *bee, Map *map) {
	if (bee->state == RETURN) {
		if (bee->row == bee->hive_ptr->row && bee->col == bee->hive_ptr->col) {
			// Add pollen to hive
			// Should only happen when bee has pollen
			// Set pollen to "/0" whenever we drop off pollen
			bool has_pollen = bee->pollen.string_info[0] != '\0';
			bee->pollen.string_info[0] != '\0';
			hive_add_pollen(bee->hive_ptr, bee->pollen);

			if (bee->flower_path_len > 0) bee->state = PATH;
			else bee->state = WANDER;
		}
		return;
	}
}

void bee_land_on_flower(Bee *bee, Cell *cell) {
	printf("Bee on flower!\n");
	Flower *flower_ptr = cell->flower_ptr;

	int l = flower_ptr->pollen_len;
	enum PollenType t = flower_ptr->pollen_type;
	union Pollen p = flower_ptr->pollen[l-1];

	switch (t) {
		case FLOAT:
			bee->pollen.float_info = p.float_info;
			break;
		case STRING:
			strncpy(bee->pollen.string_info, p.string_info, MAX_POLLEN_CHARS);
			break;
	}

	flower_ptr->pollen_len--;
	printf("Picked up pollen -> ");
	print_flower(*flower_ptr);
}

void bee_move(Bee *bee,  Map *map) {
	Cell *cell;
	BeeNode **cell_head;

	int or = bee->row;
	int oc = bee->col;
	int nr, nc;

	Trajectory t = bee_get_next_trajectory(bee, map);
	get_next_position_from_trajectory(t, map->map_size-1, or, oc, &nr, &nc);

	bee->row = nr;
	bee->col = nc;

	cell = &map->next_map[nr][nc];
	cell_head = &cell->bee_head_ptr;
	add_bee_to_cell(cell_head, *bee);
}

void add_bee_to_cell(BeeNode **head, Bee bee) {
	/* printf("Adding bee %d\n", bee.id); */
	BeeNode *new_node = (BeeNode *) malloc(sizeof(BeeNode));
	new_node->bee = bee;
	new_node->next_ptr = NULL;

	/* print_bee(bee); */

	if (*head == NULL) {
		*head = new_node;
		return;
	}

	BeeNode *current_bee_node = *head;
	while (current_bee_node->next_ptr != NULL) {
		current_bee_node = current_bee_node->next_ptr;
	}
	current_bee_node->next_ptr = new_node;
}

Trajectory normal_bee_get_next_trajectory(Bee *bee, Map *map) {
	int flower_r, flower_c;
	int hive_r, hive_c;

	if (bee->state == WANDER) 
	{
		return get_random_trajectory(bee->speed);
	}
	else if (bee->state == RETURN) 
	{
		int hive_r = bee->hive_ptr->row;
		int hive_c = bee->hive_ptr->col;

		return get_trajectory_from_target(bee->row, bee->col, bee->speed, hive_r, hive_c);
	}
	else if (bee->state == SEEK)
	{
		bee_locate_flowers(bee, map, &flower_r, &flower_c);
		return get_trajectory_from_target(bee->row, bee->col, bee->speed,flower_r, flower_c);
	}
	
	printf("Somehow bee does not have correct state, returning random direction\n");
	return get_random_trajectory(bee->speed);
}

Trajectory honey_bee_get_next_trajectory(Bee *bee, Map *map) {
	if (bee->state == WANDER) return get_random_trajectory(bee->speed);
	if (bee->state == RETURN) {
		int hive_r = bee->hive_ptr->row;
		int hive_c = bee->hive_ptr->col;

		return get_trajectory_from_target(bee->row, bee->col, bee->speed, hive_r, hive_c);
	}
	if (bee->role == FORAGER) {
		if (bee->state == DORMANT) {
			Trajectory zero_traj = { .distance = 0 };
			return zero_traj;
		}
		if (bee->state == SEEK) {
			int fr = bee->flower_location.row;
			int fc = bee->flower_location.col;
			return get_trajectory_from_target(bee->row, bee->col, bee->speed, fr, fc);
		}
	}
}

Trajectory bee_get_next_trajectory(Bee *bee,  Map *map) {
	if (bee->type == NORMAL) {
		return normal_bee_get_next_trajectory(bee, map);
	}
	else if (bee->type == HONEY) {
		return honey_bee_get_next_trajectory(bee, map);
	}
}


bool bee_check_for_flowers(Bee *bee, Map *m) {
	/* If therr are any flowers, return true most bottom left one
	 * Find bottom-left and top-right corner
	 * 	Must be within bounds if map
	 * Loop from bottom-left to top right
	 *
	 * if flower:
	 * 	if closest to bee:
	 * 	    save postion
	 *
	 * return closest flower
	 *
	 * Should work because of the way we loop through (going from bottom to top)
	 */

	int r = bee->row;
	int c = bee->col;
	int p = bee->perception;

	int top = MIN(m->map_size-1, r + p);
	int bottom = MAX(0, r - p);
	int right = MIN(m->map_size-1, c + p);
	int left = MAX(0, c - p);

	for (int y = bottom; y <= top; y++) {
		for (int x = left; x <= right; x++) {
			if (m->map[y][x].display_char == 'F') return true;
		}
	}

	return false;
}
void bee_locate_flowers(Bee *bee, Map *m, int *fr, int *fc) {
	/* If therr are any flowers, return most bottom left one
	 * Find bottom-left and top-right corner
	 * 	Must be within bounds if map
	 * Loop from bottom-left to top right
	 *
	 * if flower:
	 * 	if closest to bee:
	 * 	    save postion
	 *
	 * return closest flower
	 *
	 * Should work because of the way we loop through (going from bottom to top)
	 */

	int r = bee->row;
	int c = bee->col;
	int p = bee->perception;

	int top = MIN(m->map_size-1, r + p);
	int bottom = MAX(0, r - p);
	int right = MIN(m->map_size-1, c + p);
	int left = MAX(0, c - p);

	int min_dist = m->map_size + 1;

	*fr = -1;
	*fc = -1;

	for (int y = bottom; y <= top; y++) {
		for (int x = left; x <= right; x++) {
			if (m->map[y][x].display_char == 'F') {
				int dist = MAX(abs(y-r),abs(c-x));
				if (dist < min_dist) {
					*fr = y;
					*fc = x;
				}
			}
		}
	}
}
