#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "view_func.h"
#include "model_func.h"

char* set_str(const char* string){
	if(string == NULL) return NULL;
    char *str = (char*)calloc(strlen(string) + 1,sizeof(char));
    for(int i=0; i<strlen(string)+1; i++) str[i] = string[i];
    return str;
}

char* append(char* string, int *n, char ch){
	/// Enter:
    if((int)ch == 10 || *n >= 10) return string;
    /// Check if chars are allowed
    const char* chars_allowed = "0123456789.";
    bool continu = false;
    for(int i = 0; i < strlen(chars_allowed); i++){
		if(ch == chars_allowed[i]) {
			continu = true;
			break;
		}
	}
	if(!continu) return string;
    char *new_str;
    if(string == NULL) new_str = (char*)malloc(2);
    new_str = (char*)calloc((*n) + 1, sizeof(char));
    if(new_str != NULL) {
        int i = 0;
        for(i=0; i<*n; i++){
            new_str[i] = string[i];
        }
        new_str[i++] = ch;
        new_str[i] = '\0';
        (*n)++;
        return new_str;
    } else {
        return string;
    }
}

int if_str_has(const char* string, unsigned n, char ch){
	int c = 0;
    for(int i=0; i < n; i++){
        if(string[i] == ch)
            c++;
    }
    return c;
}

char* delend(char* string, int *n){
    if(string == NULL || *n <= 1) {
		*n = 0;
		if(*n == 1) free(string);
        return NULL;
    } else {
		(*n) -= 1;
        string[*n] = 0;
        return string;
    }
}

bool check_display(){
	int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);
	if(width < 60 || height < 45)
		return false;
	return true;
}

frame get_default_frame(void){
    frame output;
    output.name = "ABC";
    output.typ = FRAME;
    output.is_active = false;
    output.Typ = NULL;
    output.size = 0;
    return output;
}

void add_input(frame *f, const char* name, const char* val){
	f->typ = INPUT;
	f->name = set_str(name);
	f->Typ = (Typ *)malloc(1);
	f->Typ->Input = (input*)malloc(1);
	f->Typ->Input->allowed_chars = NULL;
	f->Typ->Input->length = (int)strlen(val);
	f->Typ->Input->val = set_str(val);
	f->Typ->Input->allowed_chars_length = 0;
}

void add_allowed_char(frame *f, char ch, unsigned number){
	if(f->Typ == FRAME) return;
	if(f->Typ == NULL) return;
	if(f->typ != INPUT) return;
	if(f->Typ->Input->allowed_chars == NULL){
		f->Typ->Input->allowed_chars = (ch_allow*)malloc(1);
		f->Typ->Input->allowed_chars->ch = ch;
		f->Typ->Input->allowed_chars->number = number;
		f->Typ->Input->allowed_chars_length = 1;
		return;
	} else {
		f->Typ->Input->allowed_chars = (ch_allow*)realloc(f->Typ->Input->allowed_chars,++f->Typ->Input->allowed_chars_length);
		f->Typ->Input->allowed_chars[f->Typ->Input->allowed_chars_length-1].ch = ch;
		f->Typ->Input->allowed_chars[f->Typ->Input->allowed_chars_length-1].number = number;
		return;
	}
	return;
}

void add_button(frame *f, const char* name){
	f->typ = BUTTON;
	f->name = set_str(name);
	f->Typ = (Typ*)malloc(1);
	f->Typ->Button = (button*)malloc(1);
	f->Typ->Input->val = false;
}

void add_selector(frame *f, const char* name, unsigned n,char** opts){
	f->typ = SELECTOR;
	f->name = set_str(name);
	f->Typ = (Typ*)malloc(1);
	f->Typ->Selector = (selector*)malloc(1);
	f->Typ->Selector->options_title = (char**)calloc(n, sizeof(char*));
	for(int i=0; i<n; i++)
		f->Typ->Selector->options_title[i] = set_str(opts[i]);
	f->Typ->Selector->size = n;
	f->Typ->Selector->option = 0;
}

