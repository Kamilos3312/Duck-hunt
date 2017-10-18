#include "header.h"

volatile int timer;
volatile int actionDelay;
volatile int second = 1;
volatile int moveDelay = 1;
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
    set_window_title("Duck Hunt v1.01");
    //set_close_button_callback();
    clear_to_color(screen, makecol(255, 255, 255));
    show_mouse(screen);
}

void Engine::run(){
    Point mousePos;
    Sound_f background("resources/background_original.wav");
    Sound_f shot("resources/shot.wav");
    Sound_f reload("resources/reload.wav");
    Duck duck;
    if (Image::load(&backgroundImage, "resources/background2.bmp") < 0) errorMessage("File not found or could not be opened.");
    if (Image::load(&duck.avatar, "resources/duck1.bmp") < 0)   errorMessage("File not found or could not be opened.");
    if (Image::load(&shell[0], "resources/shell.bmp") < 0)  errorMessage("File not found or could not be opened.");
    if (Image::load(&shell[1], "resources/shell.bmp") < 0)  errorMessage("File not found or could not be opened.");

    hits = 0;
    ammo = 2;

    timer = 0;
    LOCK_FUNCTION(incTimer);
    LOCK_VARIABLE(timer);
    install_int_ex(incTimer, SECS_TO_TIMER(1));

    BITMAP *time = create_bitmap(130, 12);
    points = create_bitmap(100, 10);
    draw_sprite(screen, backgroundImage, 0 ,0);
    background.play(1);
    if (ammo >= 2)  {   Image::draw(shell[1], 1135, 50);    Image::draw(shell[0], 1065, 50);   }
    if (ammo == 1)  Image::draw(shell[0], 1065, 50);

    actionDelay = 0;
    while (!key[KEY_ESC]){
        //if (!duck.visible) duck.duckSpawn();
        control(duck, mousePos, shot, reload);
        //Draw
        textprintf_centre_ex(time, font, 65, 2, makecol(255,255,255), -1,
           "Time: %02d: %02d: %02d", (((timer)/3600)%24) ,(((timer)/60)%60) ,((timer)%60));
        textprintf_centre_ex(points, font, 50, 1, makecol(255,255,255), -1,
           "Hits: %d", hits);
        draw_sprite(screen, time, 1050, 10);
        draw_sprite(screen, points, 595, 10);
        if (timer > second){
            draw_sprite(screen, backgroundImage, 0 ,0);
            if (!duck.visible) duck.duckSpawn();
        }
        if (ammo >= 2 && timer > second)  {
            second++;
            Image::draw(shell[1], 1135, 50);
            Image::draw(shell[0], 1065, 50);
        }
        if (ammo == 1 && timer > second)  {
                second++;
                Image::draw(shell[0], 1065, 50);
        }
        else if (ammo == 0 && timer > second)   second++;
        if (timer > moveDelay && duck.visible)   {
            moveDelay += 1.5;
            duck.duckMove();
        }

        //Clear bitmaps
        clear(time);
        clear(points);
    }
    background.stop();
    background.destroySamples();
    shot.destroySamples();
    allegro_exit();
}

void Engine::control(Duck duck, Point mousePos, Sound_f shot, Sound_f r){
    mousePos.x = mouse_pos >> 16;
    mousePos.y = mouse_pos & 0x0000ffff;

    if ((mouse_b & 1) && (timer > actionDelay + 1)){
        if(ammo <= 0) { reload(r);   return;  }
        else{
            shot.play(0);
            ammo--;
            //if (checkShots(duck, mousePos)){
                duck.duckDestroy();
                hits++;
            //}
        }
        actionDelay = timer;
    }

    if (((mouse_b & 2) || key[KEY_R]) && ammo < 2 && (timer > actionDelay + 1)){
        reload(r);
    }
}

void Engine::reload(Sound_f r){
    r.play(0);
    ammo = 2;
    actionDelay += 2;
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
