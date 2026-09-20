#ifndef SMIRNOVA_ZONE_TREE_HPP
#define SMIRNOVA_ZONE_TREE_HPP

#include <cstddef>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace smirnova {

  // A city zone (the warehouse itself is the root). Each node owns its
  // children exclusively through std::unique_ptr: a child has exactly one
  // owner, and that owner never changes after the node is created, so the
  // structure cannot become anything other than a tree - no node can end
  // up with two parents, and no cycle can form.
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
    // parentName == "" adds the warehouse (the tree's root); the root can
    // only be added once. Any other call attaches a new child under an
    // already-existing zone named parentName.
    ZoneAddResult addZone(const std::string& parentName, const std::string& zoneName);

    bool exists(const std::string& name) const;
    bool hasRoot() const;
    std::string rootName() const;

    // Pre-order traversal starting at the warehouse: the warehouse first,
    // then each branch is visited fully before moving to the next one.
    // This is the delivery visiting order; only zones present in
    // "targets" are included in the result.
    std::vector< std::string > planRoute(const std::set< std::string >& targets) const;

    void printMap(const std::set< std::string >& activeZones, std::ostream& out) const;

  private:
    std::unique_ptr< ZoneNode > root_;

    ZoneNode* findNode(ZoneNode* node, const std::string& name) const;
    void collectRoute(const ZoneNode* node, const std::set< std::string >& targets,
                       std::vector< std::string >& result) const;
    void printChildren(const ZoneNode* node, const std::string& prefix,
                        const std::set< std::string >& activeZones, std::ostream& out) const;
  };

}

#endif
