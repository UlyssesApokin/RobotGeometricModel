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

#ifndef RGM3_REVERSE_KINEMATIC_H_SENTRY
#define RGM3_REVERSE_KINEMATIC_H_SENTRY

int is_limit_reached(int q_num, const double *q, const double* q_limit);

double avoid_position_limiter(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q);
	
double avoid_orientation_limiter(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *q_limit, double *q);

double do_iter_step_position(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q);

double do_iter_step_orientation(double(**f)(const double*),
	const double *final,
	int count_of_pairs, int q_num, const double *delta, double *q);

double set_iteration_step(int q_num, const double *q_limit,
	unsigned long int division);

double* get_tcp_matrix(double(**f)(const double*), double *q);

double* get_absolute_error_matrix(const double *tmatrix, const double *final);

double* get_relative_error_matrix(const double *tmatrix, const double *final);

double get_max_error_value(double *errmatrix);

double get_min_error_value(double *errmatrix);

double get_average_error_value(double *errmatrix);

#endif
