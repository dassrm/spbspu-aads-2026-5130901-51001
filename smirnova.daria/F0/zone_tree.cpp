#include "zone_tree.hpp"

namespace smirnova {

  ZoneNode* ZoneTree::findNode(ZoneNode* node, const std::string& name) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->name == name) {
      return node;
    }
    for (auto& child : node->children) {
      ZoneNode* found = findNode(child.get(), name);
      if (found != nullptr) {
        return found;
      }
    }
    return nullptr;
  }

  ZoneAddResult ZoneTree::addZone(const std::string& parentName, const std::string& zoneName) {
    if (findNode(root_.get(), zoneName) != nullptr) {
      return ZoneAddResult::ZoneAlreadyExists;
    }

    if (parentName.empty()) {
      if (root_ != nullptr) {
        return ZoneAddResult::RootAlreadyExists;
      }
      root_.reset(new ZoneNode(zoneName));
      return ZoneAddResult::Added;
    }

    ZoneNode* parent = findNode(root_.get(), parentName);
    if (parent == nullptr) {
      return ZoneAddResult::ParentNotFound;
    }
    parent->children.push_back(std::unique_ptr< ZoneNode >(new ZoneNode(zoneName)));
    return ZoneAddResult::Added;
  }

  bool ZoneTree::exists(const std::string& name) const {
    return findNode(root_.get(), name) != nullptr;
  }

  bool ZoneTree::hasRoot() const {
    return root_ != nullptr;
  }

  std::string ZoneTree::rootName() const {
    return root_ != nullptr ? root_->name : std::string();
  }

  void ZoneTree::collectRoute(const ZoneNode* node, const std::set< std::string >& targets,
                               std::vector< std::string >& result) const {
    if (node == nullptr) {
      return;
    }
    if (targets.count(node->name) != 0) {
      result.push_back(node->name);
    }
    for (const auto& child : node->children) {
      collectRoute(child.get(), targets, result);
    }
  }

  std::vector< std::string > ZoneTree::planRoute(const std::set< std::string >& targets) const {
    std::vector< std::string > result;
    collectRoute(root_.get(), targets, result);
    return result;
  }

  // Prints every child of "node", each on its own line prefixed with the
  // usual "├── " / "└── " connectors, then recurses so that "prefix"
  // always reflects exactly the ancestor continuation bars ("│   ") or
  // blanks ("    ") needed at this depth.
  void ZoneTree::printChildren(const ZoneNode* node, const std::string& prefix,
                                const std::set< std::string >& activeZones, std::ostream& out) const {
    for (std::size_t i = 0; i < node->children.size(); ++i) {
      const ZoneNode* child = node->children[i].get();
      bool isLast = (i + 1 == node->children.size());

      out << prefix << (isLast ? "└── " : "├── ") << child->name;
      if (activeZones.count(child->name) != 0) {
        out << "*";
      }
      out << "\n";

      printChildren(child, prefix + (isLast ? "    " : "│   "), activeZones, out);
    }
  }

  void ZoneTree::printMap(const std::set< std::string >& activeZones, std::ostream& out) const {
    if (root_ == nullptr) {
      out << "<MAP IS EMPTY>\n";
      return;
    }

    out << root_->name;
    if (activeZones.count(root_->name) != 0) {
      out << "*";
    }
    out << "\n";

    printChildren(root_.get(), "", activeZones, out);
    if (!activeZones.empty()) {
      out << "* — есть заказы\n";
    }
  }

}
