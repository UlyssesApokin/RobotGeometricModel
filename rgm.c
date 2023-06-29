/*
Created: Ulysses Apokin
Data: 28.06.2023
Name: Robot Geometric Model (rgm.c)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14
/*Maximum number of pair*/
#define NMAX 10
/*Size of mcos and lvec matrix*/
#define SIZE_MTX 3
/*Size of S and S* (Sh) matrix*/
#define SIZE_TRANS_MTX 4

/*Type of kinematic pair*/
#define ROTARY_PAIR_TYPE_1 1
#define ROTARY_PAIR_TYPE_2 2
#define MOVING_PAIR 3

/*
Fill array of Aii* matrix
Aii* is relation between S_matrix and S*matrix
Aii* was named "mtx_s_sh"
*/
void fill_arr_of_mtx_s_sh(
	int num_of_pair,
	const double lvec[SIZE_MTX][NMAX],
	const double (*mcos)[SIZE_MTX][NMAX],
	double (*a_mtx_s_sh)[SIZE_TRANS_MTX][NMAX]
	)
{
	int count, i, j;
	enum {coord_of_new_sys = 3, initial_pos = 3};
	if (num_of_pair >= NMAX)
		return;
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
	if (num_of_pair >= NMAX)
		return;
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
	double (*mtx_s_shm1)[SIZE_TRANS_MTX][NMAX]
	)
{
	int n, i, j;
	if (num_of_pair >= NMAX)
		return;
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
	int ord_left,
	const double (*mtx_left)[SIZE_TRANS_MTX][NMAX],
	const int ord_right,
	const double (*mtx_right)[SIZE_TRANS_MTX][NMAX],
	const int ord_rez,
	double (*mtx_rez)[SIZE_TRANS_MTX][NMAX]
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
	const double (*mtx_s_sh)[SIZE_TRANS_MTX][NMAX],
	const double (*mtx_s_shm1)[SIZE_TRANS_MTX][NMAX],
	double (*mtx_s_sm1)[SIZE_TRANS_MTX][NMAX]
	)
{
	int n;
	if (num_of_pair > NMAX)
		return;
	for (n = 0; n < num_of_pair; n++)
		mult_matrix(n, mtx_s_sh, n, mtx_s_shm1, n, mtx_s_sm1);
}

/*
Fill array of A0k matix
A0k is relation between S_matrix and S0 matrix
A0k was named "mtx_s_s0"
*/
void fill_arr_of_mtx_s_s0(
	const int num_of_pair,
	const double (*mtx_s_sm1)[SIZE_TRANS_MTX][NMAX],
	double (*mtx_s_s0)[SIZE_TRANS_MTX][NMAX]
	)
{
	int i, j, n;
	for (i = 0; i < SIZE_TRANS_MTX; i++)
	{
		for (j = 0; j < SIZE_TRANS_MTX; j++)
			mtx_s_s0[i][j][0] = mtx_s_sm1[i][j][0];
	}
	for (n = 1; n < SIZE_TRANS_MTX; n++)
	mult_matrix(n-1, mtx_s_s0, n, mtx_s_sm1, n, mtx_s_s0);		
}

void print_matrix(int n, const double (*arr)[SIZE_TRANS_MTX][NMAX])
{
	int i, j;
	printf("K = %d\n\n", n+1);
	for (i = 0; i < SIZE_TRANS_MTX; i++)
	{
		for ( j = 0; j < SIZE_TRANS_MTX; j++)
			printf("%4.0f", arr[i][j][n]);
		printf("\n");
	}
	printf("\n");
}

void print_vec(int n, const double* p)
{
	int i;
	for (i = 0; i < n; i++)
		printf("%5.1f", *(p + i));
	printf("\n");
}

