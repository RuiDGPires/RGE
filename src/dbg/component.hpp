/*------------------------------------
 * Components to display tui graphics
 * To be used in conjunction with screen.hpp
 * --------------
 *  Component
 *   /\   /\
 *   ||   ||
 *   ||  TextBox - Empty Box with borders. Lines larger than the width don't break to lower lines
 *   ||   /\
 *   ||   ||
 *   ||  TitledTextBox - A TextBox with title
 *   |\_____
 *   | ____ \
 *   ||    ||
 *   ||  ScrollingTextBox - A scrolling 
 *   ||
 *   Footer
 */
/*********************************************//**
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
 * \brief NL is used for Textboxes to now when to break line, standard \n overwrites lower lines
 ************************************************/
#define NL 1 


/*********************************************//**
 * \brief Base class for tui graphics on screen
 ************************************************/
class Component {
    public:
        /**
         * \brief Enum to describe a components docking
         * TODO 
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
         * TODO
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
         * Visual representation of the component, to be called when the screen is refreshed
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

class TextBox : public Component{
    protected:
        std::vector<std::string> lines;
        int current_line, p; // Used for text input
    public:
        TextBox(int x, int y, int width, int height);
        ~TextBox();

        std::vector<std::string> str() override;
        TextBox &operator<<(std::string);
        TextBox &operator<<(std::vector<std::string>);
        TextBox &operator<<(const char *);
        TextBox &operator<<(const char);
        TextBox &operator<<(color_c);
        std::string &operator[](int);

        TextBox &clear();
};

class TitledTextBox : public TextBox {
    protected:
        std::string title;

    public:
        TitledTextBox(std::string, int x, int y, int w, int h);
        ~TitledTextBox();

        std::vector<std::string> str();
        TitledTextBox &set_title(std::string);
};

class ScrollingTextBox : public Component{
    protected:
        std::list<std::string> lines;
    public:
        ScrollingTextBox(int x, int y, int width, int height);
        ~ScrollingTextBox();

        ScrollingTextBox &operator<<(std::string);
        ScrollingTextBox &operator<<(std::vector<std::string>);
        ScrollingTextBox &operator<<(const char *);
        ScrollingTextBox &operator<<(const char);
        ScrollingTextBox &operator<<(color_c);

        std::vector<std::string> str() override;
        ScrollingTextBox &clear();
};

class Footer : public Component{
    protected:
        std::string buffer;
    public:
        Footer(int x, int width);
        ~Footer();

        std::vector<std::string> str() override;
        Footer &operator<<(std::string);
        Footer &operator<<(const char *);
        Footer &operator<<(const char);
        Footer &clear();
};
