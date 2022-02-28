#include "conf.hpp"
#include "disassembly.hpp"
#include "../common/assert.hpp"
#include <fstream>
#include <sstream>
#include <string>

#define SETTING(tok, s, var){\
    if (tok == s){\
       var = true; \
        return; \
    }\
}

bool SimpleBreakpoint::compare(u16 a, u16 b){
    switch (this->op){
        case EQ:
            return a == b;
        case GT:
            return a > b;
        case LS:
            return a < b;
        case LE:
            return a <= b;
        case GE:
            return a >= b;
        case NE:
            return a != b;
        default:
            return false;
    }
}

SimpleBreakpoint::SimpleBreakpoint(u32 a, val_type ta, operand op, u32 b, val_type tb){
    this->val_a = a;
    this->val_b = b;

    this->ta = ta;
    this->tb = tb;

    this->op = op;
}

SimpleBreakpoint::~SimpleBreakpoint(){

}

u16 SimpleBreakpoint::get_val(GameBoy &gb, u32 val,  val_type t){
    switch (t){
        case REG:
            return gb.cpu.read_reg((SharpSM83::reg_type) val);
        case MREG:
            return gb.mem_bus.read(gb.cpu.read_reg((SharpSM83::reg_type) val));
        case NUM:
            return (u16) val;
        case ADDR:
            return gb.mem_bus.read(val);
        default:
            return 0;
    }
}

bool SimpleBreakpoint::check(GameBoy &gb){
    return this->compare(get_val(gb, val_a, ta), get_val(gb, val_b, tb));
}

static std::string rule_val_to_str(u32 val, Breakpoint::val_type type){
    switch (type){
        case Breakpoint::REG:
            return decode_reg((SharpSM83::reg_type) val);
        case Breakpoint::MREG:
            return envolve(decode_reg((SharpSM83::reg_type) val));
        case Breakpoint::NUM:
            return hex(val);
        case Breakpoint::ADDR:
            return envolve(hex(val));
        default:
            return "";
    }
}

std::string SimpleBreakpoint::str(){
    std::string ret = "";

    ret += rule_val_to_str(val_a, ta);
    ret += " ";
    
    switch(op){
        case EQ:
            ret += "== ";
            break;
        case GT:
            ret += "> ";
            break;
        case LS:
            ret += "< ";
            break;
        case LE:
            ret += "<= ";
            break;
        case GE:
            ret += ">= ";
            break;
        case NE:
            ret += "!= ";
            break;
        default:
            ASSERT(false, "Unkown operand");
            break;
    }
    ret += rule_val_to_str(val_b, tb);

    return ret;
}

CompositeBreakpoint::CompositeBreakpoint(){

}

CompositeBreakpoint::CompositeBreakpoint(bool test){
    this->test = test;
}

CompositeBreakpoint::~CompositeBreakpoint(){

}

void CompositeBreakpoint::append(SimpleBreakpoint r){
    this->breakpoints.push_back(r);
}

bool CompositeBreakpoint::check(GameBoy &gb){
    if (!enabled) return false;

    for (SimpleBreakpoint rule : this->breakpoints)
        if (!rule.check(gb))
            return false;

    return true;
}

std::string CompositeBreakpoint::str(){
    size_t size = breakpoints.size();
    std::string ret = "";

    for (size_t i = 0; i < size; i++){
        ret += breakpoints[i].str();
        if (i < size - 1) ret += " && ";
    }

    return ret;
}

Conf::Conf(){

}

Conf::~Conf(){

}

std::pair<u32, Breakpoint::val_type> Conf::parse_token(std::string token){
    u32 val;
    Breakpoint::val_type t;

    size_t size = token.size();
    bool is_mem = false;
    bool is_hex = false;

    if (token[0] == '('){
        ASSERT(token[size-1] == ')', "Invalid token\n");
        is_mem = true;
        token.erase(0, 1);
        token.pop_back();
        size -= 2; 
    }

    SharpSM83::reg_type rt = parse_reg(token);
    if (rt != SharpSM83::RT_NONE){
        val = (u32) rt;
        t = is_mem ? Breakpoint::MREG: Breakpoint::REG;
    }else{
        if (size > 2)
            if (token[0] == '0' && token[1] == 'x'){
                token.erase(0, 2);
                is_hex = true;
            }
        if (is_hex){
            val = stoi(token, 0, 16);
        }else{
            ASSERT(is_int(token), "Invalid token (%s)\n", token.c_str());
            val = stoi(token, 0, 10);
        }
        
        t = is_mem ? Breakpoint::ADDR: Breakpoint::NUM;
    }

    return std::pair<u32, Breakpoint::val_type>(val, t);
}

