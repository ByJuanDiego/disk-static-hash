#include "static_hash.h"
#include "record.h"
#include "hash_function.h"

using namespace sha2;

int main() {
    int hash_table_size = 4;

    Property property(
        "./index/metadata.json",
        "./index/my_index.dat",
        hash_table_size,
        get_expected_bucket_capacity<Record>,
        true
    );

    sha256<int> hash;
    std::function<int(Record)> index = [](const Record& record) {
        return record.id;
    };

    StaticHash<int, Record, std::equal_to<>, sha256<int>,  std::function<int(Record)>> static_hash(property, index, hash);
//    Record firstRecord(1, "JuanD", 15);
//    static_hash.insert(firstRecord);

//    for (const Record& record : static_hash.search(1)) {
//        std::cout << record << std::endl;
//    }

    return EXIT_SUCCESS;
}
