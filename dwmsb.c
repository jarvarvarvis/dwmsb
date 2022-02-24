#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>

#ifndef NO_X
#include <X11/Xlib.h>	
#endif

#define LENGTH(x)      (sizeof(x) / sizeof(x[0]))
#define MAX_CMD_LENGTH 50

typedef char* (*statuscmd)();

typedef struct {
	char *icon;
	statuscmd command;
} Block;

#ifndef NO_X
void setroot();
static void (*writestatus)(char *text) = setroot;
static int setupX();
static Display *dpy;
static int screen;
static Window root;
#else
static void (*writestatus)(char *text) = pstdout;
#endif

#include "config.h"


#ifndef NO_X
void setroot(char *text)
{
	XStoreName(dpy, root, text);
	XFlush(dpy);
}

int setupX()
{
	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		fprintf(stderr, "dwmsb: Failed to open X11 display!\n");
		return 0;
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	return 1;
}
#endif


void pstdout(char *text)
{
	printf("%s\n", text);
	fflush(stdout);
}


char *readblocks()
{
	const size_t BLOCK_COUNT = LENGTH(blocks);
	size_t bufferlength = 0;

	// Get total buffer length
	char *blockicons[BLOCK_COUNT];
	char *blocktexts[BLOCK_COUNT];
	for (int i = 0; i < BLOCK_COUNT; ++i)
	{
		char *icon = blocks[i].icon;
		blockicons[i] = icon;
		char *text = (*blocks[i].command)();
		blocktexts[i] = text;
	
		bufferlength += strlen(icon) + strlen(text);
		if (i != BLOCK_COUNT - 1)
		{
			bufferlength += strlen(separator);
		}
	}
	
	// Create result string
	char *buf = malloc(sizeof(char) * (bufferlength + 1));
	memset(buf, 0, bufferlength + 1);

	for (int i = 0; i < BLOCK_COUNT; ++i)
	{
		strcat(buf, blockicons[i]);
		strcat(buf, blocktexts[i]);
		free(blocktexts[i]);
		if (i != BLOCK_COUNT - 1)
		{
			strcat(buf, separator);
		}
	}

	buf[bufferlength] = '\0';
	return buf;
}


int main(void)
{
#ifndef NO_X
	if (!setupX())
		return 1;
#endif

	while (1)
	{
		char *text = readblocks();
		writestatus(text);
		free(text);

	        /* To provide updates on every full second (as good as possible)
	         * we don’t use sleep(interval) but we sleep until the next
	         * second (with microsecond precision) plus (interval-1)
	         * seconds. We also align to 60 seconds modulo interval such
	         * that we start with :00 on every new minute. */
	        struct timeval current_timeval;
	        gettimeofday(&current_timeval, NULL);
	        struct timespec ts = 
		{ 
			interval - 1 - (current_timeval.tv_sec % interval), (10e5 - current_timeval.tv_usec) * 1000
		};
	        nanosleep(&ts, NULL);
	}

#ifndef NO_X
	XCloseDisplay(dpy);
#endif
}
