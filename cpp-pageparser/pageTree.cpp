#include "pageTree.h"
#include "page.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/foreach.hpp>

using namespace std;

namespace my
{

PageTree::PageTree(Page root)
{
    this->root = root;
    nodes.reserve(1000);
    edges.reserve(1000);
}
void PageTree::generateDOTFile()
{
    auto parent = to_string(nodes.size());
    nodes.push_back(parent + " [shape=box, label=\"" + root.getLink() + "\"];");

    generateDOT(root, parent);

    ofstream out;
    out.open("output.txt");

    BOOST_FOREACH (auto &node, nodes)
    {
        out << node << endl;
        cout << node << endl;
    }

    BOOST_FOREACH (auto &edge, edges)
    {
        out << edge << endl;
        cout << edge << endl;
    }
    out.close();
}

void PageTree::generateDOT(Page &page, string &parent)
{
    vector<Page> children = page.getChildren();
    vector<string> images = page.getImages();

    auto totalSize = nodes.size() + children.size() + images.size();
    if (totalSize >= nodes.capacity())
    {
        nodes.reserve(nodes.capacity() * 2);
        edges.reserve(edges.capacity() * 2);
    }

    BOOST_FOREACH (auto &child, children)
    {
        auto current = to_string(nodes.size());
        child.setLabel(current);
        nodes.push_back(current + " [shape=box, label=\"" + child.getLink() + "\"];");
        edges.push_back(parent + " -- " + current + ";");
    }

    BOOST_FOREACH (auto &img, images)
    {
        auto current = to_string(nodes.size());
        nodes.push_back(current + " [shape=box, label=\"" + img + "\", color=blue, style=dotted];");
        edges.push_back(parent + " -- " + current + " [color=blue];");
    }

    BOOST_FOREACH (auto &child, children)
    {
        generateDOT(child, child.getLabel());
    }
}
}