#include "component.hpp"
#include "../common/assert.hpp"
#include "../common/defs.hpp"

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

            if (writing && writing + i < size){
                size_t line_size = vec[i+writing].size();

                if (p < line_size)
                    vec[i+writing][p] = c;
                else
                    vec[i+writing].push_back(c);
                p++;
            }
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
        case GRAY_c:
            return GRAY_STR;
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
        int line_size = tmp_lines[i].size();
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

            if (j >= width - 2 + compensation) break;

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
    for (std::string s : v)
        *this << s << NL;
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
    this->set_title(title);
}

TitledTextBox::~TitledTextBox(){

}

TitledTextBox &TitledTextBox::set_title(std::string title){
    this->title = title;
    return (*this);
}

std::vector<std::string> TitledTextBox::str(){
    std::vector<std::string> ret = TextBox::str();

    size_t size = this->title.size();
    int begin = (this->width - 2 - size)/2;

    int hor_line_size = std::string().size();

    std::string line = "";
    line += BXD_TOPL;
    for (int i = 0; i < this->width - 2; i++)
        if (i >= begin && i < begin+size)
            line += title[i-begin];
        else
            line += BXD_HOR;
    line += BXD_TOPR;

    for (size_t i = 0; i < size; i++)
        ret[0] = line; 

    return ret;
}

ScrollingTextBox::ScrollingTextBox(int x, int y, int width, int height) : Component(x, y, width, height){
    ASSERT(width > 2, "Width must be higher than 2");
    ASSERT(height > 2, "Height must be higher than 2");
    this->lines = std::list<std::string>(this->height - 2);
}

ScrollingTextBox::~ScrollingTextBox(){

}

ScrollingTextBox &ScrollingTextBox::clear(){
    this->lines = std::list<std::string>(this->height - 2);
    return (*this);
}

ScrollingTextBox &ScrollingTextBox::operator<<(std::string s){
    std::string line;

    if (lines.size() > 0){
        line = lines.front(); 
        lines.pop_front();
    }

    for (char c : s){
        if (c == '\n' || line.size() > this->width - 2){
            this->lines.push_front(line);
            line = "";
        }
        if (c != '\n') line.push_back(c);
    }
    this->lines.push_front(line);
   
   if (this->lines.size() > this->height - 2)
       this->lines.resize(this->height-2);

    return (*this);
}

ScrollingTextBox &ScrollingTextBox::operator<<(const char *s){
    return (*this) << std::string(s);
}

ScrollingTextBox &ScrollingTextBox::operator<<(std::vector<std::string> v){
    size_t size = v.size();
    for (size_t i = 0; i < size; i++)
        (*this) << "\n" << v[i];
    return (*this);
}

ScrollingTextBox &ScrollingTextBox::operator<<(const char c){
    std::string line;

    if (lines.size() > 0){
        line = lines.front(); 
        lines.pop_front();
    }

    // THIS HAS SOME ISSUES WHEN COLORS ARE INVOLVED...
    if (c == '\n' || line.size() > this->width - 2){
        this->lines.push_front(line);
        line = "";
        if (c != '\n') line.push_back(c);
    }

    this->lines.push_front(line);
   
    if (this->lines.size() > this->height - 2)
        this->lines.resize(this->height-2);

    return (*this);
}

ScrollingTextBox &ScrollingTextBox::operator<<(color_c c){
    lines.front().push_back(c);
    return (*this);
}

std::vector<std::string> ScrollingTextBox::str(){
    std::vector<std::string> ret;
    std::vector<std::string> tmp_lines(lines.begin(), lines.end());

    std::string line = "";
    line += BXD_TOPL;
    for (int i = 0; i < this->width - 2; i++)
        line += BXD_HOR;
    line += BXD_TOPR;

    ret.push_back(line);
    line = "";

    for (int i = this->height - 3; i >= 0; i--){
        int compensation = 0;
        line = BXD_VER;
        int line_size = tmp_lines[i].size();
        for (int j = 0; j < this->width - 2 + compensation; j++){
            if (j >= line_size){
                line.push_back(' ');
                continue;
            }
            char c = tmp_lines[i][j];

            if (c > INIT_c && c < COUNT_c){
                line.append(parse_color((color_c) c));
                compensation++;
                continue;
            }

            if (j >= width - 2 + compensation) break;

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
