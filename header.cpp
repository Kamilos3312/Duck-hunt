#include "header.h"

volatile int timer; //Main timer
volatile int actionDelay;   //Control method delay timer
volatile int second = 1;    //Delay 1 sec timer
volatile int moveDelay = 1; //Delay duck move timer
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
    set_window_title("Duck Hunt v1.02 - No hitbox detection");    //Sets window title
    //set_close_button_callback();  //Sets default action for clicking on X button
    clear_to_color(screen, makecol(255, 255, 255)); //Sets the color inside the window
    show_mouse(screen); //Shows cursor inside window
}

void Engine::run(){
    //Create objects and load resources
    Point mousePos; //Stores the mouse X and Y
    Sound_f background("resources/background_original.wav");    //Creates object for background music and loads .wav from file
    Sound_f shot("resources/shot.wav"); //Creates object for show SFX and loads .wav from file
    Sound_f reload("resources/reload.wav"); //Creates object for reload SFX and loads .wav from file
    Duck duck;  //Creates object for duck
    Image::load(&backgroundImage, "resources/background2.bmp"); //Loads bitmap into the passed file
    Image::load(&duck.avatar, "resources/duck1.bmp");   //Loads bitmap into the passed file
    Image::load(&shell[0], "resources/shell.bmp");  //Loads bitmap into the passed file
    Image::load(&shell[1], "resources/shell.bmp");  //Loads bitmap into the passed file

    //Set default
    hits = 0;   //Resets hits
    ammo = 2;   //Sets starting ammo

    //Initiate timer
    timer = 0;  //Resets timer
    LOCK_FUNCTION(incTimer);    //Locks timer function
    LOCK_VARIABLE(timer);   //Locks timer
    install_int_ex(incTimer, SECS_TO_TIMER(1)); //Initiates timer
    //Create bitmaps for Time and Hits
    time_map = create_bitmap(130, 12);  //Creates bitmap for Time info
    points = create_bitmap(100, 10);    //Creates bitmap for Hits info
    //Load gameboard and its elements
    background.play(1); //Plays background music
    draw_sprite(screen, backgroundImage, 0 ,0); //Draws background onto the screen
    if (ammo >= 2)  {   Image::draw(shell[1], 1135, 50);    Image::draw(shell[0], 1065, 50);   }    //If ammo is 2 -> draws 2 shells onto the screen
    if (ammo == 1)  Image::draw(shell[0], 1065, 50);    //If ammo is 1 -> draws 1st shell onto the screen

    actionDelay = 0;    //Sets starting delay for control method
    while (!key[KEY_ESC]){  //If ESCAPE key is pressed -> exit the program
        //Logic
        control(duck, mousePos, shot, reload);  //Checks player input
        //Draw
        textprintf_centre_ex(time_map, font, 65, 2, makecol(255,255,255), -1,
           "Time: %02d: %02d: %02d", (((timer)/3600)%24) ,(((timer)/60)%60) ,((timer)%60)); //Sets Time text onto the bitmap
        textprintf_centre_ex(points, font, 50, 1, makecol(255,255,255), -1,
           "Hits: %d", hits);   //Sets Hits text onto the bitmap
        draw_sprite(screen, time_map, 1050, 10);    //Draws Time bitmap onto the screen
        draw_sprite(screen, points, 595, 10);   //Draws Hits bitmap onto the screen
        if (timer > second){
            draw_sprite(screen, backgroundImage, 0 ,0); //Draws background onto the screen
            if (!duck.visible) duck.duckSpawn();    //If theres no duck on the map -> spawn new duck
        }
        if (ammo >= 2 && timer > second)  { //If ammo is 2 and timer passed a second -> draw shell
            second++;   //second + 1
            Image::draw(shell[1], 1135, 50);    //Draws 2nd shell
            Image::draw(shell[0], 1065, 50);    //Draws 1st shell
        }
        if (ammo == 1 && timer > second)  { //If ammo is 1 and timer passed a second -> draw shell
                second++;   //second + 1
                Image::draw(shell[0], 1065, 50);    //Draws 1st shell
        }
        else if (ammo == 0 && timer > second)   second++;   //If the ammo is 0 and timer passed a second -> second + 1
        if (timer > moveDelay && duck.visible)   {  //If duck is visible and timer passed delay -> move duck
            moveDelay += 1.5;   //Delays the next move for 1.5s
            duck.duckMove();    //Moves duck
        }
        //Clear bitmaps
        clear(time_map);    //Clears Time bitmap
        clear(points);  //Clears Hits bitmap
    }
    //Clear memory
    background.destroySamples();    //Destroys music sample
    shot.destroySamples();  //Destroys SFX sample
    reload.destroySamples();    //Destroys SFX sample
    Image::destroy(shell[0]);   //Destroys bitmap
    Image::destroy(shell[1]);   //Destroys bitmap
    Image::destroy(duck.avatar);    //Destroys bitmap
    Image::destroy(backgroundImage);    //Destroys bitmap

    allegro_exit(); //Removes allegro libs
}

void Engine::control(Duck duck, Point mousePos, Sound_f shot, Sound_f r){
    mousePos.x = mouse_pos >> 16;   //Gets mouse X position
    mousePos.y = mouse_pos & 0x0000ffff;    //Gets mouse X position

    if ((mouse_b & 1) && (timer > actionDelay + 1)){
        if(ammo <= 0) { reload(r);   return;  } //If ammo is 0 -> reload
        else{
            shot.play(0);   //Plays shot SFX
            ammo--; //Ammo was
            //if (checkShots(duck, mousePos)){  //Checks if duck was hit
                duck.duckDestroy(); //Destroys duck
                hits++; //Increases hit counter
            //}
        }
        actionDelay = timer;    //Delay next usage of this method for 1 second
    }

    if (((mouse_b & 2) || key[KEY_R]) && ammo < 2 && (timer > actionDelay + 1)){
        reload(r);  //Reload
    }
}

void Engine::reload(Sound_f r){
    r.play(0);  //Play reload SFXX
    ammo = 2;   //Sets ammo
    actionDelay += 2;   //Delay next usage of control method for 2 seconds
}

//--------

void errorMessage(const char *message) {    //Displays error message
    allegro_message(message);   //Displays error message
    exit(EXIT_FAILURE); //Exit program with FAILURE flag
}

bool checkShots(Duck duck, Point mousePos){ //Checks if player hit a duck
    if (duck.visible &&
        (mousePos.x > duck.duckPos.x && mousePos.x < (duck.duckPos.x + duck.avatar->w)) &&
        (mousePos.y > duck.duckPos.y && mousePos.y < (duck.duckPos.y + duck.avatar->h)))
        return true;
    return false;
}
