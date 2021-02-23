#ifndef DEIFINITION_H
#define DEIFINITION_H

#include <QString>

namespace ELGO_PROC
{
    enum Proc
    {
        NONE_PROC = 0,
        ELGO_MAIN = 1,
        ELGO_CONTROL = 2,
        ELGO_VIEWER = 3,
        UNKOWN = 255
    };
    static const char *ELGOProc_enum2str[] = { "none", "elgo_main", "elgo_control",
                                         "elgo_viewer", "unkown"};
};

#endif // DEIFINITION_H
