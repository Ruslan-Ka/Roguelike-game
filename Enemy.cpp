#include "Enemy.h"

#include <iostream>

Image enemy1("./resources/tiles/enemy1.png");
Image enemy2("./resources/tiles/enemy2.png");

bool Enemy::Moved(int i) const
{
    if(coords[i].x == old_coords[i].x && coords[i].y == old_coords[i].y)
        return false;
    else
        return true;
}


void Enemy::Switch_status(GLfloat currentFrame, int i)
{
    currentFrame--;
    if (currentFrame -  enemy_count[i] > 0.1) {
        enemy_count[i] = currentFrame;
        enemy_status[i]++;
        if (enemy_status[i] > 6) {
            enemy_status[i] = 1;
        }
    }
}
void Enemy::ProcessInput(char *card, GLfloat time, int i)
{
    int move_dist = move_speed * 1;
        if (time - last_time[i] > 0.5) {
            last_time[i] = time;
            enemy_dir[i] = rand() % 4;
        }
        if (enemy_dir[i] == 0) {
            if ((card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 10) / 32] == '.' &&
                 card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 29) / 32] == '.') ||
                (card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 10) / 32] == 'T' &&
                 card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 29) / 32] == 'T') ||
                (card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 10) / 32] == 'T' &&
                 card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 29) / 32] == '.') ||
                (card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 10) / 32] == '.' &&
                 card[(31 - (coords[i].y + move_dist + 32) / 32) * 32 + (coords[i].x + 29) / 32] == 'T')) {
                mode[i] = 1;
                old_coords[i].y = coords[i].y;
                coords[i].y += move_dist;
            }
        } else if (enemy_dir[i] == 1) {
         if ((card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 10) / 32] == '.' &&
                     card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 29) / 32] == '.') ||
                    (card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 10) / 32] == 'T' &&
                     card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 29) / 32] == 'T') ||
                    (card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 10) / 32] == 'T' &&
                     card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 29) / 32] == '.') ||
                    (card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 10) / 32] == '.' &&
                     card[(31 - (coords[i].y - move_dist) / 32) * 32 + (coords[i].x + 29) / 32] == 'T')) {
            old_coords[i].y = coords[i].y;
            coords[i].y -= move_dist;
             mode[i] = 1;
        } } else if (enemy_dir[i] == 2) {
         if ((card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == '.' && card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == '.') ||
                    (card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == 'T' && card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == 'T') ||
                    (card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == '.' && card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == 'T') ||
                    (card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == 'T' && card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x - move_dist + 10) / 32] == '.')) {
            old_coords[i].x = coords[i].x;
            coords[i].x -= move_dist;
             mode[i] = 2;
        } } else if(enemy_dir[i] == 3) {
             if ((card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == '.' &&
                      card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == '.') ||
                     (card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == 'T' &&
                      card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == 'T') ||
                     (card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == '.' &&
                      card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == 'T') ||
                     (card[(31 - (coords[i].y + 29) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == 'T' &&
                      card[(31 - (coords[i].y + 5) / 32) * 32 + (coords[i].x + 32 + move_dist) / 32] == '.')) {
                old_coords[i].x = coords[i].x;
                coords[i].x += move_dist;
                 mode[i] = 1;
            }
        }
}

void Enemy::Draw_enemy(Image &screen, Image &img, int offset, Point _coords)
{
    for(int y = _coords.y, i = 0; y <= _coords.y + tileSize - 1; ++y, ++i)
    {
        for(int x = _coords.x, j = 0; x <= _coords.x + tileSize - 1; ++x, ++j)
        {
            Pixel pix = img.GetPixel(j + offset, 32 - i - 1);
            Pixel old_pix = screen.GetPixel_const(_coords.x  + j, _coords.y + i);
            pix = blend(old_pix, pix);
            screen.PutPixel(x, y, pix);
        }
    }

}

void Enemy::Draw(Image &screen,  GLfloat currentFrame, int i)
{
    int offset;
    offset = 0;

    Point _old_coords = old_coords[i];
    Point _coords = coords[i];
    if (Moved(i)) {
        Switch_status(currentFrame, i);
        offset = (enemy_status[i] - 1) * 48;
        for (int y = _old_coords.y; y <= _old_coords.y + tileSize - 1; ++y) {
            for (int x = _old_coords.x; x <= _old_coords.x + tileSize - 1; ++x) {
                screen.PutPixel(x, y, screen.GetPixel_const(x, y));
            }
        }
        old_coords = coords;
    }
    if (health[i] == true) {
        if (mode[i] == 1) {
            Draw_enemy(screen, enemy1, offset, _coords);
        } else {
            Draw_enemy(screen, enemy2, offset, _coords);
        }
    }

}