#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <libconfig.h>

#include "view_func.h"
#include "model_func.h"

/**
Cel:
- podanie wysokosci emerytury po osiagnieci wieku emerytalnego
    - netto
    - brutto

Dane wejsciowe:
    1) czas pracy (ilosc oplaconych skladek)
    2) pobory miesieczne (brutto)
    3) czas przejscia na emeryture (rok i miesiac)
    4) oczekiwana dlugosc zycia emeryta (wg tabel ZUS; w miesiacach)
    5) skladki na ubezpiecznie (19,52%)
*/


int main(){
	
	const char *title = "OBLICZ WARTOSC SWOJEJ EMERYTURY";
	int current_frame = 0;
	int N = 9;
	config_t cfg;
	initscr();
	noecho();
	curs_set(0);
	
    config_init(&cfg);
	if(!config_validaiton(&cfg)) return 0;
	
	if(!check_display()){
		printf("\nWymaga rodzielczosci conajmniej 65x45 Prosze powiekszyc terminal\n");
		config_destroy(&cfg);
		endwin();
		return 0;
	}

	create_header(title);
	
    frame frame_list[N];
    create_form(frame_list, N);
	
	insert_default_value(&cfg, frame_list, N);
	
    while(1){
		/// Mainloop
		display_frames(frame_list, N, &current_frame);
		keypad_controller(frame_list, N, &current_frame);
    }
    
    form_destroy(frame_list, N);
    config_destroy(&cfg);
    endwin();
    

    return 0;
}
