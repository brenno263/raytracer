/*
	ppm has a
	* magic number for identifying file type
	* whitespace (blank, tab, CR, LF)
	* a width, formatted in ascii characters as decimal
	* whitespace
	* height
	* whitespace
	* maximum color value
	* raster
	* 
*/

#include <stdio.h>
#include <stdint.h>

#define R 0
#define G 1
#define B 2

#define HEIGHT 256
#define WIDTH 256

//unsigned 8 bit int
uint8_t raster[HEIGHT][WIDTH][3];

int main(int argc, char * argv[])
{
	FILE* f;
	int x, y;
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			raster[y][x][R] = y;
			raster[y][x][G] = x;
			raster[y][x][B] = x * x + y * y < 128 * 128 ? 255 : 0;
		}
	}
	
	f = fopen("testing.ppm", "w");
	
	//magic number, height, width, max color intensity
	fprintf(f, "P6\n256\n256\n255\n");
	
	//now append data
	fwrite(raster, 1, sizeof(raster), f);
	
	
	return 0;
}
