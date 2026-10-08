#include "zone_tree.hpp"

namespace smirnova {

  const ZoneNode* ZoneTree::findNode(const ZoneNode* node, const std::string& name) const
  {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->name == name) {
      return node;
    }
    for (const std::unique_ptr< ZoneNode >& child : node->children) {
      const ZoneNode* found = findNode(child.get(), name);
      if (found != nullptr) {
        return found;
      }
    }
    return nullptr;
  }

  ZoneNode* ZoneTree::findNode(ZoneNode* node, const std::string& name)
  {
    const ZoneTree& self = *this;
    return const_cast< ZoneNode* >(self.findNode(node, name));
  }

  ZoneAddResult ZoneTree::addZone(const std::string& parentName, const std::string& zoneName)
  {
    if (findNode(root_.get(), zoneName) != nullptr) {
      return ZoneAddResult::ZoneAlreadyExists;
    }

    if (parentName.empty()) {
      if (root_ != nullptr) {
        return ZoneAddResult::RootAlreadyExists;
      }
      root_ = std::unique_ptr< ZoneNode >(new ZoneNode(zoneName));
      return ZoneAddResult::Added;
    }

    ZoneNode* parent = findNode(root_.get(), parentName);
    if (parent == nullptr) {
      return ZoneAddResult::ParentNotFound;
    }
    parent->children.push_back(std::unique_ptr< ZoneNode >(new ZoneNode(zoneName)));
    return ZoneAddResult::Added;
  }

  bool ZoneTree::exists(const std::string& name) const
  {
    return findNode(root_.get(), name) != nullptr;
  }

  bool ZoneTree::hasRoot() const
  {
    return root_ != nullptr;
  }

  std::string ZoneTree::rootName() const
  {
    if (root_ == nullptr) {
      return std::string();
    }
    return root_->name;
  }

  void ZoneTree::collectTargetsPreOrder(const ZoneNode* node,
    const std::set< std::string >& targets, std::vector< std::string >& result) const
  {
    if (node == nullptr) {
      return;
    }
    if (targets.count(node->name) != 0) {
      result.push_back(node->name);
    }
    for (const std::unique_ptr< ZoneNode >& child : node->children) {
      collectTargetsPreOrder(child.get(), targets, result);
    }
  }

  std::vector< std::string > ZoneTree::planRoute(const std::set< std::string >& targets) const
  {
    std::vector< std::string > result;
    collectTargetsPreOrder(root_.get(), targets, result);
    return result;
  }

  void ZoneTree::printSubtree(const ZoneNode* node, const std::string& prefix,
    const std::set< std::string >& activeZones, std::ostream& out) const
  {
    for (std::size_t i = 0; i < node->children.size(); ++i) {
      const ZoneNode* child = node->children[i].get();
      bool isLast = (i + 1 == node->children.size());

      out << prefix << (isLast ? "└── " : "├── ") << child->name;
      if (activeZones.count(child->name) != 0) {
        out << "*";
      }
      out << "\n";

      printSubtree(child, prefix + (isLast ? "    " : "│   "), activeZones, out);
    }
  }

  void ZoneTree::printMap(const std::set< std::string >& activeZones, std::ostream& out) const
  {
    if (root_ == nullptr) {
      out << "<MAP IS EMPTY>\n";
      return;
    }

    out << root_->name;
    if (activeZones.count(root_->name) != 0) {
      out << "*";
    }
    out << "\n";

    printSubtree(root_.get(), "", activeZones, out);
    if (!activeZones.empty()) {
      out << "* — есть заказы\n";
    }
  }

}
