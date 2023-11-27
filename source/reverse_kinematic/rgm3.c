#include <stdlib.h>
#include <math.h>
#include "rgm3_reverse_kinematic.h"
#include "rgm3_output.h"
#define L1 60.0
#define L2 50.0
#define L3 25.0
double tcp_xxh(const double *q) {
	return cos(q[0]) * sin(q[2]);
}
double tcp_xyh(const double *q) {
	return cos(q[0]) * cos(q[2]);
}
double tcp_xzh(const double *q) {
	return -sin(q[0]);
}
double tcp_xp(const double *q) {
	return L3 * cos(q[0]) * sin(q[2]);
}
double tcp_yxh(const double *q) {
	return sin(q[0]) * sin(q[2]);
}
double tcp_yyh(const double *q) {
	return sin(q[0]) * cos(q[2]);
}
double tcp_yzh(const double *q) {
	return cos(q[0]);
}
double tcp_yp(const double *q) {
	return L3 * sin(q[0]) * sin(q[2]);
}
double tcp_zxh(const double *q) {
	return cos(q[2]);
}
double tcp_zyh(const double *q) {
	return -sin(q[2]);
}
double tcp_zzh(const double *q) {
	return 0;
}
double tcp_zp(const double *q) {
	return L3 * cos(q[2]) + q[1] + L1 + L2;
}
double tcp_nul(const double *q) {
	return 0;
}
double tcp_one(const double *q) {
	return 1;
}
/*Количество сочленений манипулятора*/
enum {pairs = 3};
/*Массив double величиной, равной количеству
сочленений конкретного робота*/
double q[pairs] = {0.0, 0.0, PI/4};
/*Массив double равный массиву q, предназначенный
для временного хранения итерационного значения*/
double q_iter[pairs] = {0.0, 0.0, PI/4};
/*Массив double, хранящий значения ограничений
изменений обобщенных координат*/
double limit[pairs*2] = {-PI, PI, -30, 30, -5*PI/6, 5*PI/6};
/*Массив double, который будет хранить величину
приращения обобщенной координаты*/
double delta[3];
/*Переменную типа unsigned int, которая будет
хранить делитель, необходимый для вычисления
величины приращения обобщенной координаты*/
unsigned int division = 100;
/*Массив типа double на 16 элементов, который будет
хранить положение и ориентацию конечной матрицы
поворота робота*/
enum {matrix = 4};
double final[matrix*matrix] = {
    0.250,	0.433,	0.866, 	6.250,
    -0.433, -0.750,	0.500, 	-10.825,
    0.866,	-0.5,	0, 		131.651,
    0,		0, 		0,		1
};
/*А также массив на 16 элементов, который будет
хранить указатели на функции с прототипом
double()(const double*), т. е. однородную матрицу
преобразования обобщенных координат в общем виде*/
double (*tcp_matrix[matrix*matrix])(const double*) = {
	tcp_xxh, tcp_xyh, tcp_xzh, tcp_xp,
  	tcp_yxh, tcp_yyh, tcp_yzh, tcp_yp,
  	tcp_zxh, tcp_zyh, tcp_zzh, tcp_zp,
  	tcp_nul, tcp_nul, tcp_nul, tcp_one
};
/*Переменная для хранения количества итераций*/
int iter = 0;
int i;
/*указатель для хранения матрицы при текущих
значения обобщенных координат*/
double *tmatrix;
/*указатель для хранения матрицы абсолютной
ошибки*/
double *amatrix;
/*указатель для хранения матрицы относительной
ошибки*/
double *rmatrix;
int main(int argc, char **argv)
{
	do {
		/*устанавливаем приращение обобщенной координаты
		для первого сочленения*/
		delta[0] = set_iteration_step(0, limit, division);
		/*устанавливаем приращение обобщенной координаты
		для второго сочленения*/
		delta[1] = set_iteration_step(1, limit, division);
		/*устанавливаем приращение обобщенной координаты
		для третьего сочленения*/
		delta[2] = set_iteration_step(2, limit, division);
		/*Первый этап: поворот первого сочленения относительно стойки*/
		do {
			/*считаем количество итераций*/
			iter++;
			/*присваиваем новое значение обобщенной координаты*/
			q[0] = q_iter[0]; 
			/*отправляем в стандартный поток инф-ию об изменении*/
			term_print_gen_coord(iter, 3, q);
			/*итерируем обобщенную координату до тех пор*/
			q_iter[0] = do_iter_step_position(tcp_matrix, final, 3, 0, delta, q);
			/*пока возможно приближение*/
		} while (q[0] != q_iter[0]);
		/*Второй этап: достижение нужной ориентации схвата
		третьим сочленением*/
		do {
			/*считаем количество итераций*/
			iter++;
			/*присваиваем новое значение обобщенной координаты*/
			q[2] = q_iter[2];
			/*отправляем в стандартный поток инф-ию об изменении*/
			term_print_gen_coord(iter, 3, q);
			/*итерируем обобщенную координату до тех пор*/
			q_iter[2] = do_iter_step_orientation(tcp_matrix, final, 3, 2, delta, q);
			/*пока возможно приближение*/
		} while (q[2] != q_iter[2]);
		/*Третий этап: второе сочленение подводит схват в 
		необходимую точку*/
		do {
			iter++;
			q[1] = q_iter[1];
			term_print_gen_coord(iter, 3, q);
			q_iter[1] = do_iter_step_position(tcp_matrix, final, 3, 1, delta, q);
		} while (q[1] != q_iter[1]);
		/*считаем текущую матрицу однородного
		преобразования координат*/
		tmatrix = get_tcp_matrix(tcp_matrix, q);
		/*считаем абсолютную ошибку*/
		amatrix = get_absolute_error_matrix(tmatrix, final);
		/*считаем относительную ошибку*/
		rmatrix = get_relative_error_matrix(tmatrix, final);
		/*увеличиваем точность*/
		division *= 1.25;
	} while (get_average_error_of_position(rmatrix) > 0.5);
	/*отправляем в стандартный поток вывода
	финальное значение обобщенных координат*/
	term_print_gen_coord(iter, 3, q);
	/*матрица позиции полученая приближением*/
	term_print_matrix("ITERATION POSITION", tmatrix, 4, 4);
	/*матрица требуемой позиции*/
	term_print_matrix("FINAL POSITION", final, 4, 4);
	/*матрица абсолютной ошибки*/
	term_print_matrix("ABSOLUTE ERROR", amatrix, 4, 4);
	/*матрица относительной ошибки*/
	term_print_matrix("RELATIVE ERROR", rmatrix, 4, 4);
	/*суммарный вывод ошибок в процентах*/
	term_print_error(1,
		get_max_error_of_position(rmatrix),
		get_average_error_of_position(rmatrix),
		get_max_error_of_orientaion(rmatrix),
		get_average_error_of_orientaion(rmatrix)
		);
	free(rmatrix);
	free(amatrix);
	free(tmatrix);
	return 0;
}
