#pragma once
#include <avr/common.h>
#include <avr/builtins.h>
#include "joystick.h"
#define GUI_STRING_MAX_LENGTH 32
#define GUI_MAX_CHILDREN 3

typedef struct gui_menu_item_t{
	char text[GUI_STRING_MAX_LENGTH+1];
	void (*click_action)(void);
	struct gui_menu_item_t *parent; //Pointer to parent
	struct gui_menu_item_t *children[GUI_MAX_CHILDREN]; //Sub menus
	uint8_t num_children;
} gui_menu_item;

//Initializes children and parent pointers to NULL
void gui_menu_item_create(gui_menu_item *item, char *text, void (*click_action)(void));

/**
 * Adds child to parent and adds parent to child
 */
int8_t gui_add_child(gui_menu_item *parent, gui_menu_item *child);

//Initializes gui with a root node. Returns the root node
gui_menu_item *gui_init();

//Draws all menu items to oled
void gui_draw_menu(gui_menu_item *item, int8_t selected_item);

/*
* Menu loop
*/
void gui_goto_menu(gui_menu_item **gui_menu_current);
