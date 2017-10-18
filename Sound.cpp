#include "header.h"

Sound_f::Sound_f(std::string path){ //Loads audio from given path
    const char *name = path.c_str();    //Converts string into const char *
    track = load_wav(name); //Loads audio from given path into the sample
    if (!track) {   //If loading audio was unsuccessful ->
        destroy_sample(track);  //Destroys sample
        errorMessage("Audio file not found or could not be opened.");   //Displays error message;
    }
}

void Sound_f::play(int loop){   //Plays audio
    play_sample(track, 255, 128, 1000, loop);   //Play audio with loop or without it
}

void Sound_f::destroySamples() {    //Destroys samples
    stop_sample(track); //Stops sample
    if (track) destroy_sample(track);   //Destroys sample
}
