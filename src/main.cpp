#include "static_hash.h"
#include "record.h"
#include "hash_function.h"

using namespace sha2;

int main() {
    Property property("./index/metadata.dat",
                      "./index/my_index.dat",
                      40,
                      get_expected_bucket_capacity<Record>);

    sha256<int> hash;
    std::function<int(Record)> index = [&](const Record& record) {
        return record.id;
    };

    StaticHash<int, Record, std::equal_to<>, sha256<int>,  std::function<int(Record)>> static_hash(property, index, hash);

    return EXIT_SUCCESS;
}
