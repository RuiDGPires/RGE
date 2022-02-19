#include "mappers.hpp"

std::shared_ptr<Mapper> get_mapper(MapperType id){
    switch(id){

        //MCB0
        case MT_ROM_ONLY:
        default: // Unkown
            return std::make_shared<MCB0>();

        //MCB1
        case MT_MBC1:
        case MT_MBC1_RAM:
        case MT_MBC1_RAM_BATTERY:
            return std::make_shared<MCB1>();

        //MCB2
        case MT_MBC2:
        case MT_MBC2_BATTERY:
            return std::make_shared<MCB2>();

        //MCB3
        case MT_MBC3:
        case MT_MBC3_TIMER_BATTERY:
            return std::make_shared<MCB3>();

        //MCB30
        case MT_MBC3_TIMER_RAM_BATTERY:
        case MT_MBC3_RAM:
        case MT_MBC3_RAM_BATTERY:
            return std::make_shared<MCB3>();
    }
}
