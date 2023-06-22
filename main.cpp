#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <time.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define PLAYER_SIZE 20
#define PLAYER_SPEED 10
#define BULLET_SIZE 5
#define BULLET_SPEED 10
#define ENEMY_SIZE 20
#define ENEMY_ROWS 5
#define ENEMY_COLS 8
#define ENEMY_SPACING 40
#define ENEMY_SPEED 5
#define MAX_BULLETS 10

bool game_start = false;
bool game_won =false;

typedef struct {
    float x, y;
    float dx, dy;
    bool active;
} Enemy;

typedef struct {
    float x, y;
    float speed;
    bool active;
} Bullet;

Enemy enemies[ENEMY_ROWS][ENEMY_COLS];
Bullet bullets[MAX_BULLETS];
float player_x = WINDOW_WIDTH / 2;
int score = 0;
bool game_over = false;


void draw_player() {
    glColor3f(1, 1, 1);
    glRectf(player_x - PLAYER_SIZE / 2, 0, player_x + PLAYER_SIZE / 2, PLAYER_SIZE);
}

void draw_bullet(Bullet* bullet) {
    if (bullet->active) {
        glColor3f(0, 1, 0);
        glRectf(bullet->x - BULLET_SIZE / 2, bullet->y - BULLET_SIZE / 2,
            bullet->x + BULLET_SIZE / 2, bullet->y + BULLET_SIZE / 2);
    }
}

void draw_enemies() {
    for (int i = 0; i < ENEMY_ROWS; i++) {
        for (int j = 0; j < ENEMY_COLS; j++) {
            if (enemies[i][j].active) {
                glColor3f(1, 0, 0);
                glRectf(enemies[i][j].x - ENEMY_SIZE / 2, enemies[i][j].y - ENEMY_SIZE / 2,
                    enemies[i][j].x + ENEMY_SIZE / 2, enemies[i][j].y + ENEMY_SIZE / 2);
            }
        }
    }
}

void draw_text(float x, float y, char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void init_enemies() {
    for (int i = 0; i < ENEMY_ROWS; i++) {
        for (int j = 0; j < ENEMY_COLS; j++) {
            enemies[i][j].x = ENEMY_SPACING + j * ENEMY_SPACING;
            enemies[i][j].y = WINDOW_HEIGHT - (ENEMY_SPACING + i * ENEMY_SPACING);
            enemies[i][j].dx = ENEMY_SPEED;
            enemies[i][j].dy = 0;
            enemies[i][j].active = true;
        }
    }
}

void update_enemies() {
    bool reverse_direction = false;
    bool all_destroy=true;
    for (int i = 0; i < ENEMY_ROWS; i++) {
        for (int j = 0; j < ENEMY_COLS; j++) {
            if (enemies[i][j].active) {
                enemies[i][j].x += enemies[i][j].dx;
                all_destroy=false;
                if (enemies[i][j].x < ENEMY_SIZE / 2 ||
                    enemies[i][j].x > WINDOW_WIDTH - ENEMY_SIZE / 2) {
                    reverse_direction = true;
                }
                if (enemies[i][j].y < ENEMY_SIZE / 2 ||
                    enemies[i][j].y > WINDOW_HEIGHT - ENEMY_SIZE / 2) {
                    game_over = true;
                }
            }
        }
    }
    if(all_destroy){
        game_won=true;
    }
    if (reverse_direction) {
        for (int i = 0; i < ENEMY_ROWS; i++) {
            for (int j = 0; j < ENEMY_COLS; j++) {
                enemies[i][j].dx = -enemies[i][j].dx;
                enemies[i][j].y -= ENEMY_SPACING / 2;
            }
        }
    }
}

void fire_bullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player_x;
            bullets[i].y = PLAYER_SIZE;
            bullets[i].speed = BULLET_SPEED;
            bullets[i].active = true;
            break;
        }
    }
}

void update_bullet(Bullet* bullet) {
    if (bullet->active) {
        bullet->y += bullet->speed;
        if (bullet->y > WINDOW_HEIGHT) {
            bullet->active = false;
        }
        for (int i = 0; i < ENEMY_ROWS; i++) {
            for (int j = 0; j < ENEMY_COLS; j++) {
                if (enemies[i][j].active) {
                    float dist_x = enemies[i][j].x - bullet->x;
                    float dist_y = enemies[i][j].y - bullet->y;
                    float dist = sqrt(dist_x * dist_x + dist_y * dist_y);
                    if (dist < ENEMY_SIZE / 2 + BULLET_SIZE / 2) {
                        enemies[i][j].active = false;
                        bullet->active = false;
                        score += 10;
                    }
                }
            }
        }
    }
}

void draw_score() {
    char score_str[32];
    sprintf(score_str, "Score: %d", score);
    draw_text(10, WINDOW_HEIGHT - 30, score_str);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!game_start) {
        glColor3f(1.0, 1.0, 1.0);
        draw_text(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2, "Welcome to Space Invaders!");
        glColor3f(0.0, 0.8, 0.0);
        glBegin(GL_QUADS);
        glVertex2f(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 - 30);
        glVertex2f(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT / 2 - 30);
        glVertex2f(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT / 2 - 60);
        glVertex2f(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 - 60);
        glEnd();
        glColor3f(0.0, 0.0, 0.0);
        draw_text(WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2 - 50, "Press ENTER to Begin");
        glColor3f(1.0, 1.0, 1.0);
        draw_text(WINDOW_WIDTH / 2 - 228, WINDOW_HEIGHT / 2 - 200, "CREATED BY ARUN HEGDE AND MANUVARDHAN V");
        glColor3f(1.0, 1.0, 1.0);
        draw_text(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 230, "USN : 1BG20CS021 and 1BG21CS403");
    }
    else {
        draw_player();
        for (int i = 0; i < MAX_BULLETS; i++) {
            draw_bullet(&bullets[i]);
        }
        draw_enemies();
        draw_score();
        if (game_over) {
            draw_text(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, "Game Over");
        }
        if(game_won){
            draw_text(WINDOW_WIDTH/2-80,WINDOW_HEIGHT/2, "Congratulations,You have won the game!!");
        }
    }
    glutSwapBuffers();
}

void update() {
    if (!game_over) {
        update_enemies();
        for (int i = 0; i < MAX_BULLETS; i++) {
            update_bullet(&bullets[i]);
        }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (!game_start && (key == 13 || key == 32)) {
        game_start = true;
    }
    else {
        switch (key) {
        case 'a':
        case 'A':
            player_x -= PLAYER_SPEED;
            if (player_x < PLAYER_SIZE / 2) {
                player_x = PLAYER_SIZE / 2;
            }
            break;
        case 'd':
        case 'D':
            player_x += PLAYER_SPEED;
            if (player_x > WINDOW_WIDTH - PLAYER_SIZE / 2) {
                player_x = WINDOW_WIDTH - PLAYER_SIZE / 2;
            }
            break;
        case ' ':
            fire_bullet();
            break;
        case 27:
            exit(0);
            break;
        }
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    srand(time(NULL));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i < ENEMY_ROWS; i++) {
        for (int j = 0; j < ENEMY_COLS; j++) {
            enemies[i][j].active = true;
            enemies[i][j].x = j * ENEMY_SPACING + ENEMY_SIZE / 2;
            enemies[i][j].y = WINDOW_HEIGHT - (i + 1) * ENEMY_SPACING + ENEMY_SIZE / 2;
            enemies[i][j].dx = ENEMY_SPEED;
            enemies[i][j].dy = 0.0;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Space Invaders");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}
