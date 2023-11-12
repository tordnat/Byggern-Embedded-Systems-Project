#include <stdint.h>


enum game_state_enum {
    NODE1_PLAYING,
    NODE3_PLAYING,
    STOPPED
};

/**
 * Main game loop. Takes in servo and solenoid pos to execute game logic
 * Updates score and stops game if ball hits wall
*/
void game_loop(int32_t servo_pos, uint8_t solenoid);

/**
 * Sets state to NODE1_PLAYING
*/
void game_set_node1_state();

/**
 * Sets state to NODE1_PLAYING
*/
void game_set_node3_state();

/**
 * Returns current state
*/
enum game_state_enum get_state();

/**
 * Stops game
 * Sets state to STOPPED
 * Sends stopped message
 * Resets score
*/
void game_stop();
