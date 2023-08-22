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
#include "rgm_file.h"

int main (int argc, char **argv)
{
	/*
	int i;
	Robot robot;
	
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
	*/
	
	QueueOfRoboPair robot;
	int k;
	int type = 33;
	double length[3] = {1.1, 2.2, 3.3};
	double vector[3] = {0.4, 0.5, 0.6};
	double matrix[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int *ttt;
	double *vvv;
	queue_robopair_init(&robot);
	queue_robopair_put(&robot, &type, pvec, 0);
	queue_robopair_put(&robot, length, lvec, 0);
	queue_robopair_put(&robot, vector, qvec, 10);
	queue_robopair_put(&robot, matrix, rmtx, 0);
	
	ttt = queue_robopair_get(&robot, pvec, 0);
	printf("%d\n", *ttt);
	puts("________");
	vvv = queue_robopair_get(&robot, lvec, 0);
	for (k =0; k < 3; k++)
		printf("%lf\n", vvv[k]);
	puts("________");
	vvv = queue_robopair_get(&robot, qvec, 11);
	for (k =0; k < 3; k++)
		printf("%lf\n", vvv[k]);
	puts("________");
	vvv = queue_robopair_get(&robot, rmtx, 0);
	for (k =0; k < 9; k++)
		printf("%lf\n", vvv[k]);
	return 0;
}
