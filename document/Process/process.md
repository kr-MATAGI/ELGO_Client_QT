# ELGO_Client의 Process별 기능
  * ELGO_Remote는 아래의 설명에서 제외한다.

## ELGO_Update
  * config.xml의 version과 Server에서의 version을 비교하여 Solution Binary를 Update.
  * 모든 작업이 완료된다면 ELGO_Main과 ELGO_Remote를 실행

## ELGO_Main
  * 다른 Process 동작을 위한 Initialize 동작 (Resource Folder, DB 등)
  * Internet Connection Status 확인.
  * ELGO_Control과 ELGO_Viewer를 Start 하는 동작
  * OS Command Line을 실행 (e.g. Sleep Status, Search Wifi list, Internet connect, Rotate Display...)
  * PlaySchedule, PowerSchedule 에 대한 Timer를 가지고 있으며 이 Timer들은 각각의 Timeout() 함수를 통해 Schedule을 수행.
  * schedule.db 에 있는 playData 등을 참고하여 Offline mode에서 재생할 play Data를 Viewer로 전달하여 재생한다.
  * DB Table의 정보는 document-DB에서 참고.
  
## ELGO_Control
  * Online Mode의 경우, 모든 동작은 ELGO_Control과 관련있다.
  * ELGO_Remote 및 Server 간 Web Socket 동작 수행.
  * libcurl 라이브러리를 이용해 각 Resource file 혹은 JSON / XML Data를 Download 및 Parsing (JSON / XML Parser).
  * ffmpeg 라이브러리를 이용해 Video file이 Download 되었을 경우, 비디오 파일의 총 재생 길이를 구함.
  * 위 총 재생길이는 Viewer에서 자연스러운 Play Back을 하기 위해 사용된다.
  * News나 Weather Type의 Widget이 Play Data에 포함되었을 경우, 이를 30분 주기로 Update 하기 위한 Timer 동작.
  * 사용자에게 보여주고 있는 현재 화면을 Viewer에서 Capture하여 Server로 전달할 수 있다.

## ELGO_Viewer
  * 사용자에게 보여지는 모든 것과 관련된 Process
  * ELGO_Remote에 접속할 수 있는 QR Code를 Display 하고, ContentsPlayer를 통해 다양한 Image / Video / Widget 을 보여준다.
  * 보여지는 Item 및 Widget의 종류는 아래와 같다.
    
    - Clock
    - Date
    - Image
    - Video
    - News
    - Subtitle
    - Weather
    
  * QR Code의 경우, QR Code generator library를 사용한다. (MIT License)
  * Offline 모드의 경우, News / Weather의 Data는 UpdateTimer에 의한 업데이트 되지 않는다.
  
