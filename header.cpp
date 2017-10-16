#include "header.h"

void Error_Message(const char *message) {
    allegro_message(message);
    exit(EXIT_FAILURE); //do zmiany
}

struct Mouse_position{
    int x;
    int y;
}pos;

//----------------- Engine initialization methods -----------------
Engine::Engine()    { state = MENU; }

void Engine::run(){
    initialize(Keyboard | Mouse | Sound);
    initialize(2, "XD");

    if (mouse)  show_mouse(screen);
    sound_f soundtrack;
    Image image;

    /* Main loop */
    while (state != EXIT){
        switch (state){
            case EngineState::MENU:
                //Some random code here
                while (!key[KEY_ESC]);
                state = EXIT;
                break;

            case EngineState::STATE1:
                //Some random code here
                while (!key[KEY_ESC]);
                state = EXIT;
                break;

            case EngineState::STATE2:
                //Some random code here
                while (!key[KEY_ESC]);
                state = EXIT;
                break;

            default:
                break;
        }
    }
    soundtrack.destroy_samples();
    image.destroy_bitmaps();
    deinitialize();
}

void Engine::initialize(Options option) {
    int color_depth;
    allegro_init();
    if ((color_depth = desktop_color_depth()) != 0)
        set_color_depth(color_depth);
    install_timer();
    if (option & Keyboard)
        install_keyboard();
    if (option & Mouse) {
        install_mouse();
        enable_hardware_cursor();
    }
    if (option & Sound)
        install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);

    srand(time(NULL));
}

void Engine::initialize(int mode, std::string resolution) {
    int w, h;
    if (mode != 1 && mode != 2) mode = 2;
    if (resolution == "FULLHD") { w = 1920; h = 1080; }
    else if (resolution == "HD") { w = 1280; h = 720; }
    else { w = 640; h = 480; }
    if (set_gfx_mode(mode , w, h, 0, 0) < 0)   Error_Message("Could not establish the graphic mode.");
    set_window_title("2D Engine v1.01");
    clear_to_color(screen, WHITE);
}

//----------------- Other Engine methods -----------------

void Engine::keyboard_control(){
    int val;

    do {
        clear_keybuf();
        val = readkey();
        switch ((val >> 8)) {
            case KEY_UP:
                allegro_message("UP ARROW");
                break;
            case KEY_LEFT:
                allegro_message("LEFT ARROW");
                break;
            case KEY_RIGHT:
                allegro_message("RIGHT ARROW");
                break;
            case KEY_DOWN:
                allegro_message("DOWN ARROW");
                break;
            case KEY_W:
                allegro_message("W Key");
                break;
            case KEY_S:
                allegro_message("S Key");
                break;
            case KEY_A:
                allegro_message("A Key");
                break;
            case KEY_D:
                allegro_message("D Key");
                break;
            case KEY_ESC:
                break;
            case KEY_ENTER:
                allegro_message("Enter Key");
                break;
            case KEY_SPACE:
                allegro_message("Space Key");
                break;
            default:
                break;
        }
    }
    while((val >> 8) != KEY_ESC);
}

void Engine::mouse_control(){
    show_mouse(screen);
    do {
        if (mouse_b & 1); //LPM
        if (mouse_b & 2);   //PPM
        //if (mouse_b & 4);    //SPM - Zbugowane albo mam !&@%^ myszke

        //pos.x = mouse_pos >> 16;
        //pos.y = mouse_pos & 0x0000ffff;
    }
    while(!(mouse_b & 4));
}

void Engine::draw_points(std::vector<Point2D> &zbior, float color){
    for(unsigned int count = 0; count < zbior.size(); count++)
        putpixel(screen, zbior[count].x, zbior[count].y, color);
}

//----------------- draw primitives -----------------
void Engine::draw(BITMAP *bmp, int mode, int x1, int y1, int x2, int y2, float color)
{
    if (mode == 0)
        rectfill(bmp, x1, y1, x2, y2, color);
    else if (mode == 1)
        rect(bmp, x1, y1, x2, y2, color);
    else if (mode == 2)
        ellipsefill(bmp, x1, y1, x2, y2, color);
    else if (mode == 3)
        ellipse(bmp, x1, y1, x2, y2, color);
}

void Engine::draw(BITMAP *bmp, bool mode, int x1, int y1, int x2, int y2, int x3, int y3, float color)
{
    if (mode){
        line(bmp, x1, y1, x2, y2, color);
        line(bmp, x2, y2, x3, y3, color);
        line(bmp, x3, y3, x1, y1, color);
    }
    else
        triangle(bmp, x1, y1, x2, y2, x3, y3, color);
}

void Engine::draw(BITMAP *bmp, bool mode, int x1, int y1, float r, float color)
{
    if (mode)
        circle(bmp, x1, y1, r, color);
    else
        circlefill(bmp, x1, y1, r, color);
}

//----------------- other methods -----------------
void Engine::clear(BITMAP *bmp, float color){
    if (color < 0 && color > 255)
        clear_bitmap(bmp);
    else
        clear_to_color(bmp, color);
}

void Engine::rysuj_odcinek(int x1, int y1, int x2, int y2, float color) {
    if (x2 < x1){ std::swap(x1, x2);   std::swap(y1, y2);   }

    float dx = x2 - x1;
    float dy = y2 - y1;

    if (dx == 0)
        for (int y = y1; y <= y2; y++)
            putpixel(screen, x1,  y, color);

    float m = dy / dx;
    if (abs(m) < 1) {
        float y = y1;
        for(int x = x1; x <= x2; x++) { putpixel(screen, x,  int(y + 0.5), color);  y += m;   }
    }
    else {
        if (y2 < y1){   std::swap(x1, x2);   std::swap(y1, y2);   }

        m = dx / dy;
        float x = x1;
        for(int y = y1; y <= y2; y++) { putpixel(screen, int(x + 0.5),  y, color);    x += m; }
    }
}

void Engine::deinitialize(){
    allegro_exit();
}
