#include <math.h>
#include <stdlib.h>

#include "types.h"


struct Trajectory get_next_trajectory(int speed) {
	struct Trajectory t;
	t.distance = rand() % (speed+1);
	t.direction = (rand() % 9) * M_PI/4;
	return t;
}
