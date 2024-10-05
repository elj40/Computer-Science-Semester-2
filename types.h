#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include "point.h"
#define MAX_MAP_SIZE 100
#define MAX_FLOWER_POLLEN 100
#define MAX_BEE_POLLEN 200
#define MAX_POLLEN_LEN 100
#define MAX_POLLEN_CHARS 50

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define EPS 0.01

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };
enum BeeRole { FORAGER, SCOUT };
enum BeeType { NORMAL, HONEY, DESERT };
enum PollinatorState { WANDER, RETURN, SEEK, DORMANT };


union Pollen {
	float float_info;
	char string_info[MAX_POLLEN_CHARS];
};

typedef struct {
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
} Config;

typedef struct {
	int row;
	int col;
	char type;
	int pollen_len;
	enum PollenType pollen_type;
	union Pollen pollen[MAX_FLOWER_POLLEN];
} Hive;

typedef struct Bee {
	int id;
	int row;
	int col;
	int speed;
	int perception;

	Point flower_location;
	Trajectory * flower_path;
	int flower_path_len;
	int flower_path_index;

	Hive *hive_ptr;

	union Pollen pollen;
	enum BeeRole role;	
	enum PollinatorState state;
	enum BeeType type;
} Bee;


typedef struct {
	int id;
	int row;
	int col;
	int speed;
	Hive *hive_ptr;
} Wasp;

typedef struct BeeNode {
	Bee bee;
	struct BeeNode *next_ptr;
} BeeNode;

typedef struct WaspNode {
	Wasp wasp;
	struct WaspNode *next_ptr;
} WaspNode;

union Pollinator {
	Bee bee;
	Wasp wasp;
};
typedef struct {
	int row;
	int col;
	int pollen_len;
	bool fight;
	enum PollenType pollen_type;
	union Pollen pollen[MAX_FLOWER_POLLEN];
} Flower;

typedef struct {
	int distance;
	float direction;
} Trajectory;

#endif
