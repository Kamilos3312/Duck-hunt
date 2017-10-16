#include "header.h"

sound_f::sound_f(){
    playing = 0;
    if (load() < 0)
        Error_Message("Audio file not found or could not be opened.");
}

int sound_f::load(){
    track1 = load_wav("win_humans1.wav");
    if (!track1) {destroy_sample(track1);   return -1;}
    return 0;
}

void sound_f::play(){
    if (playing == 1) return;
    play_sample(track1, 255, 128, 1000, 1);
    playing = 1;
}

void sound_f::stop(){
    if (playing == 0) return;
    stop_sample(track1);
    playing = 0;
}

void sound_f::destroy_samples() {
    if (track1) destroy_sample(track1);
}
