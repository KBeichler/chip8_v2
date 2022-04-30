#include <chip8.h>
#include <io.h>
#include <time.h>


#define FREQUENCY 500
#define CYCLE_TIME 1000/500


void delay(uint32_t ms)
{

}



char *testfile = "./testrom/flightrunner.rom";

int main(int argc, char *argv[])
{
    window_init(SCREEN_HEIGHT, SCREEN_WIDTH);
    uint32_t * frb = window_get_framebuffer();
    chip8_t *chip8 = init_chip8(frb);

    FILE *f = fopen(testfile, "r");
    if (f == NULL)
    {
        printf("Error: File '%s' not found\n", testfile);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size_t romsize = ftell(f);
    
    rewind(f);  

    fread( &chip8->mem[0x200], romsize, 1, f);

    fclose(f);
    printf("-- LOADED --\nRomfile: %s\nSize: %ld\n",testfile, romsize);




    uint8_t quit = 1;
    uint16_t cycle = 0;
    uint8_t input = 0xFA;


    while (quit)
    {


        uint32_t cycle_start = SDL_GetTicks();

        //printf("OPCODE %04X\n", (chip8->mem[chip8->pc] << 8) | chip8->mem[chip8->pc+1] );
        input = window_getIO();

        if (input == 0xFF) break;
        if (input < 0x10) chip8->k = input;
        if (chip8->request_input == 1 && input > 0xF) continue; // skip if we wait for input

        emulate_cycle(chip8);
        while(SDL_GetTicks() < cycle_start + CYCLE_TIME);

        cycle++;
        cycle = cycle > 500 ? 0 : cycle;

        if (cycle % 8 == 0) // cycle speed is 2 hz, every 8 cycles update screen
        {
            window_show();
            if (chip8->dt > 0) chip8->dt--;
            if (chip8->st > 0) chip8->st--;
        }

    }

    window_close();
    close_chip8(chip8);
    return 0;
}