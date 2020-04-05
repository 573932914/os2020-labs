#include "../lib/color.h"
extern void append2screenAtpoint(int line, int column, char* str, int color);

void ignoreIntBody()
{
	append2screenAtpoint(24, 0, "Unknown interrupt1\0", RED);
}