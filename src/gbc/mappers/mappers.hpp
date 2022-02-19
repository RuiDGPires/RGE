#pragma once
#include "../../common/defs.hpp"
#include "classes.hpp"
#include <memory>

enum MapperType{
    MT_ROM_ONLY = 0,
    MT_MBC1,
    MT_MBC1_RAM,
    MT_MBC1_RAM_BATTERY,

    MT_UNK0,

    MT_MBC2,
    MT_MBC2_BATTERY,
    MT_ROM_RAM,
    MT_ROM_RAM_BATTERY,

    MT_UNK1,

    MT_MMM01,
    MT_MMM01_RAM,
    MT_MMM01_RAM_BATTERY,

    MT_UNK2,

    MT_MBC3_TIMER_BATTERY,
    MT_MBC3_TIMER_RAM_BATTERY, // MCB0
    MT_MBC3,
    MT_MBC3_RAM, // MCB0
    MT_MBC3_RAM_BATTERY, // MCB0

    MT_UNK3,
    MT_UNK4,
    MT_UNK5,
    MT_UNK6,
    MT_UNK7,

    MT_MBC5,
    MT_MBC5_RAM,
    MT_MBC5_RAM_BATTERY,
    MT_MBC5_RUMBLE,
    MT_MBC5_RUMBLE_RAM,
    MT_MBC5_RUMBLE_RAM_BATTERY,

    MT_UNK8,

    MT_MBC6,

    MT_UNK9,

    MT_MBC7_SENSOR_RUMBLE_RAM_BATTERY,
};

std::shared_ptr<Mapper> get_mapper(MapperType id);