static Breakpoint::operand parse_op(std::string token){
    if (token == "==")
        return Breakpoint::EQ;
    else if (token == ">=")
        return Breakpoint::GE;
    else if (token == "<=")
        return Breakpoint::LE;
    else if (token == ">")
        return Breakpoint::GT;
    else if (token == "<")
        return Breakpoint::LS;
    else if (token == "!=")
        return Breakpoint::NE;

    return Breakpoint::NONE;
}


#define SETFROMLINE(s, var) SETTING(line, s, var)
void Conf::parse_line(std::string line){
    std::string command;

    if (line == "") return;
    if (line[0] == '#') // comment
        return;

    if (line[0] == '$'){ // setting
        line.erase(line.begin());
        SETFROMLINE("NOINFO", no_info);

        ASSERT(false, "Invalid setting: %s\n", line.c_str());
    }

    if (line[0] == '@'){ // decorator
        line.erase(line.begin());
        if (line == "test")
            is_test = true;
        else ASSERT(false, "Invalid decorator: %s\n", line.c_str());
        return;
    }

    std::stringstream line_stream(line);
    line_stream >> command;
    if (command == "b" || command == "break"){
        std::string tok1, tok2, tok3, tok4;
        CompositeBreakpoint rule;

        rule.test = is_test;
        is_test = false;

        do{
            line_stream >> tok1;
            line_stream >> tok2;
            line_stream >> tok3;


            std::pair<u32, Breakpoint::val_type> a, b;
            a = parse_token(tok1);
            b = parse_token(tok3);

            Breakpoint::operand op = parse_op(tok2);
            ASSERT(op != Breakpoint::NONE, "Invalid operand");
             
            rule.append(SimpleBreakpoint(a.first, a.second, op, b.first, b.second));

            tok4 = "";
            line_stream >> tok4;
        }while(tok4 == "&&");
        this->breakpoints.push_back(rule);
        return;
    }
    
    return;
}

void Conf::parse(const char *file_name){
    std::ifstream file(file_name);

    ASSERT(file, "Unable to open file: %s\n", file_name);

    for(std::string line;  getline(file, line );)
        parse_line(line);
}

bool Conf::check(GameBoy &gb, bool *test){
    if (test != NULL)
        *test = false;

    size_t size = breakpoints.size();

    for (size_t i = 0; i < size; i++){
        if (breakpoints[i].check(gb)){
            if (breakpoints[i].test && test != NULL)
                *test = true;
            return true;
        }
    }
        
    return false;
}

std::string Conf::list_breakpoints(){
    std::stringstream ret("");

    size_t size = breakpoints.size();
    for (size_t i = 0; i < size; i++){
        ret << '[' << i << "] ";

        if (breakpoints[i].enabled)
            ret << breakpoints[i].str();
        else
            ret << (char) GRAY_c << breakpoints[i].str() << (char) RESET_c;

        if (i < size - 1) ret << '\n';
    }
    
    return ret.str();
}

void Conf::remove_breakpoint(int i){
    if (i == -1) 
        this->breakpoints.clear();
    else
        this->breakpoints.erase(breakpoints.begin() + i);
}

void Conf::enable_breakpoint(int i){
    if (i == -1)
        for (size_t i = 0; i < breakpoints.size(); i++)
            this->breakpoints[i].enabled = true;
    else
        this->breakpoints[i].enabled = true;
}

void Conf::disable_breakpoint(int i){
    if (i == -1)
        for (size_t i = 0; i < breakpoints.size(); i++)
            this->breakpoints[i].enabled = false;
    else
        this->breakpoints[i].enabled = false;
}
