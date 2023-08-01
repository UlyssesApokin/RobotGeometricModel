#include <stdlib.h>
#include <stdio.h>
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

void init_rgm(FILE *filename, struct Robot *robot)
{
	
}

int main (int argc, char **argv)
{
	
	return 0;
}