int main()
{
	int ord_of_pair, num_of_pair = 3;
	double betavec[3];
	int pvec[3] =
		{ROTARY_PAIR_TYPE_2, ROTARY_PAIR_TYPE_1, MOVING_PAIR};
	double qvec[SIZE_MTX] = {0.0, M_PI/2, 1.0};
	double mtx_s_sh[SIZE_TRANS_MTX][SIZE_TRANS_MTX][NMAX];
	double mtx_s_shm1[SIZE_TRANS_MTX][SIZE_TRANS_MTX][NMAX];
	double mtx_s_sm1[SIZE_TRANS_MTX][SIZE_TRANS_MTX][NMAX];
	double mtx_s_s0[SIZE_TRANS_MTX][SIZE_TRANS_MTX][NMAX];
	double mcos[SIZE_MTX][SIZE_MTX][NMAX];
	double lvec[SIZE_TRANS_MTX][NMAX];
  
	mcos[0][0][0] = 1.0; mcos[0][1][0] = 0.0; mcos[0][2][0] = 0.0;
  mcos[1][0][0] = 0.0; mcos[1][1][0] = 1.0; mcos[1][2][0] = 0.0;
  mcos[2][0][0] = 0.0; mcos[2][1][0] = 0.0; mcos[2][2][0] = 1.0;
  
  mcos[0][0][1] = 0.0; mcos[0][1][1] = 0.0; mcos[0][2][1] = 1.0;
  mcos[1][0][1] = 1.0; mcos[1][1][1] = 0.0; mcos[1][2][1] = 0.0;
  mcos[2][0][1] = 0.0; mcos[2][1][1] = 1.0; mcos[2][2][1] = 0.0;

  mcos[0][0][2] = 0.0; mcos[0][1][2] = 0.0; mcos[0][2][2] = 1.0;
  mcos[1][0][2] = 1.0; mcos[1][1][2] = 0.0; mcos[1][2][2] = 0.0;
  mcos[2][0][2] = 0.0; mcos[2][1][2] = 1.0; mcos[2][2][2] = 0.0;

  mcos[0][0][3] = 0.0; mcos[0][1][3] = 1.0; mcos[0][2][3] = 0.0;
  mcos[1][0][3] = 0.0; mcos[1][1][3] = 0.0; mcos[1][2][3] = 1.0;
  mcos[2][0][3] = 1.0; mcos[2][1][3] = 0.0; mcos[2][2][3] = 0.0;

  lvec[0][0] = 0.0; lvec[1][0] = 0.0; lvec[2][0] = 0.0;
  lvec[0][1] = 0.0; lvec[1][1] = 0.0; lvec[2][1] = 1.0;
  lvec[0][2] = 2.0; lvec[1][2] = 0.0; lvec[2][2] = 0.0;
  lvec[0][3] = 0.0; lvec[1][3] = 0.0; lvec[2][3] = 3.0;

  /* calculate and print mtx_s_sh (Aii*) */
	fill_arr_of_mtx_s_sh(num_of_pair, lvec, mcos, mtx_s_sh);
	printf("\n[Aii*]:\n\n");
	for (ord_of_pair = 0; ord_of_pair <= num_of_pair; ord_of_pair++)
		print_matrix(ord_of_pair, mtx_s_sh);
  /* calculate and print beta-vector */
	fill_arr_of_betavec(num_of_pair, pvec, betavec);
	printf("\n{beta}:");
	print_vec(num_of_pair, betavec);
  /* calculate and print mtx_s_shm1 (A(i-1),j*) */
	fill_arr_of_mtx_s_shm1(num_of_pair, betavec, qvec, mtx_s_shm1);
	printf("\n[Ai*j]:\n\n");
	for (ord_of_pair = 0; ord_of_pair < num_of_pair; ord_of_pair++)
		print_matrix(ord_of_pair, mtx_s_shm1);
  /* calculate and print mtx_s_sm1 (A(i-1),j) */
	fill_arr_of_mtx_s_sm1(num_of_pair, mtx_s_sh, mtx_s_shm1, mtx_s_sm1);
	printf("\n[Aij]:\n\n");
	for (ord_of_pair = 0; ord_of_pair < num_of_pair; ord_of_pair++)
		print_matrix(ord_of_pair, mtx_s_sm1);
  /* calculate and print mtx_s_s0 (A0i) */
	fill_arr_of_mtx_s_s0(num_of_pair, mtx_s_sm1, mtx_s_s0);
	printf("\n[A0i]:\n\n");
	for (ord_of_pair = 0; ord_of_pair < num_of_pair; ord_of_pair++)
		print_matrix(ord_of_pair, mtx_s_s0);
	return 0;
}
