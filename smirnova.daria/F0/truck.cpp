#include "truck.hpp"

namespace smirnova {

  Truck::Truck():
    nextStop_(0),
    started_(false)
  {}

  void Truck::setRoute(const std::vector< std::string >& stops, const std::string& depot)
  {
    route_ = stops;
    depot_ = depot;
    nextStop_ = 0;
    started_ = false;
  }

  void Truck::push(const Order& order)
  {
    cargo_.push(order);
  }

  bool Truck::routeReady() const
  {
    return !route_.empty();
  }

  bool Truck::empty() const
  {
    return cargo_.empty();
  }

  bool Truck::routeFinished() const
  {
    return nextStop_ >= route_.size();
  }

  std::string Truck::currentPosition() const
  {
    if (!started_ || route_.empty()) {
      return depot_;
    }
    return route_[nextStop_ - 1];
  }

  bool Truck::advance(std::ostream& out)
  {
    if (cargo_.empty() || routeFinished()) {
      return false;
    }

    const std::string& zone = route_[nextStop_];
    ++nextStop_;
    started_ = true;
    out << "<ARRIVED: " << zone << ">\n";

    while (!cargo_.empty() && cargo_.top().zone == zone) {
      const Order& order = cargo_.top();
      out << "<DELIVERED: " << order.item << " → " << order.clientName;
      out << " (" << order.phone << ")>\n";
      cargo_.pop();
    }

    if (routeFinished()) {
      out << "<ROUTE COMPLETE. RETURNING TO WAREHOUSE.>\n";
      route_.clear();
      nextStop_ = 0;
      started_ = false;
    } else {
      out << "<NEXT STOP: " << route_[nextStop_] << ">\n";
    }
    return true;
  }

  void Truck::print(std::ostream& out) const
  {
    if (cargo_.empty() && route_.empty()) {
      out << "<TRUCK IS EMPTY>\n";
      return;
    }

    out << "<TRUCK POSITION: " << currentPosition() << ">\n";

    if (!route_.empty()) {
      out << "<ROUTE: " << depot_;
      for (const std::string& stop : route_) {
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
    std::size_t index = 1;
    while (!remaining.empty()) {
      const Order& order = remaining.top();
      out << index << ". " << order.item << " → " << order.zone;
      out << " (" << order.clientName << ", " << order.phone << ")\n";
      ++index;
      remaining.pop();
    }
  }

}
