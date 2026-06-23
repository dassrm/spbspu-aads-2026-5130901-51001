#pragma once
#include <stack>
#include <vector>
#include <string>
#include "types.h"

class Truck {
    std::stack<Order>        cargo;
    std::vector<std::string> route;
    int currentStop = 0;

public:
    void setRoute(const std::vector<std::string>& r);

    bool routeReady() const;
    bool empty()      const;

    void push(const Order& order);

    bool deliver();

    void print() const;
};
