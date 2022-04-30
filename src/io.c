#include <io.h>

// global Variables needed for SDL handling
SDL_Event event;
SDL_Window *mainWindow;
SDL_Renderer * mainRenderer;
SDL_Texture * mainTexture;
void *pixels;
int pitch;
uint16_t frameDuration; 

void window_init(uint16_t screen_height, uint16_t screen_width)
{
    // SDL Main Window Init
    SDL_Init(SDL_INIT_VIDEO);   
    //create windows with *5 pixel sclaing
    mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width * 5, screen_height * 5, 0);    
    // create renderer for current window and set render size
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, screen_width, screen_height);
    //SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    //create texture to display pixels
    // use RGB888 color format
    mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_ARGB8888 , SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
    SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
}

uint32_t * window_get_framebuffer()
{
        SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
        uint32_t * ptr = ((uint32_t *)pixels);
        return ptr;
}

void window_show()
{
        SDL_UnlockTexture(mainTexture); 
        SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
        SDL_RenderPresent(mainRenderer); 
        SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);

}


//draws current framebuffer on the texture / old
void window_drawFrame(uint32_t *framebuffer)
{

    SDL_RenderClear(mainRenderer);
    SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
    uint32_t * ptr = ((uint32_t *)pixels);

    // DRAWING OF THE DATA
    // just takes the framebuffer as it was written by the ppu
    // for colors this probably needs some ajustmend
    
    for (uint16_t j = 0; j < 32; j++){
        for (uint16_t i = 0; i < 64; i++){   
                ptr[i+(j*64)] = framebuffer[i+(j*64)];
            }
    }

    // END OF DRAWING
    SDL_UnlockTexture(mainTexture); 
    SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
    SDL_RenderPresent(mainRenderer); 
    
}

void window_close()
{
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();    
}

void window_delay(uint32_t ms)
{
        SDL_Delay(ms);
}

// polls events from user io
uint8_t window_getIO(void)
{
    uint8_t key = 0xAF;;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
                case SDL_QUIT:
                return key = 0xFF;
                case SDL_KEYDOWN:
                        switch (event.key.keysym.sym){
                case SDLK_1:        
                key = 0x1;
                break;

                case SDLK_2:
                key = 0x2;
                        break;

                case SDLK_3:
                key = 0x3;
                        break;

                case SDLK_4:
                key = 0xc;
                        break;

                case SDLK_q:
                key = 0x4;
                        break;

                case SDLK_w:
                key = 0x5;
                        break;

                case SDLK_e:
                key = 0x6;
                        break;

                case SDLK_r:
                key = 0xD;
                        break;

                case SDLK_a:
                key = 0x7;
                        break;

                case SDLK_s:
                key = 0x8;
                        break;

                case SDLK_d:
                key = 0x9;
                        break;

                case SDLK_f:
                key = 0xe;
                        break;

                case SDLK_y:
                key = 0xa;
                        break;

                case SDLK_x:
                key = 0x0;
                        break;

                case SDLK_c:
                key = 0xb;
                        break;

                case SDLK_v:
                key = 0xf;
                        break;
                default:
                key = 0xAF;

                }
        }  
                
        }

        
 

    return key;
}