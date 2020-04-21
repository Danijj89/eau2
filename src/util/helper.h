#pragma once

#include <cstdlib>
#include <cstring>
#include <cassert>

/**
 * Attemps to convert a string to an unsigned integer.
 * Asserts that the conversion is successful.
 *
 * @param val the value to parse
 * @return the parsed unsigned integer.
 */
inline size_t parse_uint(char* val) {
	char *endptr = nullptr;
	size_t length = strlen(val);
	size_t uint = strtoul(val, &endptr, 10);
	assert(length == (size_t) (endptr - val));
	return uint;
}