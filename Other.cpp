#include "header.h"

//---------  Laboratorium 1  ---------

void lab1_zad2(){
    circle(screen,screen->w/2,screen->h/2,100, makecol(40,40,40));

    rect(screen,(screen->w/3),(screen->h/3),(screen->w/1.5),(screen->h/1.5), makecol(80,80,80));

    triangle(screen,(screen->w/3), (screen->h/1.5), (screen->w/2), (screen->h/4), (screen->w/1.5), (screen->h/1.5), makecol(120,120,120));

    line(screen, screen->w/4, screen->h/4.5, screen->w/1.25, screen->h/4.5, makecol(200,200,200));

    line(screen, screen->w/4, screen->h/1.25, screen->w/1.25, screen->h/1.25, makecol(200,200,200));

    line(screen, screen->w/4, screen->h/1.25, screen->w/4, screen->h/4.5, makecol(200,200,200));

    line(screen, screen->w/1.25, screen->h/1.25, screen->w/1.25, screen->h/4.5, makecol(200,200,200));

}

void lab1_zad3(){
    allegro_message("Random primitives appeared !");
    int mode = 0;
    while (!key[ KEY_P ]){
        if (mode > 4) mode = 0;
        int x1 = rand()%screen->w;
        int y1 = rand()%screen->h;

        int random = rand()%screen->h/3;

        if(mode == 0)   circlefill(screen, rand()%screen->w, rand()%screen->h, rand()%320, makecol(rand()%255,rand()%255,rand()%255));

        else if (mode == 1) line(screen, rand()%screen->w, rand()%screen->h, rand()%screen->w, rand()%screen->h, makecol(rand()%255,rand()%255,rand()%255));

        else if (mode == 2) rectfill(screen, x1, y1, x1+random , y1+random, makecol(rand()%255,rand()%255,rand()%255));

        else triangle(screen, x1, y1, x1+random, y1, (x1+random/2), (y1+random), makecol(rand()%255,rand()%255,rand()%255));

        mode++;
    }
}

//---------  Laboratorium 2  ---------

void rysujOdcinek(int x1, int y1, int x2, int y2, int col) {

    if (x2 < x1){ std::swap(x1, x2);   std::swap(y1, y2);   }

    float dx = x2 - x1;
    float dy = y2 - y1;

    if (dx == 0)
        for (int y = y1; y <= y2; y++)
            putpixel(screen, x1,  y, col);

    float m = dy / dx;
    if (abs(m) < 1) {
        float y = y1;
        for(int x = x1; x <= x2; x++) { putpixel(screen, x,  int(y + 0.5), col);  y += m;   }
    }
    else {
        if (y2 < y1){   std::swap(x1, x2);   std::swap(y1, y2);   }

        m = dx / dy;
        float x = x1;
        for(int y = y1; y <= y2; y++) { putpixel(screen, int(x + 0.5),  y, col);    x += m; }
    }
}

void lab2_zad1(){
    //keyboard_input();
}

void rys1(){
    clear_to_color(screen, makecol(255, 255, 255));
    for (int i = 0; i < 8; i++) {
        rysujOdcinek(0 ,i * screen->h/8, screen->w, i * screen->h/8, makecol(0,0,0)); //poziome
        rysujOdcinek(i * screen->w/8, 0, i * screen->w/8, screen->h, makecol(0,0,0)); //pionowe
    }
}

void rys2(){
    clear_to_color(screen, makecol(255, 255, 255));
    for (int i =0; i < 8; i++) {
        rysujOdcinek(0, i * screen->h/8, screen->w, screen->h, makecol(0,0,0));
        rysujOdcinek(screen->w, i * screen->h/8, 0, 0, makecol(0,0,0));
    }
}

void rys3(){
    clear_to_color(screen, makecol(255, 255, 255));
    for (int i = 0; i < 9; i++)
        rysujOdcinek(i * screen->w/8, 0, (8-i) * (screen->w/8), screen->h, makecol(0,0,0));

    for (int i = 1; i < 8; i++)
        rysujOdcinek(0, i * screen->h/8, screen->w,(8-i) * screen->h/8, makecol(0,0,0));
}

void rys4(){
    clear_to_color(screen, makecol(255, 255, 255));
    for (int i = 0; i < 8; i++) {
        rysujOdcinek(0, i * screen->h/8, screen->w, screen->h, makecol(0,0,0));
        rysujOdcinek(screen->w, i * screen->h/8, 0, 0, makecol(0,0,0));
        rysujOdcinek(i * screen->w/8, 0,screen->w, screen->h, makecol(0,0,0));
        rysujOdcinek(i * screen->w/8, screen->h, 0, 0, makecol(0,0,0));
    }
}

//---------  C++ Threads  ---------
void call_from_thread1() {
    //Code ...
}

void call_from_thread2() {
    //Code ...
}
