#ifndef MAIN_ENEMY_H
#define MAIN_ENEMY_H

#include "Image.h"
#include "Player.h"

struct Enemy
{

    std::vector<bool>health;
    std::vector<Point>enemy_coords;
    std::vector<Point> coords;
    std::vector<Point> old_coords;
    std::vector<GLfloat> last_time;
    std::vector<int> enemy_dir;
    std::vector<GLfloat>enemy_count;
    std::vector<int>enemy_status;
    std::vector<int>mode; // 1 - right 2 - left
    void Switch_status(GLfloat currentFrame, int i);

    explicit Enemy() {};
    bool Moved(int i) const;
    void ProcessInput(char *card, GLfloat time, int i);
    void Draw(Image &screen,  GLfloat currentFrame, int i);
    void Draw_enemy(Image &screen, Image &img, int offset, Point _coords);

private:
    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 2;
};

#endif //MAIN_ENEMY_H
