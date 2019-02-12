#pragma once

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <libconfig.h>

typedef enum {
	FRAME,
	CONTENT,
    INPUT,
    BUTTON,
    SELECTOR,
} type;

typedef  struct {
	char ch;
	unsigned number;
} ch_allow;

typedef struct {
	char* val;
	int length;
	ch_allow *allowed_chars;
	unsigned allowed_chars_length;
	float min_allowed;
	float max_allowed;
} input;

typedef struct {
	bool val;
} button;

typedef struct {
	char **options_title;
	int size;
	int option;
} selector;

typedef union Typ {
	input *Input;
	button *Button;
	selector *Selector;
} Typ;

typedef struct{
    type typ;
    int id;
    char *name;
    int lenght;
    WINDOW *win;
    bool is_active;
    Typ *Typ;
    int size;
} frame;


char* set_str(const char*);
char* append(char*, int *, char );
char* delend(char*, int *);
int if_str_has(const char*, unsigned, char);

bool check_display(void);
frame get_default_frame(void);
void add_input(frame *, const char*, const char*);
void add_allowed_char(frame *, char , unsigned );
void add_button(frame *, const char* );
void add_selector(frame *, const char* , unsigned ,char**);

bool config_validaiton(config_t *);
void insert_default_value(config_t *, frame*, unsigned);

void create_header(const char*);
void create_form(frame*, unsigned);
void form_destroy(frame*, unsigned);

void display_frames(frame *, unsigned, unsigned*);
void keypad_controller(frame *, unsigned, unsigned*);


