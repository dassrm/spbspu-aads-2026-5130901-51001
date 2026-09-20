#include "commands.hpp"

#include <iomanip>
#include <set>
#include <stdexcept>

namespace smirnova {

  namespace {

    // Thrown by a handler whenever a command cannot be carried out; caught
    // once, in executeCommand, and reported as "<INVALID COMMAND: ...>".
    class CommandError : public std::runtime_error {
    public:
      explicit CommandError(const std::string& message):
        std::runtime_error(message)
      {}
    };

    void requireGoodStream(std::istream& in) {
      if (!in) {
        throw CommandError("MALFORMED COMMAND");
      }
    }

    void handleRegClient(std::istream& in, std::ostream& out, AppState& state) {
      std::string phone;
      std::string name;
      std::string zone;
      in >> phone >> std::quoted(name) >> std::quoted(zone);
      requireGoodStream(in);

      if (!state.zones.exists(zone)) {
        throw CommandError("ZONE " + zone + " NOT FOUND");
      }
      Client client{phone, name, zone};
      if (!state.clients.insert(phone, client)) {
        throw CommandError("PHONE " + phone + " ALREADY EXISTS");
      }
      out << "<OK: CLIENT " << name << " REGISTERED IN ZONE " << zone << ">\n";
    }

    void handleDropClient(std::istream& in, std::ostream& out, AppState& state) {
      std::string phone;
      in >> phone;
      requireGoodStream(in);

      auto client = state.clients.find(phone);
      if (client == state.clients.end()) {
        throw CommandError("CLIENT " + phone + " NOT FOUND");
      }
      if (state.orders.hasOrdersForPhone(phone)) {
        throw CommandError("CLIENT " + phone + " HAS ACTIVE ORDERS");
      }
      state.clients.erase(phone);
      out << "<OK: CLIENT " << phone << " REMOVED>\n";
    }

    void handleAddWarehouse(std::istream& in, std::ostream& out, AppState& state) {
      std::string name;
      in >> std::quoted(name);
      requireGoodStream(in);

      ZoneAddResult result = state.zones.addZone("", name);
      if (result == ZoneAddResult::ZoneAlreadyExists) {
        throw CommandError("ZONE " + name + " ALREADY EXISTS");
      }
      if (result == ZoneAddResult::RootAlreadyExists) {
        throw CommandError("WAREHOUSE ALREADY EXISTS");
      }
      out << "<OK: ZONE " << name << " ADDED AS WAREHOUSE>\n";
    }

    void handleAddZone(std::istream& in, std::ostream& out, AppState& state) {
      std::string parent;
      std::string name;
      in >> std::quoted(parent) >> std::quoted(name);
      requireGoodStream(in);

      ZoneAddResult result = state.zones.addZone(parent, name);
      if (result == ZoneAddResult::ZoneAlreadyExists) {
        throw CommandError("ZONE " + name + " ALREADY EXISTS");
      }
      if (result == ZoneAddResult::ParentNotFound) {
        throw CommandError("ZONE " + parent + " NOT FOUND");
      }
      out << "<OK: ZONE " << name << " ADDED UNDER " << parent << ">\n";
    }

    void handleOrder(std::istream& in, std::ostream& out, AppState& state) {
      std::string phone;
      std::string item;
      in >> phone >> std::quoted(item);
      requireGoodStream(in);

      auto client = state.clients.find(phone);
      if (client == state.clients.end()) {
        throw CommandError("CLIENT " + phone + " NOT FOUND");
      }
      Order order{item, phone, client->second.name, client->second.zone};
      state.orders.enqueue(order);
      out << "<ADDED TO QUEUE: " << item << " → " << order.zone << " (" << order.clientName << ")>\n";
    }

    int totalItemsOnRoute(const std::vector< std::string >& route,
                           HashTable< std::string, int, StringHash >& counts) {
      int total = 0;
      for (const std::string& zone : route) {
        auto found = counts.find(zone);
        if (found != counts.end()) {
          total += found->second;
        }
      }
      return total;
    }

