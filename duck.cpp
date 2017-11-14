#include "header.h"

Duck::Duck(){
    visible = false;
}

void Duck::duckSpawn(){
    if (visible)   return;
    duckPos.x = -200;
    duckPos.y = rand()%449;
    visible = true;
}

void Duck::duckMove(){
    if (!visible)  return;
    duckPos.x = duckPos.x + (rand()%5);
    if (duckPos.x >= 1150) {    duckDestroy();  return; }
    int y = rand()%20 + (-10);
    if ((duckPos.y + y) > 449 || duckPos.y + y < 15)
        y = -y;
    duckPos.y += y;
}

void Duck::duckDestroy(){
    if (!visible)   return;
    visible = false;
}
