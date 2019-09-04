#include "page_parser.h"
#include <iostream>
#include <regex>
#include <boost/asio.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

using namespace std;
using namespace boost::xpressive;
using namespace boost::algorithm;

namespace my
{

PageParser::PageParser(string link)
    : PageParser(link, 2)
{
}

PageParser::PageParser(string link, size_t depth)
{
    this->start_link = link;
    this->depth = depth;
    visited.reserve(1000);
}
void PageParser::parsePage(Page &page)
{
    if (page.getCount() >= depth)
    {
        cout << "reached max depth" << endl;
        return;
    }

    cout << "starting parsing page: " << page.getLink() << endl;

    string html = get_page(page);
    if (html.empty())
    {
        cout << "error in http request" << endl;
        return;
    }
    //cout << html << endl;

    sregex comment = "<!--" >> -*_ >> "-->";
    html = regex_replace(html, comment, "");
    sregex script = "<" >> *_s >> "script" | "SCRIPT" >> -*_ >> "script" | "SCRIPT" >> *_s >> "/>";
    html = regex_replace(html, script, "");

    {
        //parse imgs
        regex rgx{R"regexExpression(<img\s+(.*?)(src|SRC)\s*=\s*\"(.*?)\".*>)regexExpression"};
        auto rgx_begin = std::sregex_iterator(html.begin(), html.end(), rgx);
        auto rgx_end = std::sregex_iterator();

        for (std::sregex_iterator i = rgx_begin; i != rgx_end; ++i)
        {
            std::smatch match = *i;

            auto img = match[3].str();
            if (starts_with(img, "/"))
            {
                img = page.getHost() + img;
            }
            regex rgxSlash{R"regexExpression(//)regexExpression"};
            img = regex_replace(img, rgxSlash, "/");
            page.addImage(img);
        }
    }

    {
        //parse links
        regex rgx{R"regexExpression(<(a|A)\s+(.*?)(href|HREF)\s*=\s*\"(.*?)\".*>)regexExpression"};

        auto rgx_begin = std::sregex_iterator(html.begin(), html.end(), rgx);
        auto rgx_end = std::sregex_iterator();

        for (std::sregex_iterator i = rgx_begin; i != rgx_end; ++i)
        {
            std::smatch match = *i;

            string sub(match[4].str());
            // cout << endl
            //      << "matched: " << sub << endl;

            if (!equals(sub, "/") && !contains(sub, "https"))
            {
                if (starts_with(sub, "/") || !(contains(sub, "www") || contains(sub, "http")))
                {
                    // regex relative{R"regexExpression(.*/)regexExpression"};
                    // string relativeString = page.getLink();
                    // auto rgx_match = std::sregex_iterator(relativeString.begin(), relativeString.end(), relative);
                    // string rel = rgx_match->str();
                    // cout << rel << endl;
                    // sub = rel + sub;
                    sub = page.getHost() + sub;
                }

                Page child(sub, page.getCount() + 1);

                if (std::find(visited.begin(), visited.end(), child) == visited.end())
                {
                    cout << "added to list and begin rekursion at lvl: " << child.getCount() << " with url: " << child.getLink() << endl;
                    if (visited.size() >= visited.capacity())
                    {
                        visited.reserve(visited.capacity() * 2);
                    }
                    visited.push_back(child);
                    parsePage(child);
                    page.addChild(child);
                }
                else
                {
                    cout << child.getLink() << " already parsed" << endl
                         << endl;
                }

                // cout << endl
                //      << i->str() << endl
                //      << endl;
                // count++;
            }
        }
    }
}

Page PageParser::parse()
{
    Page root(start_link, 0);
    parsePage(root);
    return root;
}

string PageParser::get_page(Page &page)
{
    string link = page.getLink();
    //delete "http://"
    {
        //bos = ^ (Anfang)
        sregex rex = bos >> "http://";
        link = regex_replace(link, rex, "");
    }

    regex rgx{R"regexExpression(//)regexExpression"};
    link = regex_replace(link, rgx, "/");

    //extract host
    string host;
    {
        //*_ = .* (beliebiges Zeichen beliebig oft)
        //löscht alles nach dem 1. slash
        //result: www.boehmwanderkarten.de
        sregex rex = "/" >> *_;
        host = regex_replace(link, rex, "");
    }

    //extract query
    string query;
    {
        //belibig oft keinen slash danach 0 oder 1 slash
        //löscht alles vor dem 1. slash
        //result: kartographie/is_netze_cyl.html
        sregex rex = bos >> *~as_xpr('/') >> !as_xpr('/');
        query = regex_replace(link, rex, "");
    }

    page.setHost(host);
    page.setQuery(query);

    cout << "HOST: " << host << " QUERY: " << query << endl;

    boost::asio::ip::tcp::iostream tcp_io(host, "http");
    tcp_io.expires_from_now(boost::posix_time::seconds(1)); // setzt den time-out auf 1 sec.

    tcp_io << "GET /" << query << " HTTP/1.1\r\n";
    tcp_io << "Host: " << host;
    tcp_io << "\r\n\r\n"
           << std::flush;

    std::stringstream answer;

    int count = 0;
    for (std::string line; std::getline(tcp_io, line);)
    {
        if (count++ == 0 && !boost::algorithm::contains(line, "200"))
        {
            return "";
        }
        answer << line << std::endl;
    }
    return answer.str();
}
}