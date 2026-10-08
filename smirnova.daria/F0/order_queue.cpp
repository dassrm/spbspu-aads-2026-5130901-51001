#include "order_queue.hpp"

namespace smirnova {

  void OrderQueue::enqueue(const Order& order)
  {
    orders_.push_back(order);
  }

  bool OrderQueue::empty() const
  {
    return orders_.empty();
  }

  std::size_t OrderQueue::size() const
  {
    return orders_.size();
  }

  bool OrderQueue::hasOrdersForPhone(const std::string& phone) const
  {
    for (const Order& order : orders_) {
      if (order.phone == phone) {
        return true;
      }
    }
    return false;
  }

  std::set< std::string > OrderQueue::activeZones() const
  {
    std::set< std::string > zones;
    for (const Order& order : orders_) {
      zones.insert(order.zone);
    }
    return zones;
  }

  HashTable< std::string, std::size_t, StringHash > OrderQueue::countPerZone() const
  {
    HashTable< std::string, std::size_t, StringHash > counts;
    for (const Order& order : orders_) {
      auto found = counts.find(order.zone);
      if (found == counts.end()) {
        counts.insert(order.zone, 1);
      } else {
        ++found->second;
      }
    }
    return counts;
  }

  std::vector< Order > OrderQueue::extractByZone(const std::string& zone)
  {
    std::vector< Order > result;
    auto it = orders_.begin();
    while (it != orders_.end()) {
      if (it->zone == zone) {
        result.push_back(*it);
        it = orders_.erase(it);
      } else {
        ++it;
      }
    }
    return result;
  }

  void OrderQueue::print(std::ostream& out) const
  {
    if (orders_.empty()) {
      out << "<QUEUE IS EMPTY>\n";
      return;
    }
    out << "<QUEUE (" << orders_.size() << " items):>\n";
    std::size_t index = 1;
    for (const Order& order : orders_) {
      out << index << ". " << order.item << " → " << order.zone;
      out << " (" << order.clientName << ", " << order.phone << ")\n";
      ++index;
    }
  }

}
