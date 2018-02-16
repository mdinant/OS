#include <system.h>
#include "render.h"

screen_t screen;

//bool renderScreenFinishedList[MAX_CPU];



//void init_screen() {
//	memset(renderScreenFinishedList, FALSE, sizeof(bool) * MAX_CPU);
//}

void renderScreenPart(int xStart, int yStart, int width, int height) {

}

void renderScreenBufferPart(char * start, char * end, char * color) {
	//memset(start, color, (end - start));
	char * ptr;

	for (ptr = start; ptr < end; ptr+=4) {
		memcpy(ptr, color, 4);
	}

}