bool config_validaiton(config_t *cfg){
	if(cfg == NULL) return false;
	if(!config_read_file(cfg, "emerytury.cfg"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(cfg),
        config_error_line(cfg), config_error_text(cfg));
        config_destroy(cfg);
        return false;
    }
}

void insert_default_value(config_t *cfg, frame *frame_list, unsigned N){
	if(cfg == NULL) return;
	config_setting_t *setting;
	setting = config_lookup(cfg, "frame_list");
    if(setting != NULL){
        int j = 0;
        for(int i=0; i<N-1; i++){
            config_setting_t *frame = config_setting_get_elem(setting, j++);
            const char *val;

            if(!config_setting_lookup_string(frame, "val", &val))
                continue;

            while(frame_list[i].typ != INPUT && i<N-1) i++;
            if(frame_list[i].Typ != NULL) frame_list[i].Typ->Input->val = (char*)val;
            if(frame_list[i].Typ != NULL) frame_list[i].Typ->Input->length = strlen((char*)val);
        }
    }
}

void create_header(const char* title){
	int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);
	WINDOW *title_frame =  newwin(5, width - 4, 1, 2);
    refresh();
    box(title_frame, 0, 0);
    mvwprintw(title_frame, 2, width/2 - strlen(title)/2, title);
    wrefresh(title_frame);
};

void create_form(frame *frame_list, unsigned M){
	int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);
    
	for(int i=0; i<9; i++) {
        frame_list[i] = get_default_frame();
        frame_list[i].win = newwin(3, width-4, 7+i*3, 2);
        box(frame_list[i].win, 0, 0);
    }

	int N = 0;
    frame_list[N].id = N;
    char *opts[] = { "NETTO", "BRUTTO" };
    add_selector(&frame_list[N++], "Pensja:" , 2, opts);

    frame_list[N].id = N;
    add_input(&frame_list[N++], "Ilosc oplaconych skladek:", "123");

    frame_list[N].id = N;
    add_input(&frame_list[N], "Pobory miesieczne:", "123");
    frame_list[N].Typ->Input->min_allowed = 100; frame_list[N].Typ->Input->max_allowed = 100000;
    add_allowed_char(&frame_list[N++], '.', 1);

    frame_list[N].id = N;
    char *months[] = { "STYCZEN", "LUTY", "MARZEC", "KWIECIEN", "MAJ", "CZERWIEC", "LIPIEC", "SIERPIEN", "WRZESIEN", "PAZDZIERNIK", "LISTOPAD", "GRUDZIEN"};
    add_selector(&frame_list[N++], "Miesiac przejscia na emeryture:" , 12, months);
    
    frame_list[N].id = N;
    char *years[82];
    for(int i=0; i<82; i++) {
			years[i] = (char *)calloc(4, sizeof(char));
			sprintf(years[i], "%d", 2019 + i);
	}
	add_selector(&frame_list[N], "Rok przejscia na emeryture:" , 82, years);
    N++;

    frame_list[N].id = N;
    add_input(&frame_list[N], "Oczekiwana dlugosc zycia emeryta [msc]:", "123");
    add_allowed_char(&frame_list[N++], '.', 1);

    frame_list[N].id = N;
    add_input(&frame_list[N], "Skladki na ubezpiecznie [\%]:", "123");
    frame_list[N].Typ->Input->min_allowed = 0.00; 
    frame_list[N].Typ->Input->max_allowed = 99.00;
    add_allowed_char(&frame_list[N++], '.', 1);

    frame_list[N].id = N;
    frame_list[N].win = newwin(3, width/2, N*3 + 8, width/2 - 1);
    add_button(&frame_list[N++], "Przelicz");

	frame_list[N].id = N;
	frame_list[N].name = set_str("");
    frame_list[N].win = newwin(height - (3*N + 14), width-4, N*3 + 12, 2);
    N++;
}

void form_destroy(frame* frame_list, unsigned N){
	for(int i=0; i<N; i++){
		if(frame_list[i].Typ != NULL) free(frame_list[i].Typ);
	}
}

