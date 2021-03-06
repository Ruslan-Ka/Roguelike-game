#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#define GLFW_DLL
#include <GLFW/glfw3.h>

struct Point
{
    int x;
    int y;
};

#include <string>
#include <vector>

constexpr int tileSize = 32;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
    std::vector<Point>trap_vector;


  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);

  int Save(const std::string &a_path);

  bool Trap_count(GLfloat currentFrame);
  int Trap()     const { return trap_status; }
  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }
  Pixel* Data_const()  { return  data_const; }

  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void  PutPixel(int x, int y, const Pixel &pix) { data[width* y + x] = pix; }
  Pixel GetPixel_const(int x, int y) { return data_const[width * y + x];}
  void  PutPixel_const(int x, int y, const Pixel &pix) { data_const[width* y + x] = pix; }

  ~Image();

private:
  GLfloat trap_count = 0;
  int trap_status = 1;
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  Pixel *data_const = nullptr;
  bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
