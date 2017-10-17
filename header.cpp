#include "header.h"

volatile int speed = 0;

void incSpeed(){
    speed++;
}
END_OF_FUNCTION(ticker)

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
    set_window_title("Duck Hunt v1.00");
    clear_to_color(screen, makecol(255,255,255));
    show_mouse(screen);
}

void Engine::run(){
    Point mousePos;
    Sound_f background("resources/win_humans1.wav");
    Sound_f shot("resources/shot.wav");
    Duck duck;
    Image::load(&backgroundImage, "resources/background.bmp");
    Image::load(&duck.avatar, "resources/duck.bmp");
    Image::load(&shell[0], "resources/shell.bmp");
    Image::load(&shell[1], "resources/shell.bmp");

    score = 0;
    ammo = 2;
    //std::thread thread1(callFromThread, background);
    //-------- Do timera --------
    /*LOCK_VARIABLE(speed);
    LOCK_FUNCTION(incSpeed);
    install_timer();
    install_int_ex(incSpeed, BPS_TO_TIMER(60));*/

    background.play(1);   //Gra muzyke tla
    Image::draw(backgroundImage, 0, 0);
    //thread1.join();   //Gra muzyke tla - tylko ze przez watek
    while (!key[KEY_ESC]){
        /*while(speed > 0){ //Tu sie zapetla i zawiesza chyba
            //Image::draw(backgroundImage, 0, 0);   //Rysuje tlo
            //if (ammo == 2)  Image::draw(shell[1], 1135, 50);  //Rysuje 2gi shell
            //if (ammo == 1)  Image::draw(shell[0], 1065, 50);  //Rysuje 1szy shell

            if (!duck.visible)  duck.duckSpawn();
            //if (duck.sec >= 5)   duck.duckDestroy();
            control(duck, mousePos, shot);
            duck.duckMove();

            speed--;
            //duck.sec++;
        }
        //duck.duckMove();*/
    }
    background.stop();
    background.destroySamples();
    shot.destroySamples();
    //remove_int(incSpeed); //Do Timera
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
                score += 20;
            }
        rest(1);
        }
    }

    if ((mouse_b & 2) || key[KEY_R]){
        reload();
    }
}

void Engine::reload(){
    rest(2);
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
