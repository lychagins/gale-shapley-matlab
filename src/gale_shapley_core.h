// This code is a revised version of the matchingR package authored by 
// Jan Tilly and Nick Janetos (https://github.com/jtilly/matchingR).

#include <stdint.h>

#ifndef galeshapley_h
#define galeshapley_h

void cpp_galeshapley(uint64_t *proposerPref, double *reviewerUtils,
	size_t M, size_t W, uint64_t *engagements);

#endif