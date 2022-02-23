#pragma once

#include <iostream>
#include <string>
#include <vector>

#define NL 1 

class Component {
    public:
        Component(int x, int y, int width, int height);
        ~Component();
        int x, y, width, height;
        virtual std::vector<std::string> str() = 0;
};

class TextBox : public Component{
    private:
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

class Footer : public Component{
    private:
        std::string buffer;
    public:
        Footer(int x, int width);
        ~Footer();

        std::vector<std::string> str() override;
        Footer &operator<<(std::string);
        Footer &operator<<(const char *);
        Footer &operator<<(const char);
};
