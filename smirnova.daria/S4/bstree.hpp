#ifndef SMIRNOVA_BSTREE_HPP
#define SMIRNOVA_BSTREE_HPP

#include <cstddef>
#include <utility>

namespace smirnova {

  template< class Key, class Value >
  class BSTIterator {
  public:
    struct Node {
      Key key;
      Value value;
      Node* left;
      Node* right;
      Node* parent;
      
      Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    BSTIterator() : node_(nullptr) {}
    explicit BSTIterator(Node* n) : node_(n) {}

    std::pair< const Key&, Value& > operator*() {
      return std::make_pair(std::cref(node_->key), std::ref(node_->value));
    }

    BSTIterator& operator++() {
      if (node_->right != nullptr) {
        node_ = node_->right;
        while (node_->left != nullptr) {
          node_ = node_->left;
        }
      } else {
        Node* parent = node_->parent;
        while (parent != nullptr && node_ == parent->right) {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    bool operator==(const BSTIterator& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const BSTIterator& other) const {
      return node_ != other.node_;
    }

  private:
    Node* node_;
  };

  template< class Key, class Value >
  class BSTConstIterator {
  public:
    using Node = typename BSTIterator< Key, Value >::Node;

    BSTConstIterator() : node_(nullptr) {}
    explicit BSTConstIterator(const Node* n) : node_(n) {}
    BSTConstIterator(const BSTIterator< Key, Value >& it) : node_(it.node_) {}

    std::pair< const Key&, const Value& > operator*() const {
      return std::make_pair(std::cref(node_->key), std::cref(node_->value));
    }

    BSTConstIterator& operator++() {
      if (node_->right != nullptr) {
        node_ = node_->right;
        while (node_->left != nullptr) {
          node_ = node_->left;
        }
      } else {
        const Node* parent = node_->parent;
        while (parent != nullptr && node_ == parent->right) {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    bool operator==(const BSTConstIterator& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const BSTConstIterator& other) const {
      return node_ != other.node_;
    }

  private:
    const Node* node_;
    friend class BSTIterator< Key, Value >;
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree {
  public:
    using Node = typename BSTIterator< Key, Value >::Node;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree() : root_(nullptr), size_(0), comp_() {}
    explicit BSTree(const Compare& comp) : root_(nullptr), size_(0), comp_(comp) {}

    ~BSTree() {
      deleteTree(root_);
    }

    void push(Key k, Value v) {
      if (root_ == nullptr) {
        root_ = new Node(k, v);
        size_ = 1;
      } else {
        insertNode(root_, nullptr, k, v);
      }
    }

    Value get(const Key& k) const {
      Node* node = findNode(root_, k);
      if (node != nullptr) {
        return node->value;
      }
      return Value();
    }

    bool drop(const Key& k) {
      if (root_ == nullptr) {
        return false;
      }
      bool found = false;
      root_ = deleteNode(root_, k, found);
      if (found) {
        size_--;
      }
      return found;
    }

    size_t height() const {
      return getHeight(root_);
    }

    size_t size() const {
      return size_;
    }

    bool empty() const {
      return size_ == 0;
    }

    iterator begin() {
      if (root_ == nullptr) {
        return iterator(nullptr);
      }
      Node* node = root_;
      while (node->left != nullptr) {
        node = node->left;
      }
      return iterator(node);
    }

    const_iterator begin() const {
      if (root_ == nullptr) {
        return const_iterator(nullptr);
      }
      const Node* node = root_;
      while (node->left != nullptr) {
        node = node->left;
      }
      return const_iterator(node);
    }

    iterator end() {
      return iterator(nullptr);
    }

    const_iterator end() const {
      return const_iterator(nullptr);
    }

    void rotateLeft(iterator it) {
      if (it.node_ == nullptr || it.node_->right == nullptr) {
        return;
      }
      Node* x = it.node_;
      Node* y = x->right;
      x->right = y->left;
      if (y->left != nullptr) {
        y->left->parent = x;
      }
      y->parent = x->parent;
      if (x->parent == nullptr) {
        root_ = y;
      } else if (x == x->parent->left) {
        x->parent->left = y;
      } else {
        x->parent->right = y;
      }
      y->left = x;
      x->parent = y;
    }

    void rotateRight(iterator it) {
      if (it.node_ == nullptr || it.node_->left == nullptr) {
        return;
      }
      Node* x = it.node_;
      Node* y = x->left;
      x->left = y->right;
      if (y->right != nullptr) {
        y->right->parent = x;
      }
      y->parent = x->parent;
      if (x->parent == nullptr) {
        root_ = y;
      } else if (x == x->parent->right) {
        x->parent->right = y;
      } else {
        x->parent->left = y;
      }
      y->right = x;
      x->parent = y;
    }

    void rotateLargeLeft(iterator it) {
      if (it.node_ == nullptr || it.node_->right == nullptr) {
        return;
      }
      rotateRight(iterator(it.node_->right));
      rotateLeft(it);
    }

    void rotateLargeRight(iterator it) {
      if (it.node_ == nullptr || it.node_->left == nullptr) {
        return;
      }
      rotateLeft(iterator(it.node_->left));
      rotateRight(it);
    }

  private:
    Node* root_;
    size_t size_;
    Compare comp_;

    void insertNode(Node* node, Node* parent, const Key& k, const Value& v) {
      if (comp_(k, node->key)) {
        if (node->left == nullptr) {
          node->left = new Node(k, v);
          node->left->parent = node;
          size_++;
        } else {
          insertNode(node->left, node, k, v);
        }
      } else if (comp_(node->key, k)) {
        if (node->right == nullptr) {
          node->right = new Node(k, v);
          node->right->parent = node;
          size_++;
        } else {
          insertNode(node->right, node, k, v);
        }
      } else {
        node->value = v;
      }
    }

    Node* findNode(Node* node, const Key& k) const {
      if (node == nullptr) {
        return nullptr;
      }
      if (comp_(k, node->key)) {
        return findNode(node->left, k);
      } else if (comp_(node->key, k)) {
        return findNode(node->right, k);
      } else {
        return node;
      }
    }

    Node* deleteNode(Node* node, const Key& k, bool& found) {
      if (node == nullptr) {
        return nullptr;
      }
      if (comp_(k, node->key)) {
        node->left = deleteNode(node->left, k, found);
        if (node->left != nullptr) {
          node->left->parent = node;
        }
      } else if (comp_(node->key, k)) {
        node->right = deleteNode(node->right, k, found);
        if (node->right != nullptr) {
          node->right->parent = node;
        }
      } else {
        found = true;
        if (node->left == nullptr && node->right == nullptr) {
          delete node;
          return nullptr;
        } else if (node->left == nullptr) {
          Node* temp = node->right;
          temp->parent = node->parent;
          delete node;
          return temp;
        } else if (node->right == nullptr) {
          Node* temp = node->left;
          temp->parent = node->parent;
          delete node;
          return temp;
        } else {
          Node* minRight = node->right;
          while (minRight->left != nullptr) {
            minRight = minRight->left;
          }
          node->key = minRight->key;
          node->value = minRight->value;
          bool dummy = false;
          node->right = deleteNode(node->right, minRight->key, dummy);
          if (node->right != nullptr) {
            node->right->parent = node;
          }
          found = true;
        }
      }
      return node;
    }

    size_t getHeight(const Node* node) const {
      if (node == nullptr) {
        return 0;
      }
      size_t leftHeight = getHeight(node->left);
      size_t rightHeight = getHeight(node->right);
      return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    void deleteTree(Node* node) {
      if (node == nullptr) {
        return;
      }
      deleteTree(node->left);
      deleteTree(node->right);
      delete node;
    }
  };

}

#endif
