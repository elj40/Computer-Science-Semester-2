#ifndef UTILS_H
#define UTILS_H

void split_string(char *buffer[], int buf_len, char *string, const char *delims);
bool string_to_int(int *n, char *s);
bool string_to_float(float *n, char *s);
void invalid_object_setup(int l);

#endif

