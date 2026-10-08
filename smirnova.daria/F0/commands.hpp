#ifndef SMIRNOVA_COMMANDS_HPP
#define SMIRNOVA_COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "hash_table.hpp"
#include "order_queue.hpp"
#include "string_hash.hpp"
#include "truck.hpp"
#include "types.hpp"
#include "zone_tree.hpp"

namespace smirnova {

  struct AppState {
    HashTable< std::string, Client, StringHash > clients;
    ZoneTree zones;
    OrderQueue orders;
    Truck truck;
    std::vector< std::string > plannedRoute;
    bool routePlanned;

    AppState():
      routePlanned(false)
    {}
  };

  using CommandHandler = void (*)(std::istream& in, std::ostream& out, AppState& state);

  void runSession(std::istream& in, std::ostream& out);

}

#endif
