#pragma once
#include "../common/defs.hpp"

// 0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01~NN	From cartridge, switchable bank via mapper (if any)
// 8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
// A000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
// C000	CFFF	4 KiB Work RAM (WRAM)	
// D000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1~7
// E000	FDFF	Mirror of C000~DDFF (ECHO RAM)	Nintendo says use of this area is prohibited.
// FE00	FE9F	Sprite attribute table (OAM)	
// FEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited
// FF00	FF7F	I/O Registers	
// FF80	FFFE	High RAM (HRAM)	
// FFFF	FFFF	Interrupt Enable register (IE)	

#define ROM0_BEGIN 0x0000	
#define ROM0_END 0x3FFF

#define ROM1_BEGIN 0x4000
#define ROM1_END 0x7FFF

#define VRA_BEGIN 0x8000
#define VRA_END 0x9FFF

#define SRA_BEGIN 0xA000
#define SRA_END 0xBFFF

#define WRA0_BEGIN 0xC000
#define WRA0_END 0xCFFF

#define WRA1_BEGIN 0xD000
#define WRA1_END 0xDFFF

#define ECH_BEGIN 0xE000
#define ECH_END 0xFDFF

#define OAM_BEGIN 0xFE00
#define OAM_END 0xFE9F

#define IO_BEGIN 0xFF00
#define IO_END 0xFF7F

#define HRA_BEGIN 0xFF80
#define HRA_END 0xFFFE

#define IE_BEGIN 0xFFFF
#define IE_END 0xFFFF

#define WRA0_SIZE WRA0_END - WRA0_BEGIN + 1
#define WRA1_SIZE WRA1_END - WRA1_BEGIN + 1
#define HRA_SIZE HRA_END - HRA_BEGIN + 1
#define IO_SIZE IO_END - IO_BEGIN + 1

const char *get_mem_name(u16 addr);
