#ifndef SMIRNOVA_TYPES_HPP
#define SMIRNOVA_TYPES_HPP

#include <string>

namespace smirnova {

  struct Client {
    std::string phone;
    std::string name;
    std::string zone;
  };

  struct Order {
    std::string item;
    std::string phone;
    std::string clientName;
    std::string zone;
  };

}

#endif
