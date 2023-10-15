#pragma once

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
int gui_add_child(gui_menu_item *parent, gui_menu_item *child);

//Returns 0 on success
//Returns -1 if there is nothing to destroy
int gui_destroy_all_children(gui_menu_item* parent);

//Initializes gui with a root node. Returns the root node
gui_menu_item *gui_init();

//Draws all menu items to oled
void gui_draw_menu(gui_menu_item *item, int8_t selected_item);

//Need to change name, should be the animation loop. Not completed yet
void gui_goto_menu(gui_menu_item *gui_menu_current, int8_t selected_item, direction *joystick_dir_ptr, direction *prev_joystick_dir_ptr);