
#include "test_common.h"

namespace
{
#ifdef WIN32
    struct OnAppExit
    {
        ~OnAppExit()
        {
            system("pause");
        }
    } __s_onexit_pause;
#endif
}
