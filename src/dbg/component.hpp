#pragma once

#define BLUE_STR "\e[1;36m"
#define YELLOW_STR "\e[1;33m"
#define BOLD_STR "\e[1;37m"
#define GREEN_STR "\e[1;32m"
#define RESET_STR "\e[0m"

#include <iostream>
#include <string>
#include <vector>
#include <list>

#define NL 1 

enum color_c {
    INIT_c = 1,
    BLUE_c,
    YELLOW_c,
    BOLD_c,
    GREEN_c,
    RESET_c,

    COUNT_c,
};

class Component {
    public:
        bool visible = true;
        bool centered = false;
        Component(int x, int y, int width, int height);
        ~Component();
        int x, y, width, height;
        virtual std::vector<std::string> str() = 0;
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

        TitledTextBox &set_title(std::string);
        TitledTextBox &clear();
};

class ScrollingTextBox : public Component{
    protected:
        std::list<std::string> lines;
    public:
        ScrollingTextBox(int x, int y, int width, int height);
        ~ScrollingTextBox();

        ScrollingTextBox &operator<<(std::string);
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
