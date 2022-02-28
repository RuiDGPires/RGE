/*********************************************//**
 * \file input.hpp
 * \brief Input system
 ************************************************/
#pragma once
#include <vector>

/**
 * Describe the function that will be called when key is pressed.
 * Key needs to be enabled for this to work
 * \see ENABLE_KEY
 */
#define EVENT(key) void ON_##key()

/**
 * Enable the action of key.
 * It's event needs to be decribed.
 * \see EVENT
 * \see bind_key
 */
#define ENABLE_KEY(key) bind_key(key, ON_##key)

/**
 * Key codes
 */
enum Key{
    K_ARROW_UP = 0,
    K_ARROW_DOWN,
    K_ARROW_LEFT,
    K_ARROW_RIGHT,
    K_CTRL_ARROW_UP,
    K_CTRL_ARROW_DOWN,
    K_CTRL_ARROW_LEFT,
    K_CTRL_ARROW_RIGHT,

    K_SPACE,
    K_EXC,
    K_DQTS,
    K_CARD,
    K_DOLLAR,
    K_PERC,
    K_AND,
    K_SQT,
    K_LPAREN,
    K_RPAREN,
    K_AST,
    K_PLUS,
    K_COMMA,
    K_MINUS,
    K_DOT,
    K_RBAR,
    K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
    K_CLN,
    K_SEMICLN,
    K_LS,
    K_GT,
    K_EQ,
    K_QUE,
    K_AT,

    K_SHFT_A, K_SHFT_B, K_SHFT_C, K_SHFT_D, K_SHFT_E, K_SHFT_F, K_SHFT_G, K_SHFT_H, K_SHFT_I, K_SHFT_J, K_SHFT_K, K_SHFT_L, K_SHFT_M, K_SHFT_N, K_SHFT_O, K_SHFT_P, K_SHFT_Q, K_SHFT_R, K_SHFT_S, K_SHFT_T, K_SHFT_U, K_SHFT_V, K_SHFT_W, K_SHFT_X, K_SHFT_Y, K_SHFT_Z,

    K_LSQR,
    K_LBAR,
    K_RSQR,
    K_HAT,
    K_UNDER,
    K_LAC,
    K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I, K_J, K_K, K_L, K_M, K_N, K_O, K_P, K_Q, K_R, K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,

    K_LBRAQ,
    K_BAR,
    K_RBRAQ,
    K_TIL,

    K_CTRL_A, K_CTRL_B, K_CTRL_C, K_CTRL_D, K_CTRL_E, K_CTRL_F, K_CTRL_G, K_CTRL_H, K_CTRL_I, K_CTRL_J, K_CTRL_K, K_CTRL_L, K_CTRL_M, K_CTRL_N, K_CTRL_O, K_CTRL_P, K_CTRL_Q, K_CTRL_R, K_CTRL_S, K_CTRL_T, K_CTRL_U, K_CTRL_V, K_CTRL_W, K_CTRL_X, K_CTRL_Y, K_CTRL_Z,


    K_DEL,
    K_ENTER,

    K_ESC,
    K_BCKSPACE,

    K_COUNT, // DO NOT CHAGNE
    K_NONE,
};

/**
 * Clears all key events, should be used at the beggining of the main function
 */
void setup_events();
/**
 * Waits for keyboard input and executes corresponding event, if defined and enabled
 */
void wait_input();
/**
 * Waits for keyboard input and returns the pressed key
 * \param non_block If set to true, skips if stdin is empty
 * \return keycode of the pressed key, K_NONE if stdin was empty and non_block was set
 */
Key get_key(bool non_block = false);
/**
 * if key is a letter, number of symbol, returns it's ascii character code, else, returns 0
 * \param key
 * \return ascii character if key is letter, number or symbol, 0 otherwise
 */
char key_to_ascii(Key key);

/**
 * Bind key to corresponding action when pressed.
 * \param key keycode of key that will execute the function
 * \param f function that will be executed when key is pressed
 */
void bind_key(Key key, void (*f)(void));
