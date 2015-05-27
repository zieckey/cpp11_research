#include "test_common.h"

#include <wdurlsplit/include/url_parser.h>

TEST_UNIT(urlparser_test)
{
    std::string url = "http://www.haosou.com?query=360";
    wdurl::UrlParser p(url);
    std::cout << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii host=" << p.host << "\n";
    H_TEST_ASSERT(p.host == "www.haosou.com");
}
