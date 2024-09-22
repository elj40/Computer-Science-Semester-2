#include <math.h>
#include <stdlib.h>

float get_next_trajectory() {
	return (rand() % 9) * M_PI/4;
}
