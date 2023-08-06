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
	int size_str = 1;
	FILE *robot_characteristics;
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
		char *type_pair, *lenght_pair, *coords, *rotation_matrix;
		enum {start = 33, end = 126};
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			if (c == '}')
			{
				printf("%s\n", str);
				size_str = 0;
			}
			size_str++;
			str = realloc(str, (size_str+1) * sizeof(char));
			str[size_str] = '\0';
			if (!str)
			{
				perror(str);
				exit(2);
			}
		}
	}
	fclose(robot_characteristics);
	free(str);
}

void analyze_text(const char *str, struct Robot *robot)
{

}

int main (int argc, char **argv)
{
	struct Robot robot;
	init_rgm(argv[1], &robot);
	return 0;
}
