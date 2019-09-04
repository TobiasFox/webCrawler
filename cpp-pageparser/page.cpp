#include "page.h"
#include <boost/algorithm/string.hpp>

using namespace std;
namespace my
{
Page::Page()
{
    this->link = "";
    this->count = 0;
    //this->children=NULL;
}

Page::Page(string link, size_t count)
{
    this->link = link;
    this->count = count;
    //this->children=NULL;
}
size_t Page::getCount()
{
    return count;
}
string Page::getLink()
{
    return link;
}

string Page::getQuery()
{
    return this->query;
}
void Page::setQuery(string &query)
{
    this->query = query;
}

string Page::getHost()
{
    return this->host;
}
void Page::setHost(string &host)
{
    this->host = host;
}

string Page::getLabel()
{
    return this->label;
}
void Page::setLabel(string &label)
{
    this->label = label;
}

vector<Page> Page::getChildren()
{
    return children;
}
void Page::setChildren(vector<Page> children)
{
    this->children = children;
}
void Page::addChild(Page &child)
{
    children.push_back(child);
}

vector<string> Page::getImages()
{
    return images;
}
void Page::setImages(vector<string> images)
{
    this->images = images;
}
void Page::addImage(string &image)
{
    images.push_back(image);
}

bool Page::operator==(const Page &other) const
{
    return boost::algorithm::equals(link, other.link) && count == other.count;
}
bool Page::operator<(const Page &other) const
{
    return link < other.link && count <= other.count;
}
}