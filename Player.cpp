#include "Player.h"

#include <iostream>

Image hero("./resources/tiles/hero.png");
Image hero1("./resources/tiles/hero1.png");
Image hero2("./resources/tiles/hero2.png");
Image hero3("./resources/tiles/hero3.png");
Image hero4("./resources/tiles/hero4.png");
Image hero5("./resources/tiles/hero5.png");

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::Switch_pos(int delta)
{
    plan_pos += delta;
}

void Player::Switch_status(GLfloat currentFrame)
{
    GLfloat diff_time = 0;
    if (mode == 4) {
        diff_time = 0.05;
    } else {
        diff_time = 0.1;
    }
    currentFrame--;
    if (currentFrame -  player_count > diff_time) {
        player_count = currentFrame;
        player_status++;
        if (player_status > 6) {
            player_status = 1;
        }
    }
}

void Player::Check_trap(char *card, bool &check_trap)
{
    if (card[(31 - (coords.y + 16) / 32) * 32 + (coords.x + 20) / 32] == 'T') {
        check_trap = true;
    } else {
        check_trap = false;
    }
}
void Player::ProcessInput(MovementDir dir, char *card, int &check_switch)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      if( (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 10) / 32] == '.' && card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 29) / 32] == '.') ||
              (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 10) / 32] == 'T' && card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 29) / 32] == 'T') ||
              (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 10) / 32] == 'T' && card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 29) / 32] == '.') ||
              (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 10) / 32] == '.' && card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 29) / 32] == 'T') ){
          old_coords.y = coords.y;
          coords.y += move_dist;
          check_switch = 0;
      } else if ( card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 16) / 32] == 'x') {
          coords.y = tileSize;
          check_switch = 1;
          Switch_pos(-7);
      } else if ( (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + coords.x / 32] == ' ') || (card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 32) / 32] == ' ') ) {
          check_switch = 2;
      } else if ( card[(31 - (coords.y + move_dist + 32) / 32) * 32 + (coords.x + 16) / 32] == 'Q') {
          check_switch = 3;
      }
      break;
    case MovementDir::DOWN:
        if( (card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 10) / 32] == '.' && card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 29) / 32] == '.') ||
                (card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 10) / 32] == 'T' && card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 29) / 32] == 'T') ||
                (card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 10) / 32] == 'T' && card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 29) / 32] == '.') ||
                (card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 10) / 32] == '.' && card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 29) / 32] == 'T')) {
            old_coords.y = coords.y;
            coords.y -= move_dist;
            check_switch = 0;
        } else if ( card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 16)/ 32] == 'x' ) {
          coords.y = 1024 - tileSize - 30;
          check_switch = 1;
          Switch_pos(7);
        } else if ( (card[(31 - (coords.y + 5 - move_dist) / 32) * 32 + coords.x / 32] == ' ') || (card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 32) / 32] == ' ') ) {
            check_switch = 2;
        } else if ( card[(31 - (coords.y - move_dist) / 32) * 32 + (coords.x + 16)/ 32] == 'Q' ) {
            check_switch = 3;
        }
          break;
    case MovementDir::LEFT:
      if( (card[(31 - (coords.y + 5) / 32) * 32 + (coords.x - move_dist + 10) / 32 ] == '.' && card[(31 - (coords.y + 29) / 32) * 32 + (coords.x - move_dist + 10) / 32] == '.') ||
              (card[(31 - (coords.y + 5) / 32) * 32 + (coords.x - move_dist + 10) / 32 ] == 'T' && card[(31 - (coords.y + 29) / 32) * 32 + (coords.x - move_dist + 10) / 32] == 'T') ||
              (card[(31 - (coords.y + 5) / 32) * 32 + (coords.x - move_dist + 10) / 32 ] == '.' && card[(31 - (coords.y + 29) / 32) * 32 + (coords.x - move_dist + 10) / 32] == 'T') ||
              (card[(31 - (coords.y + 5) / 32) * 32 + (coords.x - move_dist + 10) / 32 ] == 'T' && card[(31 - (coords.y + 29) / 32) * 32 + (coords.x - move_dist + 10) / 32] == '.')) {
          old_coords.x = coords.x;
          coords.x -= move_dist;
          check_switch =  0;
      } else if (card[(31 - (coords.y + 16) / 32) * 32 + (coords.x - move_dist + 10) / 32 ] == 'x') {
          coords.x = 1024 - 2 * tileSize - 10;
          check_switch = 1;
          Switch_pos(-1);
      } else if ((card[(31 - coords.y / 32) * 32 + (coords.x - move_dist) / 32 ] == ' ') || (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x - move_dist) / 32] == ' ')) {
          check_switch = 2;
      } else if (card[(31 - (coords.y + 16) / 32) * 32 + (coords.x - move_dist) / 32 ] == 'Q') {
          check_switch = 3;
      }
      break;
    case MovementDir::RIGHT:
      if( (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == '.' && card[(31 - (coords.y + 5) / 32) * 32 + (coords.x + 32 +move_dist) / 32] == '.') ||
              (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == 'T' && card[(31 - (coords.y + 5) / 32) * 32 + (coords.x + 32 +move_dist) / 32] == 'T') ||
              (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == '.' && card[(31 - (coords.y + 5) / 32) * 32 + (coords.x + 32 +move_dist) / 32] == 'T') ||
              (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == 'T' && card[(31 - (coords.y + 5) / 32) * 32 + (coords.x + 32 +move_dist) / 32] == '.')) {
          old_coords.x = coords.x;
          coords.x += move_dist;
          check_switch = 0;
      } else if( card[(31 - (coords.y + 16) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == 'x')  {
          coords.x = tileSize + 23;
          check_switch = 1;
          Switch_pos(1);
      } else if( (card[(31 - (coords.y + 29) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == ' ') || (card[(31 - (coords.y) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == ' ')) {
          check_switch = 2;
      } else if( card[(31 - (coords.y + 16) / 32) * 32 + (coords.x + 32 + move_dist) / 32] == 'Q')  {
          check_switch = 3;
      }
      break;
    default:
      break;
  }

}

void Player::Draw_hero(Image &screen, Image &img, int offset)
{
    for(int y = coords.y, i = 0; y <= coords.y + tileSize - 1; ++y, ++i)
    {
        for(int x = coords.x, j = 0; x <= coords.x + tileSize - 1; ++x, ++j)
        {
            Pixel pix = img.GetPixel(j + offset, 32 - i - 1);
            Pixel old_pix = screen.GetPixel_const(coords.x  + j, coords.y + i);
            pix = blend(old_pix, pix);
            screen.PutPixel(x, y, pix);
        }
    }

}

void Player::Draw(Image &screen,  GLfloat currentFrame)
{
    int offset = 0;
    if(Moved() || mode == 4)
    {
        Switch_status(currentFrame);
        offset = (player_status - 1) * 48;
        for(int y = old_coords.y; y <= old_coords.y + tileSize - 1; ++y)
        {
            for(int x = old_coords.x; x <= old_coords.x + tileSize - 1; ++x)
            {
                screen.PutPixel(x, y, screen.GetPixel_const(x, y));
            }
        }
        if (mode == 4) {
            if (player_status == 1) {
                offset = 10;
            } else if (player_status == 2) {
                offset = 56;
            } else if (player_status == 3) {
                offset = 105;
            } else if (player_status == 4) {
                offset = 159;
            } else if (player_status == 5) {
                offset = 217;
            } else {
                offset = 255;
            }
        }
        old_coords = coords;
    }

    if (mode == 1) {
        Draw_hero(screen, hero1, offset);
    } else if (mode == 2){
        Draw_hero(screen, hero2, offset);
    } else if (mode == 0) {
        Draw_hero(screen, hero, offset);
    } else if (mode == 3) {
        Draw_hero(screen, hero3, offset);
    } else if (mode == 4) {
        Draw_hero(screen, hero4, offset);
    }

}