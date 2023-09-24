#pragma once
#include <stdlib.h>
#include <avr/builtins.h>
#include <avr/common.h>
#include <string.h>
#include "oled.h"

#define GUI_STRING_MAX_LENGTH 32

typedef struct gui_menu_item_t{
	char text[GUI_STRING_MAX_LENGTH+1];
	void (*click_action)(void);     //Unsure if click_action should navigate
	struct gui_menu_item_t *parent; //Pointer to parent
	struct gui_menu_item_t **children; //Sub menus
	uint8_t num_children;
} gui_menu_item;


gui_menu_item *gui_menu_item_create(char *text, void (*click_action)(void), struct gui_menu_item_t *parent, struct gui_menu_item_t **children, uint8_t num_children);
int gui_add_child(gui_menu_item *parent, gui_menu_item *child);
int gui_destroy_all_children(gui_menu_item* parent);
//Returns root item
gui_menu_item *gui_init();
void gui_draw_menu(gui_menu_item *root);
