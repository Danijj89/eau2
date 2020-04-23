#include "common_header.h"


#include "util/arg_parser_tests.h" // done
#include "util/array_tests.h" // done
#include "util/helper_tests.h" // done
#include "util/lock_tests.h" // done
#include "util/object_tests.h" // done
#include "util/string_tests.h" // done
#include "util/sys_tests.h" // done

#include "sorer/argument_parser_tests.h" // low priority
#include "sorer/field_array_tests.h" // low priority
#include "sorer/sorer_tests.h" // low priority
#include "sorer/types_array_tests.h" // low priority

#include "serialization/serializer_tests.h" // done
#include "serialization/deserializer_tests.h" // done

#include "network/message_tests.h" // done
#include "network/network_tests.h" //initial blank test
#include "network/node_configuration_tests.h" // done
#include "network/node_info_array_tests.h" //initial blank test
#include "network/node_info_tests.h" //initial blank test
#include "network/node_tests.h" //initial blank test
#include "network/pollfds_tests.h" //initial blank test

#include "kvstore/key_array_tests.h" //initial blank test
#include "kvstore/key_tests.h" //initial blank test
#include "kvstore/kvstore_tests.h" //initial blank test
#include "kvstore/pair_tests.h" //initial blank test
#include "kvstore/value_tests.h" //initial blank test

#include "dataframe/dataframe_tests.h" //initial blank test
#include "dataframe/df_data_array_tests.h" //initial blank test
#include "dataframe/df_data_tests.h" //initial blank test
#include "dataframe/distributed_columns_tests.h" //initial blank test
#include "dataframe/fielder_tests.h" //initial blank test
#include "dataframe/row_tests.h" //initial blank test
#include "dataframe/rower_tests.h" //initial blank test
#include "dataframe/schema_tests.h" //initial blank test

#include "application/application_tests.h" //initial blank test
#include "application/reader_tests.h" //initial blank test
#include "application/writer_tests.h" //initial blank test

#include "application/wordcount/adder_tests.h" //initial blank test
#include "application/wordcount/filereader_tests.h" //initial blank test
#include "application/wordcount/summer_tests.h" //initial blank test
#include "application/wordcount/wordcount_tests.h" //initial blank test


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
