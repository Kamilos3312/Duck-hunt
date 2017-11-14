#include "header.h"

Sound_f::Sound_f(std::string path){
    const char *name = path.c_str();
    track = load_wav(name);
    if (!track) {
        destroy_sample(track);
        errorMessage("Audio file not found or could not be opened.");
    }
}

void Sound_f::play(int loop){
    play_sample(track, 255, 128, 1000, loop);
}

void Sound_f::destroySamples() {
    stop_sample(track);
    if (track) destroy_sample(track);
}
