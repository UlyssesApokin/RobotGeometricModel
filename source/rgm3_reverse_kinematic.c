#include "rgm3_reverse_kinematic.h"

double iterative_inc_of_gen_coord(double q, int sign, double delta_q)
{
	return q + pow(-1, sign) * delta_q;
}
double vector_current_final(double(***f)(double, ...), double final[3][4], double q1, ...)
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
double iteration_position_step(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...)
{
	va_list vl;
	static int sign = 0;
	double q_prev, q_iter, v_iter, v_prev;
	double q2, q3;
	va_start(vl, q1);
	q2 = va_arg(vl, double);
	q3 = va_arg(vl, double);
	v_prev = vector_current_final(f, final, q1, q2, q3);
	switch (type_q) {
	case 0:
		q_prev = q1;
		q_iter = iterative_inc_of_gen_coord(q1, sign, delta_q);
		v_iter = vector_current_final(f, final, q_iter, q2, q3);
	break;
	case 1:
		q_prev = q2;
		q_iter = iterative_inc_of_gen_coord(q2, sign, delta_q);
		v_iter = vector_current_final(f, final, q1, q_iter, q3);
	break;
	case 2:
		q_prev = q3;
		q_iter = iterative_inc_of_gen_coord(q3, sign, delta_q);
		v_iter = vector_current_final(f, final, q1, q2, q_iter);
	break;
	default:
		exit(1);
	break;
	}
	if (v_prev - v_iter > 0) {
		return q_iter;
	}
	else {
		if (sign == 1) return q_prev;
		sign = 1;
		iteration_position_step(f, final, type_q, delta_q, q1, q2, q3);
	}
	va_end(vl);
	return q_iter;
};
