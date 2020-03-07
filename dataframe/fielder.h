

#pragma once


#include "../util/object.h"
#include "../util/string.h"

/*
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
public:

    /**
     * Called before visiting a row, the argument is the row offset in
	 * the dataframe.
     * @param r the row index
     */
    virtual void start(size_t r) {};

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
     * Called for fields of type float with the value of the field.
     * @param b the value
     */
    virtual void accept(float f) {};

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
