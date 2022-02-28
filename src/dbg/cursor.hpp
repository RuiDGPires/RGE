/*********************************************//**
 * \file cursor.hpp
 * \brief Cursor movement functions
 ************************************************/
#pragma once

#include <string>

#define CURSOR_UP "\033[1A"
#define CURSOR_DOWN "\033[1B"
#define CURSOR_RIGHT "\033[1C"
#define CURSOR_LEFT "\033[1D"


/*********************************************//**
 * \brief String to move cursor to a position
 * \return String to print to stdout
 ************************************************/
std::string cursor_pos_str(int x, int y);

/*********************************************//**
 * \brief Move cursor up
 * \param n Number of steps taken 
 ************************************************/
void cursor_up(int n = 1);
/*********************************************//**
 * \brief Move cursor down
 * \param n Number of steps taken 
 ************************************************/
void cursor_down(int n = 1);
/*********************************************//**
 * \brief Move cursor left
 * \param n Number of steps taken 
 ************************************************/
void cursor_left(int n = 1);
/*********************************************//**
 * \brief Move cursor right
 * \param n Number of steps taken 
 ************************************************/
void cursor_right(int n = 1);
/*********************************************//**
 * \brief Move cursor to a position
 * \param x Coordinate (column), starts at 0
 * \param y Coordinate (row), starts at 0
 ************************************************/
void cursor_pos(int x, int y);
