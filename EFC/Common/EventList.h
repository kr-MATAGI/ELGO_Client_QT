#ifndef EVENTLIST_H
#define EVENTLIST_H

namespace COMM_EVENT
{
    enum Event
    {
        NONE_COMM_EVENT = 0,


        MAX_COMM_EVENT = 255
    };
}

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

        /**
         *  @brief  Change Device Options
         *  @param
         *          bool displayOnOff
         *          bool deviceMute
         *          bool contentPause
         */
        CHANGE_DEVICE_OPTIONS = 258,

        MAX_VIEWER_EVENT = 511
    };
}

namespace CONTROL_EVENT
{
    enum Event
    {
        NONE_CONTROL_EVENT = 512,

        /**
         *  @brief  receive WIFI information from main process
         *  @param
         *          QString wasHost,
         *          quint16 wasHostPort,
         *          QString remoteTCPHost
         */
        RECV_SERVER_INFO_FROM_MAIN = 513,

        MAX_CONTROL_EVENT = 767
    };
}

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
        *       QString ip
        */
        MAKE_QRCODE = 769,

        /**
        * @note
        *       ELGO_CONTROL -> ELGO_VIEWER
        *       Rotate Display accroding to heading enum value.
        * @param
        *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
        */
        ROTATE_DISPLAY = 770,

        MAX_VIEWER_EVENT = 1023
    };
}

#endif // EVENTLIST_H
