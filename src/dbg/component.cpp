#include "component.hpp"
#include "../common/assert.hpp"

Component::Component(int x, int y, int width, int height){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

}
Component::~Component(){

}

TextBox::TextBox(int x, int y, int width, int height) : Component(x, y, width, height){
    ASSERT(width > 2, "Width must be higher than 2");
    ASSERT(height > 2, "Height must be higher than 2");
    this->lines = std::vector<std::string>(height - 2); // -2 because of borders
    current_line = 0;
}


#define BXD_BOTR "┘"
#define BXD_BOTL "└"
#define BXD_TOPR "┐"
#define BXD_TOPL "┌"
#define BXD_HOR "─"
#define BXD_VER "│"

TextBox::~TextBox(){

}

static void arranje_lines(std::vector<std::string> &vec){
    size_t size = vec.size();

    int writing;
    for (int i = size - 1; i >= 0; i--){
        writing = 0;
        size_t line_size = vec[i].size();
        int p = 0;
        for (int j = 0; j < line_size; j++){
            if (i + writing >= size) break;
            char &c = vec[i][j];
            if (c == '\n') {writing += 1; p = 0; continue;}


            if (writing)
                vec[i+writing][p++] = c;
        }
    }
}

std::vector<std::string> TextBox::str(){
    std::vector<std::string> ret;
    std::vector<std::string> tmp_lines = this->lines;

    arranje_lines(tmp_lines); // Fix \n's 

    std::string line = "";
    line += BXD_TOPL;
    for (int i = 0; i < this->width - 2; i++)
        line += BXD_HOR;
    line += BXD_TOPR;

    ret.push_back(line);
    line = "";

    for (int i = 0; i < this->height - 2; i++){
        line = BXD_VER;
        size_t line_size = tmp_lines[i].size();
        bool hide = false;
        for (int j = 0; j < this->width - 2; j++){
            if (j >= line_size || hide){
                line.push_back(' ');
                continue;
            }
            if (j >= width - 2) break;

            char c = tmp_lines[i][j];
            if (c == '\n') {hide = true; c = ' '; continue;}
            line.push_back(c);
        }
        line += BXD_VER;
        ret.push_back(line);
    }

    //-----------
    // BOX LIMITS
    line = BXD_BOTL;
    for (int i = 0; i < this->width - 2; i++)
        line += BXD_HOR;
    line += BXD_BOTR;
    ret.push_back(line);
    //-----------
    return ret;
}

TextBox &TextBox::operator<<(std::string s){
    this->lines[current_line].append(s);
    return *this;
}

TextBox &TextBox::operator<<(const char *s){
    return (*this) << std::string(s);
}
TextBox &TextBox::operator<<(const char c){
    if (c == NL)
        this->current_line++;
    else
        this->lines[current_line].push_back(c);
    return *this;
}

TextBox &TextBox::begin(){
    current_line = 0;
    return (*this);
}

Footer::Footer(int x, int width) : Component(x, -1, width, 1){

};

Footer::~Footer(){}

std::vector<std::string> Footer::str(){
    return std::vector<std::string>(1, buffer);
}

Footer &Footer::operator<<(std::string s){
    this->buffer.append(s);
    return (*this);
}

Footer &Footer::operator<<(const char *s){
    this->buffer.append(s);
    return (*this);
}

Footer &Footer::operator<<(const char c){
    this->buffer.push_back(c);
    return (*this);
}
