#pragma once
#include <string>

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
