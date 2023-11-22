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
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define Q1_C 0
#define Q2_C 1
#define Q3_С 2
#define POS 0
#define NEG 1

double iteration_position_step(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...);

double iteration_orientat_step(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...);

double break_up_meas(double qmin, double qmax, int n);

double** current_tcp_matrix(double(***f)(double, ...), double q1, ...);

double** errror_tcp_matrix(double **t, double final[3][4]);
#endif
