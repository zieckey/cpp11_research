#include "test_common.h"

#include <osl/include/exp.h>
#include <wdurlsplit/include/url_parser.h>

TEST_UNIT(urlparser_test)
{
    std::string url = "http://www.haosou.com?query=360";
    wdurl::UrlParser p(url);
    std::cout << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii host=" << p.host << "\n";
    H_TEST_ASSERT(p.host == "www.haosou.com");
}

#define H_COMPILER_VERSION (((__GNUC__)*100) + \
                           (__GNUC_MINOR__*10) + \
                           __GNUC_PATCHLEVEL__)

void p()
{
    std::cout << "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy gcc version=" << H_COMPILER_VERSION << "\n";
}


TEST_UNIT(GCC_VERSION)
{
    std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx gcc version=" << H_COMPILER_VERSION << "\n";
    std::function< void () > f(p);
    f();
}
