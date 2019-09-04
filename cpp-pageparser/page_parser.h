#ifndef PAGE_PARSER_H
#define PAGE_PARSER_H

#include <string>
#include "page.h"

namespace my
{
class PageParser
{
private:
  size_t depth;
  std::string start_link;
  std::vector<Page> visited;
  std::string get_page(Page &page);

public:
  PageParser(std::string link);
  PageParser(std::string link, size_t depth);
  void parsePage(Page &page);
  Page parse();
};
}
#endif