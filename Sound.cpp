#include "header.h"

Sound_f::Sound_f(std::string path){
    playing = false;
    const char *name = path.c_str();
    track = load_wav(name);
    if (!track) {
        destroy_sample(track);
        errorMessage("Audio file not found or could not be opened.");
    }
}

void Sound_f::play(int loop){
    if (playing) return;
    play_sample(track, 255, 128, 1000, loop);
    playing = true;
}

void Sound_f::stop(){
    if (!playing) return;
    stop_sample(track);
    playing = false;
}

void Sound_f::destroySamples() {
    if (track) destroy_sample(track);
}
