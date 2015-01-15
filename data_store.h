#ifndef  _AFF4_DATA_STORE_H_
#define  _AFF4_DATA_STORE_H_

// This file defines the AFF4 data store abstraction.
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include "aff4_utils.h"
#include <string.h>
#include "rdf.h"

using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::ofstream;
using std::ifstream;


// Forward declerations for basic AFF4 types.
class AFF4Object;
class AFF4Stream;
class AFF4Volume;

enum DataStoreObjectWireType {
  STRING,
  INTEGER
};

/* These are the objects which are stored in the data store */
class DataStoreObject {
 public:
  // The data store can only store the following primitive types.
  int int_data = 0;
  string string_data;
  DataStoreObjectWireType wire_type = STRING;

  DataStoreObject(int data): int_data(data), wire_type(INTEGER) {};
  DataStoreObject(string data): string_data(data), wire_type(STRING) {};
  DataStoreObject(){};
};

// AFF4_Attributes are a collection of data store objects, keyed by attributes.
typedef unordered_map<string, unique_ptr<RDFValue> > AFF4_Attributes;


/** The abstract data store.

    Data stores know how to serialize RDF statements of the type:

    subject predicate value

    Where both subject and predicate are a URN into the AFF4 space, and value is
    a serialized RDFValue.
 */
class DataStore {
 public:
  virtual void Set(const URN &urn, const URN &attribute,
                   RDFValue *value) = 0;

  virtual AFF4Status Get(const URN &urn, const URN &attribute, RDFValue &value) = 0;

  // Dump ourselves to a yaml file.
  virtual AFF4Status DumpToYaml(AFF4Stream &output) = 0;
  virtual AFF4Status DumpToTurtle(AFF4Stream &output) = 0;
};


/** A purely in memory data store.

    This data store can be initialized and persisted into a Yaml file.
*/
class MemoryDataStore: public DataStore {

 private:
  // Store a collection of AFF4_Attributes at each URN.
  unordered_map<string, AFF4_Attributes> store;

 public:
  void Set(const URN &urn, const URN &attribute,
           RDFValue *value);
  AFF4Status Get(const URN &urn, const URN &attribute, RDFValue &value);

  virtual AFF4Status DumpToYaml(AFF4Stream &output);
  virtual AFF4Status DumpToTurtle(AFF4Stream &output);
};

extern MemoryDataStore oracle;

#endif //  _AFF4_DATA_STORE_H_