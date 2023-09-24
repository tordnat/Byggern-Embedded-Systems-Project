#include "gui.h"


gui_menu_item *gui_menu_item_create(char *text, void (*click_action)(void), struct gui_menu_item_t *parent, struct gui_menu_item_t **children, uint8_t num_children) {
	gui_menu_item *item = (gui_menu_item*) malloc(sizeof(gui_menu_item));
	if(item) {
		if(strlen(text) > GUI_STRING_MAX_LENGTH) {
			return NULL;
		}
		strcpy(item->text, text);
		item->click_action = click_action;
		item->parent = parent;
		item->children = NULL;
		item->num_children = 0;
	}
	return item;
}
int gui_add_child(gui_menu_item *parent, gui_menu_item *child) {
	if (parent == NULL || child == NULL) {
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


gui_menu_item *gui_init() {
	gui_menu_item *root = gui_menu_item_create("Root", NULL, NULL, NULL, 0);
	gui_add_child(root, gui_menu_item_create("Start Game", NULL, root, NULL, 0));
	gui_add_child(root, gui_menu_item_create("High Score", NULL, root, NULL, 0));
	gui_add_child(root, gui_menu_item_create("Settings", NULL, root, NULL, 0));
	gui_add_child(root, gui_menu_item_create("Debugging", NULL, root, NULL, 0));

	return root;
}

void gui_draw_menu(gui_menu_item *item, uint8_t selected_item) {
	oled_reset(); //this should be done smarter
	oled_pos(0,0);
	int has_back_btn = 0;
	if(item->parent != NULL) { //Root, no need to print back button
		oled_print_str("Back");
		has_back_btn = 1;
	}
	for(int i = 0; i < item->num_children; i++) {
		
		oled_pos(i+has_back_btn,0);
		if(i == selected_item) {
			oled_print(arrow);
		}
		oled_print_str(item->children[i]->text);
	}
}