#include "unity.h"
#include <chip8.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>


chip8_t *chip8;

extern const uint8_t CHIP8_FONT[80]; // get external font array from chip8.c

void setUp(void)
{
    uint32_t *buffer = malloc(sizeof(uint32_t) *SCREEN_HEIGHT * SCREEN_WIDTH);
    chip8 = init_chip8(buffer);

}

void tearDown(void)
{
    free(chip8->framebuffer);
    close_chip8(chip8);
}

void test_init()
{
    

    TEST_ASSERT_NOT_NULL(chip8);
    TEST_ASSERT_NOT_NULL(chip8->framebuffer);
    TEST_ASSERT_EQUAL(0, chip8->sp);
    TEST_ASSERT_EQUAL(0x200, chip8->pc);

    
    TEST_ASSERT_EQUAL_INT8_ARRAY(CHIP8_FONT,&chip8->mem[FONT_OFFSET],80);
}
// opcode tests
void opcode_0(void)
{
    // TEST SYS CALL
    execute_opcode(chip8, 0x0123);
    TEST_ASSERT_EQUAL(0x0123, chip8->pc);

    // TESTSTACK POINTER RETURN
    chip8->sp = 2;
    chip8->stack[chip8->sp] = 0x2345;
    execute_opcode(chip8, 0x00EE);
    TEST_ASSERT_EQUAL(0x2345, chip8->pc);
    TEST_ASSERT_EQUAL(1, chip8->sp);

    // TEST CLEAR SCREEN
    chip8->framebuffer[5*SCREEN_WIDTH] = 100; // set pixel on screen
    execute_opcode(chip8, 0x00E0);  // call clear
    // get empty reference array to compare to
    uint32_t *reference = (uint32_t *) calloc(sizeof(uint32_t), SCREEN_HEIGHT*SCREEN_WIDTH);
    TEST_ASSERT_EQUAL_INT8_ARRAY(reference, chip8->framebuffer, SCREEN_HEIGHT*SCREEN_WIDTH);
    free(reference);

}

void opcode_1(void)
{   
    // TEST JP addr 
    execute_opcode(chip8, 0x1123);
    TEST_ASSERT_EQUAL(0x0123, chip8->pc);

}

void opcode_2(void)
{
    // TEST CALL
    chip8->pc = 0x123;
    execute_opcode(chip8, 0x2234);

    TEST_ASSERT_EQUAL(1, chip8->sp);
    TEST_ASSERT_EQUAL(0x123, chip8->stack[chip8->sp]);
    TEST_ASSERT_EQUAL(0x0234, chip8->pc);

}

void opcode_3(void)
{
    chip8->pc = 0x100;
    chip8->v[1] = 0xFF;

    execute_opcode(chip8, 0x32FF);
    TEST_ASSERT_EQUAL(0x100, chip8->pc);
    
    execute_opcode(chip8, 0x31FF);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);

}

void opcode_4(void)
{
    chip8->pc = 0x100;
    chip8->v[0xa] = 0xFF;

    execute_opcode(chip8, 0x42FF);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);
    
    execute_opcode(chip8, 0x4aFF);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);

}

void opcode_5(void)
{
    chip8->pc = 0x100;
    chip8->v[0x1] = 0xFF;
    chip8->v[0xf] = 0xFF;

    execute_opcode(chip8, 0x52F0);
    TEST_ASSERT_EQUAL(0x100, chip8->pc);

    execute_opcode(chip8, 0x51F0);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);

}

void opcode_6(void)
{
    execute_opcode(chip8, 0x6FFA);
    TEST_ASSERT_EQUAL(0xFA, chip8->v[0xF]);

    execute_opcode(chip8, 0x6A12);
    TEST_ASSERT_NOT_EQUAL(0x00, chip8->v[0xa]);
}

void opcode_7(void)
{
    chip8->v[0x5] = 0x10;

    execute_opcode(chip8, 0x7505);
    TEST_ASSERT_EQUAL(0x15, chip8->v[0x5]);

}

