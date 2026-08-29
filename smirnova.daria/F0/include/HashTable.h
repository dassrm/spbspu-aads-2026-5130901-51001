#pragma once
#include <string>
#include <list>
#include "types.h"

class HashTable {
    static const int CAPACITY = 64;
    std::list<Client> buckets[CAPACITY];

    int hash(const std::string& key) const;

public:
    bool insert(const std::string& phone, const std::string& name, const std::string& zone);

    bool remove(const std::string& phone);

    Client* find(const std::string& phone);
};
