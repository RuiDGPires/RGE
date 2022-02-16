#include "confparser.hpp"
#include "../common/assert.hpp"
#include <fstream>
#include <sstream>
#include <string>

bool Rule::compare(u16 a, u16 b){
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

Rule::Rule(u32 a, val_type ta, operand op, u32 b, val_type tb){
    this->val_a = a;
    this->val_b = b;

    this->ta = ta;
    this->tb = tb;

    this->op = op;
}

Rule::~Rule(){

}

u16 Rule::get_val(GameBoy &gb, u32 val,  val_type t){
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

bool Rule::check(GameBoy &gb){
    return this->compare(get_val(gb, val_a, ta), get_val(gb, val_b, tb));
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

std::pair<u32, Rule::val_type> ConfParser::parse_token(std::string token, size_t line){
    u32 val;
    Rule::val_type t;

    size_t size = token.size();
    bool is_mem = false;
    bool is_hex = false;

    if (token[0] == '('){
        ASSERT(token[size-1] == ')', "Invalid token at line %lu\n", line);
        is_mem = true;
        token.erase(size-1);
        token.erase(0);
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
            ASSERT(is_number(token), "Invalid token (%s) at line %lu\n", token.c_str(), line);
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


void ConfParser::parse(const char *file_name){
    std::ifstream file(file_name);

    size_t line_number = 1;
    for(std::string line; getline(file, line ); line_number++){
        std::string tok1, tok2, tok3;

        std::stringstream line_stream(line);

        line_stream >> tok1;
        if (tok1[0] == '#') // comment
            continue;

        line_stream >> tok2;
        line_stream >> tok3;

        std::pair<u32, Rule::val_type> a, b;
        a = parse_token(tok1, line_number);
        b = parse_token(tok3, line_number);

        Rule::operand op = parse_op(tok2);
        ASSERT(op != Rule::NONE, "Invalid operand");
         
        this->rules.push_back(Rule(a.first, a.second, op, b.first, b.second));
    }
}

bool ConfParser::check(GameBoy &gb){
    size_t size = rules.size();

    for (size_t i = 0; i < size; i++)
        if (rules[i].check(gb)) return true;
        
    return false;
}
