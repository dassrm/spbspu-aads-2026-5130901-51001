#include "HashTable.h"

int HashTable::hash(const std::string& key) const {
    unsigned int h = 0;
    for (char c : key)
        h = h * 31 + static_cast<unsigned char>(c);
    return h % CAPACITY;
}

bool HashTable::insert(const std::string& phone, const std::string& name, const std::string& zone) {
    int idx = hash(phone);
    for (const auto& c : buckets[idx])
        if (c.phone == phone) return false;
    buckets[idx].push_back({phone, name, zone});
    return true;
}

bool HashTable::remove(const std::string& phone) {
    int idx = hash(phone);
    for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
        if (it->phone == phone) {
            buckets[idx].erase(it);
            return true;
        }
    }
    return false;
}

Client* HashTable::find(const std::string& phone) {
    int idx = hash(phone);
    for (auto& c : buckets[idx])
        if (c.phone == phone) return &c;
    return nullptr;
}
