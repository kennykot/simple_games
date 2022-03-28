// Copyright 2022 jewfish, camellia, sandslas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //  function usleep()
#define N 25
#define M 81

char **setup(void);
void draw(char **array);
char **logic(char **array);
int neighbours(char **array, int y, int x);
int check(char **array);
char **ravno(char **array);
int *equal(char **array, char **array_new, int *flag);

int main(void) {
    int flag = 0;
    int speed = 1000;
    int game_over = 1;
    int counter = 0;
    int command;
    char **field = NULL;
    char **field_new = NULL;
    scanf("%d", &command);
    if (command == 1) {
        speed = 400000;
    } else if (command == 2) {
        speed = 200000;
    } else if (command == 3) {
        speed = 150000;
    } else if (command == 4) {
        speed = 100000;
    } else if (command == 5) {
        speed = 75000;
    }
    field = setup();
    while (game_over > 0) {
        draw(field);
        field_new = (field);
        field = logic(field);
        usleep(speed);
        game_over = check(field);
        equal(field, field_new, &flag);
        counter++;
        if (counter == 9999 || flag == 0) {
            break;
        }
    }
    free(field_new);
    free(field);
    printf("\bGAME OVER.\n");
    return 0;
}

char **setup(void) {
    unsigned char sym;
    char** array = malloc(M * sizeof(char*));
    for (int i = 0; i < M; i++) {
        array[i] = malloc(N * sizeof(char));
    }
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            if (scanf("%c", &sym)) {
                array[y][x] = sym;
            }
        }
    }
    return array;
}

void draw(char **array) {
    printf("\e[H\e[2J\e[3J");
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            printf("%c", array[i][k]);
        }
        printf("\n");
    }
}

char **logic(char **array) {
    int sum;
    char** array_2 = malloc(M * sizeof(char*));
    for (int i = 0; i < M; i++) {
        array_2[i] = malloc(N * sizeof(char));
    }
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            sum = neighbours(array, y, x);
            if (sum == 3) {
                array_2[y][x] = 'o';
            } else if (sum == 2) {
                array_2[y][x] = array[y][x];
            } else {
                array_2[y][x] = ' ';
            }
        }
    }
    return array_2;
}

int neighbours(char **array_check, int y, int x) {
    int sum = 0;
    int string;
    int column;
    for (int i = -1; i < 2; i++) {
        for (int k = -1; k < 2; k++) {
            string = (i + y + N) % N;
            column = (k + x + M - 1) % (M - 1);
            if (array_check[string][column] == 'o') {
                sum++;
            }
        }
    }
    if (array_check[y][x] == 'o') {
        sum--;
    }
    return sum;
}

int check(char **array) {
    int able = 0;
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            if (array[y][x] == 'o') {
                able = 1;
            }
        }
    }
    return able;
}

char **ravno(char **array) {
    char** array_2 = malloc(M * sizeof(char*));
    for (int i = 0; i < M; i++) {
        array_2[i] = malloc(N * sizeof(char));
    }
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            array_2[y][x] = array[y][x];
        }
    }
    return array_2;
}

int *equal(char **array, char **array_new, int *flag) {
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            if (array[y][x] != array_new[y][x]) {
                (*flag)++;
            }
        }
    }
    return flag;
}