void opcode_8(void)
{
    // LD
    chip8->v[0x5] = 0x10;
    chip8->v[0x6] = 0xFF;

    execute_opcode(chip8, 0x8560);
    TEST_ASSERT_EQUAL(0xFF, chip8->v[0x5]);
    TEST_ASSERT_EQUAL(0xFF, chip8->v[0x6]);

    // OR
    chip8->v[0x1] = 0x10;
    chip8->v[0x2] = 0x04;

    execute_opcode(chip8, 0x8121);
    TEST_ASSERT_EQUAL(0x10 | 0x04, chip8->v[0x1]);

    // AND
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;

    execute_opcode(chip8, 0x8122);
    TEST_ASSERT_EQUAL(0x04&0x0F, chip8->v[0x1]);

    // XOR
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;

    execute_opcode(chip8, 0x8123);
    TEST_ASSERT_EQUAL(0x04^0x0F, chip8->v[0x1]);

    // ADD
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;

    execute_opcode(chip8, 0x8124);
    TEST_ASSERT_EQUAL(0x04+0x0F, chip8->v[0x1]);
    TEST_ASSERT_EQUAL(0, chip8->v[0xf]);

    chip8->v[0x1] = 0xFE;
    chip8->v[0x2] = 0x04;

    execute_opcode(chip8, 0x8124);
    TEST_ASSERT_EQUAL( (0x04+0xFE) & 0xFF, chip8->v[0x1]);
    TEST_ASSERT_EQUAL(1, chip8->v[0xf]);
    
    // SUB
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x8125);
    TEST_ASSERT_EQUAL(0x0F-0x04, chip8->v[0x1]);
    TEST_ASSERT_EQUAL(1, chip8->v[0xf]);

    chip8->v[0x3] = 0x0F;
    chip8->v[0x4] = 0xF0;
    execute_opcode(chip8, 0x8345);
    TEST_ASSERT_EQUAL( (uint8_t)(0x0F-0xF0), chip8->v[0x3]);
    TEST_ASSERT_EQUAL(0, chip8->v[0xf]);

    // SHR
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x8126);
    TEST_ASSERT_EQUAL(0xF/2, chip8->v[0x1]);
    TEST_ASSERT_EQUAL(1, chip8->v[0xf]);   

    chip8->v[0x1] = 0x08;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x8126);
    TEST_ASSERT_EQUAL(0x8/2, chip8->v[0x1]);
    TEST_ASSERT_EQUAL(0, chip8->v[0xf]);  

    // SUBN
    chip8->v[0x1] = 0x0F;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x8127);
    TEST_ASSERT_EQUAL((uint8_t) (0x04-0x0F), chip8->v[0x1]);
    TEST_ASSERT_EQUAL(0, chip8->v[0xf]);

    chip8->v[0x3] = 0x0F;
    chip8->v[0x4] = 0xF0;
    execute_opcode(chip8, 0x8347);
    TEST_ASSERT_EQUAL( (uint8_t)(0xF0-0x0F), chip8->v[0x3]);
    TEST_ASSERT_EQUAL(1, chip8->v[0xf]);

    // SNE
    chip8->v[0x1] = 0x81;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x812E);
    TEST_ASSERT_EQUAL((uint8_t)(0x81<<1), chip8->v[0x1]);
    TEST_ASSERT_EQUAL(1, chip8->v[0xf]);   

    chip8->v[0x1] = 0x71;
    chip8->v[0x2] = 0x04;
    execute_opcode(chip8, 0x812E);
    TEST_ASSERT_EQUAL((uint8_t) (0x71<<1), chip8->v[0x1]);
    TEST_ASSERT_EQUAL(0, chip8->v[0xf]);  
}

void opcode_9(void)
{

    chip8->v[0x1] = 0x71;
    chip8->v[0x2] = 0x71;
    chip8->v[0x3] = 0x70;
    chip8->pc = 0x100;
    execute_opcode(chip8, 0x9120);
    TEST_ASSERT_EQUAL(0x100, chip8->pc);

    execute_opcode(chip8, 0x9230);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);


}

void opcode_A(void)
{
    chip8->i = 0x123;
    execute_opcode(chip8, 0xA999);
    TEST_ASSERT_EQUAL(0x999, chip8->i);

}

void opcode_B(void)
{
    chip8->v[0] = 0xFF;
    chip8->pc = 0x100;
    execute_opcode(chip8, 0xB200);
    TEST_ASSERT_EQUAL(0x2FF, chip8->pc);

}


void opcode_C(void)
{
    execute_opcode(chip8, 0xC0FF);
    execute_opcode(chip8, 0xC1FF);
    TEST_ASSERT_NOT_EQUAL(chip8->v[0], chip8->v[1]);

}
// todo D

void opcode_E(void)
{
    chip8->pc = 0x100;
    chip8->v[0] = 0x8;
    chip8->k = 0x8;
    TEST_ASSERT_NOT_EQUAL(0x102, chip8->pc);
    execute_opcode(chip8, 0xE09E);
    TEST_ASSERT_EQUAL(0x102, chip8->pc);

    chip8->k = 0x9;
    TEST_ASSERT_NOT_EQUAL(0x104, chip8->pc);
    execute_opcode(chip8, 0xE0A1);
    TEST_ASSERT_EQUAL(0x104, chip8->pc);

}


