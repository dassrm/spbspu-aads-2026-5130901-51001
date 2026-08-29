#pragma once
#include <queue>
#include <set>
#include <map>
#include <string>
#include "types.h"

class OrderQueue {
    std::queue<Order> q;

public:
    void enqueue(const Order& order);
    bool dequeue(Order& out);

    bool empty() const;
    int  size()  const;

    bool hasOrdersForPhone(const std::string& phone) const;

    std::set<std::string> activeZones() const;

    std::map<std::string, int> countPerZone() const;

    void print() const;
};
