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

void Component::toggle_visible(){
    visible = !visible;
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

static std::string parse_color(color_c c){
    switch (c){
        case BLUE_c:
            return BLUE_STR;
        case YELLOW_c:
            return YELLOW_STR;
        case GREEN_c:
            return GREEN_STR;
        case BOLD_c:
            return BOLD_STR;
        case RESET_c:
            return RESET_STR;
        default:
            return "";
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
        int compensation = 0;
        line = BXD_VER;
        size_t line_size = tmp_lines[i].size();
        bool hide = false;
        for (int j = 0; j < this->width - 2 + compensation; j++){
            if (j >= line_size || hide){
                line.push_back(' ');
                continue;
            }
            char c = tmp_lines[i][j];

            if (c > INIT_c && c < COUNT_c){
                line.append(parse_color((color_c) c));
                compensation++;
                continue;
            }

            if (j >= width - 2) break;

            if (c == '\n') {hide = true; c = ' ';}
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

TextBox &TextBox::operator<<(std::vector<std::string> v){
    this->lines.insert(lines.end(), v.begin(), v.end());
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

TextBox &TextBox::operator<<(color_c c){
    return (*this) << (char) c;
}

std::string &TextBox::operator[](int n){
    return lines[n];
}

TextBox &TextBox::clear(){
    this->lines.clear();
    this->lines = std::vector<std::string>(this->height - 2, "");
    this->current_line = 0;
    return (*this);
}

TitledTextBox::TitledTextBox(std::string title, int x, int y, int w, int h) : TextBox(x, y, w, h){
        this->title = title;

    std::vector<std::string> v;
    int spaces = (this->width - 2- title.size())/2;
    if (spaces < 0) spaces = 0;

    v.push_back(std::string(spaces, ' ') + title + std::string(spaces, ' '));
    v.push_back(std::string(this->width - 2, '-'));
    
    v.insert(v.end(), this->lines.begin(), this->lines.end());
    this->lines = v;
    this->current_line = 2;
}

TitledTextBox::~TitledTextBox(){

}

TitledTextBox &TitledTextBox::set_title(std::string title){
    this->title = title;
    int spaces = (this->width - 2- title.size())/2;
    if (spaces < 0) spaces = 0;

    this->lines[0] = std::string(spaces, ' ') + title + std::string(spaces, ' ');

    return (*this);
}

TitledTextBox &TitledTextBox::clear(){
    lines.erase(lines.begin()+2, lines.end());
    std::vector<std::string> v = std::vector<std::string>(this->height - 4, "");
    lines.insert(lines.end(), v.begin(), v.end());

    this->current_line = 2;
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

Footer &Footer::clear(){
    this->buffer = "";
    return (*this);
}
