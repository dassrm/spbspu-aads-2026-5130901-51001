#ifndef SMIRNOVA_TRUCK_HPP
#define SMIRNOVA_TRUCK_HPP

#include <cstddef>
#include <ostream>
#include <stack>
#include <string>
#include <vector>

#include "types.hpp"

namespace smirnova {

  // Cargo is a stack: whatever was loaded last comes off first. load-truck
  // fills it so that the first stop's orders end up on top.
  class Truck {
  public:
    Truck();

    void setRoute(const std::vector< std::string >& stops, const std::string& depot);

    bool routeReady() const;
    bool empty() const;

    void push(const Order& order);

    // Moves to the next stop and delivers every order addressed to that
    // zone. Returns false when there is nothing left to do: no cargo, or
    // the route has already been completed.
    bool advance(std::ostream& out);

    void print(std::ostream& out) const;

  private:
    std::stack< Order > cargo_;
    std::vector< std::string > route_;
    std::string depot_;
    int currentStop_;
  };

}

#endif
