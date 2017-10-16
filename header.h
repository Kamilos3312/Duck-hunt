#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <allegro.h>
#include <string>
#include <vector>
//#include <cstdlib>
//include <algorithm>

//----------------- Defines & Flags -----------------

enum Colors{
    WHITE = 0xFFFFFF,
    BLACK = 0x000000,
    RED = 0x0000FF,
    GREEN = 0x00FF00,
    BLUE = 0xFF0000,
    CYAN = 0x00ffff,
    MAGENTA = 0xff00ff,
    YELLOW = 0xffff00
};

enum Options{ Keyboard = 0x01,    Mouse = 0x02,   Sound = 0x04 };

inline Options operator|(Options a, Options b)
{return static_cast<Options>(static_cast<int>(a) | static_cast<int>(b));}

//----------------- Structures -----------------
struct Point2D{
    int x;
    int y;
};

struct LineSegment {
    Point2D A;
    Point2D B;
};
//----------------- Classes -----------------

// Engine class
class Engine{
    bool sound;
    bool mouse;
public:
    Engine();
    ~Engine() = default;

    //Initialize
    void initialize(Options option);
    void initialize(int mode, std::string resolution);
    void run();
    //Controllers
    void keyboard_control();
    void mouse_control();
    //Draw, Clear
    void draw_points(std::vector<Point2D> &zbior, float color);
    void draw(BITMAP *bmp, int mode, int x1, int y1, int x2, int y2, float color);
    void draw(BITMAP *bmp, bool mode, int x1, int y1, int x2, int y2, int x3, int y3, float color);
    void draw(BITMAP *bmp, bool mode, int x1, int y1, float r, float color);
    void clear(BITMAP *bmp, float color);
    void rysuj_odcinek(int x1, int y1, int x2, int y2, float color);
    //Deinitialize
    void deinitialize();

protected:
    enum EngineState { MENU, STATE1, STATE2, EXIT };
    EngineState state;
};

// Sound class
class sound_f{
    SAMPLE *track1;
    SAMPLE *track2;
    int playing;

    int load();
public:
    sound_f();
    ~sound_f() = default;

    void play();
    void stop();
    void destroy_samples();
};

// Image class
class Image{
    BITMAP *image1;

    int load_image();
public:
    Image();
    ~Image() = default;

    void draw_image(BITMAP *screen, int x, int y);
    void destroy_bitmaps();
};

//----------------- Standalone functions -----------------
void Error_Message(const char *message);

#endif // HEADER_H_INCLUDED
