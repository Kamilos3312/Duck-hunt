#include "header.h"

volatile int timer;
volatile int actionDelay;
void incTimer() { timer++; }
END_OF_FUNCTION(incTimer)

volatile int counter = 0;
void increment() {counter++;}

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

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED , 1280, 720, 0, 0) < 0)
        errorMessage("Could not establish the graphic mode.");
    set_window_title("Duck Hunt v1.00 - Reworked");
    mouse_x = screen->w/2;
    mouse_y = screen->h/2;
    show_mouse(screen);
}

void Engine::run(){
    // --
    Sound_f background("resources/background.wav");
    Sound_f shot("resources/shot.wav");
    Sound_f reload("resources/reload.wav");
    Sound_f laugh("resources/laugh.wav");
    Duck duck;
    Image::load(&backgroundImage, "resources/background.bmp");
    Image::load(&duck.avatar, "resources/duck.bmp");
    Image::load(&shell, "resources/shell.bmp");
    Image::load(&dog, "resources/dog.bmp");
    Image::load(&shotgun, "resources/shotgun.bmp");

    // --
    hits = 0;
    ammo = 2;

    // --
    timer = 0;
    actionDelay = 0;
    LOCK_FUNCTION(incTimer);
    LOCK_VARIABLE(timer);
    install_int_ex(incTimer, SECS_TO_TIMER(1));

    LOCK_FUNCTION(increment);
    LOCK_VARIABLE(counter);
    install_int_ex(increment, BPS_TO_TIMER(60));
    // --
    timeMap = create_bitmap(130, 12);
    hitMap = create_bitmap(100, 10);
    BITMAP *buffer = create_bitmap(screen->w, screen->h);
    // --
    background.play(1);

    while (!key[KEY_ESC]){
        clear_bitmap(buffer);

        while (counter > 0)
        {
            clear_bitmap(timeMap);
            clear_bitmap(hitMap);
            control(duck, shot, reload, laugh);
            textprintf_centre_ex(timeMap, font, 65, 2, makecol(255,255,255), -1, "Time: %02d: %02d: %02d", (((timer)/3600)%24) ,(((timer)/60)%60) ,((timer)%60));
            textprintf_centre_ex(hitMap, font, 50, 1, makecol(255,255,255), -1, "Hits: %d", hits);
            if (duck.visible)  duck.duckMove();
            else duck.duckSpawn();
            counter--;
        }

        draw_sprite(buffer, backgroundImage, 0, 0);
        draw_sprite(buffer, duck.avatar, duck.duckPos.x, duck.duckPos.y);
        draw_sprite(buffer, shotgun, mouse_x - 150, 530);
        if (ammo >= 2 && timer >= actionDelay)  draw_sprite(buffer, shell, 1135, 50);
        if (ammo >= 1 && timer >= actionDelay)  draw_sprite(buffer, shell, 1065, 50);
        draw_sprite(buffer,timeMap, 1050, 10);
        draw_sprite(buffer,hitMap, 595, 10);

        blit(buffer, screen, 0, 0, 0, 0,screen->w, screen->h);
        rest(20);
    }
    // Clear memory
    background.destroySamples();
    shot.destroySamples();
    reload.destroySamples();
    laugh.destroySamples();
    deinitialize(duck);
}

void Engine::control(Duck &duck, Sound_f shot, Sound_f r, Sound_f laugh){
    if (mouse_b & 1 && timer > actionDelay){
        if(ammo <= 0) { reload(r);   return;  }
        else{
            shot.play(0);
            ammo--;
            if (checkShots(duck)){
                duck.duckDestroy();
                draw_sprite(screen, backgroundImage, 0, 0);
                draw_sprite(screen, dog, duck.duckPos.x, 525);
                rest(1500);
                hits++;
            }
            else
                laugh.play(0);
            actionDelay = timer + 2;
        }
    }
    else if (((mouse_b & 2) || key[KEY_R]) && ammo < 2)
        reload(r);
}

void Engine::reload(Sound_f r){
    r.play(0);
    ammo = 2;
    actionDelay = timer + 1.5;
}

void Engine::deinitialize(Duck &duck){
    Image::destroy(shell);
    Image::destroy(duck.avatar);
    Image::destroy(backgroundImage);
    Image::destroy(timeMap);
    Image::destroy(hitMap);

    allegro_exit();
}

//--------

void errorMessage(const char *message) {
    allegro_message(message);
    exit(EXIT_FAILURE);
}

bool checkShots(Duck duck){
    if ((mouse_x > duck.duckPos.x && mouse_x < (duck.duckPos.x + duck.avatar->w)) &&
        ((mouse_y > duck.duckPos.y && mouse_y < (duck.duckPos.y + duck.avatar->h))))
        return true;
    return false;
}
