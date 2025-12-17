#ifndef SOUND_H
#define SOUND_H
#include <SDL3/SDL.h>
#include <stdio.h>

// Définition des structures de données audio pour le fichier WAV
typedef struct AudioStreamInstance{
    SDL_AudioStream * stream;
    Uint8 *data;        
    Uint32 data_length;      
    SDL_AudioSpec src_spec; 
    bool is_playing;
}AudioStreamInstance;


typedef struct DeviceID{
    SDL_AudioDeviceID deviceID;
    SDL_AudioSpec obtained_spec;
}DeviceID;


// Quelques opérations relatives à AudioStreamInstance
AudioStreamInstance* LoadWAV(const char *filename, DeviceID * deviceId );
bool PlayAudioStream(AudioStreamInstance * audioStreamInstance);
void CleanUpAudioStreamInstance(AudioStreamInstance* audioStream);


// Quelques opérations relatives à DeviceID
DeviceID* InitAudioDevice(void);
void CleanUpDevice(DeviceID * deviceId);


// Fonction spéciale pour le son des pions
void PlaySoundOfPion(AudioStreamInstance* ListSoundsOfPion[4]);
void CleanUpSoundsPions(AudioStreamInstance*ListSoundsOfPion[4]);
#endif
