//lang::CwC


#pragma once


#include <cstdlib>
#include <thread>


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
const unsigned int NUM_DF_THREADS = std::thread::hardware_concurrency() - 1;
const unsigned int MAX_CHUNKS_PER_NODE = 200;


// Design Considerations; ideal capacities
// const unsigned int MAX_ROWS = 1000 * 1000 * 1000;
// const unsigned int MAX_COLS = 100;



const char APPLICATION_IP[10] = "127.0.0.1";
const unsigned int APPLICATION_PORT = 9000;

const size_t CHUNK_ELEMENTS = 100;

const int BACKLOG = 5;
const char SERVER_IP[10] = "127.0.0.1"; // nodeID_ = 0
const unsigned int SERVER_PORT = 9999;
const char NODE1_IP[10] = "127.0.0.2"; // nodeID_ = 1
const char NODE2_IP[10] = "127.0.0.3"; // nodeID_ = 2
const char NODE3_IP[10] = "127.0.0.4"; // nodeID_ = 3
const char NODE4_IP[10] = "127.0.0.5"; // nodeID_ = 4
const int PORT1 = 9001; // port for nodeId = 1
const int PORT2 = 9002; // port for nodeId = 2
const int PORT3 = 9003; // port for nodeId = 3
const int PORT4 = 9004; // port for nodeId = 4
const int NUM_NODES = 3;
const int SERVER_ID = 0;

// Testing will be performed on a single computer, assume we have a little more
// than 8 GB of main memory.
const unsigned int MAX_BYTES = 1024 * 1024;
const unsigned int MAX_BYTES_PER_NODE = MAX_BYTES / NUM_NODES;
const unsigned int MAX_BLOBS_PER_NODE = MAX_BYTES_PER_NODE / MAX_BLOB_SIZE;