/*********************************************//**
 * Components to display tui graphics
 * To be used in conjunction with screen.hpp
 * \file component.hpp
 * \brief Component class and it's inheritors description
 ************************************************/

#pragma once

#include "../common/defs.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

/*********************************************//**
 * \def NL
 * \brief Used for Textboxes to now when to break line, standard '\\n' overwrites lower lines
 ************************************************/
#define NL 1 


/*********************************************//**
 * \brief Base class for tui graphics on screen
 ************************************************/
class Component {
    public:
        /**
         * \brief Enum to describe a components docking
         * \todo implement docking 
         */
        enum CompDock{
            CDOCK_NONE,
            CDOCK_UP,
            CDOCK_DOWN,
            CDOCK_RIGHT,
            CDOCK_LEFT,
        };
        /**
         * Describes the visibility of the component
         */
        bool visible = true;

        /**
         * Describes if the component is centered on screen
         * This overrides the x position on component
         */
        bool centered = false;

        /**
         * Describe component's docking 
         */
        CompDock docking = CDOCK_NONE;

        /**
         * \brief Constructor
         * \param x The x coordinate of the top left corner of the component
         * \param y The y coordinate of the top left corner of the component
         * \param width The width of the component
         * \param height The height of the component
         */
        Component(int x, int y, int width, int height);
        /**
         * \brief Destructor
         */
        ~Component();

        /**
         * Basic properties of the component
         */
        int x, y, width, height;

        /**
         * Called when the screen is refreshed
         * \return Vector of strings, each element of the vector is a line on screen
         *
         * \brief Visual Representation
         */
        virtual std::vector<std::string> str() = 0;

        /**
         * \brief Toggle visibility
         */
        void toggle_visible();

};

/*********************************************//**
 * \brief Box with text and border
 ************************************************/
class TextBox : public Component{
    protected:
        /**
         * Vector of lines to be printed
         */
        std::vector<std::string> lines;
        /**
         * Used to overwrite lines
         */
        int current_line, p; 
    public:
        /**
         * \brief Constructor
         * \see Component
         */
        TextBox(int x, int y, int width, int height);
        /**
         * \brief Deconstructor
         */
        ~TextBox();

        std::vector<std::string> str() override;

        /**
         * Joins a string to the current working line, any '\\n' character will overwrite the line below. To move to a line under without overwriting, use NL
         * \see NL
         */
        TextBox &operator<<(std::string);
        /**
         * Appends a whole vector of lines
         * \see operator<<(std::string)
         */
        TextBox &operator<<(std::vector<std::string>);
        /**
         * \see operator<<(std::string)
         */
        TextBox &operator<<(const char *);
        /**
         * Append a character to the current working line
         * \see operator<<(std::string)
         */
        TextBox &operator<<(const char);
        /**
         * Append a color code to the line. Use this instead of ansi color codes
         * \see operator<<(const char)
         */
        TextBox &operator<<(color_c);
        /**
         * Access a line
         */
        std::string &operator[](int);

        /**
         * Clears all text from textbox, resets the working line
         */
        TextBox &clear();
};

/*********************************************//**
 * \brief Textbox with title
 ************************************************/
class TitledTextBox : public TextBox {
    protected:
        std::string title;

    public:
        /**
         * \brief Constructor
         * \see TextBox
         */
        TitledTextBox(std::string, int x, int y, int w, int h);
        ~TitledTextBox();

        std::vector<std::string> str();
        TitledTextBox &set_title(std::string);
};

class ScrollingTextBox : public Component{
        protected:
        /**
         * Vector of lines to be printed
         */
        std::list<std::string> lines;
    public:
        /**
         * \brief Constructor
         * \see Component
         */
        ScrollingTextBox(int x, int y, int width, int height);
        /**
         * \brief Destructor
         */
        ~ScrollingTextBox();

        /**
         * Joins a string to the current working line, to print a new line, start it with '\\n'
         */
        ScrollingTextBox &operator<<(std::string);
        /**
         * Appends a whole vector of lines
         * \see operator<<(std::string)
         */
        ScrollingTextBox &operator<<(std::vector<std::string>);
        /**
         * \see operator<<(std::string)
         */
        ScrollingTextBox &operator<<(const char *);
        /**
         * Append a character to the current working line
         * \see operator<<(std::string)
         */
        ScrollingTextBox &operator<<(const char);
        /**
         * Append a color code to the line. Use this instead of ansi color codes
         * \see operator<<(const char)
         */
        ScrollingTextBox &operator<<(color_c);

        std::vector<std::string> str() override;

        /**
         * Clears all text.
         */
        ScrollingTextBox &clear();
};

class Footer : public Component{
    protected:
        /**
         * Text to be printed
         */
        std::string buffer;
    public:
        /**
         * y is fixed to the lowest line on screen. Height is fixed to 1
         * \brief Constructor
         * \see Component
         */
        Footer(int x, int width);
        ~Footer();

        std::vector<std::string> str() override;
        /**
         * Joins a string to the footer. '\\n' are not recomended
         */
        Footer &operator<<(std::string);
        /**
         * \see operator<<(std::string)
         */
        Footer &operator<<(const char *);
        /**
         * Appends a character to the footer string
         * \see operator<<(std::string)
         */
        Footer &operator<<(const char);
        /**
         * Clears the text
         */
        Footer &clear();
};
