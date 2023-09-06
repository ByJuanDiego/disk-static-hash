#include "static_hash.h"
#include "record.h"
#include "hash_function.h"

using namespace sha2;

int main() {
    int hash_table_size = 20;
    std::string directory_path = "./index/";
    std::string metadata_file_name = "metadata.json";
    std::string index_file_name = "my_index.dat";
    bool is_primary_key = true;

    Property property(
        directory_path,
        metadata_file_name,
        index_file_name,
        hash_table_size,
        get_expected_bucket_capacity<Record>,
        is_primary_key
    );

    sha256<int> hash;
    std::function<int(Record)> index = [](const Record& record) {
        return record.id;
    };

    StaticHash<int, Record, std::equal_to<>, sha256<int>,  std::function<int(Record)>> static_hash(property, index, hash);

    Record firstRecord(1, "Juan Diego", 15);
    static_hash.insert(firstRecord);

    std::vector<Record> records = static_hash.search(1);
    for (const Record& record : records) {
        std::cout << record << std::endl;
    }

    return EXIT_SUCCESS;
}
