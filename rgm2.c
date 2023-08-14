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

struct Robot
{
	int nump;
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtx;
};

void fill_parameter(const char *str, struct Robot *p_robot,
	int parameter)
{
	enum {maxline = 63};
	enum {pvec, lvec, qvec, rmtx};
	char c;
	int i = 0, j = 0;
	int reading_started = 0;
	int num_of_val = 0;
	char *str_of_param = malloc(sizeof(char) * (maxline + 1));
	int *int_of_param = malloc(sizeof(int));
	double *double_of_param = malloc(sizeof(double));
	do
	{
		c = str[i];
		if (c == ']')
		{
			const char *TURNING_TYPE_1 = "TURNING_TYPE_1";
			const char *TURNING_TYPE_2 = "TURNING_TYPE_2";
			const char *SLIDING = "SLIDING";
			reading_started = 0;
			j = 0;
			if (parameter == 0)
			{
				int_of_param = realloc(int_of_param, sizeof(int) * (num_of_val + 1));
				if (!strcmp(str_of_param, TURNING_TYPE_1))
					int_of_param[num_of_val] = 1;
				else if (!strcmp(str_of_param, TURNING_TYPE_2))
					int_of_param[num_of_val] = 2;
				else if (!strcmp(str_of_param, SLIDING))
					int_of_param[num_of_val] = 3;
			}
			else
			{
				double_of_param = realloc(double_of_param, sizeof(double) * (num_of_val + 1));
				double_of_param[num_of_val] = atof(str_of_param);
			}
			num_of_val++;
		}
		if (reading_started == 1)
		{
			if (j >= maxline)
			{
				fprintf(stderr, "Buffer overflow");
				exit(2);
			}
			str_of_param[j] = str[i];
			str_of_param[j+1] = '\0';
			j++;
		}
		if (c == '[')
		{
			reading_started = 1;
		}
		i++;
	} while (c != '\0');
	if (parameter == pvec)
	{
		p_robot->pvec = int_of_param;
		p_robot->nump = num_of_val;
		free(double_of_param);
	}
	else if (parameter == lvec)
	{
		p_robot->lvec = double_of_param;
		free(int_of_param);
	}
	else if (parameter == qvec)
	{
		p_robot->qvec = double_of_param;
		free(int_of_param);
	}
	else if (parameter == rmtx)
	{
		p_robot->rmtx = double_of_param;
		free(int_of_param);
	}
	else
	{
		free(double_of_param);
		free(int_of_param);
	}
	free(str_of_param);
}

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
	while ((c = fgetc(robot_characteristics)) != EOF)
	{
		enum {start = 33, end = 126};
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			if (c == '$')
			{	
				enum {pvec, lvec, qvec, rmtx};
				enum {tp = 12, lp = 12, co = 22, rm = 15};
				const char *TYPE_PAIR = "TYPE_OF_PAIR";
				const char *LENGTH_PAIR = "SIZE_OF_PAIR";
				const char *COORDS = "GENERALIZED_COORDINATE";
				const char *ROTATION_MATRIX = "ROTATION_MATRIX";
				if (!strncmp(str, TYPE_PAIR, sizeof(char)*tp))
					fill_parameter(str, p_robot, pvec);
				if (!strncmp(str, LENGTH_PAIR, sizeof(char)*lp))
					fill_parameter(str, p_robot, lvec);
				if (!strncmp(str, COORDS, sizeof(char)*co))
					fill_parameter(str, p_robot, qvec);
				if (!strncmp(str, ROTATION_MATRIX, sizeof(char)*rm))
					fill_parameter(str, p_robot, rmtx);
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
		printf("Matrix[%d][%d][%d]:\t%lf\n", i/9, (i/3)%3,i%3, robot.rmtx[i]);
	return 0;
}
