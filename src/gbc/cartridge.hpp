#pragma once

#include "../common/defs.hpp"
#include "mappers/mappers.hpp"
#include <vector>
#include <string>
#include <memory>

typedef struct {
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} rom_header;

class Cartridge{
#ifndef DEBUG
    private:
#else
    public:
#endif
        u32 size;
        u8 *rom;
        rom_header *header;

        std::shared_ptr<Mapper> mapper;
        std::vector<u8> ram;

        bool check_sum;

        const char *lic_name();
        const char *type_name();

    public:
        Cartridge(const char *);
        Cartridge(std::string);
        ~Cartridge();

        bool write(u16 addr, u8 data);
        bool read(u16 addr, u8 *data);

        bool check();

        void print_info();
};
