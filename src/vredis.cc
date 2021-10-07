#include <cstdio>
#include "util.h"
#include "vredis.h"

namespace vectordb {

VRedis::VRedis(const std::string& path)
    :path_(path) {
}

VRedis::~VRedis() {
    delete db_meta_;
    delete db_data_;
}

bool
VRedis::Load() {

    return true;
}

bool
VRedis::Create() {
    auto b = util::RecurMakeDir2(path_);
    assert(b);

    leveldb::Options options;
    options.create_if_missing = true;
    //options.error_if_exists = true;

    auto s = leveldb::DB::Open(options, meta_path(), &db_meta_);
    assert(s.ok());

    s = leveldb::DB::Open(options, data_path(), &db_data_);
    assert(s.ok());

    return true;
}

bool
VRedis::HSet(const std::string &key, const std::string &hash_key, const std::string &hash_value) {
    std::string real_key = key + ":" + hash_key;
    auto s = db_data_->Put(leveldb::WriteOptions(), real_key, hash_value);
    assert(s.ok());
    return true;
}

leveldb::Status
VRedis::HGet(const std::string &key, const std::string &hash_key, std::string &hash_value) const {
    std::string real_key = key + ":" + hash_key;
    auto s = db_data_->Get(leveldb::ReadOptions(), real_key, &hash_value);
    return s;
}

bool
VRedis::SetTTL(const std::string &key, const std::string &value, uint64_t ttl) {
    uint64_t ts = time(nullptr) + ttl;
    char buf[64];
    snprintf(buf, sizeof(buf), "%llu", ts);

    std::string real_value = value + ":" + std::string(buf);
    auto s = db_data_->Put(leveldb::WriteOptions(), key, real_value);
    assert(s.ok());
    return true;
}

leveldb::Status
VRedis::GetTTL(const std::string &key, std::string &value) const {
    std::string real_value;
    auto s = db_data_->Get(leveldb::ReadOptions(), key, &real_value);
    if (s.IsNotFound()) {
        return s;

    } else if (s.ok()) {
        std::vector<std::string> sv;
        util::Split(real_value, ':', sv, "");
        assert(sv.size() == 2);

        uint64_t ts;
        sscanf(sv[1].c_str(), "%llu", &ts);

        if (time(nullptr) < ts) {
            value = sv[0];
            return leveldb::Status::OK();
        } else {
            return leveldb::Status::NotFound(key, "ttl expired");
        }

    } else {
        assert(0);
    }

    return leveldb::Status::OK();
}

} // namespace vectordb
