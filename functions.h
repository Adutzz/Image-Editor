#include <string.h>
#include <stdlib.h>

void free_mem(int ***img, int h, int ***copy, int ch)
{
	for (int i = 0; i < h; i++)
		free((*img)[i]);
	free((*img));

	for (int i = 0; i < ch; i++)
		free((*copy)[i]);
	free((*copy));
}

int get_error(char *file_name, char *cur_file, int *type, int *h, int *w)
{
	strcpy(cur_file, file_name);
	FILE *file = fopen(cur_file, "rt");
	if (!file) {
		return 1;
	}
	char aux[2];
	fscanf(file, "%s", aux);
	if (strcmp(aux, "P1") == 0)
		*type = 1;
	else if (strcmp(aux, "P2") == 0)
		*type = 2;
	else if (strcmp(aux, "P3") == 0)
		*type = 3;
	else if (strcmp(aux, "P4") == 0)
		*type = 4;
	else if (strcmp(aux, "P5") == 0)
		*type = 5;
	else if (strcmp(aux, "P6") == 0)
		*type = 6;
	fscanf(file, "%d%d", &(*w), &(*h));
	fclose(file);
	return 0;
}

void allocate_matrix(int ***img, int h, int w, int type, char *cur_file)
{
	int rgb = 1;
	if (type == 3 || type == 6)
		rgb = 3;

	(*img) = (int **)malloc(h * sizeof(int *));
	if (!(*img)) {
		printf("error: failed dynamic allocation\n");
		return;
	}
	for (int i = 0; i < h; i++) {
		(*img)[i] = (int *)malloc(w * rgb * sizeof(int));
		if (!(*img)[i]) {
			printf("error: failed dynamic allocation\n");
			return;
		}
	}

	if (type <= 3) {
		FILE *file = fopen(cur_file, "rt");
		if (!file) {
			fprintf(stderr, "error: file");
			return;
		}
		char aux[3];
		int maxval;
		fscanf(file, "%s\n%d %d\n%d\n", aux, &w, &h, &maxval);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w * rgb; j++) {
				fscanf(file, "%d", &((*img)[i][j]));
			}
		}
		fclose(file);
	} else {
		FILE *file = fopen(cur_file, "rb");
		if (!file) {
			fprintf(stderr, "error: file");
			return;
		}

		char header[3];
		int maxval;
		fgets(header, sizeof(header), file);
		fscanf(file, "%d %d\n%d\n", &w, &h, &maxval);

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w * rgb; j++) {
				unsigned char pixel;
				fread(&pixel, sizeof(unsigned char), 1, file);
				(*img)[i][j] = (int)pixel;
			}
		}
		fclose(file);
	}
}

void save(int **img, int h, int w, int type)
{
	int rgb = 1, ascii = 0;
	if (type == 3 || type == 6)
		rgb = 3;

	char saved_file[30], line[100], lineaux[100];
	fgets(line, sizeof(line), stdin);
	line[strlen(line) - 1] = '\0';
	strcpy(lineaux, line + 1);

	if (strstr(lineaux, "ascii") != 0) {
		ascii = 1;
		char *token = strtok(lineaux, " ");
		if (token) {
			strcpy(saved_file, token);
		}
	} else {
		strcpy(saved_file, lineaux);
	}
	FILE *file;
	if (ascii) {
		file = fopen(saved_file, "wt");
	} else {
		file = fopen(saved_file, "wb");
	}
	if (!file) {
		perror("fopen");
		return;
	}

	if (ascii == 1) {
		int new_type;
		if (type == 2 || type == 5)
			new_type = 2;
		if (type == 3 || type == 6)
			new_type = 3;
		fprintf(file, "P%d\n%d %d\n255\n", new_type, w, h);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w * rgb; j++) {
				int pixel = img[i][j];
				fprintf(file, "%d ", pixel);
			}
			fprintf(file, "\n");
		}
	} else {
		int new_type;
		if (type == 2 || type == 5)
			new_type = 5;
		if (type == 3 || type == 6)
			new_type = 6;
		fprintf(file, "P%d\n%d %d\n255\n", new_type, w, h);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w * rgb; j++) {
				unsigned char pixel = (unsigned char)img[i][j];
				fwrite(&pixel, sizeof(unsigned char), 1, file);
			}
		}
	}

	fclose(file);
	printf("Saved %s\n", saved_file);
}

void copy_dim(int *x1, int *x2, int *y1, int *y2, int *ch, int *cw, int t)
{
	if (*x1 > *x2) {
		int aux = *x1;
		*x1 = *x2;
		*x2 = aux;
	}
	if (*y1 > *y2) {
		int aux = *y1;
		*y1 = *y2;
		*y2 = aux;
	}

	int rgb = 1;
	if (t == 3 || t == 6) {
		rgb = 3;
	}
	*ch = *y2 - *y1;
	*cw = (*x2 - *x1) * rgb;
}

