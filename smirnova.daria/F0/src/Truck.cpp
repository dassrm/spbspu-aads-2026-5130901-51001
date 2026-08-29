#include <iostream>
#include "Truck.h"

void Truck::setRoute(const std::vector<std::string>& r) {
    route = r;
    currentStop = 0;
}

bool Truck::routeReady() const { return !route.empty(); }
bool Truck::empty()      const { return cargo.empty(); }

void Truck::push(const Order& order) { cargo.push(order); }

bool Truck::deliver() {
    if (cargo.empty() || currentStop >= (int)route.size()) return false;

    const std::string& zone = route[currentStop];
    std::cout << "<ARRIVED: " << zone << ">\n";

    while (!cargo.empty() && cargo.top().zone == zone) {
        const auto& o = cargo.top();
        std::cout << "<DELIVERED: " << o.item << " → "
                  << o.clientName << " (" << o.phone << "), " << zone << ">\n";
        cargo.pop();
    }

    currentStop++;
    if (currentStop < (int)route.size())
        std::cout << "<NEXT STOP: " << route[currentStop] << ">\n";
    else {
        std::cout << "<ROUTE COMPLETE.>\n";
        route.clear();
        currentStop = 0;
    }
    return true;
}

void Truck::print() const {
    if (cargo.empty()) { std::cout << "<TRUCK IS EMPTY>\n"; return; }

    if (currentStop < (int)route.size()) {
        std::cout << "<ROUTE: ";
        for (int i = currentStop; i < (int)route.size(); i++) {
            if (i > currentStop) std::cout << " → ";
            std::cout << route[i];
        }
        std::cout << ">\n";
    }

    std::stack<Order> tmp = cargo;
    std::cout << "<CARGO (top→bottom):>\n";
    int i = 1;
    while (!tmp.empty()) {
        const auto& o = tmp.top();
        std::cout << i++ << ". " << o.item << " → " << o.zone
                  << " (" << o.clientName << ", " << o.phone << ")\n";
        tmp.pop();
    }
}
