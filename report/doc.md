# Introduction

Eau2 is a distributed key-value store that provides a dataframe-like based API 
to perform operations on the stored data. The system can handle arbitrary sized 
data and its distributed nature (memory manipulation and networking logic) 
is hidden from the user, which is only aware of the existence of a key-value store 
underneath and has to implement the logic that he/she wants to perform on the data 
based on this nature.

# Architecture

We envisioned a three-layered architecture:
- an Application layer on the top
- a middle layer made out of Distributed Dataframes
- a Key-Value Store on the bottom layer 

## Application Layer

The application layer provides a high level API for the user to interact with the system. 
Among the key functionality that it will support, we have:
- a way to configure the cluster underneath (e.g. number of nodes)
- a mechanism to load data into the system
- a mechanism to store/retrieve data
- an overridable function that allows users to implement their logic


### System Configuration

To configure the underlying system that gets started, the Application will take in 
command line arguments at the start-up of the application. Among the configurations, 
we have the number of nodes in the cluster, max chunk sizes, max number of chunks.

### Loading Data

The data format that we are required to load will be in .sor format. 
To support this, we will integrate our previously built sorer (a parser for .sor files). 
To distribute the data over the network and store it in different nodes, 
we will define two constants:
- the number of elements in a chunk of data
- the maximum number of chunks a node can store

Then, the data will be distributed according to the following methodology:
- each column of the .sor file will be divided into chunks of fixed numbers 
of elements (last chuck might be smaller)
- Each chunk will be associated a Key
- For each chuck in each column (start with column 1 and send 
out all column 1 chunks before moving to column 2), 
send the max number of chucks that a node can hold to each node

The reason the order of data is as described is to attempt to keep as much data
in the same place as possible to reduce network traffic when fetching, respect
NUMA nature of current systems, and lay ground work for effective distributed
computing.

### Store & Retrieve Data

Storage & retrieval functionality will be provided through the
Dataframe class and uses a two-layer of key-values approach. 
The first layer will be used directly by the user to associate keys to Dataframes, 
while the second layer will be used by a Dataframe internally to fetch the data 
associated with a specific chunk of its data.
This idea will be hidden from the user who will interact with a Dataframe 
as usual using rows and column indexes.

### User Defined Logic

The Application provides an overridable function where users can to implement 
and run their own business logic on the data. To implement distributed computation, 
each node in the network comes with associated identifier (e.g. an integer) 
that the user can use to run computation on that specific node.

## Dataframe (Middle-Layer)

The Dataframe provides dataframe functionalities. Moreover, it expose to the user
methods to build from and save as (Put/Get) DataFrames through unique string identifiers.
However, although the Dataframe is aware of its distributed nature, 
it does not handle it directly. 
Instead, the data is managed by a Distributed Array. 
The Distributed Array (DA) stores Keys that each point to a chunk of data of 
the original data. To retrieve data using these Keys, 
the DA has access to the lower level Key-Value Store.
To mitigate the computational expense of deserializing data, 
the DA has an in-built local cache to store deserialized chunks of data. 

## Key-Value Store

The Key-Value Store encapsulates the storage of the blobs, 
eventual caching to minimize network traffic and all the networking logic 
required to connect, send, and receive data over the cluster.


# Implementation

## Application Layer

To implement this layer, we envisioned the following classes.

### Key

The Key class will store the following metadata:
- a unique string identifier
- the node location of the data the key refers to.

These metadata will be set by the constructor, and it will provide basic getters
(and maybe setters) to retrieve these information.

### Application

The Application class has the following fields:
```c
// the unique id of the node
int id;

// the Key-Value Store that is initialized by this application.
// It is stored in the application level so to have a single point of deletion,
// but it is only used in Dataframes.
KVStore store;
```

Among the methods, we have:
```c
// creates a dataframe given a .sor file
DataFrame createDataframeFromSor(char* filepath);

// A method called to start the program and which calls execute();
void run();

// A user overridable function to perform business logic
void execute();
```
## Middle-Layer

Classes in the middle-layer include:

### Distributed Array (DA)

The Distributed Array manages the distributed nature of a Dataframe.
The fields in a DA are:
```c
// an ordered array of keys that is a subset of all keys in the Key-Value Store
KeyArray* keys;

// the Key-Value Store of the node
KVStore* store;

// the types of each column in the dataframe to retrieve data with correct type
TypesArray* Schema;

// Cache slots for deserialized data
Column** cache;
```

