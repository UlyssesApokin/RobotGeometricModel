#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
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
double iterative_inc_of_gen_coord(double q, int sign, double delta_q)
{
	return q + pow(-1, sign) * delta_q;
}
double iteration_step
	(int type_q, double(***f)(double, ...), double **final,
	double q_min, double q_max, int sign,
	double q1, ...)
{
	va_list vl;
	double q2;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	va_end(vl);
	return ;
};
int main(int argc, char **argv)
{
	enum {tcp_row = 3, tcp_col = 4};
	int i;
	double (***direct_kinematics_tcp)(double, ...)
		= malloc(tcp_row * sizeof(double(**)(double, ...)));
	for (i = 0; i < tcp_row; i++) {
		direct_kinematics_tcp[i]
			= malloc(tcp_col * sizeof(double(*)(double, ...)));
	}
	direct_kinematics_tcp[0][0] = tcp_xxh;
	direct_kinematics_tcp[0][1] = tcp_xyh;
	direct_kinematics_tcp[0][2] = tcp_xzh;
	direct_kinematics_tcp[0][3] = tcp_xp;
	direct_kinematics_tcp[1][0] = tcp_yxh;
	direct_kinematics_tcp[1][1] = tcp_yyh;
	direct_kinematics_tcp[1][2] = tcp_yzh;
	direct_kinematics_tcp[1][3] = tcp_yp;
	direct_kinematics_tcp[2][0] = tcp_zxh;
	direct_kinematics_tcp[2][1] = tcp_zyh;
	direct_kinematics_tcp[2][2] = tcp_zzh;
	direct_kinematics_tcp[2][3] = tcp_zp;
	printf("\t%.2lf\n", iterative_inc_of_gen_coord(PI, 0, 2*PI/100));
	printf("\t%.2lf\n", direct_kinematics_tcp[2][1](2.0, 2.0, 1.0));
	for (i = 0; i < tcp_row; i++) {
		free(direct_kinematics_tcp[i]);
	}
	free(direct_kinematics_tcp);
	return 0;
}
