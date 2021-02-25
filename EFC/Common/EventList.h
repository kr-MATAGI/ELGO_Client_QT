#ifndef EVENTLIST_H
#define EVENTLIST_H

namespace COMM_EVENT
{
    enum Event
    {
        NONE_COMM_EVENT = 0,


        MAX_COMM_EVENT = 255
    };
};

namespace MAIN_EVENT
{
    enum Event
    {
        NONE_MAIN_EVENT = 256,

        /**
         *  @brief  receive status of process started
         *  @param  ELGO_PROC::Proc proc
         */
        PROCESS_IS_READY = 257,

        MAX_VIEWER_EVENT = 511
    };
};

namespace CONTROL_EVENT
{
    enum Event
    {
        NONE_CONTROL_EVENT = 512,

        MAX_CONTROL_EVENT = 767
    };
};

namespace VIEWER_EVENT
{
    enum Event
    {
        NONE_VIEWER_EVENT = 768,

        /**
        * @note
        *       ELGO_MAIN -> ELGO_VIEWER
        *       Viewer will make qr code image and display.
        * @param
        *       NONE
        */
        MAKE_QRCODE = 769,

        MAX_VIEWER_EVENT = 1023
    };
};

#endif // EVENTLIST_H
