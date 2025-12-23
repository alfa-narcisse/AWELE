#include <SDL3/SDL.h>
#include <stdio.h>

// Définition de nos structures de données audio pour le fichier WAV

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





// Charge un fichier WAV et crée l'audioStreamInstance correspondante
AudioStreamInstance* LoadWAV(const char *filename, DeviceID * deviceId ) {                   
    AudioStreamInstance* audioStreamInstance = (AudioStreamInstance*) SDL_malloc(sizeof(AudioStreamInstance));
    if (!audioStreamInstance){
        fprintf(stderr, "Ereur allocation: %s", SDL_GetError());
        return NULL;
    }

    if (!SDL_LoadWAV(filename, &audioStreamInstance->src_spec, &audioStreamInstance->data, &audioStreamInstance->data_length)) {
        fprintf(stderr, "Erreur: %s", SDL_GetError());
        return NULL;
    }
    
    SDL_AudioStream* stream = SDL_CreateAudioStream(&audioStreamInstance->src_spec, &deviceId->obtained_spec);
    if (!stream){
        fprintf(stderr, "Erreur création AudioStream: %s", SDL_GetError());
        return NULL;
    }

    if (!SDL_BindAudioStream(deviceId->deviceID, stream)){
        fprintf(stderr, "Error in binding device and stream: %s", SDL_GetError());
        SDL_free(stream);
        return NULL;
    }
    audioStreamInstance->is_playing = false;
    audioStreamInstance->stream = stream;
    return audioStreamInstance;
}


bool PlayAudioStream(AudioStreamInstance * audioStreamInstance){
    if (!SDL_PutAudioStreamData(audioStreamInstance->stream,audioStreamInstance->data, audioStreamInstance->data_length)){
        fprintf(stderr, "Erreur dans le puting: %s", SDL_GetError());
        return false;
    }
    audioStreamInstance->is_playing = true;
    return true;
}


DeviceID* InitAudioDevice(void){
    DeviceID* deviceId = (DeviceID*)SDL_malloc(sizeof(DeviceID));
    SDL_AudioSpec desired_spec = {
        .channels = 2,
        .format = SDL_AUDIO_F32,
        .freq = 48000
    };
    deviceId->deviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec);
    if (deviceId->deviceID==0){
        fprintf(stderr, "Erreur d'ouverture device: %s", SDL_GetError());
        SDL_free(deviceId);
        return NULL;
    }
    if(!SDL_GetAudioDeviceFormat(deviceId->deviceID, &deviceId->obtained_spec,NULL)){
        fprintf(stderr, "Erreur chargement spec final: %s", SDL_GetError());
        SDL_free(deviceId);
        return NULL;
    }
    return deviceId;
}


void CleanUpAudioStreamInstance(AudioStreamInstance* audioStream){
    if (audioStream){
        SDL_UnbindAudioStream(audioStream->stream);
        SDL_DestroyAudioStream(audioStream->stream);
        SDL_free(audioStream->data);
        SDL_free(audioStream);
    }
}


void CleanUpDevice(DeviceID * deviceId){
    if (deviceId){
        SDL_CloseAudioDevice(deviceId->deviceID);
        SDL_free(deviceId);
    }
}

/*================Fonction spéciale pour le son de déplacement du pion============================*/

void PlaySoundOfPion(AudioStreamInstance* ListSoundsOfPion[4]){
    int choice = SDL_rand(4);
    if (!PlayAudioStream(ListSoundsOfPion[choice])){
        fprintf(stderr, "Erreur de lancement du son de pion");
    }
    SDL_ResumeAudioStreamDevice(ListSoundsOfPion[choice]->stream);
    SDL_FlushAudioStream(ListSoundsOfPion[choice]->stream);
    SDL_Delay(800);
    SDL_PauseAudioStreamDevice(ListSoundsOfPion[choice]->stream);
}


void CleanUpSoundsPions(AudioStreamInstance*ListSoundsOfPion[4]){
    for (int i=0;i<4;i++){
        CleanUpAudioStreamInstance(ListSoundsOfPion[i]);
    }
}

/*===============================FIN DES FONCTIONS SPECIALES POUR PIONS============================*/
