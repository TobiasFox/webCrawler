#ifndef PAGE_TREE_H
#define PAGE_TREE_H

#include <string>
#include <vector>
#include "page.h"

namespace my
{
class PageTree
{
private:
  Page root;
  std::vector<std::string> nodes;
  std::vector<std::string> edges;
  void generateDOT(Page &page, std::string &parent);

public:
  PageTree(Page root);
  void generateDOTFile();
};
}
#endif