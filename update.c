#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "map.h"
#include "bee.h"
#include "types.h"
#include "update.h"
#include "compass.h"



/* HAND IN 3: For honey bees, their individual compass also dictates whether it is a scout or a forager. */

/* Each entity moves according to the trajectories given to it by its own compass, with the exception of what the entity may do when encountering another object in the world. See Action Rules. */

/* The range of magnitudes of an entity’s trajectories is determined by its speed parameter which is given as an input at the beginning of the simulation. For each iteration of the simulation, the magnitude of an entity move given by its compass will be a value between 0 (inclusive) and speed (inclusive). */ 

/* If a bee’s magnitude is too large and it flies over a flower or hive, it overshoots them and does not interact with them. The same applied for wasps. */
void print_wasp(Wasp w) {
	printf("Wasp:  r:%d c:%d id:%d\n", w.row, w.col, w.id);
}
void wasp_print_list(WaspNode * head) {
	WaspNode * current = head;

	while (current != NULL) {
		print_wasp(current->wasp);
		current = current->next_ptr;
	}
}

void action_bees_in_cell(Cell *c, Map *m) {};
void action_wasps_in_cell(Cell *c, Map *m) {};
void move_bees_in_cell(Cell *c, Map *m) {
	BeeNode *current = c->bee_head_ptr;
	while (current != NULL) {
		/* printf("Moving bee %d\n", current->bee.id); */
		bee_move(&current->bee, m);
		current = current->next_ptr;
	}
}

void move_wasps_in_cell(Cell *c, Map *m) {
	WaspNode *current = c->wasp_head_ptr;
	while (current != NULL) {
		/* printf("Moving wasp %d\n", current->wasp.id); */
		wasp_move(&current->wasp, m);
		current = current->next_ptr;
	}
}

void map_update( Map *map, Config *config) {
	int ms = map->map_size;
	Cell *c;
	printf("\nBefore:\n");
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];
			if ( c->bee_head_ptr != NULL) {
				printf("Bees in cell %d, %d:\n", i, j);
				/* bee_print_list(c->bee_head_ptr); */
			}
		}

	}
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];

			move_bees_in_cell(c, map);
			move_wasps_in_cell(c, map);
		}

	}
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];

			action_bees_in_cell(c, map);
			action_wasps_in_cell(c, map);
		}

	}
	printf("\nAfter current:\n");
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];
			if ( c->bee_head_ptr != NULL) {
				printf("Bees in cell %d, %d:\n", i, j);
				/* bee_print_list(c->bee_head_ptr); */
			}
		}

	}
	printf("\nAfter next:\n");
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->next_map[i][j];
			if ( c->bee_head_ptr != NULL) {
				printf("Bees in cell %d, %d:\n", i, j);
				/* bee_print_list(c->bee_head_ptr); */
			}
		}

	}

	copy_map_enitites(&map->map[0][0], &map->next_map[0][0], map->map_size);
	clear_map_cells(&map->next_map[0][0], map->map_size);
}

Trajectory wasp_get_next_trajectory( Wasp *wasp,  Map *map) {
	return get_random_trajectory(wasp->speed);
}

void remove_bee_from_cell(BeeNode **head, Bee bee) {
	BeeNode *current = *head;
	BeeNode *previous = NULL;

	while (current != NULL) {
		if (current->bee.id == bee.id) {
			/* printf("Removing bee %d\n", bee.id); */
			if (previous == NULL) { // We are at start
				*head = current->next_ptr;
			} else {
				previous->next_ptr = current->next_ptr;
			}
			/* free(current); */
			return;
		}
		previous = current;
		current = current->next_ptr;
	}
}

void remove_wasp_from_cell(WaspNode **head, Wasp wasp) {
	WaspNode *current = *head;
	WaspNode *previous = NULL;

	while (current != NULL) {
		if (current->wasp.id == wasp.id) {
			/* printf("Removing wasp %d\n", wasp.id); */
			if (previous == NULL) { // We are at start
				*head = current->next_ptr;
			} else {
				previous->next_ptr = current->next_ptr;
			}
			/* free(current); */
			return;
		}
		previous = current;
		current = current->next_ptr;
	}
}


void add_wasp_to_cell(WaspNode **head, Wasp wasp) {
	WaspNode *new_node = (WaspNode *) malloc(sizeof(WaspNode));
	new_node->wasp = wasp;
	new_node->next_ptr = NULL;

	if (*head == NULL) {
		*head = new_node;
		return;
	}

	WaspNode *current_wasp_node = *head;
	while (current_wasp_node->next_ptr != NULL) {
		current_wasp_node = current_wasp_node->next_ptr;
	}
	current_wasp_node->next_ptr = new_node;
}



void wasp_move(Wasp *wasp,  Map *map) {
	Trajectory t = wasp_get_next_trajectory(wasp, map);
	Cell *cell;
	WaspNode **cell_head;

	int or = wasp->row;
	int oc = wasp->col;
	int nr, nc;

	get_next_position_from_trajectory(t, map->map_size-1, or, oc, &nr, &nc);

	/* printf("Bee moved from %d,%d to %d,%d\n", or, oc, nr, nc); */
	wasp->row = nr;
	wasp->col = nc;

	cell = &map->next_map[nr][nc];
	cell_head = &cell->wasp_head_ptr;
	add_wasp_to_cell(cell_head, *wasp);

}
