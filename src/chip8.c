#include <chip8.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const uint8_t CHIP8_FONT[80] = {
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6 
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8 
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8_t *init_chip8(void)
{
    chip8_t * ptr = (chip8_t *) calloc(1, sizeof(chip8_t));

    
    return ptr;
}



void close_chip8(chip8_t * chip8)
{
    free(chip8);
}

void emulate_cycle(chip8_t * chip8)
{
    uint16_t opcode = (chip8->mem[chip8->pc++] << 8);
    opcode  |= chip8->mem[chip8->pc++];
    execute_opcode(chip8, opcode);

}

void execute_opcode(chip8_t * chip8, uint16_t opcode)
{

    //switch on upper nibble
    switch ( opcode & 0xF000)
    {
        case 0x0000:
            switch ( opcode & 0x00FF)
            {
                case 0xE0:
                    memset(chip8->framebuffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH);
                    break;
                case 0xEE:
                    chip8->pc = chip8->stack[chip8->sp--];
                    break;
                default:
                    chip8->pc = opcode & 0x0FFF;
                    break;
            }
            break;
        case 0x1000:
            break;
        case 0x2000:
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            break;
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default:
            break;
    }












}