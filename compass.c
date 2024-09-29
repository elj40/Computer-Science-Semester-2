#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"

/* The compass supplies its entity with a trajectory for each move, */ 
/*     where a trajectory is a single straight-line movement inion */ 
/*     that includes the direction that the entity will move in, and the magnitude (distance) of the move. */ 
/*     The direction given by the compass is a trigonometric or Cartesian direction, */ 
/*     hence 0 degrees will represent the entity moving to the right, 90 degrees moving up, and so on. */ 
/*     Note that the Compass will always return directions that are multiple of 45 degrees */ 
/*     (i.e. the entity will always move vertically, horizontally, or along a diagonal). */

Trajectory get_random_trajectory(int speed) {
	 Trajectory t;

	// t.distance = rand() % (speed+1);
	t.distance = speed;
	t.direction = (rand() % 9) * M_PI/4;
	return t;
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