void opcode_F(void)
{

    chip8->dt = 0x99;
    execute_opcode(chip8, 0xFA07);
    TEST_ASSERT_EQUAL(0x99, chip8->v[0x0A]);

    // TODO Fx0A

    //
    chip8->v[0xE] = 0x33;
    execute_opcode(chip8, 0xFE15);
    TEST_ASSERT_EQUAL(0x33, chip8->dt);

    chip8->v[0x9] = 0x05;
    execute_opcode(chip8, 0xF918);
    TEST_ASSERT_EQUAL(0x05, chip8->st);

    chip8->v[0x1] = 0x0F;
    chip8->i = 0xA0;
    execute_opcode(chip8, 0xF11E);
    TEST_ASSERT_EQUAL(0xAF, chip8->i);

    chip8->v[0x5] = 0x0F;
    chip8->i = 0x00;
    execute_opcode(chip8, 0xF529);
    TEST_ASSERT_EQUAL((5*0xf) + FONT_OFFSET, chip8->i);
    TEST_ASSERT_EQUAL_INT8_ARRAY(&CHIP8_FONT[75],&chip8->mem[chip8->i],5);

    chip8->v[0x6] = 123;
    chip8->i = 0x600;
    execute_opcode(chip8, 0xF633);
    uint8_t test[4] = {1,2,3,4};
    TEST_ASSERT_EQUAL_INT8_ARRAY(test, &chip8->mem[chip8->i],3);

    chip8->v[0] = 0x1;
    chip8->v[1] = 0x2;
    chip8->v[2] = 0x3;
    chip8->v[3] = 0x4;
    chip8->i = 0x700;
    execute_opcode(chip8, 0xF455);
    TEST_ASSERT_EQUAL_INT8_ARRAY(test, &chip8->mem[chip8->i],4);        

    chip8->v[2] = 0x00;
    execute_opcode(chip8, 0xF465);
    TEST_ASSERT_EQUAL(0x3, chip8->v[2]);

}

void window_test(void)
{
    uint32_t *ptr = chip8->framebuffer; 
    window_init(32, 64);
    chip8->framebuffer = window_get_framebuffer();
    for (uint8_t i = 0; i < 64; i++)
    {
        chip8->framebuffer[i] = 0xFFFFFFFF;
        chip8->framebuffer[i+ 1*64] = 0xFFFFFFFF;
        chip8->framebuffer[i+ 31*64] = 0xFFFFFFFF;
        //window_drawFrame(chip8->framebuffer);
        window_show();
        window_delay(50);
    }
    
    window_close();
    chip8->framebuffer = ptr;
}

void key_test(void)
{
    uint32_t *ptr = chip8->framebuffer; 
    window_init(32, 64);
    chip8->framebuffer = window_get_framebuffer();

    printf("press Key 1\n");
    uint8_t err = 0;
    uint8_t key = 0;
    while (key != 1 && err < 255)
    {
        key = window_getIO() ;
        err++;
        window_delay(50);
    }
    TEST_ASSERT_NOT_EQUAL(255, err);
    printf("press Key 2\n");
    err = 0;
    key = 0;
    while (key != 2 && err < 255)
    {
        key = window_getIO() ;
        err++;
        window_delay(50);
    }
    TEST_ASSERT_NOT_EQUAL(255, err);
    printf("press Key 3\n");
    err = 0;
    key = 0;
    while (key != 3 && err < 255)
    {
        key = window_getIO() ;
        err++;
        window_delay(50);
    }
    TEST_ASSERT_NOT_EQUAL(255, err);
    
    
    window_close();
    chip8->framebuffer = ptr;

}

int main (void)
{
    
    UNITY_BEGIN();
    printf("\n\t----Start OpCode Tests----\n");
    RUN_TEST(test_init);
    RUN_TEST(opcode_0);
    RUN_TEST(opcode_1);
    RUN_TEST(opcode_2);
    RUN_TEST(opcode_3);
    RUN_TEST(opcode_4);
    RUN_TEST(opcode_5);
    RUN_TEST(opcode_6);
    RUN_TEST(opcode_7);
    RUN_TEST(opcode_8);
    RUN_TEST(opcode_9);
    RUN_TEST(opcode_A);
    RUN_TEST(opcode_B);
    RUN_TEST(opcode_C);
    RUN_TEST(opcode_E);
    RUN_TEST(opcode_F);
    printf("\n\t---- End OpCode Tests ----\n");
    printf("\n\t----  Start IO Tests  ----\n");
    RUN_TEST(window_test);
    RUN_TEST(key_test);
    printf("\n\t----   End IO Tests   ----\n");

    return UNITY_END();
}