#pragma once
#include <avr/common.h>
#include <avr/builtins.h>
#include "joystick.h"
#define GUI_STRING_MAX_LENGTH 32

typedef struct gui_menu_item_t{
	char text[GUI_STRING_MAX_LENGTH+1];
	void (*click_action)(void);
	struct gui_menu_item_t *parent; //Pointer to parent
	struct gui_menu_item_t **children; //Sub menus
	uint8_t num_children;
} gui_menu_item;


gui_menu_item *gui_menu_item_create(char *text, void (*click_action)(void));

//Returns 0 on success. 
//Returns -1 if the input is invalid or if we are unable to allocate enough memory
int8_t gui_add_child(gui_menu_item *parent, gui_menu_item *child);

//Returns 0 on success
//Returns -1 if there is nothing to destroy
int8_t gui_destroy_all_children(gui_menu_item* parent);

//Initializes gui with a root node. Returns the root node
gui_menu_item *gui_init();

//Draws all menu items to oled
void gui_draw_menu(gui_menu_item *item, int8_t selected_item);

/*
* Need to change name, should be the animation loop. Not completed yet
* gui_menu_current - pointer to currently visible gui item
* selected_item: number for selected item. Arrow is drawn here.
* joystick_dir_ptr - pointer to the pointer direction at current time
* prev_joystick_dir_ptr - pointer to the previous joystick direction
* 
*/
void gui_goto_menu(gui_menu_item **gui_menu_current, int8_t *selected_item, direction_t *joystick_dir_ptr, direction_t *prev_joystick_dir_ptr);
