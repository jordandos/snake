#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <deque>
#include <unistd.h> // Include this header for usleep

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

class SnakeGame {
private:
    deque<Point> snake;
    Point fruit;
    Direction dir;
    bool gameOver;

public:
    SnakeGame() : fruit(0, 0) { // Initialize fruit member variable explicitly
        dir = RIGHT;
        snake.push_back(Point(WIDTH/2, HEIGHT/2));
        placeFruit();
        gameOver = false;
    }

    void placeFruit() {
        fruit.x = rand() % WIDTH;
        fruit.y = rand() % HEIGHT;
    }

    void handleInput() {
        int ch = getch();
        switch(ch) {
            case KEY_UP:
                if (dir != DOWN)
                    dir = UP;
                break;
            case KEY_DOWN:
                if (dir != UP)
                    dir = DOWN;
                break;
            case KEY_LEFT:
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case KEY_RIGHT:
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }

    void update() {
        Point newHead = snake.front();
        switch(dir) {
            case UP:
                newHead.y--;
                break;
            case DOWN:
                newHead.y++;
                break;
            case LEFT:
                newHead.x--;
                break;
            case RIGHT:
                newHead.x++;
                break;
        }

        if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
            gameOver = true;
            return;
        }

        for (const auto& segment : snake) {
            if (newHead.x == segment.x && newHead.y == segment.y) {
                gameOver = true;
                return;
            }
        }

        if (newHead.x == fruit.x && newHead.y == fruit.y) {
            placeFruit();
        } else {
            snake.pop_back();
        }

        snake.push_front(newHead);
    }

    void draw() {
        clear();
        // Draw Snake
        for (const auto& segment : snake) {
            mvprintw(segment.y, segment.x, "o");
        }
        // Draw Fruit
        mvprintw(fruit.y, fruit.x, "@");

        refresh();
    }

    bool isGameOver() {
        return gameOver;
    }
};

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));

    SnakeGame game;

    while (!game.isGameOver()) {
        game.handleInput();
        game.update();
        game.draw();
        usleep(100000); // Delay for smoother animation
    }
}