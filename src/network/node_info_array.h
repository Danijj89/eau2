#pragma once

#include "node_info.h"
#include "../util/array.h"

/**
 * Array of node informations.
 */
class NodeInfoArray {
public:
	Array* array_; // owned

	/**
	 * Default constructor
	 */
	NodeInfoArray() { this->array_ = new Array(); }

	/**
	 * Default destructor
	 */
	~NodeInfoArray() { delete this->array_; }

	/**
	 * Gets the nodeinfo at the given index.
	 * @param i index
	 * @return the nodeinfo
	 */
	NodeInfo* get(size_t i) {
		NodeInfo* v = dynamic_cast<NodeInfo*>(this->array_->get(i));
		assert(v != nullptr);
		return v;
	}

	/**
	 * Sets the given nodeinfo at the given index.
	 * @param i idx
	 * @param v nodeinfo
	 */
	void set(size_t i, NodeInfo* v) { this->array_->set(i, v); }

	/**
	 * Pushes the given item to the end of this array.
	 * NOTE: nullptr are valid values
	 *
	 * @param item the given item to be added to the end of this array
	 */
	void pushBack(NodeInfo* s) { this->array_->pushBack(s); }

	/**
	 * Returns the size of this array
	 * @return size
	 */
	size_t size() { return this->array_->size(); }

	/**
	 * Returns the node with the same info as the given one.
	 * @param n node
	 * @return node in this array
	 */
	NodeInfo* find(NodeInfo* n) {
		return dynamic_cast<NodeInfo*>(this->array_->find(n));
	}

	/**
	 * Returns the index of the node with the same info as the given one.
	 * @param n node
	 * @return the index
	 */
	size_t indexOf(NodeInfo* n) {
		for (size_t i = 0; i < this->size(); ++i) {
			NodeInfo* curr = dynamic_cast<NodeInfo*>(this->array_->get(i));
			assert(curr != nullptr);
			if (curr->equals(n)) return i;
		}
		return SIZE_MAX;
	}

	String* serialize() {
		Serializer s = Serializer();
		StrBuff sb = StrBuff();
		sb.c(*s.serializeSizeT(this->size())->get());
		for (size_t i = 0; i < this->size(); i++) {
			NodeInfo* curr = dynamic_cast<NodeInfo*>(this->array_->get(i));
			assert(curr != nullptr);
			sb.c(*curr->serialize());
		}
		return sb.get();
	}

	static NodeInfoArray* deserialize(char* buff, size_t* counter) {
		NodeInfoArray* result = new NodeInfoArray();
		Deserializer d = Deserializer();
		size_t size = d.deserializeSizeT(buff, counter);
		for (size_t i = 0; i < size; i++) {
			result->pushBack(NodeInfo::deserialize(buff, counter));
		}
		return result;
	}
};