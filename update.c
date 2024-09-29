#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "map.h"
#include "bee.h"
#include "wasp.h"
#include "types.h"
#include "update.h"
#include "compass.h"



/* HAND IN 3: For honey bees, their individual compass also dictates whether it is a scout or a forager. */

/* Each entity moves according to the trajectories given to it by its own compass, with the exception of what the entity may do when encountering another object in the world. See Action Rules. */

/* The range of magnitudes of an entity’s trajectories is determined by its speed parameter which is given as an input at the beginning of the simulation. For each iteration of the simulation, the magnitude of an entity move given by its compass will be a value between 0 (inclusive) and speed (inclusive). */ 

/* If a bee’s magnitude is too large and it flies over a flower or hive, it overshoots them and does not interact with them. The same applied for wasps. */

void action_bees_in_cell(Cell *c, Map *m) {

	BeeNode *current = c->bee_head_ptr;
	while (current != NULL) {
		/* printf("Moving bee %d\n", current->bee.id); */
		bee_action(&current->bee, m);
		current = current->next_ptr;
	}
};
void action_wasps_in_cell(Cell *c, Map *m) {
	WaspNode *current = c->wasp_head_ptr;
	while (current != NULL) {
		/* printf("Moving wasp %d\n", current->wasp.id); */
		wasp_action(&current->wasp, m);
		current = current->next_ptr;
	}
};
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

void print_flower(Flower f) {
	printf("Pollen count: %d, pollen: ", f.pollen_len);
	for (int i = 0; i < f.pollen_len; i++) {
		printf("%s, ", f.pollen[i].string_info);
	}
	printf("\n");
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
				bee_print_list(c->bee_head_ptr);
			}

			if (c->display_char == 'F') {
				print_flower(*c->flower_ptr);
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
			c = &map->next_map[i][j];

			// Do wasp actions before bee actions because wasps can kill bees and not vice versa
			action_wasps_in_cell(c, map);
			action_bees_in_cell(c, map);
		}

	}
	/* printf("\nAfter current:\n"); */
	/* for (int i = 0; i < ms; i++) { */
	/* 	for (int j = 0; j < ms; j++) { */
	/* 		c = &map->map[i][j]; */
	/* 		if ( c->bee_head_ptr != NULL) { */
	/* 			printf("Bees in cell %d, %d:\n", i, j); */
	/* 			/1* bee_print_list(c->bee_head_ptr); *1/ */
	/* 		} */
	/* 	} */

	/* } */
	/* printf("\nAfter next:\n"); */
	/* for (int i = 0; i < ms; i++) { */
	/* 	for (int j = 0; j < ms; j++) { */
	/* 		c = &map->next_map[i][j]; */
	/* 		if ( c->bee_head_ptr != NULL) { */
	/* 			printf("Bees in cell %d, %d:\n", i, j); */
	/* 			/1* bee_print_list(c->bee_head_ptr); *1/ */
	/* 		} */
	/* 	} */

	/* } */

	copy_map_enitites(&map->map[0][0], &map->next_map[0][0], map->map_size);
	clear_map_cells(&map->next_map[0][0], map->map_size);
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
			free(current);
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


