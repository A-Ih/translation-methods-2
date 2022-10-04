#pragma once
#include <memory>
#include <string>
#include <vector>

struct Tree {
  std::string node;
  std::vector<std::unique_ptr<Tree>> children;
};
