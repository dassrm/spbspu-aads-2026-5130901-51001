#pragma once
#include <string>
#include <vector>
#include <set>
#include <memory>

struct ZoneNode {
    std::string name;
    std::vector<std::shared_ptr<ZoneNode>> children;
    explicit ZoneNode(const std::string& n) : name(n) {}
};

class ZoneTree {
    std::shared_ptr<ZoneNode> root;

    std::shared_ptr<ZoneNode> findNode(std::shared_ptr<ZoneNode> node, const std::string& name) const;

    void dfs(std::shared_ptr<ZoneNode> node,
             const std::set<std::string>& targets,
             std::vector<std::string>& result) const;

    void printNode(std::shared_ptr<ZoneNode> node,
                   const std::string& prefix,
                   bool isLast,
                   const std::set<std::string>& activeZones) const;

public:
    bool addZone(const std::string& parentName, const std::string& zoneName);

    bool exists(const std::string& name) const;

    std::vector<std::string> planRoute(const std::set<std::string>& targets) const;

    void printMap(const std::set<std::string>& activeZones) const;
};
