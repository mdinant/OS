#include <system.h>
#include "render.h"

screen_t screen;



void init_screen() {

}

void renderScreenPart(int xStart, int yStart, int width, int height) {
//	unsigned char color[] = { 50, 100, 200, 255 };
//
//	int c;
//
//	char * start = &screen.bBuffer[((yStart * screen.width) + xStart) * screen.bbp];
//
//	char * ptr;
//
//	int x;
//	int y;
//	for (y = yStart; y < height; y++) {
//		for (x = xStart; x < width; x++) {
//			ptr = &start[((y * screen.width) + x) * screen.bbp];
//			memcpy(ptr, color, screen.bbp);
//		}
//	}


	memset(screen.bBuffer, 200, screen.bufSize);

}

void renderScreenBufferPart(char * start, char * end, char * color) {
	//memset(start, color, (end - start));
	char * ptr;

	for (ptr = start; ptr < end; ptr+=4) {
		memcpy(ptr, color, 4);
	}

}


