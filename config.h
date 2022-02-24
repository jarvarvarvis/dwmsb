#include <time.h>

char *datecmd() 
{
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char *s = malloc(sizeof(char) * 64);
	strftime(s, sizeof(char) * 64, "%c", tm);
	return s;
}

/* Definitions of blocks */
static const Block blocks[] =
{
	{ "Date: ", &datecmd }
};

/* The update interval in seconds */
static int interval = 5;

/* Block separator */
static char *separator = " | ";
