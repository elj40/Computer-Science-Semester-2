#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "map.h"
#include "compass.h"
#include "bee.h"

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
