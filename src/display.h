/*
 * File:    display.h
 *
 * Author:  Cian Martin Bohan (@cianmarbo)
 * Date:    April 2022
 *
 * Summary of file:
 *
 *		display.h defines constants and declarations for various
 * 		functions required for manipulating and rendering the 
 *		framebuffer.
 */

#ifndef DISPLAY_H 
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Colour byte layout is ARGB, 8 bits per pixel channel, 32 bits in total
#define COLOUR_BLACK 0xff000000
#define COLOUR_WHITE 0xffffffff
#define COLOUR_RED	 0xffff0000
#define COLOUR_GREEN 0xff00ff00
#define COLOUR_BLUE  0xff0000ff
#define COLOUR_GREY  0xff333333

#define INTERPOLATE  -1

#define FRAME_BUFFER_SIZE_BYTES (sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT)

bool program_running;

uint32_t* frame_buffer;

SDL_Texture* texture;

SDL_Window* window;
SDL_Renderer* renderer;

void clear_screen(uint32_t colour);

void draw_rect(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t colour);
void draw_right_triangle(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t colour);
void draw_grid(void);

bool init_window(void);
void setup(void);

void process_input(void);
void update(void);
void render(void);

void render_frame_buffer(void);

void destroy_window(void);
void quit(void);

#endif