#include "truck.hpp"

namespace smirnova {

  Truck::Truck():
    currentStop_(-1)
  {}

  void Truck::setRoute(const std::vector< std::string >& stops, const std::string& depot) {
    route_ = stops;
    depot_ = depot;
    currentStop_ = -1;
  }

  bool Truck::routeReady() const {
    return !route_.empty();
  }

  bool Truck::empty() const {
    return cargo_.empty();
  }

  void Truck::push(const Order& order) {
    cargo_.push(order);
  }

  bool Truck::advance(std::ostream& out) {
    int lastIndex = static_cast< int >(route_.size()) - 1;
    if (cargo_.empty() || currentStop_ >= lastIndex) {
      return false;
    }

    ++currentStop_;
    const std::string& zone = route_[static_cast< std::size_t >(currentStop_)];
    out << "<ARRIVED: " << zone << ">\n";

    while (!cargo_.empty() && cargo_.top().zone == zone) {
      const Order& order = cargo_.top();
      out << "<DELIVERED: " << order.item << " → " << order.clientName << " (" << order.phone << ")>\n";
      cargo_.pop();
    }

    if (currentStop_ < lastIndex) {
      out << "<NEXT STOP: " << route_[static_cast< std::size_t >(currentStop_ + 1)] << ">\n";
    } else {
      out << "<ROUTE COMPLETE. RETURNING TO WAREHOUSE.>\n";
      route_.clear();
      currentStop_ = -1;
    }
    return true;
  }

  void Truck::print(std::ostream& out) const {
    if (cargo_.empty() && route_.empty()) {
      out << "<TRUCK IS EMPTY>\n";
      return;
    }

    std::string position = depot_;
    if (currentStop_ >= 0 && currentStop_ < static_cast< int >(route_.size())) {
      position = route_[static_cast< std::size_t >(currentStop_)];
    }
    out << "<TRUCK POSITION: " << position << ">\n";

    if (!route_.empty()) {
      out << "<ROUTE: " << depot_;
      for (const auto& stop : route_) {
        out << " → " << stop;
      }
      out << " → " << depot_ << ">\n";
    }

    if (cargo_.empty()) {
      out << "<CARGO IS EMPTY>\n";
      return;
    }

    std::stack< Order > remaining = cargo_;
    out << "<CARGO (top→bottom):>\n";
    int index = 1;
    while (!remaining.empty()) {
      const Order& order = remaining.top();
      out << index << ". " << order.item << " → " << order.zone
          << " (" << order.clientName << ", " << order.phone << ")\n";
      ++index;
      remaining.pop();
    }
  }

}
