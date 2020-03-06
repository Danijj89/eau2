//lang::CwC


#pragma once


#include <cassert>
#include <iostream>


#include "field_array.h"
#include "types.h"
#include "types_array.h"


const char* USAGE = "Usage: ./sorer [-f] [-from] [-len] [-print_col_type] " \
             "[-print_col_idx] [-is_missing_idx]\n" \
             "\n" \
             "\t-f [filename] must be the first argument\n" \
             "\t-from [uint] must come after -f option, if used\n" \
             "\t-len [uint] must come after -f option, and if -from is used, after -from\n" \
             "\t only one of -print_col_type [uint] / -print_col_idx [uint] [uint] / -is_missing_idx [uint] [uint] can be used\n" \
             "\n" \
             "Only one option of each kind can be used.\n";

const size_t INFER_SCHEMA_LINES = 500;




inline void print_type(Types t) {

}

/**
 * Print to std out a field of type t in a given file that has its delimiters '<' and '>' pointed to
 * by start and end.
 *
 * @param file the file to read from.
 * @param s the starting byte to read from.
 * @param e the ending byte to read to.
 * @param t the type of the field
 */
inline void print_field(char *file, int start, int end, Types t) {

    // remove empty spaces in front and back
    size_t new_start = triml(file, start, end);
    size_t new_end = trimr(file, start, end);

    // check if it is an empty field
    if (new_start < new_end) {
        switch (t) {
            case Types::BOOL:
                std::cout << file[new_start] << "\n";
                break;
            case Types::INT:
                std::cout << strtoll(&file[new_start], nullptr, 10) << "\n";
                break;
            case Types::FLOAT:
                std::cout << strtold(&file[new_start], nullptr) << "\n";
                break;
            case Types::STRING:
                if (file[new_start] != '\"' && file[new_end] != '\"') {
                    std::cout << '"';
                }
                for (size_t i = new_start; i <= new_end; ++i) {
                    std::cout << file[i];
                }
                if (file[new_start] != '\"' && file[new_end] != '\"') {
                    std::cout << '"';
                }
                std::cout << "\n";
                break;
            default:
                std::cout << "Bad type encountered, this value should not be stored";
                exit(1);
        }
    } else {
        std::cout << 1 << "\n";
    }
}
