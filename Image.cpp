#include "header.h"

int Image::load(BITMAP **bmp, std::string path){
    const char *name = path.c_str();
    *bmp = load_bmp(name, NULL);
    if (!(*bmp))   {destroy_bitmap((*bmp));   return -1;}
    return 0;
}

void Image::draw(BITMAP *bmp, int x, int y){
    draw_sprite(screen, bmp, x,y);//
    //blit(bmp, screen, 0, 0, x, y, bmp->w, bmp->h);
    //draw_character_ex(screen, bmp, x, y, makecol(255, 0, 0), -1);
}

void Image::destroyBitmaps(BITMAP *bmp) {
    if (bmp) destroy_bitmap(bmp);
}
