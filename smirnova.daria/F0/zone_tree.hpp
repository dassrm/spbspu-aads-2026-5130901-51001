#ifndef SMIRNOVA_ZONE_TREE_HPP
#define SMIRNOVA_ZONE_TREE_HPP

#include <cstddef>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace smirnova {

  struct ZoneNode {
    std::string name;
    std::vector< std::unique_ptr< ZoneNode > > children;

    explicit ZoneNode(const std::string& zoneName):
      name(zoneName)
    {}
  };

  enum class ZoneAddResult {
    Added,
    ZoneAlreadyExists,
    ParentNotFound,
    RootAlreadyExists
  };

  class ZoneTree {
  public:
    ZoneAddResult addZone(const std::string& parentName, const std::string& zoneName);

    bool exists(const std::string& name) const;
    bool hasRoot() const;
    std::string rootName() const;

    std::vector< std::string > planRoute(const std::set< std::string >& targets) const;
    void printMap(const std::set< std::string >& activeZones, std::ostream& out) const;

  private:
    std::unique_ptr< ZoneNode > root_;

    const ZoneNode* findNode(const ZoneNode* node, const std::string& name) const;
    ZoneNode* findNode(ZoneNode* node, const std::string& name);

    void collectTargetsPreOrder(const ZoneNode* node, const std::set< std::string >& targets,
      std::vector< std::string >& result) const;
    void printSubtree(const ZoneNode* node, const std::string& prefix,
      const std::set< std::string >& activeZones, std::ostream& out) const;
  };

}

#endif
