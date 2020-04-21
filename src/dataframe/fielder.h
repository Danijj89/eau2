//lang:CwC

#pragma once


#include "../util/object.h"
#include "../util/string.h"


class Fielder : public Object {
public:

	/**
	 * Called before visiting a row, the argument is the row offset in
	 * the dataframe.
	 * @param r the row index
	 */
	virtual void start() {};

	/**
	 * Called for fields of type bool with the value of the field.
	 * @param b the value
	 */
	virtual void accept(bool b) {};

	/**
	 * Called for fields of type int with the value of the field.
	 * @param b the value
	 */
	virtual void accept(int i) {};

	/**
	 * Called for fields of type double with the value of the field.
	 * @param b the value
	 */
	virtual void accept(double f) {};

	/**
	 * Called for fields of type string with the value of the field.
	 * @param b the value
	 */
	virtual void accept(String* s) {};

	/**
	 * Called when all fields have been seen.
	 */
	virtual void done() {};
};
