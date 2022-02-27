#include "confparser.hpp"
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

bool SimpleRule::compare(u16 a, u16 b){
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

SimpleRule::SimpleRule(u32 a, val_type ta, operand op, u32 b, val_type tb){
    this->val_a = a;
    this->val_b = b;

    this->ta = ta;
    this->tb = tb;

    this->op = op;
}

SimpleRule::~SimpleRule(){

}

u16 SimpleRule::get_val(GameBoy &gb, u32 val,  val_type t){
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

bool SimpleRule::check(GameBoy &gb){
    return this->compare(get_val(gb, val_a, ta), get_val(gb, val_b, tb));
}

static std::string rule_val_to_str(u32 val, Rule::val_type type){
    switch (type){
        case Rule::REG:
            return decode_reg((SharpSM83::reg_type) val);
        case Rule::MREG:
            return envolve(decode_reg((SharpSM83::reg_type) val));
        case Rule::NUM:
            return hex(val);
        case Rule::ADDR:
            return envolve(hex(val));
        default:
            return "";
    }
}

std::string SimpleRule::str(){
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

CompositeRule::CompositeRule(){

}

CompositeRule::CompositeRule(bool test){
    this->test = test;
}

CompositeRule::CompositeRule(SimpleRule r){
    this->rules.push_back(r);
}
CompositeRule::CompositeRule(std::vector<SimpleRule> v){
    this->rules = v;
}
CompositeRule::~CompositeRule(){

}

void CompositeRule::append(SimpleRule r){
    this->rules.push_back(r);
}

bool CompositeRule::check(GameBoy &gb){
    if (!enabled) return false;

    for (SimpleRule rule : this->rules)
        if (!rule.check(gb))
            return false;

    return true;
}

std::string CompositeRule::str(){
    size_t size = rules.size();
    std::string ret = "";

    for (size_t i = 0; i < size; i++){
        ret += rules[i].str();
        if (i < size - 1) ret += " && ";
    }

    return ret;
}

ConfParser::ConfParser(){

}

ConfParser::~ConfParser(){

}

static SharpSM83::reg_type parse_reg(std::string token){
    for (size_t i= 0; i < token.size(); i++)
        token[i] = toupper(token[i]);

    if (token == "A")
        return SharpSM83::RT_A;
    else if (token == "F")
        return SharpSM83::RT_F;
    else if (token == "B")
        return SharpSM83::RT_B;
    else if (token == "C")
        return SharpSM83::RT_C;
    else if (token == "D")
        return SharpSM83::RT_D;
    else if (token == "E")
        return SharpSM83::RT_E;
    else if (token == "H")
        return SharpSM83::RT_H;
    else if (token == "L")
        return SharpSM83::RT_L;
    else if (token == "AF")
        return SharpSM83::RT_AF;
    else if (token == "BC")
        return SharpSM83::RT_BC;
    else if (token == "DE")
        return SharpSM83::RT_BC;
    else if (token == "HL")
        return SharpSM83::RT_HL;
    else if (token == "SP")
        return SharpSM83::RT_SP;
    else if (token == "PC")
        return SharpSM83::RT_PC;

    return SharpSM83::RT_NONE;
}

static bool is_number(const std::string &str){
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

std::pair<u32, Rule::val_type> ConfParser::parse_token(std::string token){
    u32 val;
    Rule::val_type t;

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
        t = is_mem ? Rule::MREG: Rule::REG;
    }else{
        if (size > 2)
            if (token[0] == '0' && token[1] == 'x'){
                token.erase(0, 2);
                is_hex = true;
            }
        if (is_hex){
            val = stoi(token, 0, 16);
        }else{
            ASSERT(is_number(token), "Invalid token (%s)\n", token.c_str());
            val = stoi(token, 0, 10);
        }
        
        t = is_mem ? Rule::ADDR: Rule::NUM;
    }

    return std::pair<u32, Rule::val_type>(val, t);
}

static Rule::operand parse_op(std::string token){
    if (token == "==")
        return Rule::EQ;
    else if (token == ">=")
        return Rule::GE;
    else if (token == "<=")
        return Rule::LE;
    else if (token == ">")
        return Rule::GT;
    else if (token == "<")
        return Rule::LS;
    else if (token == "!=")
        return Rule::NE;

    return Rule::NONE;
}


#define SETFROMLINE(s, var) SETTING(line, s, var)
void ConfParser::parse_line(std::string line){
    std::string command;

    if (line == "") return;
    if (line[0] == '#') // comment
        return;

    if (line[0] == '$'){ // setting
        line.erase(line.begin());
        SETFROMLINE("CLEARTERM", clear_term);
        SETFROMLINE("INFO", info);

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
        CompositeRule rule;

        rule.test = is_test;
        is_test = false;

        do{
            line_stream >> tok1;
            line_stream >> tok2;
            line_stream >> tok3;


            std::pair<u32, Rule::val_type> a, b;
            a = parse_token(tok1);
            b = parse_token(tok3);

            Rule::operand op = parse_op(tok2);
            ASSERT(op != Rule::NONE, "Invalid operand");
             
            rule.append(SimpleRule(a.first, a.second, op, b.first, b.second));

            tok4 = "";
            line_stream >> tok4;
        }while(tok4 == "&&");
        this->rules.push_back(rule);
        return;
    }
    
    return;
}

void ConfParser::parse(const char *file_name){
    std::ifstream file(file_name);

    ASSERT(file, "Unable to open file: %s\n", file_name);

    for(std::string line;  getline(file, line );)
        parse_line(line);
}

bool ConfParser::check(GameBoy &gb, bool *test){
    if (test != NULL)
        *test = false;

    size_t size = rules.size();

    for (size_t i = 0; i < size; i++){
        if (rules[i].check(gb)){
            if (rules[i].test && test != NULL)
                *test = true;
            return true;
        }
    }
        
    return false;
}

#define PRINT_BOOL(var) printf("%s: %s\n", #var, var? "True": "False")

void ConfParser::print_info(){
    PRINT_BOOL(clear_term);
    PRINT_BOOL(info);
}

std::string ConfParser::list_rules(){
    std::stringstream ret("");

    size_t size = rules.size();
    for (size_t i = 0; i < size; i++){
        ret << '[' << i << "] ";

        if (rules[i].enabled)
            ret << rules[i].str();
        else
            ret << (char) GRAY_c << rules[i].str() << (char) RESET_c;

        if (i < size - 1) ret << '\n';
    }
    
    return ret.str();
}

void ConfParser::remove_rule(int i){
    this->rules.erase(rules.begin() + i);
}

void ConfParser::enable_rule(int i){
    this->rules[i].enabled = true;
}

void ConfParser::disable_rule(int i){
    this->rules[i].enabled = false;
}
