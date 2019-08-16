#pragma once
#define SUNVOX_MAIN
#include "sunvox.h"


int AudioLibrary_init(){
	sv_load_dll();
	sv_init(NULL,44100,1,SV_INIT_FLAG_ONE_THREAD|SV_INIT_FLAG_NO_DEBUG_OUTPUT|SV_INIT_FLAG_AUDIO_INT16);
}
//loads sunvox music into specified slot
void AudioLibrary_LoadSound(int slot,char*path){
	sv_open_slot(slot);
	
	if(sv_load(slot,path)){
		
		
		sv_close_slot(slot);
	}
}
void AudioLibrary_PlaySound(int slot,int autoplay,int volume){
	sv_play_from_beginning(slot);
	sv_set_autostop(slot,!autoplay);
	sv_volume(slot,volume);
}
void AudioLibrary_StopMusic(int slot){
	sv_stop(0);
}

int AudioLibrary_deinit(){
	sv_deinit();
	sv_unload_dll();
}