DA methods include:
```c
bool get_bool(size_t col, size_t row);

int get_int(size_t col, size_t row);

float get_float(size_t col, size_t row);

String* get_string(size_t col, size_t row);
```

### Dataframe

On top of dataframe functionalities (which are the same as the one we previously built),
Dataframe provides ways to store/retrieve data on the form of Dataframe.
Another major difference is that instead of storing the data directly, it requests
data from a Distributed Array which is stored as a field of the class.

Thus, among the other fields of a Dataframe, we also have:
```c
DistributedArray* da;
```

and among the other methods, we have:
```c
// to build a dataframe from a unique string identifier
Dataframe::fromKey(char* key);

// to store the current dataframe with the given unique identifier
void storeAs(char* key);
```

## KVStore

The Key-Value Store is responsible for all the network interactions, 
serialization/deserialization, and data storage on the form of blobs.

The fields on this class include:
```c
// the current key/values that this KVStore stores
// this map stores keys referring to both data blobs as well as dataframes
KeyValueMap* map;

// The local cache containing the keys & values of the cached blobs
KeyValueMap* cache;
```
Note: the KeyValueMap are simple Hashmaps.

The methods include:
```c
// Get chunks of data in their correct column type given their associated key
BoolColumn* get_bool_column(Key key);
IntColumn* get_int_column(Key key);
FloatColumn* get_float_column(Key key);
StringColumn* get_String_column(Key key);

// Get a dataframe associated to the given key
DataFrame* get_dataframe(Key key);

// Stores a dataframe with associated key in the store
bool put_dataframe(Key, Dataframe*);

// Stores columns with associated keys in the store
bool put_bool_column(Key key, BoolColumn* col);
bool put_int_column(Key key, IntColumn* col);
bool put_float_column(Key key, FloatColumn* col);
bool put_String_column(Key key, StringColumn* col);
```

# Use Cases

To use our program, the user will create a class that extends our Application class
and override the its execute() method.
In the execute method the user will put the business logic that he/she wants to perform
on the data. 
The flow of the execute method depends on if the user wants the computation to be
distribuited or on a single node.

Single node computation example:
```c
// In DemoApplication class

void execute() override {
    Dataframe* df = this->createDataframeFromSor("test.sor", "df");
    long long sum = 0;
    for (size_t i = 0; i < df->width(); i++) {
        for (size_t j = 0; j < df->length(); j++) {
            sum += df->get_int(i, j);
        }   
    }
    df->storeAs("sum");
    Dataframe* df2 = Dataframe::fromKey("sum");
    assert(sum == df2->get_int(0,0));
}
```

Distributed computation example:
```c
// In DistributedDemoApplication class

void execute() override {
    switch (this->id) {
        case 1:
            this->sum();
            break;
	case 2:
            this->compare();
            break;
        default:
            assert(false);
    }
}

void sum() {
    Dataframe* df = this->createDataframeFromSor("test.sor", "df");
    long long sum = 0;
    for (size_t i = 0; i < df->width(); i++) {
    	for (size_t j = 0; j < df->length(); j++) {
    		sum += df->get_int(i, j);
    	}   
    }
    df->storeAs("sum");
}

void compare() {
    Dataframe* df = Dataframe::fromKey("df");
    long long sum2 = 0;
    for (size_t i = 0; i < df->width(); i++) {
    	for (size_t j = 0; j < df->length(); j++) {
    		sum2 += df->get_int(i, j);
    	}   
    }
    Dataframe* df2 = Dataframe::fromKey("sum");
    assert(sum2 == sum);
}
```

# Open Questions

- Do we need to handle both distributed storage and distributed computation?
- Is there a good way of determining the size for each chuck?
- Should we be concerned of oversized data that do not fit in our system? Should we
implement a system to check it?
- What would be a reasonable assumption of the amount of data that each node should be able to store?

# Status


We currently have a working version of the our sorer parses and dataframe, as well as the adapter.
We also have some basic networking layer implemented. The work that remains to be done includes:
- building the KV store and the networking logic associated with it 
(being able to handle large size of data is likely the most difficult part of it)
- building the distributed array which contains many arithmetic logic to properly fetch
the correct data
- modify the Dataframe class to incorporate distributed array
- implement the application layer
- create a demo to test our program
- write tons of tests

### Estimated Time of Implementation

Distributed KV store: 40+ hours

Implementing DA & Dataframe modifications: 40~ hours

Application layer implementation + demo: 30~ hours

Possible ML algorithms (random forest): 20~ hours

Overall, the amount of work will be around 140 hours. 
However, since it is usual to underestimate project times, 
the time might end up well above the estimate.
