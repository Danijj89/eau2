//lang:CwC

/**
 * Collection of helper functions.
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */


#include <cstdio>
#include <cassert>
#include <cstring>
#include "string_array.h"

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

/**
 * Tokenize a string on a given delimiter.
 *
 * @param val the string value to tokenize.
 * @param del the delimiter.
 * @return an array of strings.
 */
StringArray* tokenize(char* val, char del) {
	StringArray* result = new StringArray();
	size_t c = 0;
	size_t i = 0;
	char buff[1024] = {0};
	while (val[c] != '\0' && val[c] != '\n') {
		if (val[c] == del) {
			buff[i] = '\0';
			result->push_back(new String(buff));
			i = 0;
		} else {
			buff[i] = val[c];
			i++;
		}
		c++;
	}
	if (i > 0) {
		buff[i] = '\0';
		result->push_back(new String(buff));
	}
	return result;
}