#include "header.h"

void Image::load(BITMAP **bmp, std::string path){    //Loads bitmap from given path into the passed bitmap
    const char *name = path.c_str();    //Converts string into const char *
    *bmp = load_bmp(name, NULL);    //Loads bitmap from given path into the passed bitmap
    if (!(*bmp))   {destroy_bitmap((*bmp));   errorMessage("File not found or could not be opened.");   }   //If load was unsuccesful -> destroy bitmap and display error
}

void Image::draw(BITMAP *bmp, int x, int y){    //Draws passed bitmap onto the screen
    draw_sprite(screen, bmp, x,y);  //Draws bitmap onto the screen on given position
    //blit(bmp, screen, 0, 0, x, y, bmp->w, bmp->h);
    //draw_character_ex(screen, bmp, x, y, makecol(255, 0, 0), -1);
}

void Image::destroy(BITMAP *bmp) {  //Destroys passed bitmap
    if (bmp) destroy_bitmap(bmp);   //If bitmap is created -> destroy it
}
