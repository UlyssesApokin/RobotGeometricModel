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

#include "rgm_file.h"

struct _Data
{
	int num;
	char ***txt;
};

static void print_rgm_attr_error(const char *str, int num_of_iter,
		int attr, int misstake)
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
	switch (attr)
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

void set_data_to_queue(struct _Data *data, QueueOfRoboPair *p_robot,
		int *num_of_val, int attr)
{
	/*int i, j;
	int *idata;
	double *ddata;
	switch (attr)
	{
	case pvec:
		const char *TURNING_TYPE_1 = "TURNING_TYPE_1";
		const char *TURNING_TYPE_2 = "TURNING_TYPE_2";
		const char *SLIDING = "SLIDING";
		const char *TCP = "TOOL_CENTER_POINT";
		if (!strcmp(data[i], TURNING_TYPE_1))
			idata[i] = 1;
		else if (!strcmp(data[i], TURNING_TYPE_2))
			idata[i] = 2;
		else if (!strcmp(data[i], SLIDING))
			idata[i] = 3;
		else if (!strcmp(data[i], TCP))
			idata[i] = 0;
		break;
	case lvec:
		
		break;
	case qvec:
		
		break;
	case rmtx:
		
		break;
	}*/
}

struct _Data *init_data()
{
	enum {maxval = 9, maxline = 64};
	int i;
	struct _Data *data;
	data = (struct _Data*)malloc(sizeof(struct _Data));
	data->txt = (char***)malloc(sizeof(char**));
	data->txt[0] = (char**)malloc(sizeof(char*) * maxval);
	for (i = 0; i < maxval; i++)
		data->txt[0][i] = (char*)malloc(sizeof(char) * maxline);
	data->num = 1;
	return data;
}

void remove_data(struct _Data *data)
{
	free(data);
}
void get_data_from_rgm_section(const char *str,
		QueueOfRoboPair *p_robot, int attr)
{
	char c;
	struct _Data *data = init_data();
	int i = 0, j = 0;
	int reading_started = 0, is_error = 0;;
	int num_of_val = 0;
	do
	{
		c = str[i];
		if (c == ']')
		{
		/*	if (!reading_started)
			{
				print_rgm_attr_error(data[num_of_val], num_of_val,
						attr, misb);
				is_error = 1;
			}*/
			reading_started = 0;
			j = 0;
			set_data_to_queue(data, p_robot, &num_of_val, attr);
			num_of_val++;
		}
		if (reading_started == 1)
		{
		/*	if (j > maxline - 1)
			{
				print_rgm_attr_error(data[num_of_val], num_of_val,
						attr, inar);
				is_error = 1;
			}*/
		/*	data[num_of_val][j] = str[i];
			data[num_of_val][j+1] = '\0';*/
			j++;
		}
		if (c == '[')
		{
			/*if (reading_started)
			{
				print_rgm_attr_error(data[num_of_val], num_of_val,
					attr, misb);
				is_error = 1;
			} */
			reading_started = 1;
		}
		i++;
	} while (c != '\0');
	if (is_error)
		exit(2);
	remove_data(data);
}

void detect_rgm_section(QueueOfRoboPair *p_robot,
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

void read_rgm_file(FILE *rgm_file, QueueOfRoboPair *p_robot)
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
			detect_rgm_section(p_robot, str, &size_str, c, '$');
			size_str++;
			str = realloc(str, (size_str+1) * sizeof(char));
			str[size_str] = '\0';
		}
	}
	free(str);
}

void init_rgm(const char *filename, QueueOfRoboPair *p_robot)
{
	FILE *robot_characteristics;
	queue_robopair_init(p_robot);
	robot_characteristics = fopen(filename, "r");
	if (!robot_characteristics)
	{
		perror(filename);
		exit(1);
	}
	read_rgm_file(robot_characteristics, p_robot);
	fclose(robot_characteristics);
	
}

void dest_rgm(QueueOfRoboPair *p_robot)
{
	queue_robopair_remove(p_robot);
}

void debug_output(QueueOfRoboPair *p_robot)
{
	int i, j;
	int *out_i;
	double *out_d;
	printf("NUM_OF_PAIR:\t%d\n\n", p_robot->num);
	for (i = 0; i < p_robot->num; i++)
	{
		printf("PAIR#%d\n", i);
		out_i = queue_robopair_get(p_robot, pvec, i);
		printf("TYPE:\t%d\n", *out_i);

		printf("SIZE:\t");
		out_d = queue_robopair_get(p_robot, lvec, i);
		for (j = 0; j < 3; j++)
			printf("%.2lf, ", out_d[j]);
		puts("");
		out_d = queue_robopair_get(p_robot, qvec, i);
		printf("GENERALIZED_COORDINATE:\t%.2lf", *out_d);
		puts("");
		out_d = queue_robopair_get(p_robot, rmtx, i);
		for (j = 0; j < 9; j++)
			printf("%.2lf, ", out_d[j]);
		puts("");
		puts("");
	}
}
