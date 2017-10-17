#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){
    visible = true;
    duckPos.x = 10;
    duckPos.y = rand()%(screen->h);
    while (duckPos.y > (screen->h + avatar->h))
        duckPos.y = rand()%(screen->h);

    Image::draw(avatar, duckPos.x, duckPos.y);

    rest(1);
}

void Duck::duckMove(){
    int x = duckPos.x;
    int y = duckPos.y;
    duckPos.x = rand()%(int)(screen->w * 0.05);
    duckPos.y = rand()%(int)(screen->h * 0.05);
    int abc = rand()%2;
    if (abc == 1)
        Image::draw(avatar, x + duckPos.x, y + duckPos.y);
    else
        Image::draw(avatar, x + duckPos.x, y - duckPos.y);

    rest(1);
}

void Duck::duckDestroy(){
    visible = false;
}
