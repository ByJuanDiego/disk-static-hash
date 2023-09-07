#include "static_hash.h"
#include "record.h"
#include "hash_function.h"


using namespace sha2;

int main() {
    Property property(
            "./index/",
            "metadata.json",
            "my_index.dat",
            100,
            get_expected_bucket_capacity<Record>,
            true
    );

    std::function<int(Record)> index = [](const Record& record) {
        return record.id;
    };

    StaticHash<int, Record, std::equal_to<>, sha256<int>> static_hash(property, index);

    // insert records
     for (int i = 1; i < 100'000; i++) {
        Record record(i, (std::string("P") + std::to_string(i)).c_str(), 15);
        static_hash.insert(record);
     }

    // remove records
    for (int i = 99'000; i < 100'000; ++i) {
        std::cout << i << std::endl;
        static_hash.remove(i);
    }

    // search records
    for (int i = 1; i < 99'000; ++i) {
        std::vector<Record> records = static_hash.search(i);
    }

    int j = 0;
    for (int i = 99'000; i < 100'000; ++i) {
        try {
            static_hash.search(i);
        } catch (std::exception& e) {
            ++j;
        }
    }
    std::cout << j << std::endl;

    return EXIT_SUCCESS;
}
