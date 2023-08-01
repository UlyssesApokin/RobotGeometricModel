/*
Created: Ulysses Apokin
Data: 06.07.2023
Name: OpenGL Robot Geometric Model Visualisator (opengl_rgm.c)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>
#define M_PI 3.14

/*Size of mcos and lvec matrix*/
#define SIZE_MTX 3
/*Size of S and S* (Sh) matrix*/
#define SIZE_TRANS_MTX 4

/*Type of kinematic pair*/
#define ROTARY_PAIR_TYPE_1 1
#define ROTARY_PAIR_TYPE_2 2
#define MOVING_PAIR 3

enum {base = 1};
 
int width_scr = 1280, height_scr = 720;

struct Robot
{
    double base[3];
    double pair_1[3];
    double pair_2[3];
    double tcp[3];
};

/*
Fill array of Aii* matrix
Aii* is relation between S_matrix and S*matrix
Aii* was named "mtx_s_sh"
*/
void fill_arr_of_mtx_s_sh(
	int num_of_pair,
	const double lvec[SIZE_MTX][num_of_pair + base],
	const double (*mcos)[SIZE_MTX][num_of_pair + base],
	double (*a_mtx_s_sh)[SIZE_TRANS_MTX][num_of_pair + base]
	)
{
	int count, i, j;
	enum {coord_of_new_sys = 3, initial_pos = 3};

	for (count = 0; count <= num_of_pair; count++)
	{
		for (i = 0; i < SIZE_MTX; i++)
		{
			for (j = 0; j < SIZE_MTX; j++)
				a_mtx_s_sh[i][j][count] = mcos[i][j][count];
		}
		for (i = 0; i < initial_pos; i++)
		{
			a_mtx_s_sh[i][coord_of_new_sys][count] = lvec[i][count];
		}
		for (j = 0; j < coord_of_new_sys; j++)
		{
			a_mtx_s_sh[initial_pos][j][count] = 0.0;
		}
		a_mtx_s_sh[initial_pos][coord_of_new_sys][count] = 1.0;
	}
}

void fill_arr_of_betavec(
	int num_of_pair,
	const int *pvec,
	double *betavec
	)
{
	int n;

	for (n = 0; n < num_of_pair; n++)
	{
		if (pvec[n] == ROTARY_PAIR_TYPE_1 || pvec[n] == ROTARY_PAIR_TYPE_2)
			betavec[n] = 1.0;
		else if (pvec[n] == MOVING_PAIR)
			betavec[n] = 0.0;
		else
			return;
	}
}

/*
Fill array of A(i-1)*,j matrix
A(i-1)*,j is relation between S_matrix and (S-1)* matrix
(S-1)* matix was named "shm1" {S Hard matix Minus 1}
Continuely A(i-1)*,j was named "mtx_s_shm1
*/
void fill_arr_of_mtx_s_shm1(
	int num_of_pair,
	const double *betavec,
	const double *qvec,
	double (*mtx_s_shm1)[SIZE_TRANS_MTX][num_of_pair + base]
	)
{
	int n, i, j;

	for (n = 0; n < num_of_pair; n++)
		for (i = 0; i < SIZE_TRANS_MTX; i++)
			for(j = 0; j < SIZE_TRANS_MTX; j++)
			{
				mtx_s_shm1[i][j][n] = 0.0;
				if (i == 0 && j == 0)
					mtx_s_shm1[i][j][n] = cos(betavec[n]*qvec[n]);
				if (i == 0 && j == 1)
					mtx_s_shm1[i][j][n] = -sin(betavec[n]*qvec[n]);
				if (i == 1 && j == 0)
					mtx_s_shm1[i][j][n] = sin(betavec[n]*qvec[n]);
				if (i == 1 && j == 1)
					mtx_s_shm1[i][j][n] = cos(betavec[n]*qvec[n]);
				if (i == 2 && j == 2)
					mtx_s_shm1[i][j][n] = 1.0;
				if (i == 2 && j == 3)
					mtx_s_shm1[i][j][n] = (1.0 - betavec[n])*qvec[n];
				if (i == 3 && j == 3)
					mtx_s_shm1[i][j][n] = 1.0;
			}
}
		
void mult_matrix(
  int size_of_mtx,
	int ord_left,
	const double (*mtx_left)[SIZE_TRANS_MTX][size_of_mtx],
	const int ord_right,
	const double (*mtx_right)[SIZE_TRANS_MTX][size_of_mtx],
	const int ord_rez,
	double (*mtx_rez)[SIZE_TRANS_MTX][size_of_mtx]
	)
{
	int n, i, j;
	for (i = 0; i < SIZE_TRANS_MTX; i++)
		for (j = 0; j < SIZE_TRANS_MTX; j++)
		{
			mtx_rez[i][j][ord_rez] = 0.0;
			for (n = 0; n < SIZE_TRANS_MTX; n++)
				mtx_rez[i][j][ord_rez] =
				mtx_rez[i][j][ord_rez] +
				mtx_left[i][n][ord_left] *
				mtx_right[n][j][ord_right];
		}
}

