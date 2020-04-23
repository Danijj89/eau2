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
#include "network/network_tests.h" // done
#include "network/node_configuration_tests.h" // done
#include "network/node_info_array_tests.h" // low priority
#include "network/node_info_tests.h" // low priority
#include "network/node_tests.h" // done
#include "network/pollfds_tests.h" // low priority

#include "kvstore/key_array_tests.h" // done
#include "kvstore/key_tests.h" // done
#include "kvstore/kvstore_tests.h" // done
#include "kvstore/pair_tests.h" // done
#include "kvstore/value_tests.h" // done

#include "dataframe/dataframe_tests.h" // low priority
#include "dataframe/df_data_array_tests.h" // low priority
#include "dataframe/df_data_tests.h" // low priority
#include "dataframe/distributed_columns_tests.h" // low priority
#include "dataframe/fielder_tests.h" // low priority
#include "dataframe/row_tests.h" // done
#include "dataframe/rower_tests.h" // low priority
#include "dataframe/schema_tests.h" // done

#include "application/application_tests.h" // low priority
#include "application/reader_tests.h" // low priority
#include "application/writer_tests.h" // low priority

#include "application/wordcount/adder_tests.h" // low priority
#include "application/wordcount/filereader_tests.h" // low priority
#include "application/wordcount/summer_tests.h" // low priority
#include "application/wordcount/wordcount_tests.h" // low priority


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
