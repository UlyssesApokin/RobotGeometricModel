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

#include "rgm_fifo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <math.h>

/**
 * @pvec is vector number defining the type and sequence 
 * of kinematic pairs
 * @lvec is the number of the array of vectors defining the shape
links
 * @qvec is vector number defening the generalized coordinates
 * of kinematic pairs
 * @rmtx is the number of the array of rotation matix
 * @inar is the number of error of invalid argument
 * @misb is the number of error of misspelled square bracket
 */
enum {pvec, lvec, qvec, rmtx, inar, misb};

struct Robot
{
	int nump;
	int *pvec;
	double *lvec;
	double *qvec;
	double *rmtx;
};

void print_rgm_attr_error(const char *str, int num_of_iter,
		int param, int misstake)
{
	switch (misstake)
	{
	case inar:
		fprintf(stderr, "Invalid argument:");
		break;
	case misb:
		fprintf(stderr, "Misspelled square bracket:");
		break;
	default:
		fprintf(stderr, "API is denied\n");
		return;
	}
	switch (param)
	{
	case pvec:
		fprintf(stderr, "TYPE_OF_PAIR");
		break;
	case lvec:
		fprintf(stderr, "SIZE_OF_PAIR");
		break;
	case qvec:
		fprintf(stderr, "GENERALIZED_COORDINATE");
		break;
	case rmtx:
		fprintf(stderr, "ROTATION_MATRIX");
		break;
	default:
		fprintf(stderr, "non-existent parameter");
	}
	fprintf(stderr, "#%d", num_of_iter);
	fprintf(stderr, "<%s>\n", str);
}
void set_struct_robot(void *data, struct Robot *p_robot, int attr)
{
	
}

void get_data_from_rgm_section(const char *str, struct Robot *p_robot, int attr)
{
	enum {maxline = 63};
	char c;
	int i = 0, j = 0;
	int reading_started = 0;
	int num_of_val = 0;
}

void fill_param(const char *str, struct Robot *p_robot, int param)
{
	enum {maxline = 63};
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
			if (!reading_started)
			{
				print_rgm_attr_error(str_of_param, num_of_val-1,
						param, misb);
			}
			reading_started = 0;
			j = 0;
			
			if (param == pvec)
			{
				const char *TURNING_TYPE_1 = "TURNING_TYPE_1";
				const char *TURNING_TYPE_2 = "TURNING_TYPE_2";
				const char *SLIDING = "SLIDING";
				int_of_param = realloc(int_of_param,
						sizeof(int) * (num_of_val + 1));
				if (!strcmp(str_of_param, TURNING_TYPE_1))
					int_of_param[num_of_val] = 1;
				else if (!strcmp(str_of_param, TURNING_TYPE_2))
					int_of_param[num_of_val] = 2;
				else if (!strcmp(str_of_param, SLIDING))
					int_of_param[num_of_val] = 3;
				else
				{
					print_rgm_attr_error(str_of_param,
							num_of_val, param, inar);
				}
			}
			else
			{
				double_of_param = realloc(double_of_param,
						sizeof(double) * (num_of_val + 1));
				double_of_param[num_of_val] = atof(str_of_param);
				if (errno == ERANGE)
				{
					print_rgm_attr_error(str_of_param,
							num_of_val, param, inar);
				}
			}
			num_of_val++;
		}
		if (reading_started == 1)
		{
			if (j >= maxline)
			{
				print_rgm_attr_error(str_of_param, num_of_val,
						param, inar);
			}
			str_of_param[j] = str[i];
			str_of_param[j+1] = '\0';
			j++;
		}
		if (c == '[')
		{
			if (reading_started)
			{
				print_rgm_attr_error(str_of_param, num_of_val,
					param, misb);
			} 
			reading_started = 1;
		}
		i++;
	} while (c != '\0');
	if (param == pvec)
	{
		free(double_of_param);
		p_robot->pvec = int_of_param;
		p_robot->nump = num_of_val;
	}
	else
	{
		free(int_of_param);
		switch (param)
		{
		case lvec:
			p_robot->lvec = double_of_param;
			break;
		case qvec:
			p_robot->qvec = double_of_param;
			break;
		case rmtx:
			p_robot->rmtx = double_of_param;
			break;
		default:
			free(double_of_param);
		}
	}
	free(str_of_param);
}

void read_rgm_section(struct Robot *p_robot,
		const char *str, int *size_str, char cur_ch, char des_ch)
{
	enum {tp = 12, lp = 12, co = 22, rm = 15};
	const char *T_PAIR = "TYPE_OF_PAIR";
	const char *L_PAIR = "SIZE_OF_PAIR";
	const char *COORDS = "GENERALIZED_COORDINATE";
	const char *R_MATRIX = "ROTATION_MATRIX";
	if (cur_ch == des_ch)
	{	
		if (!strncmp(str, T_PAIR, sizeof(char)*tp))
			get_data_from_rgm_section(str, p_robot, pvec);
		if (!strncmp(str, L_PAIR, sizeof(char)*lp))
			get_data_from_rgm_section(str, p_robot, lvec);
		if (!strncmp(str, COORDS, sizeof(char)*co))
			get_data_from_rgm_section(str, p_robot, qvec);
		if (!strncmp(str, R_MATRIX, sizeof(char)*rm))
			get_data_from_rgm_section(str, p_robot, rmtx);
		*size_str = 0;
	}
	
}

void read_rgm_file(FILE *rgm_file, struct Robot *p_robot)
{
	char c;
	int size_str = 1;
	char *str = malloc((size_str+1) * sizeof(char));
	while ((c = fgetc(rgm_file)) != EOF)
	{
		enum {start = 33, end = 126};
		if (c >= start && c <= end)
		{
			str[size_str - 1] = c;
			read_rgm_section(p_robot, str, &size_str, c, '$');
			size_str++;
			str = realloc(str, (size_str+1) * sizeof(char));
			str[size_str] = '\0';
		}
	}
	free(str);
}

void init_rgm(const char *filename, struct Robot *p_robot)
{
	FILE *robot_characteristics;
	robot_characteristics = fopen(filename, "r");
	if (!robot_characteristics)
	{
		perror(filename);
		exit(1);
	}
	read_rgm_file(robot_characteristics, p_robot);
	fclose(robot_characteristics);
	
}

void dest_rgm(struct Robot *p_robot)
{
	free(p_robot->pvec);
	free(p_robot->lvec);
	free(p_robot->qvec);
	free(p_robot->rmtx);
}

int main (int argc, char **argv)
{
	int i;
	struct Robot robot;
	if (argc > 1)
		init_rgm(argv[1], &robot);
	else
		fprintf(stderr, "To few arguments\n");
	for (i = 0; i < robot.nump; i++)
		printf("Type of pair[%d]:\t%d\n", i, robot.pvec[i]);
	for (i = 0; i < robot.nump*3; i++)
		printf("Size of pair[%d][%d]:\t%lf\n", i/3, i%3, robot.lvec[i]);
	for (i = 0; i < robot.nump; i++)
		printf("Coords[%d]:\t%lf\n", i, robot.qvec[i]);
	for (i = 0; i < (robot.nump+1)*9; i++)
		printf("Matrix[%d][%d][%d]:\t%lf\n", i/9, (i/3)%3,i%3, robot.rmtx[i]);
	dest_rgm(&robot);
	return 0;
}
