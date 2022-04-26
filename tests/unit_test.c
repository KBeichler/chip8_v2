#include "unity.h"
#include <chip8.h>
#include <stdio.h>

void setUp(void)
{

}

void tearDown(void)
{

}

void test_init()
{
    chip8_t *chip8 = init_chip8();

    TEST_ASSERT_NOT_NULL(chip8);
    TEST_ASSERT_NOT_NULL(chip8->framebuffer);
}

int main (void)
{
    
    UNITY_BEGIN();
    RUN_TEST(test_init);


    return UNITY_END();
}