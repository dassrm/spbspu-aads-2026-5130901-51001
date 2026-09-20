#include "order_queue.hpp"

namespace smirnova {

  void OrderQueue::enqueue(const Order& order) {
    orders_.push_back(order);
  }

  bool OrderQueue::empty() const {
    return orders_.empty();
  }

  std::size_t OrderQueue::size() const {
    return orders_.size();
  }

  bool OrderQueue::hasOrdersForPhone(const std::string& phone) const {
    for (const Order& order : orders_) {
      if (order.phone == phone) {
        return true;
      }
    }
    return false;
  }

  std::set< std::string > OrderQueue::activeZones() const {
    std::set< std::string > zones;
    for (const Order& order : orders_) {
      zones.insert(order.zone);
    }
    return zones;
  }

  HashTable< std::string, int, StringHash > OrderQueue::countPerZone() const {
    HashTable< std::string, int, StringHash > counts;
    for (const Order& order : orders_) {
      auto it = counts.find(order.zone);
      if (it == counts.end()) {
        counts.insert(order.zone, 1);
      } else {
        ++(it->second);
      }
    }
    return counts;
  }

  std::vector< Order > OrderQueue::extractByZone(const std::string& zone) {
    std::vector< Order > result;
    for (auto it = orders_.begin(); it != orders_.end();) {
      if (it->zone == zone) {
        result.push_back(*it);
        it = orders_.erase(it);
      } else {
        ++it;
      }
    }
    return result;
  }

  void OrderQueue::print(std::ostream& out) const {
    if (orders_.empty()) {
      out << "<QUEUE IS EMPTY>\n";
      return;
    }
    out << "<QUEUE (" << orders_.size() << " items):>\n";
    int index = 1;
    for (const Order& order : orders_) {
      out << index << ". " << order.item << " → " << order.zone
          << " (" << order.clientName << ", " << order.phone << ")\n";
      ++index;
    }
  }

}
