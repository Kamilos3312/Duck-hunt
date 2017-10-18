#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <allegro.h>
#include <ctime>
#include <string>
#include <thread>

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
    int sec;

    Duck();
    ~Duck() = default;

    void duckSpawn();
    void duckMove();
    void duckDestroy();
};

// Sound class
class Sound_f{
    SAMPLE *track;
    bool playing;

public:
    Sound_f(std::string path);
    ~Sound_f() = default;

    void play(int loop);
    void stop();
    void destroySamples();
};

// Engine class
class Engine{
    BITMAP *backgroundImage;
    BITMAP *shell[2];
    int hits;
    int ammo;

    //Controllers
    void reload();
    void control(Duck duck, Point mousePos, Sound_f shot);

public:
    Engine();
    ~Engine() = default;

    void run();
};

// Image class
class Image{
public:
    static int load(BITMAP **bmp, std::string path);
    static void draw(BITMAP *bmp, int x, int y);
    static void destroyBitmaps(BITMAP *bmp);
};

//----------------- Standalone functions -----------------
void errorMessage(const char *message);

bool checkShots(Duck duck, Point mousePos);

void callFromThread(Sound_f track);

#endif // HEADER_H_INCLUDED
