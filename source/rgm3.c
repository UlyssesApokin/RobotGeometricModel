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
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "rgm3_reverse_kinematic.h"
#define PI 3.14
#define L1 60
#define L2 50
#define L3 25
double tcp_xxh(double *q)
{
	return cos(q[0]) * sin(q[2]);
}
double tcp_xyh(double *q)
{
	return cos(q[0]) * cos(q[2]);
}
double tcp_xzh(double *q)
{
	return -sin(q[0]);
}
double tcp_xp(double *q)
{
	return L3 * cos(q[0]) * sin(q[2]);
}
double tcp_yxh(double *q)
{
	return sin(q[0]) * sin(q[2]);
}
double tcp_yyh(double *q)
{
	return sin(q[0]) * cos(q[2]);
}
double tcp_yzh(double *q)
{
	return cos(q[0]);
}
double tcp_yp(double *q)
{
	return L3 * sin(q[0]) * sin(q[2]);
}
double tcp_zxh(double *q)
{
	return cos(q[2]);
}
double tcp_zyh(double *q)
{
	return -sin(q[2]);
}
double tcp_zzh(double *q)
{
	return 0;
}
double tcp_zp(double *q)
{
	return L3 * cos(q[2]) + q[1] + L1 + L2;
}

int main(int argc, char **argv)
{
	int iter = 0, jj;
	int a, b;
	double **em;
	double q_iter, q_iter2;
	double q[0]min = -PI, q[0]max = +PI,
		q[1]min = -30, q[1]max = 30,
		q[2]min = -5*PI/6, q[2]max = 5*PI/6;
	int break_up_low = 50;
	double dq[0], dq[1], dq[2];
	
	enum {tcp_row = 3, tcp_col = 4};
	double q[0] = 0, q[1] = 0, q[2] = PI/4;
	double final[3][4] = {
		{0.664, -0.664, 0.342, 16.612},
		{-0.242, 0.242, 0.970, -6.046},
		{-0.707, -0.707, 0, 77.732}
	};
	double final2[3][4] = {
		{0.250, 0.433, 0.866, 6.250},
		{-0.433, -0.750, 0.500, -10.825},
		{0.866, -0.5, 0, 131.651}
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
	
	
	for (jj =0; jj < 7; jj++) {
		if (jj == 1) break_up_low = 100;
		if (jj == 2) break_up_low = 200;
		if (jj == 3) break_up_low = 400;
		if (jj == 4) break_up_low = 800;
		if (jj == 5) break_up_low = 1600;
		if (jj == 6) break_up_low = 3200;
		printf("\tApproximation #%2d:\t STEP = %3d\n", jj+1, break_up_low);
		
		/* 1 STAGE*/
		puts("\t\tTURNING q[0] (POSITION) and SLIDING q[1] (POSITION)");
		iter = 0;
		dq[1] = set_iteration_step(q[1]min, q[1]max, break_up_low);
		dq[0] = set_iteration_step(q[0]min, q[0]max, break_up_low);
		q_iter = q[0];
		q_iter2 = q[1];
		do {
			iter++;
			q[0] = q_iter;
			q_iter = do_iter_step_position(dir_kin_tcp, final2, 0, dq[0], q_iter, q[1], q[2]);
			printf("Iter #%3d:\tq[0]=%2.4lf\n", iter, q_iter);
			if (q[0] == q_iter) break;
			q[1] = q_iter2;
			q_iter2 = do_iter_step_position(dir_kin_tcp, final2, 1, dq[2], q[0], q_iter2, q[2]);
			printf("\t\tq[1]=%2.4lf\n", q_iter2);
			if (q[1] == q_iter2) break;
		} while (1);
				
		/* 2 STAGE */
		puts("\t\tSLIDIND q[1] (POSITION) AND TURNING q[2] (POSITION)");
		iter = 0;
		dq[1] = set_iteration_step(q[1]min, q[1]max, break_up_low);
		dq[2] = set_iteration_step(q[2]min, q[2]max, break_up_low);
		q_iter = q[1];
		q_iter2 = q[2];
		do {
			iter++;
			q[1] = q_iter;
			q_iter = do_iter_step_position(dir_kin_tcp, final2, 1, dq[1], q[0], q_iter, q[2]);
			printf("Iter #%3d:\tq[1]=%2.4lf\n", iter, q_iter);
			if (q[1] == q_iter) break;
			q[2] = q_iter2;
			q_iter2 = do_iter_step_position(dir_kin_tcp, final2, 2, dq[2], q[0], q[1], q_iter2);
			printf("\t\tq[2]=%2.4lf\n", q_iter2);
			if (q[2] == q_iter2) break;
		} while (1);
		
		/* 3 STAGE */
		puts("\t\tTURNING q[2] (ORIENTATION)");
		iter = 0;
		dq[2] = set_iteration_step(q[2]min, q[2]max, break_up_low);
		q_iter = q[2];
		do {
			iter++;
			q[2] = q_iter;
			q_iter = do_iter_step_orientation(dir_kin_tcp, final2, 2, dq[2], q[0], q[1], q_iter);
			printf("Iter #%3d:\tq[2]=%2.4lf\n", iter, q_iter);
		} while (q[2] != q_iter);
		
		puts("VALUE OF ERROR:");
		em = get_error_matrix(get_tcp_matrix(dir_kin_tcp, q[0], q[1], q[2]), final2);
		for (a = 0; a < 3; a++) {
			printf("|\t");
		for (b = 0; b < 4; b++) {
			printf("%2.4lf\t", em[a][b]);
		}
		puts("|");
	}
	}
	

	
	
	for (i = 0; i < tcp_row; i++) {
		free(dir_kin_tcp[i]);
	}
	free(dir_kin_tcp);
	return 0;
}
