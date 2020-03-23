# Introduction

Eau2 is a platform that is built on top of a distributed key-value store.
It allows users to perform analysis on data in terms of a dataframe
abstraction. The system is designed to handle arbitrary sized data by
distributing storage across several storage nodes. The distributed nature (memory manipulation and networking logic) of the system is hidden from the
users, who primarily work with dataframes and save intermediate results by
assigning keys to the resulting dataframes.

To run the prepared example program that comes with the repository, please use
`make docker_run`. To run the tests and memory check, please use
`make valgrind`.

# Architecture

We envisioned a three-layered architecture:
- an Application layer on the top
- a middle layer made out of DataFrames
- a distributed key-value store on the bottom layer

## Application Layer

The application layer provides a high level API for the user to interact with the system.
Among the key functionality that it will support, we have:
- a way to configure the cluster underneath (e.g. number of nodes)
- a mechanism to load data into the system
- a mechanism to store/retrieve data
- an overridable function that allows users to implement their logic


### System Configuration

To configure the underlying system that gets started, the Application will
take in command line arguments at the start-up of the application. Among the
configurations, we have the number of nodes in the cluster, max chunk sizes,
max number of chunks.

### Loading Data

We only support the SOR file format. We will integrate our previously built
sorer (a parser for .sor files). To distribute the data over the network and
store it in different nodes,
we will define two constants:
- the size of a standard chunk of data
- the number of elements in a chunk of data for each type
- the maximum number of chunks a node can store

Then, the data will be distributed according to the following methodology:
- each column of the .sor file will be divided into chunks of fixed numbers
of elements (last chuck might be smaller)
- Each chunk will be associated a programmatically generated Key
- For each chuck in each column (start with first column and send out all
chunks in that columnbefore moving to column 2), send the max number of chucks
that a node can hold to each node

The reason the order of data is as described is to attempt to keep as much data
in the same place as possible to reduce network traffic when fetching, respect
NUMA nature of current systems, and lay ground work for effective distributed
computing.

### Store & Retrieve Data

The user is largely unaware of the storage and retrieval strategy. We assume
that all the data is initially located on one machine. When the users create a
DataFrame object based the the .sor file that contains the data, the data is
automatically distributed through the key-value store.

The user can associate a key/name with a dataframe in a similar manner as
declaring a variable, although with a different syntac. Unbeknownst to the user
this put the DataFrame object into the key-value store, and thus allow all
nodes in the network to access the DataFrame object and work with the
information stored in the DataFrame object. Thus, the user can use keys/names
of DataFrame objects in his or her application like variables names known to
all nodes.

### User Defined Logic

The Application provides an overridable function where users can to implement
and run their own business logic on the data. To implement distributed computation, each node in the network comes with associated identifier (e.g.
an integer) that the user can use to run computation on that specific node.

## Dataframe (Middle-Layer)

The DataFrame provides dataframe functionalities. Moreover, it expose to the
user methods to save the value of a DataFrame to a key/name. With the key/name
a DataFrame object can be recreated on the same node or in a different node.

A DataFrame is made up of columns. A column is a distributed array. A column
creates the illusion that the column is built on top of a plain array. In
addition, it has a pushBack method to append new values to the column during
the creation of the DataFrame. Updates are not support to simplify
implementation. A column also includes a cache to mitigate the impact of its
distributed nature.

To support flexible use of DataFrame objects, we created a number of ways for
users to create DataFrame objects (from an array of allowable objects,
from reading in data from a SOR file, etc).

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
DataFrame::createDataframeFromSor(char* filepath);

// A method called to start the program and which calls execute();
void run_();

// A user overridable function to perform business logic
void execute_();
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

// Cache slots for a Value
Value* cache;

// Type of data allowed to be stored within the distributed array
SerDesTypes type;
```

DA methods include:
```c
bool getBool(size_t col, size_t row);

int getInt(size_t col, size_t row);

float getFloat(size_t col, size_t row);

String* getString(size_t col, size_t row);
```

### Dataframe

On top of dataframe functionalities (which are the same as the one we
previously built), Dataframe provides ways to store/retrieve data of the form
of Dataframe.

In addition to our previously built methods, we have:
```c
// to build a dataframe from a unique string identifier
Dataframe::fromKey(char* key);
```

## KVStore

The Key-Value Store is responsible for all the network interactions,
serialization/deserialization, and data storage on the form of blobs.

Notable fields on this class include:
```c
// Id of the Key Value Store node_infos_
int id_;

// Holding all the keys
KeyArray* keys_;

// The local cache containing the keys & values of the cached blobs
ValueArray* values_;
```

The methods include:
```c
// Get chunks of data in their correct column type given their associated key
void put(Key* k, Value* v);
Value* get(Key* k);
Value* waitAndGet(Key* k);
```

# Use Cases

To use our program, the user will create a class that extends our Application
class and override the its execute() method. In the execute method the user
will put the business logic that he/she wants to perform on the data.
The flow of the execute method depends on if the user wants the computation to
be distribuited or on a single node.

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
    df->storeAs(Key("sum"));
    Dataframe* df2 = Dataframe::fromKey(Key("sum"));
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

We currently have a working version of the our sorer parser and dataframe, as
well as the adapter. We also have some basic networking layer implemented. The
work that remains to be done includes:
- building the KV store and the networking logic associated with it
(being able to handle large size of data is likely the most difficult part of it)
- building the distributed array which contains many arithmetic logic to
properly fetch the correct data
- modify the Dataframe class to incorporate distributed array
- implement the application layer
- create a demo to test our program
- write tons of tests

### Estimated Time of Implementation

Distributed KV store: 40+ hours

Implementing DA & Dataframe modifications: 40~ hours

Application layer implementation + demo: 30~ hours

Possible ML algorithms (random forest)/ 7 Degrees of Linus: 20~ hours

Overall, the amount of work will be around 140 hours.
However, since it is usual to underestimate project times,
the time might end up well above the estimate.
