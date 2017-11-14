#include "header.h"

void Image::load(BITMAP **bmp, std::string path){
    const char *name = path.c_str();
    *bmp = load_bmp(name, NULL);
    if (!(*bmp))   {destroy_bitmap((*bmp));   errorMessage("File not found or could not be opened.");   }
}

void Image::destroy(BITMAP *bmp) {
    if (bmp) destroy_bitmap(bmp);
}
