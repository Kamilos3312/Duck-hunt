#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){ //Spawns duck
    if (visible)   return;
    duckPos.x = -200;    //Default X for duck spawn
    duckPos.y = rand()%449; //Rand Y for duck spawn
    visible = true;
}

void Duck::duckMove(){  //Move duck
    if (!visible)  return;
    duckPos.x = duckPos.x + (rand()%5);  //Rands new X for duck to move to
    if (duckPos.x >= 1150) {    duckDestroy();  return; }   //If a new X is beyond window area -> destroy duck
    int y = rand()%20 + (-10);    //Rands new Y for duck to move to
    if ((duckPos.y + y) > 449 || duckPos.y + y < 15)   //If a new Y is beyond window area ->
        y = -y; //changes Y sign
    duckPos.y += y;
}

void Duck::duckDestroy(){   //Destroy duck
    if (!visible)   return;
    visible = false;
}
