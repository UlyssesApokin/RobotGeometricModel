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
	int iter = 0, jj;
	int a, b;
	double **em;
	double q_iter, q_iter2;
	double q1min = -PI, q1max = +PI,
		q2min = -30, q2max = 30,
		q3min = -5*PI/6, q3max = 5*PI/6;
	int break_up_low = 50;
	double dq1, dq2, dq3;
	
	enum {tcp_row = 3, tcp_col = 4};
	double q1 = 0, q2 = 0, q3 = PI/4;
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
		puts("\t\tTURNING Q1 (POSITION) and SLIDING Q2 (POSITION)");
		iter = 0;
		dq2 = break_up_meas(q2min, q2max, break_up_low);
		dq1 = break_up_meas(q1min, q1max, break_up_low);
		q_iter = q1;
		q_iter2 = q2;
		do {
			iter++;
			q1 = q_iter;
			q_iter = iteration_position_step(dir_kin_tcp, final2, 0, dq1, q_iter, q2, q3);
			printf("Iter #%3d:\tq1=%2.4lf\n", iter, q_iter);
			if (q1 == q_iter) break;
			q2 = q_iter2;
			q_iter2 = iteration_position_step(dir_kin_tcp, final2, 1, dq3, q1, q_iter2, q3);
			printf("\t\tq2=%2.4lf\n", q_iter2);
			if (q2 == q_iter2) break;
		} while (1);
				
		/* 2 STAGE */
		puts("\t\tSLIDIND Q2 (POSITION) AND TURNING Q3 (POSITION)");
		iter = 0;
		dq2 = break_up_meas(q2min, q2max, break_up_low);
		dq3 = break_up_meas(q3min, q3max, break_up_low);
		q_iter = q2;
		q_iter2 = q3;
		do {
			iter++;
			q2 = q_iter;
			q_iter = iteration_position_step(dir_kin_tcp, final2, 1, dq2, q1, q_iter, q3);
			printf("Iter #%3d:\tq2=%2.4lf\n", iter, q_iter);
			if (q2 == q_iter) break;
			q3 = q_iter2;
			q_iter2 = iteration_position_step(dir_kin_tcp, final2, 2, dq3, q1, q2, q_iter2);
			printf("\t\tq3=%2.4lf\n", q_iter2);
			if (q3 == q_iter2) break;
		} while (1);
		
		/* 3 STAGE */
		puts("\t\tTURNING Q3 (ORIENTATION)");
		iter = 0;
		dq3 = break_up_meas(q3min, q3max, break_up_low);
		q_iter = q3;
		do {
			iter++;
			q3 = q_iter;
			q_iter = iteration_orientat_step(dir_kin_tcp, final2, 2, dq3, q1, q2, q_iter);
			printf("Iter #%3d:\tq3=%2.4lf\n", iter, q_iter);
		} while (q3 != q_iter);
		
		puts("VALUE OF ERROR:");
		em = errror_tcp_matrix(current_tcp_matrix(dir_kin_tcp, q1, q2, q3), final2);
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
