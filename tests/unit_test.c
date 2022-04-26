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

int main (void)
{
    
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(opcode_0);

    return UNITY_END();
}