#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <math.h>

enum mtrx {rotation = 3, transformation};
enum pair {rotary_type_1, rotary_type_2, moving};

struct Robot
{
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtrx;
};

void analyze_text(const char *str, struct Robot *robot);

void init_rgm(const char *filename, struct Robot *robot)
{
	char c;
	char *str;
	char *type_pair, *lenght_pair, *coords, *rotation_matrix;
	const char *TYPE_PAIR = "TYPE_PAIR";
	const char *LENGTH_PAIR = "LENGTH_PAIR";
	const char *COORDS = "COORDS";
	const char *ROTATION_MATRIX = "ROTATION_MATRIX";
	int size_str = 1;
	enum {tp = 9, lp = 11, co = 6, rm = 15, cm = 7};
	enum {start = 33, end = 126};
	FILE *robot_characteristics;
	type_pair = malloc(sizeof(char));
	lenght_pair = malloc(sizeof(char));
	coords = malloc(sizeof(char));
	rotation_matrix = malloc(sizeof(char));
	robot_characteristics = fopen(filename, "r");
	if (!robot_characteristics)
	{
		perror(filename);
		exit(1);
	}
	str = malloc((size_str+1) * sizeof(char));
	str[size_str] = '\0';
	while ((c = fgetc(robot_characteristics)) != EOF)
	{
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			if (c == '}')
			{
				if (!strncmp(str, TYPE_PAIR, sizeof(char)*tp))
				{
					type_pair = malloc((size_str+1) * sizeof(char));
					strcpy(type_pair, str);
				}
				if (!strncmp(str, LENGTH_PAIR, sizeof(char)*lp))
				{
					lenght_pair = malloc((size_str+1) * sizeof(char));
					strcpy(lenght_pair, str);

				}
				if (!strncmp(str, COORDS, sizeof(char)*co))
				{
					coords = malloc((size_str+1) * sizeof(char));
					strcpy(coords, str);
				}
				if (!strncmp(str, ROTATION_MATRIX, sizeof(char)*rm))
				{
					rotation_matrix = malloc((size_str+1) * sizeof(char));
					strcpy(rotation_matrix, str);
				}
				size_str = 0;
			}
			size_str++;
			str = realloc(str, (size_str+1) * sizeof(char));
			if (!str)
			{
				perror(str);
				exit(2);
			}
			str[size_str] = '\0';
		}
	}
	printf("%s\n", type_pair);
	printf("%s\n", lenght_pair);
	printf("%s\n", coords);
	printf("%s\n", rotation_matrix);
	fclose(robot_characteristics);
	free(str);
	free(type_pair);
	free(lenght_pair);
	free(coords);
	free(rotation_matrix);
}

void analyze_text(const char *str, struct Robot *robot)
{

}

int main (int argc, char **argv)
{
	struct Robot robot;
	if (argc > 1)
		init_rgm(argv[1], &robot);
	else
		puts("To few arguments");
	return 0;
}
