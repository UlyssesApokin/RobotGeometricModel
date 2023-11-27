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
#include <math.h>
enum {rots = 9, mtxs = 4, poss = 3, extr = 3, clim = 2};

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
double get_min_element(double *arr, int num)
{
	int i;
	double min = arr[0];
	for (i = 0; i < num; i++) {
		if (arr[i] < min)
			min = arr[i];
	}
	return min;
}	
double iterative_inc_of_gen_coord(double q, int sign, double delta)
{
	return q + pow(-1, sign) * delta;
}
double get_displacement_vector(double(**f)(const double*),
	const double *final, double *q)
{
	int i;
	double r[3];
	for (i = 0; i < (mtxs-1); i++) {
		r[i] = pow((f[i*mtxs + 3](q) - final[i*mtxs + 3]), 2);
	}
	return sqrt(r[0] + r[1] + r[2]);
}
double get_diff_btwn_axes(double(**f)(const double*),
	const double *final, int axis, int axis_h, double *q)
{
	return fabs(f[axis*mtxs + axis_h](q) - final[axis*mtxs + axis_h]);
}
int is_limit_reached(int q_num, const double* q_limit, const double *q)
{
	return((q[q_num] < q_limit[clim*q_num])
		|| (q[q_num] > q_limit[clim*q_num+1]));
}
double* get_max_displacement_vector(double(**f)(const double*),
	const double *final, double *q_min, double *q_max, double *q_aver)
{
	double *v = malloc(extr*sizeof(double));
	v[0] = get_displacement_vector(f, final, q_min);
	v[1] = get_displacement_vector(f, final, q_max);
	v[2] = get_displacement_vector(f, final, q_aver);
	return v;
}
double avoid_position_limiter(double(**f)(const double*), const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q)
{
	int i;
	double *q_min = malloc(count_of_pairs * sizeof(double));
	double *q_max = malloc(count_of_pairs * sizeof(double));
	double *q_aver = malloc(count_of_pairs * sizeof(double));
	double *v, rez;
	for (i = 0; i < count_of_pairs; i++)
		q_min[i] = q_max[i] = q_aver[i] = q[i];
	q_min[q_num] = q_limit[clim*q_num];
	q_max[q_num] = q_limit[clim*q_num+1];
	q_aver[q_num] =
		(q_limit[clim*q_num+1] + q_limit[clim*q_num]) / 2.0;
	v = get_max_displacement_vector(f, final, q_min, q_max, q_aver);
	rez = (get_min_element(v, extr) == v[0]) ? q_min[q_num]
		: ((get_max_element(v, extr) == v[1]) ? q_max[q_num] : q_aver[q_num]);
	free(q_min);
	free(q_max);
	free(q_aver);
	free(v);
	return rez;
}
double *get_max_diff_btwn_axes(double(**f)(const double*), const double *final,
	double *q_min, double *q_max, double *q_aver)
{
	int i;
	double *v = malloc(extr*sizeof(double));
	double *d = malloc(extr*sizeof(double));
	for (i = 0; i < extr; i++)
		d[i] = get_diff_btwn_axes(f, final, i, i, q_min);
	v[0] = get_max_element(d, extr);
	for (i = 0; i < extr; i++)
		d[i] = get_diff_btwn_axes(f, final, i, i, q_max);
	v[1] = get_max_element(d, extr);
	for (i = 0; i < extr; i++)
		d[i] = get_diff_btwn_axes(f, final, i, i, q_aver);
	v[2] = get_max_element(d, extr);
	free(d);
	return v;
}
double avoid_orientation_limiter(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q)
{
	int i;
	double *q_min = malloc(count_of_pairs * sizeof(double));
	double *q_max = malloc(count_of_pairs * sizeof(double));
	double *q_aver = malloc(count_of_pairs * sizeof(double));
	double *v, rez;
	for (i = 0; i < count_of_pairs; i++)
		q_min[i] = q_max[i] = q_aver[i] = q[i];
	q_min[q_num] = q_limit[clim*q_num];
	q_max[q_num] = q_limit[clim*q_num+1];
	q_aver[q_num] =
		(q_limit[clim*q_num+1] + q_limit[clim*q_num]) / 2.0;
	v = get_max_diff_btwn_axes(f, final, q_min, q_max, q_aver);
	rez = (get_min_element(v, extr) == v[0]) ? q_min[q_num]
		: ((get_max_element(v, extr) == v[1]) ? q_max[q_num] : q_aver[q_num]);
	free(q_min);
	free(q_max);
	free(q_aver);
	free(v);
	return rez;
}
double do_iter_step_position(double(**f)(const double*), const double *final,
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
double do_iter_step_orientation(double(**f)(const double*), const double *final,
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
		do_iter_step_orientation(f, final, count_of_pairs, q_num, delta, q_iter);
	}
	return q_iter[q_num];
}
double set_iteration_step(int q_num, const double *q_limit, unsigned int division)
{
	return((q_limit[clim*q_num+1] - q_limit[clim*q_num]) / (double)division);
}
double set_simple_iteration_step(double gap, unsigned int division)
{
	return gap / (double)division;
}
double* get_tcp_matrix(double(**f)(const double*), double *q)
{
	int i, j;
	double *tmatrix = malloc(mtxs*mtxs * sizeof(double));
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
	tmatrix[i*mtxs + j] = f[i*mtxs + j](q);
	}
	return tmatrix;
}
double* get_absolute_error_matrix(const double *tmatrix, const double *final)
{
	int i, j;
	double *errmatrix = malloc(mtxs*mtxs * sizeof(double));
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++) {
			errmatrix[i*mtxs + j] = tmatrix[i*mtxs + j] - final[i*mtxs + j];
		}
	return errmatrix;
}
double* get_relative_error_matrix(const double *tmatrix, const double *final)
{
	int i, j;
	double *errmatrix = malloc(mtxs*mtxs * sizeof(double));
	for (i = 0; i < mtxs; i++)
		for (j = 0; j < mtxs; j++) {
			if (final[i*mtxs + j] != 0) {
				errmatrix[i*mtxs + j]
					= fabs(tmatrix[i*mtxs + j] - final[i*mtxs + j])
						/ fabs(final[i*mtxs + j]);
			} else {
				errmatrix[i*mtxs + j] = 0;
			}
			errmatrix[i*mtxs + j] *= 100.0;
		}
	return errmatrix;
}
double get_max_error_value(double *errmatrix, int num_of_elemets)
{
	int i, n;
	double rez;
	double *ferrmatrix = malloc(num_of_elemets * sizeof(double));
	int *sign = malloc(num_of_elemets * sizeof(int));
	for (i = 0; i < (num_of_elemets); i++) {
		ferrmatrix[i] = fabs(errmatrix[i]);
		sign[i] = (errmatrix[i] >= 0) ? 0 : 1;
	}
	for (i = 0; i < (num_of_elemets); i++)
		if (ferrmatrix[i] == get_max_element(ferrmatrix, num_of_elemets))
			n = i;
	rez =  pow(-1, sign[n]) * get_max_element(ferrmatrix, num_of_elemets);
	free(ferrmatrix);
	free(sign);
	return rez;
}
double get_average_error_value(double *errmatrix, int num_of_elemets)
{
	int i;
	double err = 0;
	for (i = 0; i < (num_of_elemets); i++)
		err += fabs(errmatrix[i]);
	err /= num_of_elemets;
	return err;
}
double get_max_error_of_position(double *errmatrix)
{
	double pos[poss] = {errmatrix[3], errmatrix[7], errmatrix[11]};
	return get_max_error_value(pos, poss);
}
double get_average_error_of_position(double *errmatrix)
{
	double pos[poss] = {errmatrix[3], errmatrix[7], errmatrix[11]};
	return get_average_error_value(pos, poss);
}
double get_max_error_of_orientaion(double *errmatrix)
{
	double rot[rots] = {
		errmatrix[0], errmatrix[1], errmatrix[2],
		errmatrix[4], errmatrix[5], errmatrix[6],
		errmatrix[8], errmatrix[9], errmatrix[10]
	};
	return get_max_error_value(rot, rots);
}
double get_average_error_of_orientaion(double *errmatrix)
{
	double rot[rots] = {
		errmatrix[0], errmatrix[1], errmatrix[2],
		errmatrix[4], errmatrix[5], errmatrix[6],
		errmatrix[8], errmatrix[9], errmatrix[10]
	};
	return get_average_error_value(rot, rots);
}
