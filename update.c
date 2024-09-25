#include <stdio.h>
#include <math.h>

#include "types.h"
#include "update.h"
#include "compass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define EPS 0.01

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
void bee_move(struct Bee *bee, struct Map *map) {
	struct Trajectory t = get_next_trajectory(bee->speed);

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
	struct Trajectory t = get_next_trajectory(wasp->speed);

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
