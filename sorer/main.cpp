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
	// get argument inputs
	p.parse(argc, argv);

	Sorer s = Sorer();
	// parse file using sorer
	s.initialize_sorer(p.getFilename(), p.getFrom(), p.getLen());

	// execute user requested command
    char* command = p.getCommand();
    if (strcmp(command, "-print_col_type") == 0) {
        s.printColType(p.getVal1());
    } else if (strcmp(command, "-is_missing_idx") == 0) {
    	s.printIsMissing(p.getVal1(), p.getVal2());
	} else if (strcmp(command, "-print_col_idx") == 0) {
    	s.printField(p.getVal1(), p.getVal2());
    }

    return 0;
}
