# ELGO_Client_QT의 Sequence Diagram
* 아래는 핵심이 되는 기능들의 Sequence Diagram 이다.
* 주의 - 상용을 위한 Binary 만들기 전, DownloadAdditionalWidgetInfo() 와 UpdateWidgetTimeout() 에서 nx, ny의 hard coding을 제거
* 기기의 Root Password는 elgo2580* 으로 설정해야한다.

## 포함된 프로세스
* ELGO_Main
* ELGO_Control
* ELGO_Viewer
* ELGO_Update
* ELGO_Remote

## System booting
![Start_Flow](/document/ReadMe-Flow-Chart/Start_Process_Flow.jpg)


## ELGO_Main의 Initialize
<img src="/document/ReadMe-Flow-Chart/Main_Initialize.jpg" width="300px" height="800px" alt="Main_Init"></img></br>

## Start process
  * Start Proccess 시퀀스의 경우 Viewer 가 Process Ready를 Main에 보낸 후 Control을 StartProcess() 하도록 변경하였음.
<img src="/document/ReadMe-Flow-Chart/Start_Client.jpg" width="1200px" height="1000px" alt="Start Flow"></img></br>


## Connect with contents server
<img src="/document/ReadMe-Flow-Chart/ContentServer_Connection.jpg" width="1200px" height="1000px" alt="connect2Server"></img></br>


## Single Play Event
<img src="/document/ReadMe-Flow-Chart/SignlePlay_Event.jpg" width="1200px" height="1000px" alt="singlePlay"></img></br>

## Play Schedule Event
<img src="/document/ReadMe-Flow-Chart/PlaySchedule_Event.jpg" width="1200px" height="1000px" alt="playSchedule"></img></br>

## Power Schedule Event
<img src="/document/ReadMe-Flow-Chart/PowerSchedule_Event.jpg" width="1200px" height="1000px" alt="powerSchedule"></img></br>

## Screen Capture Event
<img src="/document/ReadMe-Flow-Chart/Screen-Capture-Event.jpg" width="1200px" height="1000px" alt="screenCapture"></img></br>
