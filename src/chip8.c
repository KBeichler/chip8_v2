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
    memcpy(ptr->mem, CHIP8_FONT, 80);
    ptr->pc = 0x200;

    
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

    uint8_t idx = (opcode & 0xF00) >> 8;
    uint8_t idy = (opcode & 0x0F0) >> 4;

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
            chip8->pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            chip8->stack[ ++(chip8->sp) ] = chip8->pc;
            chip8->pc = opcode & 0x0FFF;
            break;
        case 0x3000:      
            if ( chip8->v[ idx ] == (opcode & 0xFF) ) chip8->pc += 2;
            break;
        case 0x4000:     
            if ( chip8->v[ idx ] != (opcode & 0xFF) ) chip8->pc += 2;
            break;
        case 0x5000:
            if ( chip8->v[ idx ] == chip8->v[ idy ] ) chip8->pc += 2;
            break;
        case 0x6000:
            chip8->v[idx] = opcode & 0x00FF;
            break;
        case 0x7000:
            chip8->v[idx] += opcode & 0x00FF;
            break;
        case 0x8000: // Logic and Arithemtic
            switch (opcode & 0x000F)
            {
                case 0x0: // LD Vx Vy
                    chip8->v[idx] = chip8->v[idy];
                    break;
                case 0x1: // OR Vx Vy
                    chip8->v[idx] |= chip8->v[idy];
                    break;
                case 0x2: // AND Vx Vy
                    chip8->v[idx] &= chip8->v[idy];
                    break;
                case 0x3: // XOr Vx Vy
                    chip8->v[idx] ^= chip8->v[idy];
                    break;
                case 0x4: // ADD Vx Vy -> carry VF
                    chip8->v[0xF] = (chip8->v[idx] + chip8->v[idy]) > 0xFF;
                    chip8->v[idx] += chip8->v[idy];
                    break;
                case 0x5: // SUB Vx Vy - VF = not Borrow
                    chip8->v[0xF] = (chip8->v[idx] > chip8->v[idy]);
                    chip8->v[idx] -= chip8->v[idy];
                    break;
                case 0x6: // SHR Vx ( div by 2 ) -> VF = LSB
                    chip8->v[0xF] = chip8->v[idx] & 0x1;
                    chip8->v[idx] >>= 1;
                    break;
                case 0x7: // SUBN Vy Vx - VF = not Borrow
                    chip8->v[0xF] = (chip8->v[idx] < chip8->v[idy]);
                    chip8->v[idx] = chip8->v[idy] - chip8->v[idx];
                    break;
                case 0xE: // SHL Vx ( * 2 ) - VF = MSB
                    chip8->v[0xF] = (chip8->v[idx] & 0x80) != 0;
                    chip8->v[idx] <<= 1;
                    break;
                default: 
                    // error
                    break;
            }            
            break;
        case 0x9000: // SNE
            if ( chip8->v[ idx ] != chip8->v[ idy ] ) chip8->pc += 2;
            break;
        case 0xA000: // LD I
            chip8->i = opcode & 0x0FFF;
            break;
        case 0xB000: // JP V0, addr
            chip8->pc = (uint16_t) ( (opcode & 0x0FFF) + chip8->v[0] );
            break;
        case 0xC000: // RND Vx & byte
            {
                uint8_t num = (uint8_t) rand();
                chip8->v[idx] = num & (opcode & 0xFF);
            }
            break;
        case 0xD000: // DRW
            break;
        case 0xE000: // SKIP IF KEY
            switch (opcode & 0x00FF)
            {
                case 0x9E:
                    if (chip8->v[idx] == chip8->k) chip8->pc += 2;
                    break;
                case 0xA1:
                    if (chip8->v[idx] != chip8->k) chip8->pc += 2;
                    break;
                default:
                    break;
            }
            break;
        case 0xF000: // HW
            switch (opcode & 0x00FF)
            {
                case 0x07: // LD Vx DT
                    chip8->v[idx] = chip8->dt;
                    break;
                case 0x0A: // LD Vx k - keypress into Vx
                    break;
                case 0x15: // LD DT Vx
                    chip8->dt = chip8->v[idx];
                    break;
                case 0x18: // LD ST Vx 
                    chip8->st = chip8->v[idx];
                    break;
                case 0x1E: // ADD I Vx
                    chip8->i += chip8->v[ idx ];
                    break;
                case 0x29: // Get adress of Sprite Vx
                    chip8->i = chip8->v[ idx ] * 5;
                    break;
                case 0x33: // store BCD of Vx at mem[i]                   
                    chip8->mem[chip8->i] = chip8->v[idx] / 100;
                    chip8->mem[chip8->i+1] = (chip8->v[idx]%100) / 10;
                    chip8->mem[chip8->i+2] = chip8->v[idx] % 10;                    
                    break;
                case 0x55:
                    for (uint8_t j = 0; j <= idx; j++)
                    {
                        chip8->mem[ chip8->i + j] = chip8->v[j];
                    }
                    break;
                case 0x65:
                    for (uint8_t j = 0; j <= idx; j++)
                    {
                        chip8->v[j] = chip8->mem[ chip8->i + j]; 
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }












}