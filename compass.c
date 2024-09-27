#include <math.h>
#include <stdlib.h>

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
