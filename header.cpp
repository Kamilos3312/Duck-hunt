#include "header.h"

volatile int timer; //Main timer
volatile int actionDelay;   //Delay for player action
volatile int second;    //Delay 1 sec timer
void incTimer() { timer++; }    //Main timer inc function
END_OF_FUNCTION(incTimer)

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
    set_window_title("Duck Hunt v1.03 - Official Release");    //Sets window title
    //set_close_button_callback();  //Sets default action for clicking on X button
    clear_to_color(screen, makecol(255, 255, 255)); //Sets the color inside the window
    mouse_x = screen->w/2;  //Sets Mouse cursor in the middle of the screen
    mouse_y = screen->h/2;  //Sets Mouse cursor in the middle of the screen
    show_mouse(screen); //Shows cursor inside window
}

void Engine::run(){
    //Create objects and load resources
    Sound_f background("resources/background.wav"); //Loads audio from given path into the sample
    Sound_f shot("resources/shot.wav"); //Loads audio from given path into the sample
    Sound_f reload("resources/reload.wav"); //Loads audio from given path into the sample
    Duck duck;  //Creates duck object
    Image::load(&backgroundImage, "resources/background.bmp");  //Loads image from given path into the bitmap
    Image::load(&duck.avatar, "resources/duck.bmp");    //Loads image from given path into the bitmap
    Image::load(&shell[0], "resources/shell.bmp");  //Loads image from given path into the bitmap
    Image::load(&shell[1], "resources/shell.bmp");  //Loads image from given path into the bitmap

    //Set default
    hits = 0;   //Hit counter
    ammo = 2;   //Available ammo

    //Initiate timer
    timer = 0;  //Resets timer
    second = 0; //0 No start delay
    actionDelay = 0;    //Resets action delay
    LOCK_FUNCTION(incTimer);    //Locks timer function
    LOCK_VARIABLE(timer);   //Locks timer
    install_int_ex(incTimer, SECS_TO_TIMER(1)); //Timer will tick every 1 second
    //Create bitmaps for Time and Hits
    timeMap = create_bitmap(130, 12);   //Creates bitmap for time
    hitMap = create_bitmap(100, 10);    //Creates bitmap for hit counter
    //Load gameboard and its elements
    background.play(1);

    while (!key[KEY_ESC]){
        control(duck, shot, reload);
        if (timer > second){
            textprintf_centre_ex(timeMap, font, 65, 2, makecol(255,255,255), -1, "Time: %02d: %02d: %02d", (((timer)/3600)%24) ,(((timer)/60)%60) ,((timer)%60));
            textprintf_centre_ex(hitMap, font, 50, 1, makecol(255,255,255), -1, "Hits: %d", hits);
            Image::draw(backgroundImage, 0 ,0);
            draw_sprite(screen, timeMap, 1050, 10);
            draw_sprite(screen, hitMap, 595, 10);
            if (duck.visible)  duck.duckMove();
            else duck.duckSpawn();
            Image::draw(duck.avatar, duck.duckPos.x, duck.duckPos.y);  //Draws duck onto the screen
            if (ammo >= 2 && timer >= actionDelay)  Image::draw(shell[1], 1135, 50);
            if (ammo >= 1 && timer >= actionDelay)  Image::draw(shell[0], 1065, 50);
            second++;
        }
        clear_bitmap(timeMap);
        clear_bitmap(hitMap);
    }
    //Clear memory
    background.destroySamples();    //Destroys music sample
    shot.destroySamples();  //Destroys SFX sample
    reload.destroySamples();    //Destroys SFX sample
    Image::destroy(shell[0]);   //Destroys bitmap
    Image::destroy(shell[1]);   //Destroys bitmap
    Image::destroy(duck.avatar);    //Destroys bitmap
    Image::destroy(backgroundImage);    //Destroys bitmap
    Image::destroy(timeMap);    //Destroys bitmap
    Image::destroy(hitMap); //Destroys bitmap

    allegro_exit(); //Removes allegro libs
}

void Engine::control(Duck &duck, Sound_f shot, Sound_f r){
    if (mouse_b & 1 && timer > actionDelay){
        if(ammo <= 0) { reload(r);   return;  } //If ammo is 0 -> reload
        else{
            shot.play(0);   //Plays shot SFX
            ammo--; //Ammo was used
            if (checkShots(duck)){  //Checks if duck was hit
                duck.duckDestroy();
                hits++; //Increases hit counter
            }
            actionDelay = timer + 1;
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

//--------

void errorMessage(const char *message) {    //Displays error message
    allegro_message(message);   //Displays error message
    exit(EXIT_FAILURE); //Exit program with FAILURE flag
}

bool checkShots(Duck duck){ //Checks if player hit a duck
    if ((mouse_x > duck.duckPos.x && mouse_x < (duck.duckPos.x + duck.avatar->w)) &&
        ((mouse_y > duck.duckPos.y && mouse_y < (duck.duckPos.y + duck.avatar->h))))
        return true;
    return false;
}
