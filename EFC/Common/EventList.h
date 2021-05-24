#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QDataStream>

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

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Search available wifi list
         *  @param
         *          NONE
         */
        SEARCHING_WIFI_LIST = 261,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Connect new wifi
         *  @param
         *          QString ssid
         *          QString password
         *          bool encryption
         */
        CONNECT_NEW_WIFI = 262,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          For manage custom/fixed play schedule
         *  @param
         *          PlayDataType    playDataType
         *          [ CustomPlayDataJson  customPlayData ||
         *            FixedPlayDataJson   fixedPlayData ]
         *          QVector<ScheduleJson::PlaySchedule> playScheduleList
         */
        UPDATE_PLAY_SCHEDULE_LIST = 263,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Clear All Play Schedule List
         *          Cause by single play event
         *  @param
         *          NONE
         */
        CLEARE_ALL_PLAY_SCHEDULE_LIST = 264,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Clear Play Schedule ID by clearPlaySchedule Event
         *  @param
         *          QString scheduleId
         */
        DELETE_PLAY_SCHEDULE_BY_ID = 265,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Add PlayData to DB
         *          Cause by single play event
         *  @param
         *          PlayJson::PlayDataType  playJson
         *          [ CustomPlayDataJson  customPlayData ||
         *            FixedPlayDataJson   fixedPlayData ]
         */
        ADD_PLAY_DATA_TO_DB = 266,

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Update Power Schedule
         *  @param
         *          ScheduleJson::PowerSchedule powerSchedule
         */
        UPDATE_POWER_SCHEDULE_LIST = 267,


        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Delete Power Schedule by ID
         *  @param
         *          QString scheduleId
         */
        DELETE_POWER_SCHEDULE_BY_ID = 268,


        /**
         *  @note
         *          ELGO_VIEWER -> ELGO_MAIN
         *          Save Current Playing PlayData to DB
         *  @param
         *          int playDataId
         *          PlayJson::PlayDataType type
         */
        SAVE_PLAYING_DATA_TO_DB = 269,

        /**
         * @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Screen Rotation Command line
         * @param
         *          quint8  heading
         */
        MAIN_ROTATE_SCREEN = 270,

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
         *       ELGO_VIEWER -> ELGO_CONTORL
         *       Receive screen capture response from elgo_viewer
         * @param
         *       QString payload.src
         *       QString payload.dest
         *       QString imagePath
         *       bool    bIsSuccessed
         */
        RESPONSE_SCREEN_CAPTURE = 515,

        /**
         * @note
         *       ELGO_MAIN -> ELGO_CONTROL
         *       Finish searching and update wifi list
         * @param
         *       int    wifiCnt
         *       [Loop]
         *       QString    ssid
         *       int    freq
         *       int    signal
         *       bool   bEncryption
         *       [End Loop]
         */
        UPDATE_WIFI_LIST = 516,

        /**
         * @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          New WIFI connection result
         * @param
         *          bool bIsConnect
         *          QString newIP
         */
        WIFI_CONNECTION_RESULT = 517,


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
         *       QString    ip
         *       QString    deviceName
         */
        MAKE_QRCODE = 769,

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Rotate Display accroding to heading enum value.
         * @param
         *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
         */
        VIEWER_ROTATE_SCREEN = 770,

        /**
         * @note
         *       ELGO_CONTROL, ELGO_MAIN -> ELGO_VIEWER
         *       Send custom play data information
         * @param
         *       CustomPlayDataJson customPlayData
         */
        PLAY_CUSTOM_PLAY_DATA = 771,

        /**
         * @note
         *       ELGO_CONTROL, ELGO_MAIN -> ELGO_VIEWER
         *       Send fixed play data information
         * @param
         *       FixedPlayDataJson fixedPlayData
         */
        PLAY_FIXED_PLAY_DATA = 772,

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


        /**
         * @note
         *       ELGO_MAIN -> ELGO_VIEWER
         *       ADD Custom PlayData after system booting
         * @param
         *       QVector<PlayJson::CustomPlayDataJson>  customPlayDataList
         */
        ADD_CUSTOM_PLAY_DATA_LIST = 776,

        /**
         * @note
         *       ELGO_MAIN -> ELGO_VIEWER
         *       ADD Fixed PlayData after system booting
         * @param
         *       QVector<PlayJson::FixedPlayDataJson>  fixedPlayDataList
         */
        ADD_FIXED_PLAY_DATA_LIST = 777,


        /**
         * @note
         *       ELGO_MAIN -> ELGO_VIEWER
         *       Contents Player item pause or play
         * @param
         *       bool isPause
         */
        UPDATE_PLAYER_PAUSE_STATUS = 778,


        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Start Close Main Window Timer
         * @param
         *       NONE
         */
        CLOSE_MAIN_WINDOW_BY_ERROR = 779,

        MAX_VIEWER_EVENT = 1023
    };
}

#endif // EVENTLIST_H
