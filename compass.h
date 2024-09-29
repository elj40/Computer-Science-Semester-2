#ifndef COMPASS_H
#define COMPASS_H

Trajectory get_random_trajectory(int speed);
Trajectory get_trajectory_from_target(int r, int c,int speed, int tr, int tc);
void get_next_position_from_trajectory(Trajectory t, int n, int r, int c, int *nr, int *nc);

#endif
