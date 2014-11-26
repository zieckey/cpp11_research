
#include "test_common.h"


#ifdef WIN32
    #ifdef _DEBUG
        #pragma comment(lib, "gtestd.lib")
        #pragma comment(lib, "gtest_maind.lib")
    #else
        #pragma comment(lib, "gtest.lib")
        #pragma comment(lib, "gtest_main.lib")
    #endif
#endif

#ifdef WIN32
struct OnExit
{
    ~OnExit()
    {
        system("pause");
    }
} __s_onexit_pause;
#endif
