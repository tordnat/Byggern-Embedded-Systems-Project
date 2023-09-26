#include "gui.h"


gui_menu_item *gui_menu_item_create(char *text, void (*click_action)(void)) {
	gui_menu_item *item = (gui_menu_item*) malloc(sizeof(gui_menu_item));
	if(item) {
		if(strlen(text) > GUI_STRING_MAX_LENGTH) {
			return NULL;
		}
		strcpy(item->text, text);
		item->click_action = click_action;
		item->parent = NULL;
		item->children = NULL;
		item->num_children = 0;
	}
	return item;
}
int gui_add_child(gui_menu_item *parent, gui_menu_item *child) {
	child->parent = parent;
	if (parent == NULL) {
		return -1; // Invalid
	}
	if (parent->num_children == 0) {//Allocate memory for first child
		parent->children = (gui_menu_item**)malloc(sizeof(gui_menu_item*)); 
	} else {//Expand allocated memory to fit one more child
		parent->children = (gui_menu_item**)realloc(parent->children, (parent->num_children + 1) * sizeof(gui_menu_item*));
	}

	if (parent->children == NULL) {
		return -1; // Memory allocation error
	}

	parent->children[parent->num_children] = child;
	child->parent = parent;
	parent->num_children++;

	return 0;
	
}
int gui_destroy_all_children(gui_menu_item* parent) {
	if (parent == NULL || parent->num_children == 0) {
		return -1; // Nothing to destroy
	}

	// Recursively destroy each child
	for (int i = 0; i < parent->num_children; i++) {
		gui_menu_item* child = parent->children[i];
		gui_destroy_all_children(child); // Destroy child's children (if any)
		free(child);
		parent->children[i] = NULL;
	}

	// Free the children array
	free(parent->children);
	parent->children = NULL;
	parent->num_children = 0;
	return 0;
}

void menu_start_game_func() {
	
}
void menu_high_score_func() {
	
}
void menu_settings_func() {

}
void menu_calibrate_func() {

}
void menu_debugging_func() {
	
}

gui_menu_item *gui_init() {
	gui_menu_item *root = gui_menu_item_create("Root", NULL);
	gui_add_child(root, gui_menu_item_create("Start Game", NULL));
	gui_add_child(root, gui_menu_item_create("High Score", NULL));
	gui_add_child(root, gui_menu_item_create("Settings", NULL));
	
	//Should be an easier way to index the items in the array. Enum or define?
	//Setting
	gui_add_child(root->children[2], gui_menu_item_create("Calibrate", NULL));
	gui_add_child(root->children[2], gui_menu_item_create("Debugging", NULL));

	return root;
}

void gui_draw_menu(gui_menu_item *item, int8_t selected_item) {
	oled_reset(); //this should be done smarter, should only reset single pages, not all of them.
	for(int i = 0; i < item->num_children; i++) {
		oled_pos(i,0);
		if(i == selected_item) {
			oled_print(arrow);
		}
		oled_print_str(item->children[i]->text);
	}
}

void gui_goto_menu(gui_menu_item *gui_menu_current, int8_t selected_item, direction *joystick_dir_ptr, direction *prev_joystick_dir_ptr) {
	direction tmp_joystick_dir = * joystick_dir_ptr;
	direction tmp_joystick_dir_prev = * prev_joystick_dir_ptr;

	if(tmp_joystick_dir == DOWN && tmp_joystick_dir_prev == NEUTRAL) {
		selected_item += 1;
		if(selected_item >= gui_menu_current->num_children) selected_item = gui_menu_current->num_children-1; //Fix this, not effective
		gui_draw_menu(gui_menu_current, selected_item); //Should not draw entire menu for each selection. Clear and draw page should be enough
	}
	if(tmp_joystick_dir == UP && tmp_joystick_dir_prev == NEUTRAL) {
		selected_item -= 1;
		if(selected_item <= 0) selected_item = 0;			 //Fix this, not effective
		gui_draw_menu(gui_menu_current, selected_item);
	}
	//Go to selected child
	if(tmp_joystick_dir == RIGHT && tmp_joystick_dir_prev == NEUTRAL) {
		if(gui_menu_current->num_children != 0) { //To ensure next if does not select from an empty array
			if(gui_menu_current->children[selected_item]->num_children != 0) {
				gui_menu_current = gui_menu_current->children[selected_item];
				selected_item = 0;
				gui_draw_menu(gui_menu_current, selected_item);
			}
		}
	}
	//Go to parent
	if(tmp_joystick_dir == LEFT && tmp_joystick_dir_prev == NEUTRAL) {
		if(gui_menu_current->parent != NULL){ //Check for no parent
			gui_menu_current = gui_menu_current->parent;
			gui_draw_menu(gui_menu_current, selected_item);
			selected_item = 0;
		}
	}
	(*prev_joystick_dir_ptr) = tmp_joystick_dir;
}