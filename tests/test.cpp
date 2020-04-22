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

#include "network/message_tests.h"
#include "network/network_tests.h"
#include "network/node_configuration_tests.h" // done
#include "network/node_info_array_tests.h"
#include "network/node_info_tests.h"
#include "network/node_tests.h"
#include "network/pollfds_tests.h"

#include "kvstore/key_array_tests.h"
#include "kvstore/key_tests.h"
#include "kvstore/kvstore_tests.h"
#include "kvstore/pair_tests.h"
#include "kvstore/value_tests.h"

#include "dataframe/dataframe_tests.h"
#include "dataframe/df_data_array_tests.h"
#include "dataframe/df_data_tests.h"
#include "dataframe/distributed_columns_tests.h"
#include "dataframe/fielder_tests.h"
#include "dataframe/row_tests.h"
#include "dataframe/rower_tests.h"
#include "dataframe/schema_tests.h"

#include "application/application_tests.h"
#include "application/reader_tests.h"
#include "application/writer_tests.h"

#include "application/wordcount/adder_tests.h"
#include "application/wordcount/filereader_tests.h"
#include "application/wordcount/summer_tests.h"
#include "application/wordcount/wordcount_tests.h"


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
