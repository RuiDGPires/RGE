#include "cpu.hpp"

SharpSM83::SharpSM83(){
    this->running = false;
}

SharpSM83::~SharpSM83(){

}

void SharpSM83::connect_bus(Bus *bus){
    this->bus = bus;
}

void SharpSM83::write(u16 addr, u8 data){
    this->bus->write(addr, data);
}

u8 SharpSM83::read(u16 addr){
    return this->bus->read(addr);
}


void SharpSM83::clock(){

}