void sel(int ***img, int x1, int x2, int y1, int y2, int ***cp, int t,
		 int s, int *ch, int *cw)
{
	if (x1 > x2) {
		int aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y1 > y2) {
		int aux = y1;
		y1 = y2;
		y2 = aux;
	}

	int rgb = 1;
	if (t == 3 || t == 6) {
		rgb = 3;
	}

	// Free previous selection memory
	if (s == 1 && *cp) {
		for (int i = 0; i < *ch; i++) {
			free((*cp)[i]);
		}
		free(*cp);
		*cp = NULL;
	}

	*ch = y2 - y1;
	*cw = (x2 - x1) * rgb;

	*cp = (int **)malloc(*ch * sizeof(int *));
	if (!*cp) {
		printf("Error: Failed dynamic allocation\n");
		return;
	}
	for (int i = 0; i < *ch; i++) {
		(*cp)[i] = (int *)malloc(*cw * sizeof(int));
		if (!(*cp)[i]) {
			printf("Error: Failed dynamic allocation\n");
			return;
		}
	}

	int l = 0, k = 0;
	for (int i = y1; i < y2; i++) {
		for (int j = x1 * rgb; j < x2 * rgb; j++) {
			(*cp)[l][k] = (*img)[i][j];
			k++;
		}
		l++;
		k = 0;
	}

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void s_all(int ***img, int h, int w, int ***copy, int t, int s,
		   int *ch, int *cw)
{
	int rgb = 1;
	if (t == 3 || t == 6)
		rgb = 3;

	// Free previous selection memory
	if (s == 1 && *copy) {
		for (int i = 0; i < *ch; i++) {
			free((*copy)[i]);
		}
		free(*copy);
		*copy = NULL;
	}

	*ch = h;
	*cw = w * rgb;

	*copy = (int **)malloc(h * sizeof(int *));
	if (!*copy) {
		printf("Error: Failed dynamic allocation\n");
		return;
	}
	for (int i = 0; i < h; i++) {
		(*copy)[i] = (int *)malloc(*cw * sizeof(int));
		if (!(*copy)[i]) {
			printf("Error: Failed dynamic allocation\n");

			// Free any previously allocated rows
			for (int j = 0; j < i; j++) {
				free((*copy)[j]);
			}
			free(*copy);
			*copy = NULL;
			return;
		}
	}

	// Copy entire image into `copy`
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < *cw; j++) {
			(*copy)[i][j] = (*img)[i][j];
		}
	}

	printf("Selected ALL\n");
}

void crop(int ***img, int ***copy, int *h, int *w, int *ch, int *cw, int type)
{
	for (int i = 0; i < *h; i++)
		free((*img)[i]);
	free(*img);

	*img = (int **)malloc(*ch * sizeof(int *));
	if (!*img) {
		printf("Error: Failed dynamic allocation\n");
		return;
	}
	for (int i = 0; i < *ch; i++) {
		(*img)[i] = (int *)malloc(*cw * sizeof(int));
		if (!(*img)[i]) {
			printf("Error: Failed dynamic allocation\n");
			for (int j = 0; j < i; j++) {
				free((*img)[j]);
			}
			free(*img);
			return;
		}
	}

	for (int i = 0; i < *ch; i++) {
		for (int j = 0; j < *cw; j++) {
			(*img)[i][j] = (*copy)[i][j];
		}
	}

	*h = *ch;
	int rgb = 1;
	if (type == 3 || type == 6) {
		rgb = 3;
	}
	*w = *cw / rgb;

	printf("Image cropped\n");
}

void rotate(int ***img, int *h, int *w, int angle, int type)
{
	int rgb = 1;
	if (type == 3 || type == 6)
		rgb = 3;

	int **rotated = (int **)malloc(*w * sizeof(int *));
	if (!rotated) {
		printf("error: failed dynamic allocation\n");
		return;
	}
	for (int i = 0; i < *w; i++) {
		rotated[i] = (int *)malloc(*h * rgb * sizeof(int));
		if (!rotated[i]) {
			printf("error: failed dynamic allocation\n");
			return;
		}
	}

	// Rotated to the left
	int l = 0, k = 0;
	for (int i = *h - 1; i >= 0; i--) {
		for (int j = 0; j < *w * rgb; j++) {
			rotated[l][k] = (*img)[i][j];
			k++;
		}
		l++;
		k = 0;
	}

	for (int i = 0; i < *h; i++)
		free((*img)[i]);
	free((*img));

	*img = rotated;
	int temp = *h;
	*h = *w;
	*w = temp;
	angle = 0;
	printf("Image rotated by %d degrees\n", angle);
}
