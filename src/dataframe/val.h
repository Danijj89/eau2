#pragma once


#include "../util/object.h"
#include "../util/string.h"

class Bool;
class Int;
class Double;
class Str;

/**
 * Superclass of a boxed primitive value.
 * The types supported are:
 * - 'B': boolean
 * - 'I': integer
 * - 'D': double
 * - 'S': string
 */
class Val : public Object {
public:
	char type_;

	Val(char type) : type_(type) {}
	
	char getType() {
		return this->type_;
	}
	
	virtual Bool* asBool() {
		return nullptr;
	}
	
	virtual Int* asInt() {
		return nullptr;
	}
	
	virtual Double* asDouble() {
		return nullptr;
	}
	
	virtual Str* asStr() {
		return nullptr;
	}
};

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