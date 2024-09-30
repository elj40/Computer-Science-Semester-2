#ifndef POINT_H
#define POINT_H

typedef struct Point {
	int row;
	int col;
} Point;

typedef struct PathNode {
	Point point;
	struct PathNode *next;
	struct PathNode *previous;
} PathNode;

#endif
