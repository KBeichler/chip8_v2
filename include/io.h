#ifndef _IO_
#define IO
#include <stdint.h>
#include <SDL.h>
#include <time.h>




void window_init(uint16_t screen_height, uint16_t screen_width);
void window_drawFrame(uint32_t *framebuffer);
void window_close(void);
void window_delay(uint32_t ms);
uint8_t window_getIO(void);

uint32_t * window_get_framebuffer();

void window_show();


#endif