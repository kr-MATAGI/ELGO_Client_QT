#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QDataStream>

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
         *  @note
         *          ELGO_VIEWER, CONTROL -> ELGO_MAIN
         *          receive status of process started
         *  @param
         *          ELGO_PROC::Proc proc
         */
        PROCESS_IS_READY = 257,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Change Device Options
         *  @param
         *          bool displaySleep
         *          bool deviceMute
         *          bool contentPause
         */
        UPDATE_DEVICE_OPTIONS = 258,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Change display sleep status
         *  @param
         *          bool bDisplaySleep
         */
        UPDATE_DISPLAY_SLEEP = 259,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          System Reboot
         *  @param
         *          NONE
         */
        SYSTEM_REBOOT_MAIN = 260,

        MAX_VIEWER_EVENT = 511
    };
}

namespace CONTROL_EVENT
{
    enum Event
    {
        NONE_CONTROL_EVENT = 512,

        /**
         *  @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          receive WIFI information from main process
         *  @param
         *          QString wasHost,
         *          quint16 wasHostPort,
         *          QString remoteTCPHost
         *          bool bIsDisplaySleep
         */
        RECV_SERVER_INFO_FROM_MAIN = 513,

        /**
         *  @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          Update display sleep status
         *  @param
         *          bool isDisplaySleep
         */
        UPDATE_DISPLAY_SLEEP_STATUS = 514,

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Receive screen capture response from elgo_viewer
         * @param
         *       QString payload.src
         *       QString payload.dest
         *       QString imagePath
         *       bool    bIsSuccessed
         */
        RESPONSE_SCREEN_CAPTURE = 515,

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
         *       ELGO_CONTROL -> ELGO_VIEWER
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

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Send custom play data information
         * @param
         *       CustomPlayDataJson customPlayData
         */
        CUSTOM_PLAY_DATA = 771,

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Send fixed play data information
         * @param
         *       FixedPlayDataJson customPlayData
         */
        FIXED_PLAY_DATA = 772,

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Send custom play data information
         *       with schedules
         * @param
         *       CustomPlayDataJson customPlayData
         *       QList<PlaySchedules> schedules
         */
        CUSTOM_PLAY_SCHEDULES = 773,

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Send fixed play data information
         *       with schedules
         * @param
         *       FixedPlayDataJson customPlayData
         *       QList<PlaySchedules> schedules
         */
        FIXED_PLAY_SCHEDULES = 774,


        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       current Screen capture on elgo_viewer
         *       if capture is successed, response to elgo_control
         *       Response Event : RESPONSE_SCREEN_CAPTURE
         * @param
         *       QString payload.src
         *       QString payload.dest
         */
        REQUEST_SCREEN_CAPTURE = 775,


        MAX_VIEWER_EVENT = 1023
    };
}

#endif // EVENTLIST_H
