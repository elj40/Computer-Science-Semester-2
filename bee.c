#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "map.h"
#include "utils.h"
#include "update.h"
#include "compass.h"
#include "bee.h"

void bee_land_on_flower(Bee *bee, Cell *cell);
void print_bee(Bee b) {
	printf("Bee:  r:%d c:%d id:%d\n", b.row, b.col, b.id);
}
void bee_print_list(BeeNode * head) {
	BeeNode * current = head;

	while (current != NULL) {
		print_bee(current->bee);
		current = current->next_ptr;
	}
}

/* - if bee lands on flower and doesnt already have pollen, collect one pollen */ 
/* - if 2+ bees land on flower: */
/*     - enough pollen for all: each gets one */
/*     - not enough: no pollen collected (pollen destroyed?), bees continue as if not on flower */
/* - if bee lands on wasp hive, it dies */
/* - if bee lands at same place as wasp, it dies */
/* - bees that die with pollen destroy pollen */
/* - if bee lands on flower with wasp: bees die and flower keeps its pollen */
/* - if 2+ bee scouts inform of a flower at same time, foragers go to closest one */
void bee_action(Bee * bee, Map *map) {
	Cell *cell;
	Cell *next_cell;
	BeeNode *bee_current_head;

	int r = bee->row;
	int c = bee->col;
	cell = &map->map[r][c];
	next_cell = &map->next_map[r][c];

	printf("Doing bee action: %c %d %d\n", cell->display_char, r, c );

	if (cell->display_char == 'W') {
		remove_bee_from_cell(&next_cell->bee_head_ptr, *bee);
		return;
	}
	if (cell->display_char == 'F' 
		&& cell->flower_ptr->pollen_len > 0 
		&& (bee->state == SEEK || bee->state == WANDER)) 
	{
		int bee_count = bee_linked_list_len(next_cell->bee_head_ptr);
		printf("Found %d bees at flower\n", bee_count);
		bee_print_list(next_cell->bee_head_ptr);
		bee_land_on_flower(bee, cell);
		return;
	}
	
}

void bee_land_on_flower(Bee *bee, Cell *cell) {
	Flower *flower_ptr = cell->flower_ptr;

	int l = flower_ptr->pollen_len;
	enum PollenType t = flower_ptr->pollen_type;
	union Pollen p = flower_ptr->pollen[l-1];

	switch (t) {
		case FLOAT:
			bee->pollen.float_info = p.float_info;
			break;
		case STRING:
			/* bee->pollen.string_info = p.string_info; */
			strncpy(bee->pollen.string_info, p.string_info, MAX_POLLEN_CHARS);
			break;
	}

	flower_ptr->pollen_len--;
	printf("Picked up pollen -> ");
	print_flower(*flower_ptr);

	bee->state = RETURN;
}

void bee_move(Bee *bee,  Map *map) {
	Trajectory t = bee_get_next_trajectory(bee, map);
	Cell *cell;
	BeeNode **cell_head;

	int or = bee->row;
	int oc = bee->col;
	int nr, nc;

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

Trajectory bee_get_next_trajectory( Bee *bee,  Map *map) {
	int flower_r, flower_c;
	bee_check_for_flowers(bee, map, &flower_r, &flower_c);

	if (bee->state == WANDER) 
	{
		if (flower_r != -1) 
		{
			bee->state = SEEK;
			return get_trajectory_from_target(bee->row, bee->col, bee->speed, flower_r, flower_c);
		}
			
		return get_random_trajectory(bee->speed);
	}
	else if (bee->state == RETURN) 
	{
		return get_random_trajectory(bee->speed);
	}
	else if (bee->state == SEEK)
	{
		return get_trajectory_from_target(bee->row, bee->col, bee->speed,flower_r, flower_c);
	}
	
	printf("Somehow bee does not have correct state, returning random direction\n");
	return get_random_trajectory(bee->speed);
}

void bee_check_for_flowers( Bee *bee,  Map *m, int *fr, int *fc) {
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
