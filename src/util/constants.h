//lang::CwC


#pragma once


#include <cstdlib>


const char* SORER_USAGE = "Usage: ./sorer [-f] [-from] [-len] [-print_col_type] " \
             "[-print_col_idx] [-is_missing_idx]\n" \
             "\n" \
             "\t-f [filename] must be the first argument\n" \
             "\t-from [uint] must come after -f option, if used\n" \
             "\t-len [uint] must come after -f option, and if -from is used, after -from\n" \
             "\t only one of -print_col_type [uint] / -print_col_idx [uint] [uint] / -is_missing_idx [uint] [uint] can be used\n" \
             "\n" \
             "Only one option of each kind can be used.\n";

const size_t INFER_SCHEMA_LINES = 500;

const unsigned int MAX_BUFF_SIZE = 400;
const unsigned int MAX_BLOB_SIZE = 16 * 1024;
const unsigned int MAX_BOOL_ELEMENTS = MAX_BLOB_SIZE;
const unsigned int MAX_INT_ELEMENTS = MAX_BLOB_SIZE / 4;
const unsigned int MAX_DOUBLE_ELEMENTS = MAX_BLOB_SIZE / 8;
const unsigned int MAX_STRING_ELEMENTS = MAX_BLOB_SIZE / 256;
const unsigned int MAX_NUM_THREADS = 24;
const char SERVER_IP[10] = "127.0.0.1";
const char APPLICATION_IP[10] = "127.0.0.1";
const unsigned int PORT = 9000;
const unsigned int APPLICATION_PORT = 9000;


// Design Considerations; ideal capacities
// const unsigned int MAX_ROWS = 1000 * 1000 * 1000;
// const unsigned int MAX_COLS = 100;

// Testing will be performed on a single computer, assume we have a little more
// than 8 GB of main memory.
//const unsigned int MAX_BYTES = 8 * 1024 * 1024 * 1024;
//const unsigned int NODES = 4;
//const unsigned int MAX_BYTES_PER_NODE = MAX_BYTES / NODES;
//const unsigned int MAX_BLOBS_PER_NODE = MAX_BYTES_PER_NODE / MAX_BLOB_SIZE;
