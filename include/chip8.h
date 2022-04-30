#ifndef _CHIP_8_
#define _CHIP_8_

#include <stdint.h>


// #define Window Size
#define SCREEN_HEIGHT   32
#define SCREEN_WIDTH    64

#define FONT_OFFSET 2


typedef struct{
    //HW
    uint8_t mem[0x1000];
    uint16_t stack[16];
    uint32_t *framebuffer;
    uint8_t k; // KEY buffer

    //registers
    // General Purpose Registers V0 to VF
    uint8_t v[0x10];
    // Address Register
    uint16_t i;
    // progrma Counter
    uint16_t pc;
    //Stack pointer
    uint8_t sp;
    //timer registers - active if nonzero. decrements with 60hz
    uint8_t dt; // delay timer, 
    uint8_t st; // sound timer, plays buzzer whiel nonzero
    
    // Emulator Flags
    uint8_t request_input;
    uint8_t drawFlag;

}chip8_t;





extern chip8_t *init_chip8(uint32_t * framebuffer);
void close_chip8(chip8_t * chip8);

void emulate_cycle(chip8_t * chip8);

void execute_opcode(chip8_t * chip8, uint16_t opcode);



#endif  