#include <stdio.h>
#include "sox_api2.h"

#define ROWS    (30)
#define COLS    (40)
#define VSPACE   (4)
#define HSPACE   (2)

char points[ROWS][COLS];
char ships[ROWS][COLS];

int draw_grid()
{
    int r, c;
    for (r = 0; r < ROWS; r++) {
        if (r % HSPACE == 0) {
            for (c = 0; c < COLS; c++) {
                printf("-");
            }
        } else {
            for (c = 0; c < COLS; c++) {
                if (points[r][c]) {
                    printf("%c", points[r][c]);
                }
                else if (c % VSPACE == 1) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

int place_ships()
{
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            points[r][c] = ships[r][c];
        }
    }
    return 0;
}

int boom()
{
    printf("%c", 0x7);
    return 0;
}

int main()
{
    ships[3][3] = '#';
    ships[3][7] = '#';
    ships[3][11] = '#';
    place_ships();
    points[5][3] = 'o';
    points[5][7] = 'x';
    draw_grid();
    boom();
    return 0;
}
