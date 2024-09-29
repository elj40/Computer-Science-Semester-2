#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "map.h"
#include "compass.h"
#include "wasp.h"

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
Trajectory wasp_get_next_trajectory( Wasp *wasp,  Map *map) {
	return get_random_trajectory(wasp->speed);
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

void wasp_action(Wasp *wasp, Map *map) {
	Cell *cell;
	BeeNode *current_bee_node;
	BeeNode *next_bee_node;

	int or = wasp->row;
	int oc = wasp->col;

	cell = &map->next_map[or][oc];

	// if bee lands at same place as wasp, it dies
	// so free all current bees from memory and then set bee pointer to null
	// will have to change if not all bees die

	bee_free_linked_list(cell->bee_head_ptr);
	cell->bee_head_ptr = NULL;

	// Do something like this if we need to remove only some bees with conditions
	/* current_bee_node = cell->bee_head_ptr; */
	/* while (current_bee_node != NULL) { */
	/* 	next_bee_node = current_bee_node->next_ptr; */	
	/* 	remove_bee_from_cell(cell, current_bee_node->bee); */
	/* 	current_bee_node = next_bee_node; */
	/* } */
	
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
