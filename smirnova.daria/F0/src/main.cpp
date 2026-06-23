#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include "types.h"
#include "HashTable.h"
#include "ZoneTree.h"
#include "OrderQueue.h"
#include "Truck.h"
#include "parser.h"

int main() {
    HashTable  clients;
    ZoneTree   zones;
    OrderQueue orderQueue;
    Truck      truck;

    std::vector<std::string> plannedRoute;
    bool routePlanned = false;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        auto args = parseLine(line);
        if (args.empty()) continue;

        const std::string& cmd = args[0];

        if (cmd == "reg-client") {
            if (args.size() != 4) { std::cout << "<INVALID COMMAND>\n"; continue; }
            const auto& phone = args[1], &name = args[2], &zone = args[3];

            if (!zones.exists(zone))
                std::cout << "<INVALID COMMAND: ZONE " << zone << " NOT FOUND>\n";
            else if (!clients.insert(phone, name, zone))
                std::cout << "<INVALID COMMAND: PHONE " << phone << " ALREADY EXISTS>\n";
            else
                std::cout << "<OK: CLIENT " << name << " REGISTERED IN ZONE " << zone << ">\n";
        }

        else if (cmd == "drop-client") {
            if (args.size() != 2) { std::cout << "<INVALID COMMAND>\n"; continue; }
            const auto& phone = args[1];

            if (!clients.find(phone))
                std::cout << "<INVALID COMMAND: CLIENT " << phone << " NOT FOUND>\n";
            else if (orderQueue.hasOrdersForPhone(phone))
                std::cout << "<INVALID COMMAND: CLIENT " << phone << " HAS ACTIVE ORDERS>\n";
            else {
                clients.remove(phone);
                std::cout << "<OK: CLIENT " << phone << " REMOVED>\n";
            }
        }

        else if (cmd == "add-zone") {
            if (args.size() == 2) {
                if (!zones.addZone("", args[1]))
                    std::cout << "<INVALID COMMAND: ZONE " << args[1] << " ALREADY EXISTS OR ROOT EXISTS>\n";
                else
                    std::cout << "<OK: ZONE " << args[1] << " ADDED>\n";
            } else if (args.size() == 3) {
                const auto& parent = args[1], &name = args[2];
                if (!zones.addZone(parent, name))
                    std::cout << "<INVALID COMMAND: PARENT " << parent << " NOT FOUND OR ZONE EXISTS>\n";
                else
                    std::cout << "<OK: ZONE " << name << " ADDED UNDER " << parent << ">\n";
            } else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }

        else if (cmd == "order") {
            if (args.size() != 3) { std::cout << "<INVALID COMMAND>\n"; continue; }
            const auto& phone = args[1], &item = args[2];

            Client* client = clients.find(phone);
            if (!client)
                std::cout << "<INVALID COMMAND: CLIENT " << phone << " NOT FOUND>\n";
            else {
                orderQueue.enqueue({item, phone, client->name, client->zone});
                std::cout << "<ADDED TO QUEUE: " << item << " → "
                          << client->zone << " (" << client->name << ")>\n";
            }
        }

        else if (cmd == "plan-route") {
            if (orderQueue.empty()) {
                std::cout << "<INVALID COMMAND: QUEUE IS EMPTY>\n";
                continue;
            }

            auto active = orderQueue.activeZones();
            plannedRoute = zones.planRoute(active);

            if (plannedRoute.empty()) {
                std::cout << "<INVALID COMMAND: NO ZONES WITH ORDERS FOUND IN TREE>\n";
            } else {
                routePlanned = true;
                auto counts    = orderQueue.countPerZone();
                int  totalItems = 0;

                std::cout << "<ROUTE PLANNED:>\n";
                for (int i = 0; i < (int)plannedRoute.size(); i++) {
                    int cnt = counts[plannedRoute[i]];
                    totalItems += cnt;
                    std::cout << i + 1 << ". " << plannedRoute[i]
                              << " — " << cnt << (cnt == 1 ? " заказ" : " заказа") << "\n";
                }
                std::cout << "<TOTAL STOPS: " << plannedRoute.size()
                          << ", TOTAL ITEMS: " << totalItems << ">\n";
            }
        }

        else if (cmd == "load-truck") {
            if (!routePlanned) {
                std::cout << "<INVALID COMMAND: ROUTE NOT PLANNED>\n";
                continue;
            }

            std::vector<Order> allOrders;
            Order tmp;
            while (orderQueue.dequeue(tmp))
                allOrders.push_back(tmp);

            std::sort(allOrders.begin(), allOrders.end(), [&](const Order& a, const Order& b) {
                auto posA = std::find(plannedRoute.begin(), plannedRoute.end(), a.zone) - plannedRoute.begin();
                auto posB = std::find(plannedRoute.begin(), plannedRoute.end(), b.zone) - plannedRoute.begin();
                return posA < posB;
            });

            truck.setRoute(plannedRoute);
            for (int i = (int)allOrders.size() - 1; i >= 0; i--)
                truck.push(allOrders[i]);

            routePlanned = false;

            std::cout << "<TRUCK LOADED: " << allOrders.size() << " items>\n";
            std::cout << "<STACK top→bottom:";
            for (const auto& o : allOrders)
                std::cout << " [" << o.item << "/" << o.zone << "]";
            std::cout << ">\n";
        }

        else if (cmd == "deliver") {
            if (truck.empty())
                std::cout << "<INVALID COMMAND: TRUCK IS EMPTY>\n";
            else
                truck.deliver();
        }

        else if (cmd == "show-truck") {
            truck.print();
        }

        else if (cmd == "show-queue") {
            orderQueue.print();
        }

        else if (cmd == "show-map") {
            zones.printMap(orderQueue.activeZones());
        }

        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
