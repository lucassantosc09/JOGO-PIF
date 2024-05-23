#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "keyboard.h"
#include "screen.h"

#define WIDTH 20
#define HEIGHT 20
#define SCORE_FILE "scores.txt"

typedef struct Point {
    int x, y;
} Point;

typedef struct SnakeSegment {
    Point position;
    struct SnakeSegment *next;
} SnakeSegment;

typedef struct {
    SnakeSegment *head;
    SnakeSegment *tail;
    int length;
    Point direction;
} Snake;

typedef struct {
    Point position;
} Food;

void initGame(Snake *snake, Food *food) {
    SnakeSegment *headSegment = (SnakeSegment *)malloc(sizeof(SnakeSegment));
    headSegment->position.x = WIDTH / 2;
    headSegment->position.y = HEIGHT / 2;
    headSegment->next = NULL;

    snake->head = headSegment;
    snake->tail = headSegment;
    snake->length = 1;
    snake->direction.x = 0;
    snake->direction.y = 0;

    food->position.x = rand() % WIDTH;
    food->position.y = rand() % HEIGHT;
}

void drawGame(Snake *snake, Food *food) {
    screenClear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int isSnake = 0;
            SnakeSegment *current = snake->head;
            while (current != NULL) {
                if (current->position.x == j && current->position.y == i) {
                    screenGotoxy(j + SCRSTARTX, i + SCRSTARTY);
                    printf("#");
                    isSnake = 1;
                    break;
                }
                current = current->next;
            }
            if (!isSnake) {
                if (food->position.x == j && food->position.y == i) {
                    screenGotoxy(j + SCRSTARTX, i + SCRSTARTY);
                    printf("O");
                }
            }
        }
    }
    screenUpdate();
}

void updateSnake(Snake *snake) {
    SnakeSegment *newHead = (SnakeSegment *)malloc(sizeof(SnakeSegment));
    newHead->position.x = snake->head->position.x + snake->direction.x;
    newHead->position.y = snake->head->position.y + snake->direction.y;
    newHead->next = snake->head;
    snake->head = newHead;

    if (newHead->position.x >= WIDTH) newHead->position.x = 0;
    if (newHead->position.x < 0) newHead->position.x = WIDTH - 1;
    if (newHead->position.y >= HEIGHT) newHead->position.y = 0;
    if (newHead->position.y < 0) newHead->position.y = HEIGHT - 1;
}

void checkFood(Snake *snake, Food *food) {
    if (snake->head->position.x == food->position.x && snake->head->position.y == food->position.y) {
        snake->length++;
        food->position.x = rand() % WIDTH;
        food->position.y = rand() % HEIGHT;
    } else {
        SnakeSegment *current = snake->head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
        snake->tail = current;
    }
}

void saveScore(int score) {
    FILE *file = fopen(SCORE_FILE, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%d\n", score);
    fclose(file);
}

void screenInitGame() {
    screenClear();
    screenBoxEnable();
    for (int i = SCRSTARTY; i <= SCRENDY; i++) {
        screenGotoxy(SCRSTARTX, i);
        printf("%c", BOX_VLINE);
        screenGotoxy(SCRENDX, i);
        printf("%c", BOX_VLINE);
    }
    for (int j = SCRSTARTX; j <= SCRENDX; j++) {
        screenGotoxy(j, SCRSTARTY);
        printf("%c", BOX_HLINE);
        screenGotoxy(j, SCRENDY);
        printf("%c", BOX_HLINE);
    }
    screenGotoxy(SCRSTARTX, SCRSTARTY);
    printf("%c", BOX_UPLEFT);
    screenGotoxy(SCRENDX, SCRSTARTY);
    printf("%c", BOX_UPRIGHT);
    screenGotoxy(SCRSTARTX, SCRENDY);
    printf("%c", BOX_DWNLEFT);
    screenGotoxy(SCRENDX, SCRENDY);
    printf("%c", BOX_DWNRIGHT);
    screenBoxDisable();
    screenUpdate();
}

int main() {
    Snake snake;
    Food food;
    char command;
    int score = 0;

    srand(time(NULL));
    initGame(&snake, &food);
    keyboardInit();
    screenInitGame();

    while (1) {
        if (keyhit()) {
            command = readch();
            switch (command) {
                case 'w': snake.direction.x = 0; snake.direction.y = -1; break;
                case 's': snake.direction.x = 0; snake.direction.y = 1; break;
                case 'a': snake.direction.x = -1; snake.direction.y = 0; break;
                case 'd': snake.direction.x = 1; snake.direction.y = 0; break;
                case 'q': saveScore(score); return 0;
            }
        }
        updateSnake(&snake);
        checkFood(&snake, &food);
        drawGame(&snake, &food);
        usleep(100000);  // 100 ms delay
        score = snake.length - 1;
    }

    return 0;
}

