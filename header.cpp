#include "header.h"

volatile int timer;
volatile int actionDelay;
void incTimer() { timer++; }
END_OF_FUNCTION(incTimer)

volatile int counter = 0;
void increment() {counter++;}

Engine::Engine(){
    int color_depth;
    allegro_init(); //Installs allegro libraries
    if ((color_depth = desktop_color_depth()) != 0) //Checks for users system color depth
        set_color_depth(color_depth);   //Sets color depth
    install_timer();    //Installs timer
    install_keyboard(); //Installs keyboard
    install_mouse();    //Installs mouse
    install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);    //Installs sound
    enable_hardware_cursor();   //Enables hardware cursor

    srand(time(NULL));  //Generate seed

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED , 1280, 720, 0, 0) < 0)    //Sets window with 1280 width and 720 height
        errorMessage("Could not establish the graphic mode.");  //If creating window was unsuccessful -> shows error message
    set_window_title("Duck Hunt v1.00 - Reworked");    //Sets window title
    mouse_x = screen->w/2;  //Sets Mouse cursor in the middle of the screen
    mouse_y = screen->h/2;  //Sets Mouse cursor in the middle of the screen
    show_mouse(screen); //Shows cursor inside window
}

void Engine::run(){
    //Create objects and load resources
    Sound_f background("resources/background.wav"); //Loads audio from given path into the sample
    Sound_f shot("resources/shot.wav"); //Loads audio from given path into the sample
    Sound_f reload("resources/reload.wav"); //Loads audio from given path into the sample
    Sound_f laugh("resources/laugh.wav");
    Duck duck;  //Creates duck object
    Image::load(&backgroundImage, "resources/background.bmp");  //Loads image from given path into the bitmap
    Image::load(&duck.avatar, "resources/duck.bmp");    //Loads image from given path into the bitmap
    Image::load(&shell[0], "resources/shell.bmp");  //Loads image from given path into the bitmap
    Image::load(&shell[1], "resources/shell.bmp");  //Loads image from given path into the bitmap
    Image::load(&dog, "resources/dog.bmp");     //Loads image from given path into the bitmap
    Image::load(&shotgun, "resources/shotgun.bmp");

    //Set default
    hits = 0;   //Hit counter
    ammo = 2;   //Available ammo

    //Initiate timer
    timer = 0;  //Resets timer
    actionDelay = 0;    //Resets action delay
    LOCK_FUNCTION(incTimer);    //Locks timer function
    LOCK_VARIABLE(timer);   //Locks timer
    install_int_ex(incTimer, SECS_TO_TIMER(1)); //Timer will tick every 1 second

    LOCK_FUNCTION(increment);
    LOCK_VARIABLE(counter);
    install_int_ex(increment, BPS_TO_TIMER(60));
    //Create bitmaps for Time and Hits
    timeMap = create_bitmap(130, 12);   //Creates bitmap for time
    hitMap = create_bitmap(100, 10);    //Creates bitmap for hit counter
    BITMAP *buffer = create_bitmap(screen->w, screen->h);
    //Load gameboard and its elements
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
        if (ammo >= 2 && timer >= actionDelay)  draw_sprite(buffer, shell[1], 1135, 50);
        if (ammo >= 1 && timer >= actionDelay)  draw_sprite(buffer, shell[0], 1065, 50);
        draw_sprite(buffer,timeMap, 1050, 10);
        draw_sprite(buffer,hitMap, 595, 10);

        blit(buffer, screen, 0, 0, 0, 0,screen->w, screen->h);
        rest(20);
    }
    //Clear memory
    background.destroySamples();    //Destroys music sample
    shot.destroySamples();  //Destroys SFX sample
    reload.destroySamples();    //Destroys SFX sample
    laugh.destroySamples();
    deinitialize(duck);
}

void Engine::control(Duck &duck, Sound_f shot, Sound_f r, Sound_f laugh){
    if (mouse_b & 1 && timer > actionDelay){
        if(ammo <= 0) { reload(r);   return;  } //If ammo is 0 -> reload
        else{
            shot.play(0);   //Plays shot SFX
            ammo--;     //Ammo was used
            if (checkShots(duck)){  //Checks if duck was hit
                duck.duckDestroy();
                draw_sprite(screen, backgroundImage, 0, 0);
                draw_sprite(screen, dog, duck.duckPos.x, 525);
                rest(1500);
                hits++; //Increases hit counter
            }
            else
                laugh.play(0);
            actionDelay = timer + 2;
        }
    }
    else if (((mouse_b & 2) || key[KEY_R]) && ammo < 2)
        reload(r);  //Reload
}

void Engine::reload(Sound_f r){
    r.play(0);
    ammo = 2;
    actionDelay = timer + 1.5;
}

void Engine::deinitialize(Duck &duck){
    Image::destroy(shell[0]);   //Destroys bitmap
    Image::destroy(shell[1]);   //Destroys bitmap
    Image::destroy(duck.avatar);    //Destroys bitmap
    Image::destroy(backgroundImage);    //Destroys bitmap
    Image::destroy(timeMap);    //Destroys bitmap
    Image::destroy(hitMap); //Destroys bitmap

    allegro_exit(); //Removes allegro libs
}

//--------

void errorMessage(const char *message) {    //Displays error message
    allegro_message(message);   //Displays error message
    exit(EXIT_FAILURE);
}

bool checkShots(Duck duck){ //Checks if player hit a duck
    if ((mouse_x > duck.duckPos.x && mouse_x < (duck.duckPos.x + duck.avatar->w)) &&
        ((mouse_y > duck.duckPos.y && mouse_y < (duck.duckPos.y + duck.avatar->h))))
        return true;
    return false;
}