    void handlePlanRoute(std::istream&, std::ostream& out, AppState& state) {
      if (state.orders.empty()) {
        throw CommandError("QUEUE IS EMPTY");
      }

      std::set< std::string > targets = state.orders.activeZones();
      std::vector< std::string > route = state.zones.planRoute(targets);
      if (route.empty()) {
        throw CommandError("NO REACHABLE ZONES WITH ORDERS");
      }

      state.plannedRoute = route;
      state.routePlanned = true;

      HashTable< std::string, int, StringHash > counts = state.orders.countPerZone();
      int total = totalItemsOnRoute(route, counts);
      std::string depot = state.zones.rootName();

      out << "<ROUTE PLANNED: " << depot;
      for (const std::string& zone : route) {
        out << " → " << zone;
      }
      out << " → " << depot << ">\n";
      out << "<STOPS: " << route.size() << ", TOTAL ITEMS: " << total << ">\n";
    }

    void handleLoadTruck(std::istream&, std::ostream& out, AppState& state) {
      if (!state.routePlanned) {
        throw CommandError("ROUTE NOT PLANNED");
      }

      state.truck.setRoute(state.plannedRoute, state.zones.rootName());

      // Visit stops in reverse and, within a stop, orders in reverse
      // arrival order, so that pushing them one by one leaves the first
      // stop's earliest order on top of the stack.
      std::vector< Order > pushOrder;
      for (auto stop = state.plannedRoute.rbegin(); stop != state.plannedRoute.rend(); ++stop) {
        std::vector< Order > zoneOrders = state.orders.extractByZone(*stop);
        for (auto order = zoneOrders.rbegin(); order != zoneOrders.rend(); ++order) {
          pushOrder.push_back(*order);
        }
      }
      for (const Order& order : pushOrder) {
        state.truck.push(order);
      }
      state.routePlanned = false;

      out << "<TRUCK LOADED: " << pushOrder.size() << " items>\n";
      out << "<STACK top→bottom:";
      for (auto order = pushOrder.rbegin(); order != pushOrder.rend(); ++order) {
        out << " [" << order->item << "/" << order->zone << "]";
      }
      out << ">\n";
    }

    void handleNextStop(std::istream&, std::ostream& out, AppState& state) {
      if (state.truck.empty()) {
        throw CommandError("TRUCK IS EMPTY");
      }
      if (!state.truck.advance(out)) {
        throw CommandError("ROUTE ALREADY COMPLETE");
      }
    }

    void handleShowTruck(std::istream&, std::ostream& out, AppState& state) {
      state.truck.print(out);
    }

    void handleShowQueue(std::istream&, std::ostream& out, AppState& state) {
      state.orders.print(out);
    }

    void handleShowMap(std::istream&, std::ostream& out, AppState& state) {
      state.zones.printMap(state.orders.activeZones(), out);
    }

    HashTable< std::string, CommandHandler, StringHash > buildDispatchTable() {
      HashTable< std::string, CommandHandler, StringHash > table;
      table.insert("reg-client", &handleRegClient);
      table.insert("drop-client", &handleDropClient);
      table.insert("add-warehouse", &handleAddWarehouse);
      table.insert("add-zone", &handleAddZone);
      table.insert("order", &handleOrder);
      table.insert("plan-route", &handlePlanRoute);
      table.insert("load-truck", &handleLoadTruck);
      table.insert("next-stop", &handleNextStop);
      table.insert("show-truck", &handleShowTruck);
      table.insert("show-queue", &handleShowQueue);
      table.insert("show-map", &handleShowMap);
      return table;
    }

  }

  void runSession(std::istream& in, std::ostream& out) {
    AppState state;
    HashTable< std::string, CommandHandler, StringHash > dispatch = buildDispatchTable();

    std::string command;
    while (in >> command) {
      auto handler = dispatch.find(command);
      if (handler == dispatch.end()) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      try {
        handler->second(in, out, state);
      } catch (const CommandError& error) {
        out << "<INVALID COMMAND: " << error.what() << ">\n";
        in.clear();
      }
    }
  }

}
