#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "display.h"
#include "file.h"

//writes ppm image

void write_ppm(size_t buffer_size) {
	
	FILE* file = fopen("render.ppm", "wb");
	fprintf(file, "P6\n%i %i\n255\n", WINDOW_WIDTH, WINDOW_HEIGHT);
	fwrite(frame_buffer, buffer_size, 1, file);
	fclose(file);
}

//dumps raw buffer contents to a file

void dump_raw_buffer_content(size_t buffer_size) {
	
	FILE* file = fopen("raw_buffer_dump.txt", "wb");
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int colour_value = frame_buffer[(WINDOW_WIDTH * y) + x];
			
			uint8_t a = colour_value >> 24;
			uint8_t r = colour_value >> 16;
			uint8_t g = colour_value >> 8;
			uint8_t b = colour_value;
			
			fprintf(file, "Pixel at coordinate %d, %d   A   R   G   B\n", x, y);
			fprintf(file, "                             %d   %d  %d  %d\n", a, r, g, b);
			fprintf(file, "\n");
			
		}
	}
	
	fclose(file);
}

//initialize SDL, window

void render_frame_buffer(void) {
	
	SDL_UpdateTexture(texture, NULL, frame_buffer, (WINDOW_WIDTH * sizeof(uint32_t)));
	
	SDL_RenderCopy(
		renderer,
		texture,
		NULL,
		NULL
	);
}

bool init_window(void) {
	
	if(SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "There was an error initialising SDL.....\n");
		return false;
	}
	
	window = SDL_CreateWindow(
		"render3",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);
	
	if(!window) {
		fprintf(stderr, "There was an error creating the window.....\n");
		return false;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	if(!renderer) {
		fprintf(stderr, "There was an error creating the SDL renderer......");
		return false;
	}
	
	return true;
}

//setup frame buffer

void setup(void) {
	
	frame_buffer = (uint32_t*) malloc(FRAME_BUFFER_SIZE_BYTES);
	
	if(!frame_buffer) {
		fprintf(stderr, "There was an error allocating memory for the frame buffer....\n");
		program_running = false;
		return;
	}
	
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);
}

//process input

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	
	switch(event.type) {
		case SDL_QUIT:
			program_running = false;
			break;
	}
}

//update stuff

void update(void) {

}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	render_frame_buffer();
	
	clear_screen(COLOUR_BLACK);
	draw_grid();
	draw_right_triangle(80, 40, 200, 200, INTERPOLATE);
	draw_rect(300, 300, 200, 200, COLOUR_RED);
	
	SDL_RenderPresent(renderer);
}

//draws right angled triangles

void draw_right_triangle(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t colour) {
	
	int tri_width = 0;
	
	if((x_pos < WINDOW_WIDTH) && (y_pos < WINDOW_HEIGHT)) {
			for(int y = y_pos; y < (y_pos + height); y++) {
				for(int x = x_pos; x < (x_pos + tri_width); x++) {
					if(colour == INTERPOLATE) {
					
						float r = (float)x / (float) (x_pos + tri_width);
						float g = (float)y / (float) (y_pos + height);
						float b = 0.5;
					
						float a = 0xff;
						float ir = (float)255.99f * r;
						float ig = (float)255.99f * g;
						float ib = (float)255.99f * b;
					
						uint32_t colour = (uint32_t)a;
						colour <<= 8;
						colour += (uint32_t)ir;
						colour <<= 8;
						colour += (uint32_t)ig;
						colour <<= 8;
						colour += (uint32_t)ib;
					
						frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
					} else {
						frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
					}
				
			}
			
			if(tri_width < width) {
				tri_width++;
			}
		}
	}
}

//draws a rectangle

void draw_rect(uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t height, uint32_t colour) {
	
	if((x_pos < WINDOW_WIDTH) && (y_pos < WINDOW_HEIGHT)) {
		for(int y = y_pos; y < (y_pos + height); y++) {
			for(int x = x_pos; x < (x_pos + width); x++) {
				if(colour == INTERPOLATE) {
					
					float r = (float)x / (float) (x_pos + width);
					float g = (float)y / (float) (y_pos + height);
					float b = 0.5;
					
					float a = 0xff;
					float ir = (float)255.99f * r;
					float ig = (float)255.99f * g;
					float ib = (float)255.99f * b;
					
					uint32_t colour = (uint32_t)a;
					colour <<= 8;
					colour += (uint32_t)ir;
					colour <<= 8;
					colour += (uint32_t)ig;
					colour <<= 8;
					colour += (uint32_t)ib;
					
					frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
					
				} else {
					frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
				}
			}
		}
	} else {
		fprintf(stderr, "The x and y positions of ");
	}
}

//draws a grid

void draw_grid(void) {
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			if((x % 30) == 0  || (y % 30) == 0) {
				frame_buffer[(WINDOW_WIDTH * y) + x] = COLOUR_GREY;
			} else {
				frame_buffer[(WINDOW_WIDTH * y) + x] = COLOUR_BLACK;
			}
		}
	}
}

//clears buffer with selected colour mode

void clear_screen(uint32_t colour) {
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			if(colour == INTERPOLATE) {
				
				float r, g, b;
				
				r = (float)x / (float)WINDOW_WIDTH;
				g = (float)y / (float)WINDOW_HEIGHT;
				b = 0.5;
				
				float a, ir, ig, ib;
				
				a = 0xff;
				ir = (float)255.99f * r;
				ig = (float)255.99f * g;
				ib = (float)255.99f * b;
				
				uint32_t colour = (uint32_t)a;
				colour <<= 8;
				colour += (uint32_t)ir;
				colour <<= 8;
				colour += (uint32_t)ig;
				colour <<= 8;
				colour += (uint32_t)ib;
				
				frame_buffer[(WINDOW_WIDTH * y) + x] = colour;

			} else {
				frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
			}
		}
	}
}

void destroy_window(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
}

void quit(void) {
	free(frame_buffer);
	destroy_window();
}