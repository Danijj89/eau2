//lang::CwC


#include "argument_parser.h"
#include "sorer.h"

/**
 * sorer: This program reads a file passed in with -f option.
 *
 * Optionally, -from tells you the location where you start the read in the file
 *             -len tells you how many bytes to read from the file
 *             -print_col_type asks for the type of the col indicated
 *             -print_col_idx asks for the value of the field at the col, idx
 *             -is_missing_idx asks whether the field at col, idx is an empty/missing value
 *
 * Unless, -print_col_type, print_col_idx, is_missing_idx options are used, nothing will happen.
 *
 * If those options are used, a -f must be included.
 *
 * If no -len or no -from options are given, we assume the whole file must be read.
 */


int main(int argc, char **argv) {

	ArgumentParser p = ArgumentParser();
	p.parse(argc, argv);

	Sorer s = Sorer();
	s.initialize_sorer(p.getFilename(), p.getFrom(), p.getLen());

    char* command = p.getCommand();
    if (strcmp(command, "-print_col_type") == 0) {
        s.printColType(p.getVal1());
    } else {
        size_t field_start = columnar[uint1]->get_start(uint2);
        size_t field_end = columnar[uint1]->get_end(uint2);
        if (strcmp(output_arg, "-is_missing_idx") == 0) {
            if (field_end - field_start < 2) {
                std::cout << 1 << '\n';
            } else {
                std::cout << 0 << '\n';
            }
        } else {
            print_field(file, field_start, field_end, schema->get(uint1));
        }
    }

	// If none of the three flags that require an output
	// is given, return directly.
	if (!output_arg) {
		return 0;
	}
    // delete everything
	size_t num_col = schema->len();
    for (size_t k = 0; k < num_col; ++k) {
        delete columnar[k];
    }
    delete[] columnar;
    delete schema;
    munmap(file, ask);
    close(fd);
    return 0;
}
