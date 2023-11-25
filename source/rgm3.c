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
#include <math.h>
#include "rgm3_reverse_kinematic.h"
#include "rgm3_output.h"
#define L1 60.0
#define L2 50.0
#define L3 25.0
double tcp_xxh(const double *q)
{
	return cos(q[0]) * sin(q[2]);
}
double tcp_xyh(const double *q)
{
	return cos(q[0]) * cos(q[2]);
}
double tcp_xzh(const double *q)
{
	return -sin(q[0]);
}
double tcp_xp(const double *q)
{
	return L3 * cos(q[0]) * sin(q[2]);
}
double tcp_yxh(const double *q)
{
	return sin(q[0]) * sin(q[2]);
}
double tcp_yyh(const double *q)
{
	return sin(q[0]) * cos(q[2]);
}
double tcp_yzh(const double *q)
{
	return cos(q[0]);
}
double tcp_yp(const double *q)
{
	return L3 * sin(q[0]) * sin(q[2]);
}
double tcp_zxh(const double *q)
{
	return cos(q[2]);
}
double tcp_zyh(const double *q)
{
	return -sin(q[2]);
}
double tcp_zzh(const double *q)
{
	return 0;
}
double tcp_zp(const double *q)
{
	return L3 * cos(q[2]) + q[1] + L1 + L2;
}
double tcp_zero(const double *q)
{
	return 0;
}
double tcp_nulo(const double *q)
{
	return 1;
}

int main(int argc, char **argv)
{
	enum{mtxs = 4}; /*matrix size*/
	int iter = 0, numero, i;
	/*
	 * tmatrix - current tcp-matrix
	 * rmatrix - relative error matrix
	 * amatrix - absolute error matrix
	 */
	double *tmatrix, *rmatrix, *amatrix;
	double q_iter[] = {0.0, 0.0, PI/4}; /*initial generalized coordination */
	double q[] = {0.0, 0.0, PI/4}; /*initial generalized coordination */
	/*limit of changes of generalized coordination*/
	double limit[] = {-PI, PI, -30, 30, -5*PI/6, 5*PI/6};
	int division = 50; /*deivide of iteration*/
	double delta[3]; /*step of iteration*/
	/*final position*/
	double final2[] = {
		0.250, 0.433, 0.866, 6.250,
		-0.433, -0.750, 0.500, -10.825,
		0.866, -0.5, 0, 131.651,
		0, 0, 0, 1
	};
	/*double final2[] = {
		-0.378401, 0.173178, -0.909297, -9.46003,
		0.826822, -0.378401, -0.416147, 20.6705,
		-0.416147, -0.909297, 0, 101.596,
		0, 0, 0, 1
	};*/
	
	/*equation of tool center point matrix*/
	double (**tcp_matrix)(const double*)
		= malloc(mtxs*mtxs * sizeof(double(*)(const double*)));
	tcp_matrix[0] = tcp_xxh;
	tcp_matrix[1] = tcp_xyh;
	tcp_matrix[2] = tcp_xzh;
	tcp_matrix[3] = tcp_xp;
	tcp_matrix[4] = tcp_yxh;
	tcp_matrix[5] = tcp_yyh;
	tcp_matrix[6] = tcp_yzh;
	tcp_matrix[7] = tcp_yp;
	tcp_matrix[8] = tcp_zxh;
	tcp_matrix[9] = tcp_zyh;
	tcp_matrix[10] = tcp_zzh;
	tcp_matrix[11] = tcp_zp;
	tcp_matrix[12] = tcp_zero;
	tcp_matrix[13] = tcp_zero;
	tcp_matrix[14] = tcp_zero;
	tcp_matrix[15] = tcp_nulo;
	
	
	for (i = 0; i < 3; i++) {
		delta[0] = set_iteration_step(0, limit, division);
		delta[1] = set_iteration_step(1, limit, division*200);
		delta[2] = set_iteration_step(2, limit, division);
		/*turning Q1*/
		do {
			iter++;
			q[0] = q_iter[0];
			term_print_gen_coord(iter, 3, q);
			q_iter[0] = do_iter_step_position(tcp_matrix, final2, 3, 0, delta, q);
		} while (q[0] != q_iter[0]);
		/*turning Q3*/
		do {
			iter++;
			q[2] = q_iter[2];
			term_print_gen_coord(iter, 3, q);
			q_iter[2] = do_iter_step_orientation(tcp_matrix, final2, 3, 2, delta, q);
		} while (q[2] != q_iter[2]);
		/*sliding Q1*/
		do {
			iter++;
			q[1] = q_iter[1];
			term_print_gen_coord(iter, 3, q);
			q_iter[1] = do_iter_step_position(tcp_matrix, final2, 3, 1, delta, q);
		} while (q[1] != q_iter[1]);
		division *= 2;
		
		tmatrix = get_tcp_matrix(tcp_matrix, q);
		amatrix = get_absolute_error_matrix(tmatrix, final2);
		rmatrix = get_relative_error_matrix(tmatrix, final2);
	}
	term_print_gen_coord(iter, 3, q);
	term_print_matrix("ITERATION POSITION", tmatrix, 4, 4);
	term_print_matrix("FINAL POSITION", final2, 4, 4);
	term_print_matrix("ABSOLUTE ERROR", amatrix, 4, 4);
	term_print_matrix("RELATIVE ERROR", rmatrix, 4, 4);
	free(rmatrix);
	free(tmatrix);
	free(amatrix);
	free(tcp_matrix);
	return 0;
}
