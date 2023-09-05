#include "static_hash.h"
#include "record.h"

int main() {
    Property property("./index/metadata.dat",
                      "./index/my_index.dat",
                      40,
                      get_expected_bucket_capacity<Record>);

    StaticHash<int, Record> static_hash(property);

    return EXIT_SUCCESS;
}
