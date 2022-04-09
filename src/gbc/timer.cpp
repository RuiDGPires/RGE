#include "timer.hpp"

Timer::Timer(){
    this->div = 0xAC00;
}
Timer::~Timer(){
}

void Timer::tick(){
    u16 previous = div;

    div++;

    bool update = false;

    switch(tac & (0b11)) {
        case 0b00:
            update = (previous & (1 << 9))  && (!(div & (1 << 9)));
            break;
        case 0b01:
            update = (previous & (1 << 3)) && (!(div & (1 << 3)));
            break;
        case 0b10:
            update = (previous & (1 << 5)) && (!(div & (1 << 5)));
            break;
        case 0b11:
            update = (previous & (1 << 7)) && (!(div & (1 << 7)));
            break;
    }

    if (update && tac & (1 << 2)) {
        tima++;

        if (tima == 0xFF) {
            tima = tma;

            cpu->request_interrupt(SharpSM83::IT_TIMER);
        }
    }
}

void Timer::write(u16 address, u8 value) {
    switch(address) {
        case 0xFF04:
            //DIV
            div = 0;
            break;

        case 0xFF05:
            //TIMA
            tima = value;
            break;

        case 0xFF06:
            //TMA
            tma = value;
            break;

        case 0xFF07:
            //TAC
            tac = value;
            break;
    }
}

u8 Timer::read(u16 address) {
    switch(address) {
        case 0xFF04:
            return div >> 8;
        case 0xFF05:
            return tima;
        case 0xFF06:
            return tma;
        case 0xFF07:
            return tac;
    }
    return 0;
}

void Timer::connect(SharpSM83 *cpu){
    this->cpu = cpu;
}
