#include "header.h"

void Image::load(BITMAP **bmp, std::string path){    //Loads bitmap from given path into the passed bitmap
    const char *name = path.c_str();    //Converts string into const char *
    *bmp = load_bmp(name, NULL);    //Loads bitmap from given path into the passed bitmap
    if (!(*bmp))   {destroy_bitmap((*bmp));   errorMessage("File not found or could not be opened.");   }   //If load was unsuccesful -> destroy bitmap and display error
}

void Image::destroy(BITMAP *bmp) {  //Destroys passed bitmap
    if (bmp) destroy_bitmap(bmp);   //If bitmap is created -> destroy it
}
