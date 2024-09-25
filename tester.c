#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "utils.h"
#include "types.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void test_compass(int row, int col, int speed, int size, int moves);
int round_away_from_zero(float n);

int main(int argc, char* argv[]) {

	int n;
	bool b;

	printf("string_to_int:\n");
	b = string_to_int(&n, "12345");
	assert(b && "Boolean not correct");
	if (b) assert(n == 12345 && "Number not correct");
	printf(".");

	b = string_to_int(&n, "12f45");
	assert(!b && "Boolean not correct");
	if (b) assert(n == 12345 && "Number not correct");
	printf(".");

	printf("\n");
	printf("string_to_float:\n");
	float f;
	float eps = 0.00001;

	b = string_to_float(&f, "213");
	assert(b && "string_to_float failed");
	if (b) assert(f == 213.0 && "Number not correct");
	printf(".");

	b = string_to_float(&f, "213.12");
	assert(b && "string_to_float failed");
	if (b) assert(f > 213.12 - eps && f < 213.2 + eps && "Number not correct");
	printf(".");

	b = string_to_float(&f, "0.123456");
	assert(b && "string_to_float failed");
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");
	printf(".");

	
	b = string_to_float(&f, "0.123.456");
	assert(!b && "string_to_float failed");
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");
	printf(".");

	printf("\n");
	printf("test_compass\n");
	test_compass(1,1,1,10,8);
	printf("\n");
	test_compass(2,1,2,10,8);
	printf("\n");
	test_compass(8,8,2,10,8);
	return 0;
}

void test_compass(int row, int col, int speed, int size, int moves) {
	/* Tests finding the next position of an entitiy given a direction and speed */
	/* Must stay within an n by n grid where n = size */
	// Here we loop around all the axes
	// TODO: convert this into an assertable function
	//
	//
	float epsilon = 0.001;
	for (int s = 1; s < 4; s++) {
		printf("\n");
		for (int i = 0; i < moves; i++) {
			/* float dir = get_next_trajectory(); */
			float dir = i * M_PI/4;



			float cos_dir = cos(dir);
			float sin_dir = sin(dir);
			
			int dc = 0;
			int dr = 0;

			if (sin_dir > epsilon) dr = s;
			else if(sin_dir < -epsilon) dr = -s;

			if (cos_dir > epsilon) dc = s;
			else if(cos_dir < -epsilon) dc = -s;
			
			int nc = col + dc;
			int nr = row + dr;

			nc = MIN(size-1, MAX(nc, 0));
			nr = MIN(size-1, MAX(nr, 0));







			printf("r:%d, c:%d, dist:%d, dir:%5.1f -> nr:%d, nc:%d\n",row, col, s, dir / M_PI * 180, nr, nc);
		}
	}
}

int round_away_from_zero(float n) {
	if (n < 0) return (int)-round(-n);
	return (int)round(n);
}
