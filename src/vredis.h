#ifndef __VREDIS_H__
#define __VREDIS_H__

#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <leveldb/db.h>

namespace vectordb {

class VRedis {
  public:
    VRedis(const std::string& path);
    ~VRedis();
    VRedis(const VRedis&) = delete;
    VRedis& operator=(const VRedis&) = delete;

    std::string meta_path() const {
        return path_ + "/meta";
    }

    std::string data_path() const {
        return path_ + "/data";
    }

    leveldb::DB* db_data() {
        return db_data_;
    }

    bool Load();
    bool Create();

    bool HSet(const std::string &key, const std::string &hash_key, const std::string &hash_value);
    leveldb::Status HGet(const std::string &key, const std::string &hash_key, std::string &hash_value) const;

    bool SetTTL(const std::string &key, const std::string &value, uint64_t ttl);
    leveldb::Status GetTTL(const std::string &key, std::string &value) const;

  private:
    std::string path_;
    leveldb::DB* db_meta_;
    leveldb::DB* db_data_;
};

} // namespace vectordb

#endif
