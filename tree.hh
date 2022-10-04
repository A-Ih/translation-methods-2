#pragma once
#include <memory>
#include <string>
#include <vector>

struct Tree {
  std::string node;
  std::vector<std::unique_ptr<Tree>> children;
};

inline std::size_t TreeToDotHelper(std::ostream& os, const Tree* node,
                                   std::size_t& id) {
  std::size_t thisId = ++id;
  os << "n" << thisId << " [label=\"" << node->node << "\"]\n";
  for (auto& child : node->children) {
    auto childId = TreeToDotHelper(os, child.get(), id);
    os << "n" << thisId << " -> "
       << "n" << childId << "\n";
  }
  return thisId;
}

inline void TreeToDot(std::ostream& os, const Tree* node) {
  os << "strict digraph {\n";
  std::size_t id = 0;
  TreeToDotHelper(os, node, id);
  os << "}\n";
}
