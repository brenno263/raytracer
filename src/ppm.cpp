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
