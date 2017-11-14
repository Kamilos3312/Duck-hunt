#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <allegro.h>
//#include <ctime>
#include <string>

//----------------- Structures -----------------
struct Point{
    int x;
    int y;
};

//----------------- Classes -----------------
class Duck{
public:
    Point duckPos;
    BITMAP *avatar;
    bool visible;

    Duck();
    ~Duck() = default;

    void duckSpawn();
    void duckMove();
    void duckDestroy();
};

class Sound_f{
    SAMPLE *track;
public:
    Sound_f(std::string path);
    ~Sound_f() = default;

    void play(int loop);
    void destroySamples();
};

class Engine{
    BITMAP *backgroundImage;
    BITMAP *shell;
    BITMAP *timeMap;
    BITMAP *hitMap;
    BITMAP *dog;
    BITMAP *shotgun;
    int hits;
    int ammo;

    void reload(Sound_f r);
    void control(Duck &duck, Sound_f shot, Sound_f r, Sound_f laugh);
    void deinitialize(Duck &duck);

public:
    Engine();
    ~Engine() = default;

    void run();
};

class Image{
public:
    static void load(BITMAP **bmp, std::string path);
    static void draw(BITMAP *bmp, int x, int y);
    static void destroy(BITMAP *bmp);
};

//----------------- Standalone functions -----------------
void errorMessage(const char *message);

bool checkShots(Duck duck);

#endif // HEADER_H_INCLUDED
