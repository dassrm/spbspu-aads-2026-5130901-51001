#include <iostream>
#include "OrderQueue.h"

void OrderQueue::enqueue(const Order& order) { q.push(order); }

bool OrderQueue::dequeue(Order& out) {
    if (q.empty()) return false;
    out = q.front();
    q.pop();
    return true;
}

bool OrderQueue::empty() const { return q.empty(); }
int  OrderQueue::size()  const { return static_cast<int>(q.size()); }

bool OrderQueue::hasOrdersForPhone(const std::string& phone) const {
    std::queue<Order> tmp = q;
    while (!tmp.empty()) {
        if (tmp.front().phone == phone) return true;
        tmp.pop();
    }
    return false;
}

std::set<std::string> OrderQueue::activeZones() const {
    std::set<std::string> zones;
    std::queue<Order> tmp = q;
    while (!tmp.empty()) {
        zones.insert(tmp.front().zone);
        tmp.pop();
    }
    return zones;
}

std::map<std::string, int> OrderQueue::countPerZone() const {
    std::map<std::string, int> counts;
    std::queue<Order> tmp = q;
    while (!tmp.empty()) {
        counts[tmp.front().zone]++;
        tmp.pop();
    }
    return counts;
}

void OrderQueue::print() const {
    if (q.empty()) { std::cout << "<QUEUE IS EMPTY>\n"; return; }
    std::queue<Order> tmp = q;
    std::cout << "<QUEUE (" << q.size() << " items):>\n";
    int i = 1;
    while (!tmp.empty()) {
        const auto& o = tmp.front();
        std::cout << i++ << ". " << o.item << " → " << o.zone
                  << " (" << o.clientName << ", " << o.phone << ")\n";
        tmp.pop();
    }
}
