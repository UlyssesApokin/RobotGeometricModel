#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <math.h>

enum mtrx {rotation = 3, transformation};
enum pair {rotary_type_1, rotary_type_2, moving};

struct Robot
{
	int num;
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtrx;
};

void fill_type_pair(const char *str, struct Robot *p_robot);
void fill_length_pair(const char *str, struct Robot *p_robot);
void fill_coords(const char *str, struct Robot *p_robot);
void fill_rotation_matrix(const char *str, struct Robot *p_robot);

void init_rgm(const char *filename, struct Robot *p_robot)
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
		enum {tp = 9, lp = 11, co = 6, rm = 15, cm = 7};
		enum {start = 33, end = 126};
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			if (c == '$')
			{	
				printf("String:\t%s\n", str);
				const char *TYPE_PAIR = "TYPE_PAIR";
				const char *LENGTH_PAIR = "LENGTH_PAIR";
				const char *COORDS = "COORDS";
				const char *ROTATION_MATRIX = "ROTATION_MATRIX";
				if (!strncmp(str, TYPE_PAIR, sizeof(char)*tp))
					fill_type_pair(str, p_robot);
				if (!strncmp(str, LENGTH_PAIR, sizeof(char)*lp))
					fill_length_pair(str, p_robot);
				if (!strncmp(str, COORDS, sizeof(char)*co))
					fill_coords(str, p_robot);
				if (!strncmp(str, ROTATION_MATRIX, sizeof(char)*rm))
					fill_rotation_matrix(str, p_robot);
				size_str = 0;
			}
			size_str++;
			str = realloc(str, (size_str+1) * sizeof(char));
			str[size_str] = '\0';
		}
	}
	fclose(robot_characteristics);
	free(str);
}

void fill_type_pair(const char *str, struct Robot *p_robot)
{
	enum {maxline = 14};
	char c = '\0';
	char *type_pair = malloc(sizeof(char) * maxline);
	int *vec_of_pair;
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		const char *ROTARY_TYPE_1 = "ROTARY_TYPE_1";
		const char *ROTARY_TYPE_2 = "ROTARY_TYPE_2";
		const char *MOVING = "MOVING";
		c = str[i];
		if (c == ']')
		{
			is_value = 0;
			j = 0;
			if (!strcmp(type_pair, ROTARY_TYPE_1))
			{
				vec_of_pair = realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 1;
			}
			else if (!strcmp(type_pair, ROTARY_TYPE_2))
			{
				vec_of_pair = realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 2;
			}
			else if (!strcmp(type_pair, MOVING))
			{
				vec_of_pair = realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 3;
			}
			else
			{
				fprintf(stderr, "Invalid argument:TYPE_PAIR#%d:<%s>\n", num_of_pair, type_pair);
				exit(2);
			}
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr, "Invalid argument:TYPE_PAIR#%d:<%s>\n", num_of_pair, type_pair);
				exit(2);
			}
			type_pair[j] = str[i];
			type_pair[j+1] = '\0';
			j++;
		}
		if (c == '[')
			is_value = 1;
		i++;
	} while (c != '\0');
	p_robot->pvec = vec_of_pair;
	p_robot->num = num_of_pair;
	free(type_pair);

}

void fill_length_pair(const char *str, struct Robot *p_robot)
{
	enum {maxline = 33};
	char c = '\0';
	char *length_pair = malloc(maxline * sizeof(char));
	double *p_lvec;
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		
		c = str[i];
		if (c == ']')
		{
			int t;
			j = 0;
			is_value = 0;
			p_lvec = realloc(p_lvec, sizeof(double)*(num_of_pair+1));
			t = atof(length_pair);
			if (errno == ERANGE)
			{
				fprintf(stderr, "Invalid argument:LENGTH_PAIR#%d:<%s>\n", num_of_pair, length_pair);
				exit(2);
			}
			p_lvec[num_of_pair] = t;
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr, "Invalid argument:LENGTH_PAIR#%d:<%s>\n", num_of_pair, length_pair);
				exit(2);
			}
			length_pair[j] = str[i];
			length_pair[j+1] = '\0';
			j++;
		}
		if (c == '[')
			is_value = 1;
		i++;
	} while (c != '\0');
	p_robot->lvec = p_lvec;
	free(length_pair);
}

void fill_coords(const char *str, struct Robot *robot)
{
	
}

void fill_rotation_matrix(const char *str, struct Robot *robot)
{
	
}

int main (int argc, char **argv)
{
	int i;
	struct Robot robot;
	if (argc > 1)
		init_rgm(argv[1], &robot);
	else
		fprintf(stderr, "To few arguments\n");
	/*Debug output*/
	for (i = 0; i < robot.num; i++)
		printf("Type of pair[%d]:\t%d\n", i, robot.pvec[i]);
	for (i = 0; i < robot.num*3; i++)
		printf("Length_of_pair[%d]:\t%lf\n", i/3, robot.lvec[i]);
	return 0;
}
