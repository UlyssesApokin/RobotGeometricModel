#ifndef RGM3_REVERSE_KINEMATIC_H_SENTRY
#define RGM3_REVERSE_KINEMATIC_H_SENTRY
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
double iterative_inc_of_gen_coord(double q, int sign, double delta_q);

double vector_current_final(double(***f)(double, ...),
	double final[3][4], double q1, ...);

double iteration_position_step(double(***f)(double, ...), double final[3][4],
	int type_q, double delta_q, double q1, ...);

#endif
