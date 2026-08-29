#include <iostream>
#include "ZoneTree.h"

std::shared_ptr<ZoneNode> ZoneTree::findNode(std::shared_ptr<ZoneNode> node, const std::string& name) const {
    if (!node) return nullptr;
    if (node->name == name) return node;
    for (const auto& child : node->children) {
        auto result = findNode(child, name);
        if (result) return result;
    }
    return nullptr;
}

void ZoneTree::dfs(std::shared_ptr<ZoneNode> node,
                   const std::set<std::string>& targets,
                   std::vector<std::string>& result) const {
    if (!node) return;
    if (targets.count(node->name))
        result.push_back(node->name);
    for (const auto& child : node->children)
        dfs(child, targets, result);
}

void ZoneTree::printNode(std::shared_ptr<ZoneNode> node,
                         const std::string& prefix,
                         bool isLast,
                         const std::set<std::string>& activeZones) const {
    if (!node) return;

    std::cout << prefix;
    if (!prefix.empty())
        std::cout << (isLast ? "└── " : "├── ");
    std::cout << node->name;
    if (activeZones.count(node->name))
        std::cout << "*";
    std::cout << "\n";

    std::string childPrefix = prefix.empty() ? "" : prefix + (isLast ? "    " : "│   ");
    for (int i = 0; i < (int)node->children.size(); i++)
        printNode(node->children[i], childPrefix, i == (int)node->children.size() - 1, activeZones);
}

bool ZoneTree::addZone(const std::string& parentName, const std::string& zoneName) {
    if (findNode(root, zoneName)) return false;

    if (parentName.empty()) {
        if (root) return false;
        root = std::make_shared<ZoneNode>(zoneName);
        return true;
    }

    auto parent = findNode(root, parentName);
    if (!parent) return false;
    parent->children.push_back(std::make_shared<ZoneNode>(zoneName));
    return true;
}

bool ZoneTree::exists(const std::string& name) const {
    return findNode(root, name) != nullptr;
}

std::vector<std::string> ZoneTree::planRoute(const std::set<std::string>& targets) const {
    std::vector<std::string> result;
    dfs(root, targets, result);
    return result;
}

void ZoneTree::printMap(const std::set<std::string>& activeZones) const {
    if (!root) { std::cout << "<MAP IS EMPTY>\n"; return; }
    printNode(root, "", true, activeZones);
    if (!activeZones.empty())
        std::cout << "* — есть заказы\n";
}
