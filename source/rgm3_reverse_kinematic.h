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

int is_limit_reached(double q, double q_min, double q_max);

double do_iter_step_position(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...);
/*
double avoid_limit_of_sliding(double(***f)(double, ...), double final[3][4],
	int type_q, double q_min, double q_max, double q1, ...);
*/
double do_iter_step_orientation(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...);

double set_iteration_step(double qmin, double qmax, int n);

double** get_tcp_matrix(double(***f)(double, ...), double q1, ...);

double** get_error_matrix(double **t, double final[3][4]);


#endif
