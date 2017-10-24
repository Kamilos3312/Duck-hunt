#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <allegro.h>
#include <ctime>
#include <string>

//----------------- Structures -----------------
struct Point{   //Point coordinates in the 2 dimensional space
    int x;  //Position horizontally
    int y;  //Position vertically
};

//----------------- Classes -----------------
class Duck{
public:
    Point duckPos;  //Duck position on the map
    BITMAP *avatar; //Ducks avatar
    bool visible;   //Duck visible = true, invisible = false

    Duck();
    ~Duck() = default;

    void duckSpawn();   //Spawns new duck
    void duckMove();    //Moves duck
    void duckDestroy(); //Destroys duck
};

class Sound_f{
    SAMPLE *track;  //Sample which will contain the audio
public:
    Sound_f(std::string path);  //Loads audio from given path into the sample
    ~Sound_f() = default;

    void play(int loop);    //Plays audio
    void destroySamples();  //Destroys sample
};

class Engine{
    BITMAP *backgroundImage;    //Bitmap which will contain background image
    BITMAP *shell[2];   //Bitmaps which will contain shell images
    BITMAP *timeMap;   //Bitmap which will contain playing time
    BITMAP *hitMap; //Bitmap which will contain Hit counter
    BITMAP *dog;
    BITMAP *cursor;
    int hits;   //Hit counter
    int ammo;   //Available ammo

    void reload(Sound_f r); //Reloads the gun
    void control(Duck &duck, Sound_f shot, Sound_f r);   //Steering method which contains game logic

public:
    Engine();
    ~Engine() = default;

    void run(); //Starts the engine
};

class Image{
public:
    static void load(BITMAP **bmp, std::string path);   //Loads bitmap from given path into the passed bitmap
    static void draw(BITMAP *bmp, int x, int y);    //Draws passed bitmap onto the screen
    static void destroy(BITMAP *bmp);   //Destroys passed bitmap
};

//----------------- Standalone functions -----------------
void errorMessage(const char *message); //Displays error message

bool checkShots(Duck duck); //Checks if player hit a duck

#endif // HEADER_H_INCLUDED
