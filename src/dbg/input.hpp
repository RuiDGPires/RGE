#pragma once
#include <vector>

#define EVENT(key) void ON_##key()

#define ENABLE_KEY(key) bind_key(key, ON_##key)

enum special_flg{
    FLG_CTRL = 1 << 31,
    FLG_SHFT = 1 << 30,
};

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

    K_COUNT, // DO NOT CHAGNE
    K_NONE,
};

void setup_events();
void wait_input();
Key get_key();

void bind_key(Key key, void (*f)(void));

