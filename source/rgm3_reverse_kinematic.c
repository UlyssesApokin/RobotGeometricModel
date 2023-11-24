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

#include "rgm3_reverse_kinematic.h"
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
enum {mtxs = 4, clim = 2};

double get_max_element(double *arr, int num)
{
	int i;
	double max = arr[0];
	for (i = 0; i < num; i++) {
		if (arr[i] > max)
			max = arr[i];
	}
	return max;
}	
double iterative_inc_of_gen_coord(double q, int sign, double delta)
{
	return q + pow(-1, sign) * delta;
}
double get_displacement_vector(double(**f)(double*),
	const double *final, double *q)
{
	int i;
	double r[3];
	for (i = 0; i < (mtxs-1); i++) {
		r[i] = pow((f[i*mtxs + 3](q) - final[i*mtxs + 3]), 2);
	}
	return sqrt(r[0] + r[1] + r[2]);
}
double get_diff_btwn_axes(double(**f)(double*),
	const double *final, int axis, int axis_h, double *q)
{
	return fabs(f[axis*mtxs + axis_h](q) - final[axis*mtxs + axis_h]);
}
int is_limit_reached(int q_num, const double *q, const double* q_limit)
{
	return((q[q_num] < q_limit[clim*q_num])
		|| (q[q_num] > q_limit[clim*q_num+1]));
}
double do_iter_step_position(double(**f)(double*), const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q)
{
	static int sign = 0;
	int i;
	double q_iter[count_of_pairs];
	for (i = 0; i < count_of_pairs; i++)
		q_iter[i] = q[i];
	q_iter[q_num] = iterative_inc_of_gen_coord(q[q_num], sign, delta[q_num]);
	if (get_displacement_vector(f, final, q)
		- get_displacement_vector(f, final, q_iter) > 0) {
		return q_iter[q_num];
	}
	else {
		if (sign == 1) {
			sign = 0;
			return q[q_num];
		}
		sign = 1;
		do_iter_step_position(f, final, count_of_pairs, q_num, delta, q);
	}
	return q_iter[q_num];
}
double do_iter_step_orientation(double(**f)(double*), const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q)
{
	static int sign = 0;
	int i;
	double q_iter[count_of_pairs], errors[mtxs-1];
	for (i = 0; i < count_of_pairs; i++)
		q_iter[i] = q[i];
	q_iter[q_num] = iterative_inc_of_gen_coord(q[q_num], sign, delta[q_num]);
	for (i = 0; i < (mtxs-1); i++) {
		errors[i] = get_diff_btwn_axes(f, final, i, i, q)
			- get_diff_btwn_axes(f, final, i, i, q_iter);
	}
	if (get_max_element(errors, mtxs-1) > 0) {
		return q_iter[q_num];
	}
	else {
		if (sign == 1) {
			sign = 0;
			return q[q_num];
		}
		sign = 1;
		do_iter_step_orientation(f, final, count_of_pairs, q_num, delta, q);
	}
	return q_iter[q_num];
}
double set_iteration_step(int q_num, const double *q_limit, int division)
{
	return((q_limit[clim*q_num+1] - q_limit[clim*q_num]) / (double)division);
}
double* get_tcp_matrix(double(**f)(double*), double *q)
{
	int i, j;
	double *tmatrix = malloc(mtxs*mtxs * sizeof(double));
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++) {
	tmatrix[i*mtxs + j] = f[i*mtxs + j](q);
	}
	return tmatrix;
}
double* get_error_matrix(double *et, const double *final)
{
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++) {
			et[i*mtxs + j] = et[i*mtxs + j] - final[i*mtxs + j];
		}
	return et;
}
