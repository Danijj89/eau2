#pragma once


#include "../util/string.h"
#include "../util/string_array.h"
#include "../network/node_info.h"
#include "../util/helper.h"
#include "../network/NodeInfoArray.h"

/**
 * Deserializer: This class will contain all the methods for
 * deserializing objects that has been serialized. Eventually, it will
 * contain all methods needed to deserialize a serialized dataframe.
 * It is used to recover objects packed into the body of the messages
 * sent between nodes. It will eventually support all deserialization
 * needs of the network layer. A Deserializer object is meant to be reused.
 *
 * Author: zhan.d@husky.neu.edu, cao.yuan1@husky.neu.edu
 */


class Deserializer {
public:

	/**
	 * This method deserializes a serialized bool.
	 * @method deserialize_bool
	 * @param  buff             pointer to buffer containing the int
	 * @return                  the recovered bool
	 */
	bool deserialize_bool(char* buff) {
		return (bool)buff[0];
	}

	/**
	 * This method deserializes a serialized char.
	 * @method deserialize_char
	 * @param  buff             pointer to buffer containing the char
	 * @return                  the recovered char
	 */
	char deserialize_char(char* buff) {
		return buff[0];
	}

	/**
	 * This method deserializes a serialized int.
	 * @method deserialize_int
	 * @param  buff            pointer to buffer containing the int
	 * @return                 the recovered int
	 */
	int deserialize_int(char* buff) {
		int* val = (int*)buff;
		return val[0];
	}

	/**
	 * This method deserializes a serialized float.
	 * @method deserialize_float
	 * @param  buff              pointer to buffer containing the float
	 * @return                   the recovered float
	 */
	float deserialize_float(char* buff) {
		float* val = (float*)buff;
		return val[0];
	}

	/**
	 * This method deserializes a serialized String.
	 * @method deserialize_float
	 * @param  buff              pointer to buffer containing the String
	 * @return                   the recovered String
	 */
	String* deserialize_string(char* buff) {
		String* s = new String(buff);
		return s;
	}

	/**
	 * This method deserializes a serialized StringArray.
	 * This method make use of the deserialize_string method.
	 * @method deserialize_string_array
	 * @param  buff                     pointer to buffer containing the StringArray
	 * @param  n                        the number of Strings in StringArray
	 * @return                          the recovered StringArray
	 */
	StringArray* deserialize_string_array(char* buff, size_t n) {
		StringArray* sa = new StringArray();
		size_t count = 0;
		for (size_t i = 0; i < n; ++i) {
			String* s = this->deserialize_string(&buff[count]);
			count += s->size() + 1; // for the '\0'
			sa->push_back(s);
		}
		return sa;
	}

	/**
	 * This method deserializes a serialized NodeInfo.
	 * @method deserialize_node_info
	 * @param  buff                  pointer to buffer containing the NodeInfo
	 * @return                       the recovered NodeInfo
	 */
	NodeInfo* deserialize_node_info(char* buff) {
		String* s = this->deserialize_string(buff);
		StringArray* tokens = tokenize(s->c_str(), ':');
		delete s;
		NodeInfo* ni = new NodeInfo(tokens->get(0)->clone(), atoi(tokens->get(1)->c_str()));
		delete tokens;
		return ni;
	}

	/**
	 * This method deserializes a serialized NodeInfoArray.
	 * @method deserialize_node_info_array
	 * @param  buff                        pointer to buffer containing the NodeInfoArray
	 * @param  n                           the number of NodeInfo in the NodeInfoArray
	 * @return                             the recovered NodeInfoArray
	 */
	NodeInfoArray* deserialize_node_info_array(char* buff, size_t n) {
		NodeInfoArray* nia = new NodeInfoArray();
		size_t count = 0;
		for (size_t i = 0; i < n; ++i) {
			NodeInfo* ni = this->deserialize_node_info(&buff[count]);
			count += ni->to_string()->size() + 1; // for the '\0'
			nia->pushBack(ni);
		}
		return nia;
	}
};