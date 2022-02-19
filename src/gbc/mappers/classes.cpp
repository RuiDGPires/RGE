#include "classes.hpp"

Mapper::Mapper(){}
Mapper::~Mapper(){}

MCB0::MCB0(){}
MCB0::~MCB0(){}
u16 MCB0::map_read(u16 addr){
    return addr;
}
u16 MCB0::map_write(u16 addr, u8 data){
    return addr;
}

MCB1::MCB1(){}
MCB1::~MCB1(){}
u16 MCB1::map_read(u16 addr){
    return addr;

}
u16 MCB1::map_write(u16 addr, u8 data){
    return addr;
}

MCB2::MCB2(){}
MCB2::~MCB2(){}
u16 MCB2::map_read(u16 addr){
    return 0;
}
u16 MCB2::map_write(u16 addr, u8 data){
    return 0;
}

MCB3::MCB3(){}
MCB3::~MCB3(){}
u16 MCB3::map_read(u16 addr){
    return 0;
}
u16 MCB3::map_write(u16 addr, u8 data){
    return 0;
}

MCB30::MCB30(){}
MCB30::~MCB30(){}
u16 MCB30::map_read(u16 addr){
    return 0;
}
u16 MCB30::map_write(u16 addr, u8 data){
    return 0;
}
