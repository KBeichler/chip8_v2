#include "unity.h"
#include <chip8.h>
#include <stdio.h>
#include <stdlib.h>

chip8_t *chip8;

void setUp(void)
{
    chip8 = init_chip8();
}

void tearDown(void)
{
    close_chip8(chip8);
}

void test_init()
{
    TEST_ASSERT_NOT_NULL(chip8);
    TEST_ASSERT_NOT_NULL(chip8->framebuffer);
    TEST_ASSERT_EQUAL(0, chip8->sp);
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
    chip8->framebuffer[5][0] = 100; // set pixel on screen
    execute_opcode(chip8, 0x00E0);  // call clear
    // get empty reference array to compare to
    uint8_t *reference = (uint8_t *) calloc(sizeof(uint8_t), SCREEN_HEIGHT*SCREEN_WIDTH);
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
    chip8->v[0x5] = 0x10;
    chip8->v[0x6] = 0xFF;

    execute_opcode(chip8, 0x8560);
    TEST_ASSERT_EQUAL(0xFF, chip8->v[0x5]);
    TEST_ASSERT_EQUAL(0xFF, chip8->v[0x6]);

}

int main (void)
{
    
    UNITY_BEGIN();
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
    return UNITY_END();
}