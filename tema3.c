#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "functions.h"
int main(void)
{
	char command[20], file_name[30], current_file[30] = "none";
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, type = 0, height = 0, width = 0;
	int **img, err, exit = 0, empty = 1, **copy, ch = 0, cw = 0, sel_all = 0;
	int was_load = 0, was_sel = 0;
	scanf("%s", command);
	while (exit == 0) {
		if (strcmp(command, "LOAD") == 0) {
			was_load = 1;
			if (empty == 0) {
				for (int i = 0; i < height; i++)
					free(img[i]);
				free(img);
				empty = 1;
			}
			scanf("%s", file_name);
			err = get_error(file_name, current_file, &type, &height, &width);
			if (err == 0) {
				allocate_matrix(&img, height, width, type, current_file);
				printf("Loaded %s\n", file_name);
				empty = 0;
			} else
				printf("Failed to load %s\n", file_name);
		}
		if (strcmp(command, "SELECT") == 0) {
			if (strcmp(current_file, "none") == 0) {
				printf("No image loaded\n");
			} else {
				char aux[10];
				scanf("%s", aux);
				if (isdigit(aux[0])) {
					sel_all = 0;
					x1 = 0;
					for (unsigned int i = 0; i < strlen(aux); i++) {
						x1 = x1 * 10 + (aux[i] - '0');
					}
					scanf("%d%d%d", &y1, &x2, &y2);
					sel(&img, x1, x2, y1, y2, &copy, type, was_sel, &ch, &cw);
					copy_dim(&x1, &x2, &y1, &y2, &ch, &cw, type);
				} else {
					s_all(&img, height, width, &copy, type, was_sel, &ch, &cw);
					sel_all = 1;
				}
			}
		}
		if (strcmp(command, "CROP") == 0) {
			if (strcmp(current_file, "none") == 0) {
				printf("No image loaded\n");
			} else {
				if (sel_all == 1) {
					int rgb = 1;
					if (type == 3 || type == 6)
						rgb = 3;
					ch = height;
					cw = width * rgb;
				}
				crop(&img, &copy, &height, &width, &ch, &cw, type);
			}
		}
		if (strcmp(command, "ROTATE") == 0) {
			int angle;
			scanf("%d", &angle);
			if (was_load == 1)
				rotate(&img, &height, &width, angle, type);
			else
				rotate(&copy, &ch, &cw, angle, 1);
		}
		if (strcmp(command, "SAVE") == 0) {
			save(img, height, width, type);
		}
		if (strcmp(command, "EXIT") == 0) {
			exit = 1;
		}
		if (exit == 0)
			scanf("%s", command);
	}
	if (empty == 0)
		free_mem(&img, height, &copy, ch);
	return 0;
}
