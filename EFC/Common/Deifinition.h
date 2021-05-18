#ifndef DEIFINITION_H
#define DEIFINITION_H

#include <QString>

namespace ELGO_SYS
{
    enum Proc
    {
        NONE_PROC = 0,
        ELGO_MAIN = 1,
        ELGO_CONTROL = 2,
        ELGO_VIEWER = 3,
        ELGO_UPDATE = 4,
        ELGO_REMOTE = 5,
        UNKOWN_PROC = 255
    };
    static const char *ELGOProc_enum2str[] = { "none", "elgo_main", "elgo_control",
                                               "elgo_viewer", "elgo_update", "elgo_remote",
                                               "unkown" };
}

#endif // DEIFINITION_H
