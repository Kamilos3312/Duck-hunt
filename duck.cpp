#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){ //Spawns duck
    if (visible)   return;
    duckPos.x = -40;    //Default X for duck spawn
    duckPos.y = rand()%525; //Rand Y for duck spawn
    while (duckPos.y > (screen->h - avatar->h)) //If Y is beyond window area ->
        duckPos.y = rand()%525; //Rands new Y
    visible = true;
}

void Duck::duckMove(){  //Move duck
    if (!visible)  return;
    duckPos.x = duckPos.x + (rand()%110 + 50);  //Rands new X for duck to move to
    if (duckPos.x >= 1150) {    duckDestroy();  return; }   //If a new X is beyond window area -> destroy duck
    int y = rand()%500 -250;    //Rands new Y for duck to move to
    if ((duckPos.y + y) > 525 || duckPos.y + y < 15)   //If a new Y is beyond window area ->
        y = -y; //changes Y sign
        duckPos.y += y;
    //Image::draw(avatar, duckPos.x, duckPos.y + y);  //Draws duck onto the screen
}

void Duck::duckDestroy(){   //Destroy duck
    if (!visible)   return;
    visible = false;
}
