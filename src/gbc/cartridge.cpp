#include "cartridge.hpp"
#include "mem_map.hpp"
#include <stdio.h>

// taken from https://github.com/rockytriton/LLD_gbemu/
static const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM",
    "ROM+RAM+BATTERY",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY", // MCB30
    "MBC3",
    "MBC3+RAM", // MCB30
    "MBC3+RAM+BATTERY", // MCB30
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LIC_CODE[0xA5] = {
    "None",
    "Nintendo R&D1",
    "Capcom",
    "Electronic Arts",
    "Hudson Soft",
    "b-ai",
    "kss",
    "pow",
    "PCM Complete",
    "san-x",
    "Kemco Japan",
    "seta",
    "Viacom",
    "Nintendo",
    "Bandai",
    "Ocean/Acclaim",
    "Konami",
    "Hector",
    "Taito",
    "Hudson",
    "Banpresto",
    "Ubi Soft",
    "Atlus",
    "Malibu",
    "angel",
    "Bullet-Proof",
    "irem",
    "Absolute",
    "Acclaim",
    "Activision",
    "American sammy",
    "Konami",
    "Hi tech entertainment",
    "LJN",
    "Matchbox",
    "Mattel",
    "Milton Bradley",
    "Titus",
    "Virgin",
    "LucasArts",
    "Ocean",
    "Electronic Arts",
    "Infogrames",
    "Interplay",
    "Broderbund",
    "sculptured",
    "sci",
    "THQ",
    "Accolade",
    "misawa",
    "lozc",
    "Tokuma Shoten Intermedia",
    "Tsukuda Original",
    "Chunsoft",
    "Video system",
    "Ocean/Acclaim",
    "Varie",
    "Yonezawa/s’pal",
    "Kaneko",
    "Pack in soft",
    "Konami (Yu-Gi-Oh!)"
};

size_t get_file_size(FILE *file){
  // Missing error checking
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

const char *Cartridge::lic_name() {
    if (this->header->new_lic_code <= 0xA4) {
        return LIC_CODE[this->header->lic_code];
    }

    return "UNKNOWN";
}

const char *Cartridge::type_name() {
    if (this->header->type <= 0x22) {
        return ROM_TYPES[this->header->type];
    }

    return "UNKNOWN";
}

Cartridge::Cartridge(const char *file_name){
    FILE *file = fopen(file_name, "rb");

    if (file == NULL){
        perror("Unable to open file\n");
        exit(1);
    }

    size_t file_size = get_file_size(file);

    rom = (u8 *) malloc(sizeof(u8) * file_size);
    this->size = file_size;

    fread(rom, 1, file_size, file);

    fclose(file);

    this->header = (rom_header *) (rom + 0x100);
    this->header->title[15] = 0;
    
    u16 x = 0;
    for (u16 i=0x0134; i<=0x014C; i++) {
        x = x - rom[i] - 1;
    }

    this->check_sum =  (x & 0xFF) != 0;

    this->mapper = get_mapper((MapperType) this->header->type);
}

Cartridge::Cartridge(std::string file_name) : Cartridge(file_name.c_str()){

}

Cartridge::~Cartridge(){
    free(rom);
    rom = NULL;
}

bool Cartridge::write(u16 addr, u8 data){
    u32 new_addr;
    bool rom = false;

    if (!this->mapper->map_write(addr, &new_addr, data, &rom))
        return false;
     
    if (!rom)
        this->ram[new_addr] = data;
    return true;
}

bool Cartridge::read(u16 addr, u8 *data){
    u32 new_addr;
    bool rom = false;

    if (!this->mapper->map_read(addr, &new_addr, &rom))
        return false;

    if (rom)
        *data = this->rom[new_addr];
    else
        *data = this->ram[new_addr];

    return true;
}

bool Cartridge::check(){
    return check_sum;
}

std::vector<std::string> Cartridge::info(){
    std::vector<std::string> vec;
    char buffer[50];
    sprintf(buffer, "Title    : %s", this->header->title);
    vec.push_back(std::string(buffer));
    sprintf(buffer, "Type     : %2.2X (%s)", this->header->type, this->type_name());
    vec.push_back(std::string(buffer));
    sprintf(buffer, "ROM Size : %d KB", 32 << this->header->rom_size);
    vec.push_back(std::string(buffer));
    sprintf(buffer, "RAM Size : %2.2X", this->header->ram_size);
    vec.push_back(std::string(buffer));
    sprintf(buffer, "LIC Code : %2.2X (%s)", this->header->lic_code, this->lic_name());
    vec.push_back(std::string(buffer));
    sprintf(buffer, "ROM Vers : %2.2X", this->header->version);
    vec.push_back(std::string(buffer));
    return vec;
}
