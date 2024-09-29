#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "map.h"
#include "types.h"
#include "update.h"
#include "compass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define EPS 0.01

/* HAND IN 3: For honey bees, their individual compass also dictates whether it is a scout or a forager. */

/* Each entity moves according to the trajectories given to it by its own compass, with the exception of what the entity may do when encountering another object in the world. See Action Rules. */

/* The range of magnitudes of an entity’s trajectories is determined by its speed parameter which is given as an input at the beginning of the simulation. For each iteration of the simulation, the magnitude of an entity move given by its compass will be a value between 0 (inclusive) and speed (inclusive). */ 

/* If a bee’s magnitude is too large and it flies over a flower or hive, it overshoots them and does not interact with them. The same applied for wasps. */
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
void get_next_position_from_trajectory(Trajectory t, int n, int r, int c, int *nr, int *nc) {
	float dir = t.direction;
	int s     = t.distance;

	float cos_dir = cos(dir);
	float sin_dir = sin(dir);

	int dc = 0;
	int dr = 0;

	if (sin_dir > EPS) dr = s;
	else if(sin_dir < -EPS) dr = -s;

	if (cos_dir > EPS) dc = s;
	else if(cos_dir < -EPS) dc = -s;

	*nc = c + dc;
	*nr = r + dr;

	*nc = MIN(n, MAX(*nc, 0));
	*nr = MIN(n, MAX(*nr, 0));
}

void move_bees_in_cell(Cell *c, Map *m) {
	BeeNode *current = c->bee_head_ptr;
	while (current != NULL) {
		/* printf("Moving bee %d\n", current->bee.id); */
		bee_move(&current->bee, m);
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
				bee_print_list(c->bee_head_ptr);
			}
		}

	}
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];

			move_bees_in_cell(c, map);

			/* action_bees_in_cell(*c); */
			/* move_wasps_in_cell(*c); */
			/* action_wasps_in_cell(*c); */
		}

	}
	printf("\nAfter current:\n");
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->map[i][j];
			if ( c->bee_head_ptr != NULL) {
				printf("Bees in cell %d, %d:\n", i, j);
				bee_print_list(c->bee_head_ptr);
			}
		}

	}
	printf("\nAfter next:\n");
	for (int i = 0; i < ms; i++) {
		for (int j = 0; j < ms; j++) {
			c = &map->next_map[i][j];
			if ( c->bee_head_ptr != NULL) {
				printf("Bees in cell %d, %d:\n", i, j);
				bee_print_list(c->bee_head_ptr);
			}
		}

	}

	copy_map_enitites(&map->map[0][0], &map->next_map[0][0], map->map_size);
	clear_map_cells(&map->next_map[0][0], map->map_size);
	/* 	for (int i = 0; i < map->hive_len; i++) { */
	/* 		hive_update(&map->hives[i], map, config); */
	/* 	} */
}

/* void hive_update( Hive *hive,  Map *map, Config *config) { */
/* 	for (int i = 0; i < hive->pollinator_len; i++) { */
/* 		switch (hive->type) { */
/* 			case 'W': */
/* 				wasp_move(&hive->pollinators[i].wasp, map); */
/* 				break; */
/* 			case 'H': */
/* 			case 'B': */
/* 			case 'D': */
/* 				bee_move(&hive->pollinators[i].bee, map); */
/* 				/1* bee_action(&hive->pollinators[i].bee, map); *1/ */
/* 				break; */
/* 			default: */
/* 				printf("Unknown hive type in update"); */
/* 				break; */
/* 		} */
/* 	} */
/* } */
/* /1* All types of bees have a perception range at which they can “see” flowers. If any bee except the scout bee sees a flower within its perception range that contains pollen, it will move in a direction towards the flower on the next turn(s) (as long as it is not carrying an item), with the magnitude given by the trajectory. In contrast, if a scout bee sees a flower within its perception range that contains pollen, it will immediately fly back to its hive to inform the foragers. If there are multiple flowers in the range of the bee, the bee will always go towards the flower that is closest (i.e. within less perception range), and is the bottom-most and left-most within that proximity. *1/ */ 

/* /1* When entities know the location of their destination and can fly towards the destination, they first move diagonally in the direction of the destination until they can move in a straight line towards the destination. The distance they move is always determined by their compass. *1/ */

/* // TODO: do the random distribution with speed thingy */
Trajectory get_trajectory_from_target(int r, int c,int speed, int tr, int tc) {
	Trajectory t = { .distance = speed };
	if (tr == r && tc >  c) t.direction= 0 * M_PI/4;
	else if (tr >  r && tc >  c) t.direction= 1 * M_PI/4;
	else if (tr >  r && tc == c) t.direction= 2 * M_PI/4;
	else if (tr >  r && tc <  c) t.direction= 3 * M_PI/4;
	else if (tr == r && tc <  c) t.direction= 4 * M_PI/4;
	else if (tr <  r && tc <  c) t.direction= 5 * M_PI/4;
	else if (tr <  r && tc == c) t.direction= 6 * M_PI/4;
	else if (tr <  r && tc >  c) t.direction= 7 * M_PI/4;
	else if (tr == r && tc == c) {
		t.distance = 0;
		printf("Already at target\n");
	}
	else { 
		printf("No idea where the target is\n");
		t.distance = 0;
	}
	return t;
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


void bee_move( Bee *bee,  Map *map) {
	Trajectory t = bee_get_next_trajectory(bee, map);
	Cell *cell;
	BeeNode **cell_head;

	int or = bee->row;
	int oc = bee->col;
	int nr, nc;

	get_next_position_from_trajectory(t, map->map_size-1, or, oc, &nr, &nc);

	/* printf("Bee moved from %d,%d to %d,%d\n", or, oc, nr, nc); */
	bee->row = nr;
	bee->col = nc;

	cell = &map->next_map[nr][nc];
	cell_head = &cell->bee_head_ptr;
	add_bee_to_cell(cell_head, *bee);

	/* printf("Bees at next cell\n"); */
	/* bee_print_list(*cell_head); */

	/* cell = &map->next_map[or][oc]; */
	/* cell_head = &cell->bee_head_ptr; */
	/* remove_bee_from_cell(cell_head, *bee); */ 


}


/* void wasp_move( Wasp *wasp,  Map *map) { */
/* 	Trajectory t = get_random_trajectory(wasp->speed); */

/* 	int nr, nc; */
/* 	get_next_position_from_trajectory(t, map->map_size-1, wasp->row, wasp->col, &nr, &nc); */

/* 	printf("Wasp moved from %d,%d to %d,%d\n", wasp->row, wasp->col, nr, nc); */

/* 	if (map->map[wasp->row][wasp->col] == 'w') { */ 
/* 		map->map[wasp->row][wasp->col] = ' '; */
/* 	} */

/* 	if (map->map[nr][nc] == ' ') { */
/* 		map->map[nr][nc] = 'w'; */
/* 	} */

/* 	wasp->row = nr; */
/* 	wasp->col = nc; */
/* } */
