#include "header.h"

volatile int timer;
void incTimer() { timer++; }
END_OF_FUNCTION(incTimer)

Engine::Engine(){
    int color_depth;
    allegro_init();
    if ((color_depth = desktop_color_depth()) != 0)
        set_color_depth(color_depth);
    install_timer();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
    enable_hardware_cursor();

    srand(time(NULL));

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED , 1280, 720, 0, 0) < 0)   errorMessage("Could not establish the graphic mode.");
    set_window_title("Duck Hunt v0.09");
    //set_close_button_callback();  //Fun
    clear_to_color(screen, makecol(255, 255, 255));
    show_mouse(screen);
}

void Engine::run(){
    Point mousePos;
    Sound_f background("resources/win_humans1.wav");
    Sound_f shot("resources/shot.wav");
    Duck duck;
    Image::load(&backgroundImage, "resources/background.bmp");
    Image::load(&duck.avatar, "resources/duck.bmp");
    Image::load(&shell[0], "resources/shell2.bmp");
    Image::load(&shell[1], "resources/shell2.bmp");

    hits = 0;
    ammo = 2;

    timer = 0;
    LOCK_FUNCTION(incTimer);
    LOCK_VARIABLE(timer);
    install_int_ex(incTimer, MSEC_TO_TIMER(1));

    BITMAP *time = create_bitmap(150, 12);
    points = create_bitmap(100, 10);
    draw_sprite(screen, backgroundImage, 0 ,0);
    background.play(1);   //Gra muzyke tla
    if (ammo >= 2)  {   Image::draw(shell[1], 1135, 50);    Image::draw(shell[0], 1065, 50);   }
    if (ammo == 1)  Image::draw(shell[0], 1065, 50);
    while (!key[KEY_ESC]){
        if (!duck.visible)  duck.duckSpawn();

        if (mouse_b & 1)    { shot.play(0);   hits++; }
        //if (duck jest wiecej niz 5sekund)   duck.duckDestroy();
        //control(duck, mousePos, shot);
        //duck.duckMove();

        //duck.sec++;
        //duck.duckMove();*/
        //Draw
        textprintf_centre_ex(time, font, 75, 2, makecol(255,255,255), -1,
           "Timer: %02d: %02d: %02d", (((timer/1000)/3600)%24) ,(((timer/1000)/60)%60) ,((timer/1000)%60));
        textprintf_centre_ex(points, font, 50, 1, makecol(255,255,255), -1,
           "Hits: %d", hits);
        draw_sprite(screen, time, 1000, 10);
        draw_sprite(screen, points, 700, 10);

        //Clear bitmaps
        clear(time);
        clear(points);
    }
    background.stop();
    background.destroySamples();
    shot.destroySamples();
    //remove_int(inctimer); //Do Timera
    allegro_exit();
}

void Engine::control(Duck duck, Point mousePos, Sound_f shot){
    mousePos.x = mouse_pos >> 16;
    mousePos.y = mouse_pos & 0x0000ffff;

    if (mouse_b & 1){
        if(ammo <= 0) reload();
        else{
            shot.play(0);
            ammo--;
            if (checkShots(duck, mousePos)){
                duck.duckDestroy();
                hits++;
            }
        }
    }

    if ((mouse_b & 2) || key[KEY_R]){
        reload();
    }
}

void Engine::reload(){
    rest(2000);
    ammo = 2;
}

void errorMessage(const char *message) {
    allegro_message(message);
    exit(EXIT_FAILURE);
}

bool checkShots(Duck duck, Point mousePos){
    if (duck.visible &&
        (mousePos.x > duck.duckPos.x && mousePos.x < (duck.duckPos.x + duck.avatar->w)) &&
        (mousePos.y > duck.duckPos.y && mousePos.y < (duck.duckPos.y + duck.avatar->h)))
        return true;
    return false;
}

void callFromThread(Sound_f track){
    track.play(1);
}
