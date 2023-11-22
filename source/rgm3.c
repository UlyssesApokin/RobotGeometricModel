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

#include <stdio.h>
#include "rgm3_reverse_kinematic.h"
#define PI 3.14
#define L1 60
#define L2 50
#define L3 25
double tcp_xxh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return cos(q1) * sin(q3);
}
double tcp_xyh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return cos(q1) * cos(q3);
}
double tcp_xzh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	q3++;
	va_end(vl);
	return -sin(q1);
}
double tcp_xp(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return L3 * cos(q1) * sin(q3);
}
double tcp_yxh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return sin(q1) * sin(q3);
}
double tcp_yyh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return sin(q1) * cos(q3);
}
double tcp_yzh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	q3++;
	va_end(vl);
	return cos(q1);
}
double tcp_yp(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return L3 * sin(q1) * sin(q3);
}
double tcp_zxh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return cos(q3);
}
double tcp_zyh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	va_end(vl);
	return -sin(q3);
}
double tcp_zzh(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q2++;
	q3 = va_arg(vl, double);
	q3++;
	va_end(vl);
	return 0;
}
double tcp_zp(double q1, ...)
{
	va_list vl;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	va_end(vl);
	return L3 * cos(q3) + q2 + L1 + L2;
}

int main(int argc, char **argv)
{
	enum {tcp_row = 3, tcp_col = 4};
	double q, q_prev, q1 = -PI/3, q2 = 0.0, q3 = PI/2;
	q_prev = q = q1;
	q_prev += 1;
	double final[3][4] = {
		{0.664, -0.664, 0.342, 16.612},
		{-0.242, 0.242, 0.970, -6.046},
		{-0.707, -0.707, 0, 77.732}
	};
	int i;
	double (***dir_kin_tcp)(double, ...)
		= malloc(tcp_row * sizeof(double(**)(double, ...)));
	for (i = 0; i < tcp_row; i++) {
		dir_kin_tcp[i]
			= malloc(tcp_col * sizeof(double(*)(double, ...)));
	}
	dir_kin_tcp[0][0] = tcp_xxh;
	dir_kin_tcp[0][1] = tcp_xyh;
	dir_kin_tcp[0][2] = tcp_xzh;
	dir_kin_tcp[0][3] = tcp_xp;
	dir_kin_tcp[1][0] = tcp_yxh;
	dir_kin_tcp[1][1] = tcp_yyh;
	dir_kin_tcp[1][2] = tcp_yzh;
	dir_kin_tcp[1][3] = tcp_yp;
	dir_kin_tcp[2][0] = tcp_zxh;
	dir_kin_tcp[2][1] = tcp_zyh;
	dir_kin_tcp[2][2] = tcp_zzh;
	dir_kin_tcp[2][3] = tcp_zp;
	i = 0;
	printf("iter: %3d\t%.4lf\n", i, q);
	while (q_prev != q) {
		i++;
		q_prev = q;
		q = iteration_position_step(dir_kin_tcp, final, 0, 2*PI/100, q, q2, q3);
		printf("iter: %3d\t%.4lf\n", i, q);
	}
	for (i = 0; i < tcp_row; i++) {
		free(dir_kin_tcp[i]);
	}
	free(dir_kin_tcp);
	return 0;
}
