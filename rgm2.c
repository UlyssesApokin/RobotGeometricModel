/*
Copyright (c) <2023> <Ulysses Apokin (Jelisej Apokin)>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
	int nump;
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtx;
};

void fill_type_pair(const char *str, struct Robot *p_robot);
void fill_length_pair(const char *str, struct Robot *p_robot);
void fill_coords(const char *str, struct Robot *p_robot);
void fill_rotation_matrix(const char *str, struct Robot *p_robot);

void init_rgm(const char *filename, struct Robot *p_robot)
{
	int size_str = 1;
	char c;
	char *str = malloc((size_str+1) * sizeof(char));
	FILE *robot_characteristics;
	robot_characteristics = fopen(filename, "r");
	if (!robot_characteristics)
	{
		perror(filename);
		exit(1);
	}
	str[size_str] = '\0';
	while ((c = fgetc(robot_characteristics)) != EOF)
	{
		enum {tp = 12, lp = 12, co = 22, rm = 15};
		enum {start = 33, end = 126};
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			if (c == '$')
			{	
				/*printf("String: %s\n\n", str);*/
				const char *TYPE_PAIR = "TYPE_OF_PAIR";
				const char *LENGTH_PAIR = "SIZE_OF_PAIR";
				const char *COORDS = "GENERALIZED_COORDINATE";
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
	enum {maxline = 15};
	char c = '\0';
	char *type_pair = malloc(sizeof(char) * maxline);
	int *vec_of_pair;
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		const char *TURNING_TYPE_1 = "TURNING_TYPE_1";
		const char *TURNING_TYPE_2 = "TURNING_TYPE_2";
		const char *SLIDING = "SLIDING";
		c = str[i];
		if (c == ']')
		{
			is_value = 0;
			j = 0;
			if (!strcmp(type_pair, TURNING_TYPE_1))
			{
				vec_of_pair =
				realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 1;
			}
			else if (!strcmp(type_pair, TURNING_TYPE_2))
			{
				vec_of_pair =
				realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 2;
			}
			else if (!strcmp(type_pair, SLIDING))
			{
				vec_of_pair =
				realloc(vec_of_pair, (num_of_pair + 1) * sizeof(int));
				vec_of_pair[num_of_pair] = 3;
			}
			else
			{
				fprintf(stderr,
					"Invalid argument:TYPE_PAIR#%d:<%s>\n",
						num_of_pair, type_pair);
				exit(2);
			}
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr,
					"Invalid argument:TYPE_PAIR#%d:<%s>\n",
						num_of_pair, type_pair);
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
	p_robot->nump = num_of_pair;
	free(type_pair);

}

void fill_length_pair(const char *str, struct Robot *p_robot)
{
	enum {maxline = 33};
	char c = '\0';
	char *length_pair = malloc(maxline * sizeof(char));
	double *p_lvec = malloc(sizeof(double));
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		c = str[i];
		if (c == ']')
		{
			double t;
			j = 0;
			is_value = 0;
			p_lvec = realloc(p_lvec, sizeof(double)*(num_of_pair+1));
			t = (float)atof(length_pair);
			if (errno == ERANGE)
			{
				fprintf(stderr,
					"Invalid argument:LENGTH_PAIR#%d:<%s>\n",
						num_of_pair, length_pair);
				exit(2);
			}
			p_lvec[num_of_pair] = t;
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr,
					"Invalid argument:LENGTH_PAIR#%d:<%s>\n",
						num_of_pair, length_pair);
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

void fill_coords(const char *str, struct Robot *p_robot)
{
	enum {maxline = 33};
	char c = '\0';
	char *coords = malloc(maxline * sizeof(char));
	double *p_qvec = malloc(sizeof(double));
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		
		c = str[i];
		if (c == ']')
		{
			double t;
			j = 0;
			is_value = 0;
			p_qvec = realloc(p_qvec, sizeof(double)*(num_of_pair+1));
			t = atof(coords);
			if (errno == ERANGE)
			{
				fprintf(stderr,
					"Invalid argument:COORDS#%d:<%s>\n",
						num_of_pair, coords);
				exit(2);
			}
			p_qvec[num_of_pair] = t;
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr,
					"Invalid argument:COORDS#%d:<%s>\n",
						num_of_pair, coords);
				exit(2);
			}
			coords[j] = str[i];
			coords[j+1] = '\0';
			j++;
		}
		if (c == '[')
			is_value = 1;
		i++;
	} while (c != '\0');
	p_robot->qvec = p_qvec;
	free(coords);	
}

void fill_rotation_matrix(const char *str, struct Robot *p_robot)
{
	enum {maxline = 33};
	char c = '\0';
	char *rot_matrix = malloc(maxline * sizeof(char));
	double *rmtx = malloc(sizeof(double));
	int i = 0, j = 0;
	int is_value = 0;
	int num_of_pair = 0;
	do
	{
		
		c = str[i];
		if (c == ']')
		{
			double t;
			j = 0;
			is_value = 0;
			rmtx = realloc(rmtx, sizeof(double)*(num_of_pair+1));
			t = atof(rot_matrix);
			if (errno == ERANGE)
			{
				fprintf(stderr,
					"Invalid argument:COORDS#%d:<%s>\n",
						num_of_pair, rot_matrix);
				exit(2);
			}
			rmtx[num_of_pair] = t;
			num_of_pair++;
		}
		if (is_value == 1)
		{
			if (j >= maxline - 1)
			{
				fprintf(stderr,
					"Invalid argument:COORDS#%d:<%s>\n",
						num_of_pair, rot_matrix);
				exit(2);
			}
			rot_matrix[j] = str[i];
			rot_matrix[j+1] = '\0';
			j++;
		}
		if (c == '[')
			is_value = 1;
		i++;
	} while (c != '\0');
	p_robot->rmtx = rmtx;
	free(rot_matrix);		
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
	for (i = 0; i < robot.nump; i++)
		printf("Type of pair[%d]:\t%d\n", i, robot.pvec[i]);
	for (i = 0; i < robot.nump*3; i++)
		printf("Size of pair[%d][%d]:\t%lf\n", i/3, i%3, robot.lvec[i]);
	for (i = 0; i < robot.nump; i++)
		printf("Coords[%d]:\t%lf\n", i, robot.qvec[i]);
	for (i = 0; i < (robot.nump+1)*9; i++)
		printf("Matrix[%d][%d]:\t%lf\n", i/3,i%3, robot.rmtx[i]);
	return 0;
}
