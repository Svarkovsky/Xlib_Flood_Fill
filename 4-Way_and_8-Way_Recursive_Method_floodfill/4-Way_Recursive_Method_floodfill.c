#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gfx.h"
// cc 4-Way_Recursive_Method_floodfill.c gfx.c -o 4-Way_Recursive_Method_floodfill -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lm

#define ysize 600
#define xsize 600
#define Xsize xsize / 2        // условный центр
#define Ysize ysize / 2        // 300
#define A ysize / 60           // 10
#define B ysize - (ysize / 60) // 590
#define D ysize / 60           // 10
#define C ysize - (ysize / 60) // 590
#define N xsize / 10           // 60

void info(void)
{
#define couleur(param) printf("\033[%sm", param)
    couleur("32");
    printf("Ivan Svarkovsky (c) 2019 \n");
    couleur("0");
}
void m_fill(int n, int m, int r, int a[n][m][r], int step)
{
    // последовательное заполнение массива a[n][0][0] и  a[n][0][1]
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int k = 0; k < r; k++)
            {
                a[i][j][k] = step * i + (step / 2); //step * i;    step + (step / 2) * i + (step / 2);
            }
}
void m_print(int n, int m, int r, int a[][m][r]) // а старшую (первую) размерность можно опустить !!!
{
    // вывод массива
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < r; k++)
            {
                printf("%6d -> [%d][%d][%d] ", a[i][j][k], i, j, k);
            }
            printf("\n");
        }
    }
}
void m_delmem(int n, int m, int r, int a[][m][r])
{
    // отказ от использования, предварительно заполнили 0
    if (a != NULL)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                for (int k = 0; k < r; k++)
                {
                    a[i][j][k] = 0;
                }
            }
        }
        free(a);
        a = NULL;
    }
}
void draw_borders(int n) // очертим границы
{
    gfx_point(Xsize, Ysize);              // Нарисовать одну точку в центре
    gfx_point(A - n, D - n);              // верхняя левая
    gfx_point(B + n, D - n);              // верхняя правая
    gfx_point(A - n, C + n);              // нижняя левая
    gfx_point(B + n, C + n);              // нижняя правая
    gfx_line(A - n, D - n, B + n, D - n); // горизонтальная верхняя
    gfx_line(A - n, C + n, B + n, C + n); // горизонтальная нижняя
    gfx_line(A - n, D - n, A - n, C + n); // вертикальная левая
    gfx_line(B + n, C + n, B + n, D - n); // вертикальная правая
}
void diagonal()
{
    gfx_line(B, C, A, D); // диагональ
    gfx_line(A, C, B, D); // диагональ
}
void cell_division(int n) // деление на клетки
{
    for (int i = 0; i <= C; i = (i + n)) //
    {
        gfx_line(A + i, D, A + i, C); // вертикальная левая
        gfx_line(A, D + i, B, D + i); // горизонтальная верхняя
    }
}
void m_filling(int x, int y, int step) // заливка одной ячейки
{
    step = step - 4;
    x = x - (A / 2) + 2; //
    y = y - (A / 2) + 2;
    int t_A = x;
    int t_D = y; //
    for (;;)
    {
        // printf("%d t_A %d t_D \n", t_A, t_D);
        gfx_point(t_A, t_D); // Нарисовать одну точку в (х, у)
        if (t_A < x + step)  // выводим точки пока не достигнем
        {                    //  определенного края
            ++t_A;           // ход вправо
        }
        else if (t_D < y + step) //
        {
            t_A = x;
            ++t_D; // ход вниз
        }
        else
        {
            break;
        }
    }
}
void wall(int n, int m, int r, int a[][m][r], int step)
{
    int i, j, k;
    j = 1;
    k = 0;
    for (i = 0; i < n - 2; i++)
    {
        m_filling(a[j][k][k], a[i][j][k], step);
        m_filling(a[i][j][k], a[j][k][k], step);
    }
    j = N - 2;
    k = 0;
    for (i = 1; i < n - 1; i++)
    {
        m_filling(a[j][k][k], a[i][k][k], step);
        m_filling(a[i][k][k], a[j][k][k], step);
    }
}
void random_wall(int n, int m, int r, int a[][m][r], int step)
{
    srand(time(NULL));
    for (int r = 0; r < n; r++)
    {
        int i = 3 + rand() % (A * 5);
        int j = 3 + rand() % (A * 5); // начальное значение + rand() % конечное значение
        m_filling(a[i][m][m], a[j][m][m], step);
    }
}
int random_position(int n, int m, int r, int a[][m][r])
{
    srand(time(NULL));
    return a[3 + rand() % (A * 5)][m][r];
}
int m_flood_fill(int x, int y, int fill_color, int wall_color, int replaceable_color_0)
{
    //if (GetPix(x, y) == (replaceable_color_0 && replaceable_color_1)) //
    if (GetPix(x, y) == replaceable_color_0)
    {
        m_filling(x, y, A);
        // Basic 4 Way Recursive Method
        m_flood_fill(x - A, y, fill_color, wall_color, replaceable_color_0); //
        m_flood_fill(x, y + A, fill_color, wall_color, replaceable_color_0); //
        m_flood_fill(x, y - A, fill_color, wall_color, replaceable_color_0); //
        m_flood_fill(x + A, y, fill_color, wall_color, replaceable_color_0); //
    }
}

int main()
{
    info();
    gfx_open(xsize, ysize, "Flood fill - Graphics Program");
    int n = N;
    int m = 1;
    int r = 2;
    int nn = n * m * r;
    int *a = malloc(sizeof(*a) * nn);
    m_fill(n, m, r, (int(*)[m][r])a, A);
   // m_print(n, m, r, (int(*)[m][r])a);

    gfx_color(30, 30, 30); // Grey   Red =  30 Green =  30 Blue =  30 -- RGB int = 1973790
    cell_division(A);
    gfx_color(0, 0, 255); // Blue     // gfx_color(255, 0, 0); // Red

    for (int i = 0; i > -9; --i)
    {
        draw_borders(i);
    }

    random_wall(n, m, r, (int(*)[m][r])a, A);

    int fill_color = 16711680;         // RED (Red = 255 Green =   0 Blue =   0 -- RGB int = 16711680)
    int wall_color = 255;              // BLUE (Red =   0 Green =   0 Blue = 255 -- RGB int = 255)
    int replaceable_color_0 = 0;       // черный фон
    // int replaceable_color_1 = 1973790; // серая сетка ячеек

    // gfx_color(0, 0, 255); // Blue    gfx_color(255, 255, 255); // White    gfx_color(255, 0, 0); // Red

    gfx_color(255, 0, 0); // Red
    
    m_flood_fill(random_position(n, m, r, (int(*)[m][r])a), random_position(n, m, r, (int(*)[m][r])a), fill_color, wall_color, replaceable_color_0);

    
    gfx_color(0, 0, 0); //	Black
    m_filling(A, D, C);

    m_delmem(n, m, r, (int(*)[m][r])a);
    gfx_wait();
    return 0;
}
// x - 1, y | x + 1, y | x, y - 1 | x, y + 1
