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

  // Orders wait here, in arrival order, until a route is planned and the
  // truck is loaded. extractByZone removes every order for one zone while
  // preserving the arrival order of everything that is left behind.
  class OrderQueue {
  public:
    void enqueue(const Order& order);

    bool empty() const;
    std::size_t size() const;

    bool hasOrdersForPhone(const std::string& phone) const;

    std::set< std::string > activeZones() const;
    HashTable< std::string, int, StringHash > countPerZone() const;

    std::vector< Order > extractByZone(const std::string& zone);

    void print(std::ostream& out) const;

  private:
    std::list< Order > orders_;
  };

}

#endif
