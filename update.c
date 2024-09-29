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

void add_wasp_to_cell(WaspNode **head, Wasp wasp) {
	/* printf("Adding wasp %d\n", wasp.id); */
	WaspNode *new_node = (WaspNode *) malloc(sizeof(WaspNode));
	new_node->wasp = wasp;
	new_node->next_ptr = NULL;

	/* print_wasp(wasp); */

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

void bee_move( Bee *bee,  Map *map) {
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