/*
Fill array of A(i-1),j matrix
A(i-1),j is relation between S_matrix and (S-1) matrix
A(i-1),j was named "mtx_s_sm1"
*/
void fill_arr_of_mtx_s_sm1(
	int num_of_pair,
	const double (*mtx_s_sh)[SIZE_TRANS_MTX][num_of_pair + base],
	const double (*mtx_s_shm1)[SIZE_TRANS_MTX][num_of_pair + base],
	double (*mtx_s_sm1)[SIZE_TRANS_MTX][num_of_pair + base]
	)
{
	int n;

	for (n = 0; n < num_of_pair; n++)
		mult_matrix(num_of_pair + base, n, mtx_s_sh, n, mtx_s_shm1,
			n, mtx_s_sm1);
}

/*
Fill array of A0k matix
A0k is relation between S_matrix and S0 matrix
A0k was named "mtx_s_s0"
*/
void fill_arr_of_mtx_s_s0(
	int num_of_pair,
	const double (*mtx_s_sm1)[SIZE_TRANS_MTX][num_of_pair +base],
	double (*mtx_s_s0)[SIZE_TRANS_MTX][num_of_pair + base]
	)
{
	int i, j, n;
	for (i = 0; i < SIZE_TRANS_MTX; i++)
	{
		for (j = 0; j < SIZE_TRANS_MTX; j++)
			mtx_s_s0[i][j][0] = mtx_s_sm1[i][j][0];
	}
	for (n = 1; n < SIZE_TRANS_MTX; n++)
	mult_matrix(num_of_pair + base, n-1, mtx_s_s0, n, mtx_s_sm1,
		n, mtx_s_s0);		
}

void append_arr_of_mtx_s_s0(
  int num_of_pair,
  const double (*mtx_s_sm1)[SIZE_TRANS_MTX][num_of_pair +base],
  const double (*mtx_s_sh)[SIZE_TRANS_MTX][num_of_pair +base],
  double (*mtx_s_s0)[SIZE_TRANS_MTX][num_of_pair + base]
  )
{
  mult_matrix(num_of_pair + base, 2, mtx_s_s0, 3, mtx_s_sh,
	3, mtx_s_s0);
}

void create_points(
	int num_of_pair, int ord_of_pair,
	const double (*mtx)[4][num_of_pair+base],
	double *coord
	)
{
	enum {x, y, z, mov};
	int i;
	if (num_of_pair <= 0)
		return;
	if (ord_of_pair > num_of_pair)
		return;
	for (i = x; i <= z; i++)
		coord[i] = mtx[i][mov][ord_of_pair];
}

