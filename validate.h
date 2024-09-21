#ifndef VALIDATE_H
#define VALIDATE_H

bool validate_gui_mode(int argc,char* argv[], int *gm);
bool validate_config_line(char *config_line, int config_len, int *s, int *d, int *t, int *a);

#endif
