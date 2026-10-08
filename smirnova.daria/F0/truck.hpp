#ifndef SMIRNOVA_TRUCK_HPP
#define SMIRNOVA_TRUCK_HPP

#include <cstddef>
#include <ostream>
#include <stack>
#include <string>
#include <vector>

#include "types.hpp"

namespace smirnova {

  class Truck {
  public:
    Truck();

    void setRoute(const std::vector< std::string >& stops, const std::string& depot);
    void push(const Order& order);

    bool routeReady() const;
    bool empty() const;

    bool advance(std::ostream& out);
    void print(std::ostream& out) const;

  private:
    std::stack< Order > cargo_;
    std::vector< std::string > route_;
    std::string depot_;
    std::size_t nextStop_;
    bool started_;

    bool routeFinished() const;
    std::string currentPosition() const;
  };

}

#endif