void init_rgm(struct Robot *robot)
{
	enum {p0, p1, p2, p3};
    int num_of_pair = 3; /*init*/
	double betavec[num_of_pair];
	int pvec[] =
		{ROTARY_PAIR_TYPE_2, MOVING_PAIR, ROTARY_PAIR_TYPE_1}; /*init*/
	double qvec[] = {0, -1, M_PI/2}; /*init*/
	double mtx_s_sh[SIZE_TRANS_MTX][SIZE_TRANS_MTX][num_of_pair + base];
	double mtx_s_shm1[SIZE_TRANS_MTX][SIZE_TRANS_MTX][num_of_pair+base];
	double mtx_s_sm1[SIZE_TRANS_MTX][SIZE_TRANS_MTX][num_of_pair+base];
	double mtx_s_s0[SIZE_TRANS_MTX][SIZE_TRANS_MTX][num_of_pair + base];
	double mcos[SIZE_MTX][SIZE_MTX][num_of_pair + base]; /*init*/
	double lvec[SIZE_TRANS_MTX][num_of_pair + base]; /*init*/
	/*Matrix M0*/
	mcos[0][0][0] = 1.0;	mcos[0][1][0] = 0.0;	mcos[0][2][0] = 0.0;
	mcos[1][0][0] = 0.0;	mcos[1][1][0] = 1.0;	mcos[1][2][0] = 0.0;
	mcos[2][0][0] = 0.0;	mcos[2][1][0] = 0.0; 	mcos[2][2][0] = 1.0;
	/*Matrix M1*/
	mcos[0][0][1] = 0.0;	mcos[0][1][1] = -1.0;	mcos[0][2][1] = 0.0;
	mcos[1][0][1] = 1.0;	mcos[1][1][1] = 0.0;	mcos[1][2][1] = 0.0;
	mcos[2][0][1] = 0.0;	mcos[2][1][1] = 0.0;	mcos[2][2][1] = 1.0;
	/*Matrix M3*/
	mcos[0][0][2] = 0.0;	mcos[0][1][2] = -1.0;	mcos[0][2][2] = 0.0;
	mcos[1][0][2] = 0.0;	mcos[1][1][2] = 0.0;	mcos[1][2][2] = -1.0;
	mcos[2][0][2] = 1.0;	mcos[2][1][2] = 0.0;	mcos[2][2][2] = 0.0;
	/*Matrix M4*/
	mcos[0][0][3] = 0.0;	mcos[0][1][3] = 0.0;	mcos[0][2][3] = 1.0;
	mcos[1][0][3] = 0.0; 	mcos[1][1][3] = -1.0;	mcos[1][2][3] = 0.0;
	mcos[2][0][3] = 1.0;	mcos[2][1][3] = 0.0;	mcos[2][2][3] = 0.0;
	/*X axis		Y axis		Z axis*/
	/*L0*/
	lvec[0][0] = 0.0;	lvec[1][0] = 0.0;	lvec[2][0] = 0.0;
	/*L1*/
	lvec[0][1] = 0.0;	lvec[1][1] = 0.0;	lvec[2][1] = 6.0;
	/*L2*/
	lvec[0][2] = 0.0;	lvec[1][2] = 0.0;	lvec[2][2] = 3.0;
	/*L3*/
	lvec[0][3] = 2.0;	lvec[1][3] = 0.0;	lvec[2][3] = 0.0;
	fill_arr_of_mtx_s_sh(num_of_pair, lvec, mcos, mtx_s_sh);
    fill_arr_of_betavec(num_of_pair, pvec, betavec);
    fill_arr_of_mtx_s_shm1(num_of_pair, betavec, qvec, mtx_s_shm1);
    fill_arr_of_mtx_s_sm1(num_of_pair, mtx_s_sh, mtx_s_shm1, mtx_s_sm1);
    fill_arr_of_mtx_s_s0(num_of_pair, mtx_s_sm1, mtx_s_s0);
    append_arr_of_mtx_s_s0(num_of_pair, mtx_s_sm1, mtx_s_sh, mtx_s_s0);
	create_points(num_of_pair, p0, mtx_s_s0, robot->base);
	create_points(num_of_pair, p1, mtx_s_s0, robot->pair_1);
	create_points(num_of_pair, p2, mtx_s_s0, robot->pair_2);
	create_points(num_of_pair, p3, mtx_s_s0, robot->tcp);
}

void print_std_robot(const struct Robot *robot)
{
    int i;
    for (i = 0; i < 3; i++)
		printf("Robot.base[%d] = %lf\n", i, robot->base[i]);
	putchar('\n');
	for (i = 0; i < 3; i++)
		printf("Robot.pair_1[%d] = %lf\n", i, robot->pair_1[i]);
	putchar('\n');
	for (i = 0; i < 3; i++)
		printf("Robot.pair_2[%d] = %lf\n", i, robot->pair_2[i]);
	putchar('\n');
	for (i = 0; i < 3; i++)
		printf("Robot.tcp[%d] = %lf\n", i, robot->tcp[i]);
	putchar('\n');	
}

void draw_display(void)
{
    struct Robot robot;
    enum {s = 25, y = 10, b = 50, d = 50, width_line = 5};
    init_rgm(&robot);
    print_std_robot(&robot);
    glClearColor(0.7f, 0.7f, 0.75f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 5, 20);
    glLineWidth(width_line);
    glBegin(GL_LINES);
        glVertex3f(robot.base[1]*s+b, robot.base[2]*s+d, robot.base[0]);
        glVertex3f(robot.pair_1[1]*s+b, robot.pair_1[2]*s+d, robot.pair_1[0]/y);
        glVertex3f(robot.pair_1[1]*s+b, robot.pair_1[2]*s+d, robot.pair_1[0]/y);
        glVertex3f(robot.pair_2[1]*s+b, robot.pair_2[2]*s+d, robot.pair_2[0]/y);
        glVertex3f(robot.pair_2[1]*s+b, robot.pair_2[2]*s+d, robot.pair_2[0]/y);
        glVertex3f(robot.tcp[1]*s+b, robot.tcp[2]*s+d, robot.tcp[0]/y);
    glEnd();
    glColor3ub(5, 255, 20);
    glLineWidth(width_line-3);
    glBegin(GL_LINES);
        glVertex3f(robot.base[1]*s+b, robot.base[2]*s+d, robot.base[0]/y);
        glVertex3f(robot.tcp[1]*s+b, robot.tcp[2]*s+d, robot.tcp[0]/y);
    glEnd();
    glFinish();
    
}

void reshape_scr(int w, int h)
{
    width_scr = w;
    height_scr = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
}

void read_keyboard(unsigned char key, int x, int y)
{
#define ESCAPE 27
    if (key == ESCAPE)
        exit(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width_scr, height_scr);
    glutCreateWindow("OpenGL Robot Geometic Model Visualisator");
    glutDisplayFunc(draw_display);
    glutReshapeFunc(reshape_scr);
    glutKeyboardFunc(read_keyboard);
    glutMainLoop();
    return 0;
}
