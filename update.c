#include <stdio.h>
#include <math.h>

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


void get_next_position_from_trajectory(struct Trajectory t, int n, int r, int c, int *nr, int *nc) {
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

void map_update(struct Map *map, struct Config *config) {
	for (int i = 0; i < map->hive_len; i++) {
		hive_update(&map->hives[i], map, config);
	}
}

void hive_update(struct Hive *hive, struct Map *map, struct Config *config) {
	for (int i = 0; i < hive->pollinator_len; i++) {
		switch (hive->type) {
			case 'W':
				wasp_move(&hive->pollinators[i].wasp, map);
				break;
			case 'H':
			case 'B':
			case 'D':
				bee_move(&hive->pollinators[i].bee, map);
				/* bee_action(&hive->pollinators[i].bee, map); */
				break;
			default:
				printf("Unknown hive type in update");
				break;
		}
	}
}
/* All types of bees have a perception range at which they can “see” flowers. If any bee except the scout bee sees a flower within its perception range that contains pollen, it will move in a direction towards the flower on the next turn(s) (as long as it is not carrying an item), with the magnitude given by the trajectory. In contrast, if a scout bee sees a flower within its perception range that contains pollen, it will immediately fly back to its hive to inform the foragers. If there are multiple flowers in the range of the bee, the bee will always go towards the flower that is closest (i.e. within less perception range), and is the bottom-most and left-most within that proximity. */ 

/* When entities know the location of their destination and can fly towards the destination, they first move diagonally in the direction of the destination until they can move in a straight line towards the destination. The distance they move is always determined by their compass. */

// TODO: do the random distribution with speed thingy
struct Trajectory get_trajectory_from_target(int r, int c,int speed, int tr, int tc, ) {
	struct Trajectory t = { .distance = speed };
	if (tr == r && tc >  c) t.direction= 0 * M_PI/4;
	else if (tr >  r && tc >  c) t.direction= 1 * M_PI/4;
	else if (tr >  r && tc == c) t.direction= 2 * M_PI/4;
	else if (tr >  r && tc <  c) t.direction= 3 * M_PI/4;
	else if (tr == r && tc <  c) t.direction= 4 * M_PI/4;
	else if (tr <  r && tc <  c) t.direction= 5 * M_PI/4;
	else if (tr <  r && tc == c) t.direction= 6 * M_PI/4;
	else if (tr <  r && tc >  c) t.direction= 7 * M_PI/4;
	printf("No idea where the target is, if not on same spot");
	t.distance = 0;
	return t;
}

struct Trajectory bee_get_next_trajectory(struct Bee *bee) {
	int flower_r, flower_c;
	bee_check_for_flowers(bee->row, bee->col, bee->perception, &flower_r, &flower_c);

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
void bee_move(struct Bee *bee, struct Map *map) {
	struct Trajectory t = bee_get_next_trajectory(bee);

	int nr, nc;
	get_next_position_from_trajectory(t, map->map_size-1, bee->row, bee->col, &nr, &nc);

	printf("Bee moved from %d,%d to %d,%d\n", bee->row, bee->col, nr, nc);

	if (map->map[bee->row][bee->col] == 'b') { 
		map->map[bee->row][bee->col] = ' ';
	}

	if (map->map[nr][nc] == ' ') {
		map->map[nr][nc] = 'b';
	}

	bee->row = nr;
	bee->col = nc;
}

void wasp_move(struct Wasp *wasp, struct Map *map) {
	struct Trajectory t = get_random_trajectory(wasp->speed);

	int nr, nc;
	get_next_position_from_trajectory(t, map->map_size-1, wasp->row, wasp->col, &nr, &nc);

	printf("Wasp moved from %d,%d to %d,%d\n", wasp->row, wasp->col, nr, nc);

	if (map->map[wasp->row][wasp->col] == 'w') { 
		map->map[wasp->row][wasp->col] = ' ';
	}

	if (map->map[nr][nc] == ' ') {
		map->map[nr][nc] = 'w';
	}

	wasp->row = nr;
	wasp->col = nc;
}
