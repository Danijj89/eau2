#pragma once


#include "../util/object.h"
#include "../util/string.h"

class Bool;
class Int;
class Double;
class Str;

/**
 * Superclass of a primitive value boxer.
 * Used by the Row class to box primitives.
 * They are created empty. Values can be set.
 * The types supported are:
 * - 'B': boolean
 * - 'I': integer
 * - 'D': double
 * - 'S': string
 */
class Val : public Object {
public:
	char type_; // type of val

	Val(char type) : type_(type) {}

	/**
	 * Get the type of this val
	 * @return
	 */
	char getType() {
		return this->type_;
	}

	/**
	 * Return this val as a Bool
	 * @return
	 */
	virtual Bool* asBool() {
		return nullptr;
	}

	/**
	 * Return this val as an Int
	 * @return
	 */
	virtual Int* asInt() {
		return nullptr;
	}

	/**
	 * Return this val as a Double
	 * @return
	 */
	virtual Double* asDouble() {
		return nullptr;
	}

	/**
	 * Return this val as a Str
	 * @return
	 */
	virtual Str* asStr() {
		return nullptr;
	}
};

/**
 * Represents a class to box a boolean value
 */
class Bool : public Val {
public:
	bool val_;
	
	Bool() : Val('B') {}
	
	bool get() {
		return this->val_;
	}
	
	void set(bool val) {
		this->val_ = val;
	}
	
	Bool* asBool() override {
		return this;
	}
};

/**
 * Represents a class to box a integer value
 */
class Int : public Val {
public:
	int val_;

	Int() : Val('I') {}

	int get() {
		return this->val_;
	}

	void set(int val) {
		this->val_ = val;
	}
	
	Int* asInt() override {
		return this;
	}
};

/**
 * Represents a class to box a double value
 */
class Double : public Val {
public:
	double val_;

	Double() : Val('D') {}

	double get() {
		return this->val_;
	}

	void set(double val) {
		this->val_ = val;
	}
	
	Double* asDouble() override {
		return this;
	}
};

/**
 * Represents a class to box a String value
 */
class Str : public Val {
public:
	String* val_ = nullptr;

	Str() : Val('S') {}
	
	~Str() {
		delete this->val_;
	}

	String* get() {
		return this->val_;
	}

	void set(String* val) {
		delete this->val_;
		this->val_ = new String(*val);
	}
	
	Str* asStr() override {
		return this;
	}
};