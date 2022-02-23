#pragma once

#include <iostream>
#include <string>
#include <vector>

#define NL 1 

class Component {
    public:
        bool visible = true;
        Component(int x, int y, int width, int height);
        ~Component();
        int x, y, width, height;
        virtual std::vector<std::string> str() = 0;
        void toggle_visible();
};

class TextBox : public Component{
    protected:
        std::vector<std::string> lines;
        int current_line;
    public:
        TextBox(int x, int y, int width, int height);
        ~TextBox();


        std::vector<std::string> str() override;
        TextBox &operator<<(std::string);
        TextBox &operator<<(const char *);
        TextBox &operator<<(const char);
        
        TextBox &begin();
};

class TitledTextBox : public TextBox {
    protected:
        std::string title;

    public:
        TitledTextBox(std::string, int x, int y, int w, int h);
        ~TitledTextBox();

        TitledTextBox &set_title(std::string);
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
};
