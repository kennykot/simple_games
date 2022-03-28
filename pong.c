// Copyright 2022 jewfish, johnie, ironrobb
#include <stdio.h>
#include <stdlib.h>  // system()
#include <unistd.h>  // usleep()
#include <fcntl.h>   // fcntl() - manipulate file descriptor

int WIDTH;
int HEIGHT;
int LEFT_RACKET_X;
int RIGHT_RACKET_X;
int NET_X;

int left_racket_y;
int right_racket_y;
int score_left;
int score_right;
int ball_x;
int ball_y;
int ball_dir_x;
int ball_dir_y;
int game_over;
int winner;
int allow_next_step;
char mode;
char command;
double speed;  // delay time in microseconds(1e-6)

int setup(void);
int input(void);
int logic(void);
int draw(void);
int abs(int n);

int main(void) {
    setup();
    while (!game_over) {
        allow_next_step = 0;
        input();
        logic();
        if (allow_next_step) {
            draw();
        }
        usleep(speed);
    }

    return 0;
}

int setup(void) {
    WIDTH = 81;
    HEIGHT = 26;
    LEFT_RACKET_X = 1;
    RIGHT_RACKET_X = 80;
    NET_X = 40;
    speed = 100000;
    left_racket_y = HEIGHT / 2;
    right_racket_y = HEIGHT / 2;
    score_left = 0;
    score_right = 0;
    ball_x = LEFT_RACKET_X + 1;
    ball_y = left_racket_y;
    ball_dir_x = 1;
    ball_dir_y = 1;
    printf("Select mode (0 - step by step mode, 1 - interactive mode) : ");
    scanf("%c", &mode);
    if (mode != '1' && mode != '0') {
        printf("\nError input, try again\n");
        game_over = 1;
    } else {
        draw();
    }
    system("stty cbreak");  // input without press "Enter"
    return 0;
}

int draw(void) {
    printf("\e[H\e[2J\e[3J");
    for (int y = 0; y <= HEIGHT; y++) {
        for (int x = 0; x <= WIDTH; x++) {
            if (winner == 0) {
                if (y == 0 && x == NET_X - 4) {
                    printf(" %.2d - %.2d ", score_left, score_right);
                    x += 9;
                }
                if (x == LEFT_RACKET_X && (y == left_racket_y ||
                    y == left_racket_y - 1 || y == left_racket_y + 1)) {
                    printf("]");
                    continue;
                }
                if (x == RIGHT_RACKET_X && (y == right_racket_y ||
                    y == right_racket_y - 1 || y == right_racket_y + 1)) {
                    printf("[");
                    continue;
                }
                if (x == ball_x && y == ball_y) {
                    printf("o");
                    continue;
                }
                if ((x == NET_X) && (y != 0) && (y != HEIGHT)) {
                    printf("|");
                    continue;
                }
                if (y == 0) {
                    if (x == 10) {
                        printf("---- LEFT PLAYER ----");
                        x += 20;
                    } else if (x == WIDTH - 30) {
                        printf("---- RIGHT PLAYER ---");
                        x += 20;
                    } else {
                       printf("-");
                    }
                    continue;
                }
                if (y == HEIGHT) {
                    if (x == 7) {
                        printf("- A - UP, Z - DOWN --");
                        x += 20;
                    } else if ((x == NET_X - 8) && (mode == '0')) {
                        printf("- SPACE = SKIP -");
                        x += 15;
                    } else if (x == WIDTH - 27) {
                        printf("- K - UP, M - DOWN --");
                        x += 20;
                    } else {
                       printf("-");
                    }
                    continue;
                }
            } else {
                if (winner == 1 && y == 13 && x == NET_X - 10) {
                    printf(" LEFT PLAYER WINS!  ");
                    x += 20;
                } else if (winner == 2 && y == 13 && x == NET_X - 10) {
                    printf(" RIGHT PLAYER WINS! ");
                    x += 20;
                } else if ((y == 11 || y == 15) && x == NET_X - 10) {
                    printf("--------------------");
                    x += 20;
                }
                if (y == 0 || y == HEIGHT) {
                    printf("-");
                    continue;
                }
            }
            if ((x == 0 || x == WIDTH) && (y != 0) && (y != HEIGHT)) {
                printf("|");
                continue;
            }
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}

int input(void) {
    int old = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, old | O_NONBLOCK);
    command = getchar();
    fcntl(0, F_SETFL, old);
    if ((command == 'a' || command == 'A') && left_racket_y > 2) {
        left_racket_y--;
    }
    if ((command == 'z' || command == 'Z') && left_racket_y < HEIGHT - 2) {
        left_racket_y++;
    }
    if ((command == 'k' || command == 'K') && right_racket_y > 2) {
        right_racket_y--;
    }
    if ((command == 'm' || command == 'M') && right_racket_y < HEIGHT - 2) {
        right_racket_y++;
    }
    if (command == ' ' || command == 'm' || command == 'M' ||
        command == 'k' || command == 'K' || command == 'z' ||
        command == 'Z' || command == 'a' || command == 'A') {
        allow_next_step = 1;
        return 0;
    }
    if (mode == '1') {
        allow_next_step = 1;
    }
    draw();
    return 0;
}

int logic(void) {
    if (ball_x == 1 || (ball_x == LEFT_RACKET_X + 1
                        && abs(left_racket_y - ball_y) <= 1)) {
        ball_dir_x = 1;
        if (ball_x == 1 && allow_next_step) {
            score_right++;
            speed -= (score_left + score_right + 1) * 60.0;
        }
    }
    if (ball_x == WIDTH - 1 || (ball_x == RIGHT_RACKET_X - 1
                                && abs(right_racket_y - ball_y) <= 1)) {
        ball_dir_x = -1;
        if (ball_x == WIDTH - 1 && allow_next_step) {
            score_left++;
            speed -= (score_left + score_right + 1) * 60.0;
        }
    }
    if (ball_y == 1) {
        ball_dir_y = 1;
    }
    if (ball_y == HEIGHT - 1) {
        ball_dir_y = -1;
    }
    if (allow_next_step) {
        ball_x += ball_dir_x;
        ball_y += ball_dir_y;
    }
    if (score_left == 21) {
        winner = 1;
        game_over = 1;
    }
    if (score_right == 21) {
        winner = 2;
        game_over = 1;
    }
    return 0;
}

int abs(int n) {
    if (n < 0) {
        return -1 * n;
    } else {
        return n;
    }
}
