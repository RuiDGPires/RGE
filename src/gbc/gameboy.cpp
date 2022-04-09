#include "gameboy.hpp"
#include <iostream>

GameBoy::GameBoy(){
    this->slot = NULL;
    this->cpu.connect_bus(&mem_bus);
    this->mem_bus.connect(&cpu);
    this->mem_bus.connect(&joypad);
    this->mem_bus.connect(&timer);
}

GameBoy::~GameBoy(){
    if (this->slot != NULL){
        delete this->slot;
        this->slot = NULL;        
    }
}

void GameBoy::load_rom(const char *file_name){
    if (this->slot != NULL){
        delete this->slot;
        this->slot = NULL;
    }

    this->slot = new Cartridge(file_name);

    if (!(this->slot->check())){
        std::cout << "INVALID ROM\n";
        delete this->slot;
        this->slot = NULL;
    }else{
        this->mem_bus.connect(this->slot);
    }
}

void GameBoy::turn_on(){
    if (slot == NULL){
        std::cout << "NO ROM INSERTED\n";
    }
}

void GameBoy::turn_off(){

}

void GameBoy::clock(){
    do{
        for (int i = 0; i < 4; i++)
            timer.tick();
        cpu.clock();
    }while(cpu.cycles != 0 && debug_mode);
}

void GameBoy::print_cartridge_info(){
    std::vector<std::string> info = this->slot->info();

    for (size_t i = 0; i < info.size(); i++)
        std::cout << info[i] << '\n';
}
