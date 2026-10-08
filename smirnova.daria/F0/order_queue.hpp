#ifndef SMIRNOVA_ORDER_QUEUE_HPP
#define SMIRNOVA_ORDER_QUEUE_HPP

#include <cstddef>
#include <list>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "hash_table.hpp"
#include "string_hash.hpp"
#include "types.hpp"

namespace smirnova {

  class OrderQueue {
  public:
    void enqueue(const Order& order);

    bool empty() const;
    std::size_t size() const;
    bool hasOrdersForPhone(const std::string& phone) const;

    std::set< std::string > activeZones() const;
    HashTable< std::string, std::size_t, StringHash > countPerZone() const;

    std::vector< Order > extractByZone(const std::string& zone);

    void print(std::ostream& out) const;

  private:
    std::list< Order > orders_;
  };

}

#endif
