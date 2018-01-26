/*
 * render.h
 *
 *  Created on: Jan 26, 2018
 *      Author: root
 */

#ifndef RENDER_H_
#define RENDER_H_

typedef struct {
	char *bBuffer;
	size_t bufSize;
	int width;
	int height;
	int bbp;
} screen_t;





void renderScreenPart(int xStart, int yStart, int width, int height);
void renderScreenBufferPart(char * start, char * end, char * color);
#endif /* RENDER_H_ */
