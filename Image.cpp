#include "header.h"

Image::Image(){
    if (load_image() < 0)
        Error_Message("Image file not found or could not be opened.");
}

int Image::load_image(){
    image1 = load_bmp("resources/marbles.bmp", NULL);
    if (!image1)   {destroy_bitmap(image1);   return -1;}
    return 0;
}

void Image::draw_image(BITMAP *screen, int x, int y){
    draw_sprite(screen, image1, x,y);
    //blit(image, screen, 0, 0, x, y, image->w, image->h);
}

void Image::destroy_bitmaps() {
    if (image1) destroy_bitmap(image1);
}
