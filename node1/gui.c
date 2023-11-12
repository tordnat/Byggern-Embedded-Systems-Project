#include "gui.h"
#include <stdlib.h>
#include <avr/builtins.h>
#include <avr/common.h>
#include <string.h>
#include <stdio.h>
#include "oled.h"
#include "joystick.h"
#include "pong_game.h"

void gui_menu_item_create(gui_menu_item *item, char *text, void (*click_action)(void)) {
	if(item) {
		if(strlen(text) > GUI_STRING_MAX_LENGTH) {
			strcpy(item->text, "String too long");
		} else {
			strcpy(item->text, text);
		}
		item->click_action = click_action;
		item->parent = NULL;
		item->num_children = 0;
	}
}

int8_t gui_add_child(gui_menu_item *parent, gui_menu_item *child) {
	parent->children[parent->num_children] = child;
	child->parent = parent;
	parent->num_children++;
	return 0;
}


static void menu_start_game_func() {
	game_start_pong();
}

static void menu_settings_func() {
}

static void menu_debug_func() {
}

gui_menu_item *gui_init() {
	static gui_menu_item root;
	gui_menu_item_create(&root, "root", NULL);
	
	static gui_menu_item start_game;
	gui_menu_item_create(&start_game, "Start Game", menu_start_game_func);
	gui_add_child(&root, &start_game);
	
	static gui_menu_item settings;
	gui_menu_item_create(&settings, "Settings", menu_settings_func);
	gui_add_child(&root, &settings);
	
	static gui_menu_item debug;
	gui_menu_item_create(&debug, "I/0 Debug", menu_debug_func);
	gui_add_child(&settings, &debug);
	
	static gui_menu_item debug2;
	gui_menu_item_create(&debug2, "CAN Debug", menu_debug_func);
	gui_add_child(&settings, &debug2);
	
	gui_draw_menu(&root, 0);

	return &root;
}

void gui_draw_menu(gui_menu_item *item, int8_t selected_item) {
	oled_reset(); //this should be done smarter, should only reset single pages, not all of them.
	for(int8_t i = 0; i < item->num_children; i++) {
		oled_pos(i,0);
		if(i == selected_item) {
			oled_print(char_arrow);
		}
		oled_print_str(item->children[i]->text);
	}
}

void gui_game_pong(){ // Should implement \n -newline in the future
	char score[8];
	char high_score[8];
	sprintf(score, "%u", game_get_score());
	sprintf(high_score, "%u", game_get_high_score());
	oled_reset();
	oled_pos(0,0);
	oled_print_str("Pong Playing");
	oled_pos(1,0);
	oled_print_str("Score:");
	oled_print_str(score);
	oled_pos(2,0);
	oled_print_str("High Score:");
	oled_pos(3,0);
	oled_print_str(high_score);
}

static direction_t prev_joystick_dir = NEUTRAL;
static int8_t selected_item = 0;

void gui_goto_menu(gui_menu_item **gui_menu_current) {
	if (game_get_is_playing()) return gui_game_pong(); // Make way for gui_game
	position_t joystick_pos = joystick_get_position();
	uint8_t		joystick_button = joystick_get_button();
	direction_t joystick_dir = joystick_get_direction(joystick_pos);
	if(prev_joystick_dir != NEUTRAL) {
		prev_joystick_dir = joystick_dir;
		return;
	}
	prev_joystick_dir = joystick_dir;
	switch(joystick_dir) {
		case DOWN: //Navigate down
			printf("down %s\n\r",(*gui_menu_current)->text);
			selected_item += 1;
			if(selected_item >= (*gui_menu_current)->num_children) {
				selected_item = (*gui_menu_current)->num_children-1;
			}
			gui_draw_menu((*gui_menu_current), selected_item);
		break;
		case UP: //Navigate up
			printf("up %s\n\r",(*gui_menu_current)->text);
			selected_item -= 1;
			if(selected_item <= 0) selected_item = 0;
			gui_draw_menu((*gui_menu_current), selected_item);
		break;
		case RIGHT: //Go to selected child
			printf("right %s\n\r",(*gui_menu_current)->text);
			(*gui_menu_current)->children[selected_item]->click_action();
			if((*gui_menu_current)->num_children != 0) { //To ensure next if does not select from an empty array
				if((*gui_menu_current)->children[selected_item]->num_children != 0) {
					(*gui_menu_current) = (*gui_menu_current)->children[selected_item];
					selected_item = 0;
					gui_draw_menu((*gui_menu_current), selected_item);
					
				}
			}
		break;
		case LEFT: //Go to parent
			printf("left %s\n\r",(*gui_menu_current)->text);
			if((*gui_menu_current)->parent != NULL){ //Check for no parent
				(*gui_menu_current) = (*gui_menu_current)->parent;
				selected_item = 0;
				gui_draw_menu((*gui_menu_current), selected_item);
			}
		break;
	}
}