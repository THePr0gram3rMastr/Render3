#include "display.h"
#include "file.h"

int main(int argc, char** argv) {
	
	program_running = init_window();
	
	setup();
	
	while(program_running) {
		process_input();
		update();
		render();
	}
	
	write_ppm(FRAME_BUFFER_SIZE_BYTES);
	dump_raw_buffer_content(FRAME_BUFFER_SIZE_BYTES);
	
	quit();
	
	return 0;
}