void display_frames(frame *frame_list, unsigned N, unsigned *current_frame){
	refresh();
	
	int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);
    
	/// Show all frame
        for(int i=0; i<N; i++){
            /// Clear current frame
            if(i == *current_frame)
                wclear(frame_list[i].win);         
            box(frame_list[i].win, 0, 0);
            
            /// Inverse color in current frame
            if(i == *current_frame) wattron(frame_list[i].win, A_REVERSE);
            mvwprintw(frame_list[i].win, 1, 2, frame_list[i].name);
            if(i == *current_frame) wattroff(frame_list[i].win, A_REVERSE);
            
            /// Show value for frame
            switch(frame_list[i].typ){
				case FRAME:
					break;
				case INPUT:
					wprintw(frame_list[i].win, " ");
					if(frame_list[i].Typ->Input->length){
						if(i == *current_frame) wattron(frame_list[i].win, A_REVERSE);
						mvwprintw(frame_list[i].win, 1, width - frame_list[i].Typ->Input->length - 6, frame_list[i].Typ->Input->val);
						if(i == *current_frame) wattroff(frame_list[i].win, A_REVERSE);
					}
					break;
				case BUTTON:
					wprintw(frame_list[i].win, " ");
					break;
				case SELECTOR:
					mvwprintw(frame_list[i].win, 1, width -	strlen(frame_list[i].Typ->Selector->options_title[frame_list[i].Typ->Selector->option]) - 11, " < ");
					if(i == *current_frame) wattron(frame_list[i].win, A_REVERSE);
					wprintw(frame_list[i].win, frame_list[i].Typ->Selector->options_title[frame_list[i].Typ->Selector->option]);
					if(i == *current_frame) wattroff(frame_list[i].win, A_REVERSE);
					wprintw(frame_list[i].win, " > ");
					break;
				default:
					break;
			}
			wrefresh(frame_list[i].win);
        }
}

void keypad_controller(frame* frame_list, unsigned N, unsigned *cf){
		int current_frame = *cf;
		/// Get current pushed key
        keypad(frame_list[current_frame].win, true);
        int g = wgetch(frame_list[current_frame].win);
        switch(g){
            case KEY_UP:
				current_frame = (current_frame - 1 >= 0) ? current_frame-1 : (N - 2);
                frame_list[current_frame].is_active = true;
                break;
            case KEY_DOWN:
				current_frame = (current_frame + 1 < N-1) ? current_frame+1 : 0;
                frame_list[current_frame].is_active = true;
                break;
            case KEY_RIGHT:
                if(frame_list[current_frame].typ == SELECTOR){
                    frame_list[current_frame].Typ->Selector->option++;
                    frame_list[current_frame].Typ->Selector->option %= frame_list[current_frame].Typ->Selector->size;
                }
                break;
            case KEY_LEFT:
                if(frame_list[current_frame].typ == SELECTOR){
                    frame_list[current_frame].Typ->Selector->option--;
                    frame_list[current_frame].Typ->Selector->option = (frame_list[current_frame].Typ->Selector->option < 0) ? frame_list[current_frame].Typ->Selector->size- 1 : frame_list[current_frame].Typ->Selector->option;
                }
                break;
			case KEY_BACKSPACE:
				if(frame_list[current_frame].typ == INPUT) {
					frame_list[current_frame].Typ->Input->val = delend(frame_list[current_frame].Typ->Input->val, &frame_list[current_frame].Typ->Input->length);
				}
				break;
			case (int)'.':
				if(frame_list[current_frame].typ == INPUT)
						if(!if_str_has(frame_list[current_frame].Typ->Input->val, frame_list[current_frame].Typ->Input->length, '.')){
							char *temp = set_str(append(frame_list[current_frame].Typ->Input->val, &frame_list[current_frame].Typ->Input->length, g));
							frame_list[current_frame].Typ->Input->val = temp;		
							break;
						}
            default:
				if(frame_list[current_frame].typ == INPUT){
					char *temp = set_str(append(frame_list[current_frame].Typ->Input->val, &frame_list[current_frame].Typ->Input->length, g));
					frame_list[current_frame].Typ->Input->val = temp;
					break;
				}
				
				if(frame_list[current_frame].typ == BUTTON){
					result_controller(frame_list, N);
           		}
				break;
        }
        *cf = current_frame;
}
