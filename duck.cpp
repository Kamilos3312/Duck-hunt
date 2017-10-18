#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){
    duckPos.x = 10;
    duckPos.y = rand()%525;
    while (duckPos.y > (screen->h - avatar->h))
        duckPos.y = rand()%(screen->h);
    visible = true;
    Image::draw(avatar, duckPos.x, duckPos.y);
}

void Duck::duckMove(){
    duckPos.x = duckPos.x + (rand()%110 + 50);
    if (duckPos.x >= 1150) duckDestroy();
    int y = rand()%600 -300;
    if ((duckPos.y + y) > 525 || duckPos.y + y < 0)
        y = -y;
    Image::draw(avatar, duckPos.x, duckPos.y + y);
}

void Duck::duckDestroy(){
    visible = false;    //To nie dzia³a :(
}
