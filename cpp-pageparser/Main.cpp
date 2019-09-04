#include "page.h"
#include "page_parser.h"
#include "pageTree.h"

int main(int arg_num, char *args[])
{
    using namespace my;

    // PageParser parser("www.boost.org/doc/libs/1_64_0/doc/html/boost_asio.html", 3);
    PageParser parser("http://www.boehmwanderkarten.de/kartographie/is_netze_cyl.html", 1);
    Page test = parser.parse();

    //creating output file
    PageTree tree(test);
    tree.generateDOTFile();
    return 0;
}

//graph of partial result: http://rise4fun.com/Agl/Z1zQ