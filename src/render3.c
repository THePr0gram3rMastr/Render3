#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PIXEL_STRIDE 3

#define BLACK 0x000000
#define WHITE 0xffffff
#define RED   0xff0000
#define GREEN 0x00ff00
#define BLUE  0x0000ff
#define GREY  0x333333

#define CLEAR_COLOUR_BLACK			0x1
#define CLEAR_COLOUR_WHITE			0x2
#define CLEAR_COLOUR_INTERPOLATED	0x3

uint8_t* frame_buffer;

//writes ppm image

void write_ppm(size_t buffer_size) {
	
	FILE* file = fopen("render.ppm", "wb");
	fprintf(file, "P6\n%i %i\n255\n", WINDOW_WIDTH, WINDOW_HEIGHT);
	fwrite(frame_buffer, buffer_size, 1, file);
	fclose(file);
}

//draws a rectangle

void draw_rect(int x_pos, int y_pos, int width, int height) {
	for(int y = y_pos; y < (y_pos + height); y++) {
		for(int x = x_pos; x < (x_pos + width); x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
			frame_buffer[idx + 0] = 0xff;
			frame_buffer[idx + 1] = 0x00;
			frame_buffer[idx + 2] = 0x00;
			
		}
	}
}

//draws a grid

void draw_grid() {
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
			if( (x % 30) == 0  || (y % 30) == 0) {
				frame_buffer[idx + 0] = 0x33;
				frame_buffer[idx + 1] = 0x33;
				frame_buffer[idx + 2] = 0x33;
			} else {
				frame_buffer[idx + 0] = 0x00;
				frame_buffer[idx + 1] = 0x00;
				frame_buffer[idx + 2] = 0x00;
			}
		}
	}
}

//clears buffer with selected colour mode

void clear_screen(uint8_t clear_value) {
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
			if(clear_value == CLEAR_COLOUR_INTERPOLATED) {
				
				float r, g, b;
				
				r = (float)x / (float)WINDOW_WIDTH;
				g = (float)y / (float)WINDOW_HEIGHT;
				b = 0.5;
				
				float ir, ig, ib;
				
				ir = (float)255.99f * r;
				ig = (float)255.99f * g;
				ib = (float)255.99f * b;
				
				frame_buffer[idx + 0] = ir;
				frame_buffer[idx + 1] = ig;
				frame_buffer[idx + 2] = ib;
			
			} else if(clear_value == CLEAR_COLOUR_BLACK)  {
				frame_buffer[idx + 0] = 0x00;
				frame_buffer[idx + 1] = 0x00;
				frame_buffer[idx + 2] = 0x00;
			
			} else if(clear_value == CLEAR_COLOUR_WHITE) {
				frame_buffer[idx + 0] = 0xff;
				frame_buffer[idx + 1] = 0xff;
				frame_buffer[idx + 2] = 0xff;
			}
		}
	}
}

//dumps raw buffer contents to a file

void raw_buffer_dump(size_t buffer_size) {
	
	FILE* file = fopen("raw_buffer_dump.txt", "wb");
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
			fprintf(file, "Pixel at coordinate %d, %d   R   G   B\n", x, y);
			fprintf(file, "                             %d   %d  %d\n", frame_buffer[idx + 0], frame_buffer[idx + 1], frame_buffer[idx + 2]);
			fprintf(file, "\n");
			
		}
	}
	
	fclose(file);
}

int main(int argc, char** argv) {
	
	size_t buffer_size_bytes = WINDOW_WIDTH * WINDOW_HEIGHT * PIXEL_STRIDE; //2560*1440*3 bytes
	
	frame_buffer = (uint8_t*)malloc(buffer_size_bytes);
	
	if(frame_buffer == NULL) {
		printf("There was an error when attempting to request %lu bytes of memory", buffer_size_bytes);
	}
	
	clear_screen(CLEAR_COLOUR_BLACK);
	//draw_grid(pixel_buffer);
	draw_rect(400, 300, 100, 100);
	write_ppm(buffer_size_bytes);
	
	raw_buffer_dump(buffer_size_bytes);
	
	free(frame_buffer);
	
	return 0;
}