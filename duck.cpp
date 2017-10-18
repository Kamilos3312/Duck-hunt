#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){
    if (visible) return;
    visible = true;
    duckPos.x = 10;
    duckPos.y = rand()%(screen->h);
    while (duckPos.y > (screen->h - avatar->h))
        duckPos.y = rand()%(screen->h);

    Image::draw(avatar, duckPos.x, duckPos.y);
}

void Duck::duckMove(){
    if (!visible) return;
    int x = rand()%(int)70 + (screen->w/2 * 0.10);
    if ((x + duckPos.x) > 1280) duckDestroy();
    int y = rand()%(int)(screen->h * 0.7);
    if ((y + duckPos.y) > 600)
        y = -y;
    int abc = rand()%2;
    if (abc !=1 || y < 0)
        Image::draw(avatar, duckPos.x + x, y + duckPos.y);
    else
        Image::draw(avatar, duckPos.x + x, y - duckPos.y);
}

void Duck::duckDestroy(){
    visible = false;
}
