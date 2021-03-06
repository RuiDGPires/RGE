//********************************************
// ONLY ONE SCREEN MAY BE UP AT A GIVEN TIME
//********************************************
#include "screen.hpp"
#include "cursor.hpp"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <csignal>


static Screen *_screen_;
//*********************************
// TERMIOS MAGIC
#include <termios.h>
static struct termios saved_attributes;

static void reset_input_mode (void){
    std::cout << "\e[?25h";
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

static void set_input_mode (void){
    struct termios tattr;

/* Make sure stdin is a terminal. */
    if (!isatty (STDIN_FILENO))
    {
        fprintf (stderr, "Not a terminal.\n");
        exit (EXIT_FAILURE);
    }

/* Save the terminal attributes so we can restore them later. */
    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit (reset_input_mode);

/* Set the funny terminal modes. */
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);	/* Clear ICANON and ECHO. */
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);

    std::cout << "\e[?25l"; // Hide cursor
                          //
                          // "\e[?25h" to show cursor
}
//******************

Screen::Screen(){
    _screen_ = this;
    set_input_mode();
    update_term_size();
    std::signal(SIGWINCH, Screen::sig_handler);
}

Screen::~Screen(){
    std::cout   << "\033[2J" // Clear screen
                << "\033[H"; // Move cursor to home
    reset_input_mode();
}

void Screen::sig_handler(int sig){
    (void) sig;
    _screen_->update_term_size();
}

void Screen::update_term_size(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 

    term_w = w.ws_col;
    term_h = w.ws_row;
    this->refresh();
}

void Screen::refresh(){
    std::cout << "\033[2J"; // Clear screen
    for (int i = 0; i < components.size(); i++){
        if (!(components[i]->visible)) continue;

        int x;
        if (components[i]->centered)
            x = term_w/2 - components[i]->width/2;
        else
            x = components[i]->x;

        if (components[i]->docking == Component::CDOCK_RIGHT)
            components[i]->width = term_w - components[i]->x; 

        int y = components[i]->y;

        std::vector<std::string> lines = components[i]->str();

        if (y == -1){
            y = term_h - lines.size() + 1;
        }

        for (std::string line : lines){
            cursor_pos(x, y++);
            std::cout << line;
        }
    }

    fflush(stdout);
}

Screen &Screen::operator<<(Component &comp){
    this->components.push_back(&comp);
    return (*this);
}
