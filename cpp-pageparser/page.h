#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>

namespace my
{
class Page
{
private:
  size_t count;
  std::string link;
  std::string host;
  std::string label;
  std::string query;
  std::vector<Page> children;
  std::vector<std::string> images;

public:
  Page();
  Page(std::string link, size_t depth);
  size_t getCount();

  std::string getLink();

  std::string getQuery();
  void setQuery(std::string &query);

  std::string getLabel();
  void setLabel(std::string &label);

  std::string getHost();
  void setHost(std::string &host);

  std::vector<Page> getChildren();
  void setChildren(std::vector<Page> children);
  void addChild(Page &page);

  std::vector<std::string> getImages();
  void setImages(std::vector<std::string> images);
  void addImage(std::string &image);

  bool operator==(const Page &other) const;
  bool operator<(const Page &other) const;
};
}
#endif