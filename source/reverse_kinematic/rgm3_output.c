/*
Copyright (c) 2023 Ulysses Apokin (Jelisej Apokin)

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

#include "rgm3_output.h"
#include <stdio.h>

void term_print_matrix(const char *subs, const double *matrix, int row, int col)
{
	int i, j;
	printf("%s\n", subs);
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%3.4lf\t\t", matrix[i*row + j]);
		}
		printf("\n");
	}
}
void term_print_error(int percent,
	double max_error_pos, double aver_error_pos,
	double max_error_ort, double aver_error_ort)
{
	printf("Max error of position:\t%3.4lf", max_error_pos);
	if (percent) puts("  %");
	printf("Average error of position:\t%3.4lf", aver_error_pos);
	if (percent) puts("  %");
	if (max_error_ort != 0) {
		printf("Max error of orientation:\t%3.4lf", max_error_ort);
		if (percent) puts("  %");
	}
	if (aver_error_ort != 0) {
		printf("Average error of orientation:\t%3.4lf", aver_error_ort);
		if (percent) puts("  %");
	}
}
void term_print_gen_coord(int num_of_iter, int num_of_q, double *q)
{
	int i;
	printf("Iter #%3d:\t", num_of_iter);
	for (i = 0; i < num_of_q; i++)
		printf("q%d = %3.4lf\t", i+1, q[i]);
	printf("\n");
}
