#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WINDOW_WIDTH 2560
#define WINDOW_HEIGHT 1440
#define PIXEL_STRIDE 3

#define BLACK 0x000000
#define WHITE 0xffffff
#define RED   0xff0000
#define GREEN 0x00ff00
#define BLUE  0x0000ff
#define GREY  0x333333


#define CLEAR_BLACK          0x1
#define CLEAR_COLOR_EXAMPLE  0x2

//writes ppm image

void write_ppm(uint8_t* frame_buffer, size_t buffer_size) {
	
	FILE* file = fopen("render.ppm", "wb");
	fprintf(file, "P6\n%i %i\n255\n", WINDOW_WIDTH, WINDOW_HEIGHT);
	fwrite(frame_buffer, buffer_size, 1, file);
	fclose(file);
}

//clears buffer with a grid

void draw_rect(int x_pos, int y_pos, int width, int height) {
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
		}
	}
}

void draw_grid(uint8_t* frame_buffer) {
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

//clears buffer with an interpolated colour example

void clear_screen(uint8_t* frame_buffer) {
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
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
			
		}
	}
}

//dumps raw buffer contents to a file

void raw_buffer_dump(uint8_t* buffer, size_t buffer_size) {
	
	FILE* file = fopen("raw_buffer_dump.txt", "wb");
	
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			
			int idx = ((WINDOW_WIDTH * y) + x) * PIXEL_STRIDE;
			
			fprintf(file, "Pixel at coordinate %d, %d   R   G   B\n", x, y);
			fprintf(file, "                             %d   %d  %d\n", buffer[idx + 0], buffer[idx + 1], buffer[idx + 2]);
			fprintf(file, "\n");
			
		}
	}
	
	fclose(file);
}

int main(int argc, char** argv) {
	
	size_t buffer_size_bytes = WINDOW_WIDTH * WINDOW_HEIGHT * PIXEL_STRIDE; //2560*1440*3 bytes
	
	uint8_t* pixel_buffer = (uint8_t*)malloc(buffer_size_bytes);
	
	if(pixel_buffer == NULL) {
		printf("There was an error when attempting to request %lu bytes of memory", buffer_size_bytes);
	}
	
	clear_screen(pixel_buffer);
	//draw_grid(pixel_buffer);
	write_ppm(pixel_buffer, buffer_size_bytes);
	
	raw_buffer_dump(pixel_buffer, buffer_size_bytes);
	
	free(pixel_buffer);
	
	printf("%lu", sizeof(size_t));
	
	return 0;
}