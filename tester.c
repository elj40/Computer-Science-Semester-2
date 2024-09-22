#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "validate.h"
#include "utils.h"
#include "types.h"
#include "compass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void test_compass(int row, int col, int speed, int size, int moves);
int cast_to_int(float n);

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
	return 0;
}

void test_compass(int row, int col, int speed, int size, int moves) {
	for (int i = 0; i < moves; i++) {
		/* float dir = get_next_trajectory(); */
		float dir = i * M_PI/4;
		
		float dc = speed*cos(dir);
		float dr = speed*sin(dir);

		int i_dc = cast_to_int(dc);
		int i_dr = cast_to_int(dr);
		
		int nc = col + i_dc;
		int nr = row + i_dr;

		nc = MIN(size, MAX(nc, 0));
		nr = MIN(size, MAX(nr, 0));

		printf("%d, %d, %f\n", nr, nc, dir / M_PI * 180);
	}
}

int cast_to_int(float n) {
	if (n < 0) return (int)-round(-n);
	return (int)round(n);
}
