#include "static_hash.h"
#include "record.h"
#include "hash_function.h"


using namespace sha2;

int main() {
    Property property(
            "./index/",
            "metadata.json",
            "my_index.dat",
            200,
            get_expected_bucket_capacity<Record>(),
            true
    );

    std::function<const char*(Record&)> get_indexed_field = [](Record& record) {
        return record.name;
    };

    std::function<bool(const char*, const char*)> equal_to = [](const char* str1, const char* str2){
        return !strcmp(str1, str2);
    };

    StaticHash<const char*, Record, decltype(equal_to), sha256<const char*>> static_hash(property, get_indexed_field, equal_to);

    // insert records
    for (int i = 1; i < 100'000; i++) {
        std::string s = std::string("P") + std::to_string(i);
        Record record(i, s.c_str(), 15);
        static_hash.insert(record);
    }

    // search records
    for (int i = 1; i < 100'000; i++) {
        std::string s = std::string("P") + std::to_string(i);

        for (Record& record: static_hash.search(s.c_str())) {
            std::cout << record << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
