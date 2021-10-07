#include <unistd.h>
#include <iostream>
#include "vredis.h"
#include "util.h"

int
main(int argc, char **argv) {
    {
        vectordb::VRedis vredis("./test_hash");
        vredis.Create();
        std::string key = "key";
        vredis.HSet(key, "name", "bruce");
        vredis.HSet(key, "age", "30");

        std::string value;
        auto s = vredis.HGet(key, "name", value);
        assert(s.ok());
        std::cout << "hget key name: " << value << std::endl;

        s = vredis.HGet(key, "age", value);
        assert(s.ok());
        std::cout << "hget key age: " << value << std::endl;
    }

    {
        vectordb::VRedis vredis("./test_ttl");
        vredis.Create();

        auto b = vredis.SetTTL("kkk", "vvv", 5);
        assert(b);

        std::string value;
        auto s = vredis.GetTTL("kkk", value);
        std::cout << "get kkk: " << value << " " << s.ToString() << std::endl;

        sleep(7);

        s = vredis.GetTTL("kkk", value);
        std::cout << "get kkk: " << s.ToString() << std::endl;

        std::string real_value;
        s = vredis.db_data()->Get(leveldb::ReadOptions(), "kkk", &real_value);
        std::cout << "get kkk, real value: " << real_value << std::endl;

        vredis.db_data()->CompactRange(nullptr, nullptr);
        sleep(3);
        s = vredis.db_data()->Get(leveldb::ReadOptions(), "kkk", &real_value);
        std::cout << "get kkk, real value: " << s.ToString() << std::endl;
    }

    return 0;
}
