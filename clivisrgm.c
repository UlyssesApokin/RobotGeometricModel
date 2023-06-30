/*
Created: Ulysses Apokin
Data: 29.06.2023
Name: Command Line Interfase Visualisator Robot Geometric Model (clivisrgm.c)
*/
 
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>

#define NMAX 10
#define SIZE_MTX 3
#define SIZE_TRANS_MTX 4

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

void print_line(
    double y1, double x1, double y2, double x2,
    char bold, char light
    )
{
    const double light_step= 0.25;
    double y = y1, x = x1;
    if ((y2 - y1) != 0)
    {
        do
        {
            move((int)y, (int)x);
            addch(light);
            y = (y1 > y2) ? y - light_step : y + light_step;
            x = ((y - y1)*(x2 - x1))/(y2 - y1) + x1;
        } while((int)y != (int)y2);
        y = y1, x = x1;
        do
        {
            move((int)y, (int)x);
            addch(bold);
            y = (y1 > y2) ? y - 1.0 : y + 1.0;
            x = ((y - y1)*(x2 - x1))/(y2 - y1) + x1;
        } while((int)y != (int)y2);
    }
    else if ((x2 - x1) != 0)
    {
        do
        {
            move((int)y, (int)x);
            addch(light);
            x = (x1 > x2) ? x - light_step : x + light_step;
            y = ((x - x1)*(y2 - y1))/(x2 - x1) + y1;
        } while((int)x != (int)x2);
        y = y1, x = x1;
        do
        {
            move((int)y, (int)x);
            addch(bold);
            x = (x1 > x2) ? x - 1.0 : x + 1.0;
            y = ((x - x1)*(y2 - y1))/(x2 - x1) + y1;
        } while((int)x != (int)x2);
    }
    else
    {
        move((int)y, (int)x);
        addch(bold);
    }
    refresh();
}

void print_projection(
    const double p[SIZE_TRANS_MTX][NMAX],
    int a_axis, int b_axis,
    int y, int x
    )
{
    int i, j;
    enum {num_of_axis = 2, scale = 5};
    double points[num_of_axis][NMAX];
    for (j = 0; j < NMAX; j++)
    {
        points[0][j] = p[a_axis][j]*scale;
        points[1][j] = p[b_axis][j]*scale;
    }
    points[0][3] = points[0][3] + points[0][2] + points[0][1] + points[0][0];
    points[0][2] = points[0][2] + points[0][1] + points[0][0];
    points[0][1] = points[0][1] + points[0][0];
    points[1][3] = points[1][3] + points[1][2] + points[1][1] + points[1][0];
    points[1][2] = points[1][2] + points[1][1] + points[1][0];
    points[1][1] = points[1][1] + points[1][0];
    for (j = 0; j < 3; j++)
        print_line(points[0][j]+y, points[1][j]+x,
        points[0][j+1]+y, points[1][j+1]+x, '#', '_');
        
    print_line(y-1, x-1, y+10, x-1, '|', '|');
    move(y+10, x-1);
    if (a_axis == X_AXIS)
        addch('X');
    else if (a_axis == Y_AXIS)
        addch('Y');
    else if (a_axis == Z_AXIS)
        addch('Z');
        
    print_line(y-1, x-1, y-1, x+10, '-', '-');
    move(y-1, x+10);
    if (b_axis == X_AXIS)
        addch('X');
    else if (b_axis == Y_AXIS)
        addch('Y');
    else if (b_axis == Z_AXIS)
        addch('Z');
        
    move(y-2,x);
    if (b_axis == X_AXIS)
        addch('X');
    else if (b_axis == Y_AXIS)
        addch('Y');
    else if (b_axis == Z_AXIS)
        addch('Z');
    move(y-2, x+1);
    addch('-');
    move(y-2, x+2);
    if (a_axis == X_AXIS)
        addch('X');
    else if (a_axis == Y_AXIS)
        addch('Y');
    else if (a_axis == Z_AXIS)
        addch('Z'); 
    refresh();
}    

int main()
{
    int row, col;
    initscr();
    getmaxyx(stdscr, row, col);
    curs_set(0);
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
    /*X*/              /*Y*/            /*Z*/
    lvec[0][0] = 0.0; lvec[1][0] = 0.0; lvec[2][0] = 0.0;
    lvec[0][1] = 0.0; lvec[1][1] = 0.0; lvec[2][1] = 1.0;
    lvec[0][2] = 2.0; lvec[1][2] = 0.0; lvec[2][2] = 0.0;
    lvec[0][3] = 0.0; lvec[1][3] = 0.0; lvec[2][3] = 3.0;
    /*X-Y*/
    print_projection(lvec, Y_AXIS, X_AXIS, 24, 40);
    print_projection(lvec, Y_AXIS, Z_AXIS, 24, 8);
    print_projection(lvec, Z_AXIS, X_AXIS, 2, 40);
    sleep(600);
    endwin();
    return 0;
}
