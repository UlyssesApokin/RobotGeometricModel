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

double maximum(double *arr, int num)
{
	int i;
	double max = arr[0];
	for (i = 0; i < num; i++) {
		if (arr[i] > max)
			max = arr[i];
	}
	return max;
}	
double iterative_inc_of_gen_coord(double q, int sign, double delta_q)
{
	return q + pow(-1, sign) * delta_q;
}
double get_displacement_vector(double(***f)(double, ...),
	double final[3][4], double q1, ...)
{
	va_list vl;
	enum { row = 3 };
	int i;
	double q2, q3;
	double r[3];
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	va_end(vl);
	for (i = 0; i < row; i++) {
		r[i] = pow((f[i][3](q1, q2, q3) - final[i][3]), 2);
	}
	return sqrt(r[0] + r[1] + r[2]);
}
double orientat_current_final(double(***f)(double, ...),
	double final[3][4], int axis, int axis_h, double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	va_end(vl);
	return fabs(f[axis][axis_h](q1, q2, q3) - final[axis][axis_h]);
}
double do_iter_step_position(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...)
{
	va_list vl;
	static int sign = 0;
	double q_prev, q_iter, v_iter, v_prev;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	v_prev = get_displacement_vector(f, final, q1, q2, q3);
	switch (type_q) {
	case 0:
		q_prev = q1;
		q_iter = iterative_inc_of_gen_coord(q1, sign, delta_q);
		v_iter = get_displacement_vector(f, final, q_iter, q2, q3);
	break;
	case 1:
		q_prev = q2;
		q_iter = iterative_inc_of_gen_coord(q2, sign, delta_q);
		v_iter = get_displacement_vector(f, final, q1, q_iter, q3);
	break;
	case 2:
		q_prev = q3;
		q_iter = iterative_inc_of_gen_coord(q3, sign, delta_q);
		v_iter = get_displacement_vector(f, final, q1, q2, q_iter);
	break;
	default:
		exit(1);
	break;
	}
	if (v_prev - v_iter > 0) {
		return q_iter;
	}
	else {
		if (sign == 1) {
			sign = 0;
			return q_prev;
		}
		sign++;
		do_iter_step_position(f, final, type_q, delta_q, q1, q2, q3);
	}
	va_end(vl);
	return q_iter;
};
double do_iter_step_orientation(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...)
{
	va_list vl;
	enum {row = 3};
	static int sign = 0;
	int i;
	double q_prev, q_iter, v_iter[3], v_prev[3];
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	for (i = 0; i < row; i++)
		v_prev[i] = orientat_current_final(f, final, i, i, q1, q2, q3);
	switch (type_q) {
	case 0:
		q_prev = q1;
		q_iter = iterative_inc_of_gen_coord(q1, sign, delta_q);
		for (i = 0; i < row; i++)
			v_iter[i] = orientat_current_final(f, final, i, i, q_iter, q2, q3);
	break;
	case 1:
		q_prev = q2;
		q_iter = iterative_inc_of_gen_coord(q2, sign, delta_q);
		for (i = 0; i < row; i++)
			v_iter[i] = orientat_current_final(f, final, i, i, q1, q_iter, q3);
	break;
	case 2:
		q_prev = q3;
		q_iter = iterative_inc_of_gen_coord(q3, sign, delta_q);
		for (i = 0; i < row; i++)
			v_iter[i] = orientat_current_final(f, final, i, i, q1, q2, q_iter);
	break;
	default:
		exit(1);
	break;
	}
	for (i = 0; i < row; i++)
		v_prev[i] = v_prev[i] - v_iter[i];
	v_prev[0] = maximum(v_prev, sizeof(v_prev)/sizeof(*v_prev));
	if (v_prev[0] > 0) {
		return q_iter;
	}
	else {
		if (sign == 1) {
			sign = 0;
			return q_prev;
		}
		sign++;
		do_iter_step_orientation(f, final, type_q, delta_q, q1, q2, q3);
	}
	va_end(vl);
	return q_iter;
};
double set_iteration_step(double qmin, double qmax, int n)
{
	return((qmax - qmin) / (double)n);
}
double** get_tcp_matrix(double(***f)(double, ...), double q1, ...)
{
	va_list vl;
	int i, j;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	double **t = malloc(3*sizeof(double*));
	for (i = 0; i < 3; i++)
		t[i] = malloc(4*sizeof(double));
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++) {
	t[i][j] = f[i][j](q1, q2, q3);
	}
	va_end(vl);
	return t;
}
double** get_error_matrix(double **t, double final[3][4])
{
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++) {
			t[i][j] = t[i][j] - final[i][j];
		}
	return t;
}
