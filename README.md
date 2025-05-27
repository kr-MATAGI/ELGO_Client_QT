# ELGO Client QT
디지털 사이니지를 위한 QT 기반 클라이언트 시스템입니다.

## 📌 주요 공지사항
- 상용 바이너리 빌드 전 `DownloadAdditionalWidgetInfo()` 및 `UpdateWidgetTimeout()`의 nx, ny 하드코딩 제거 필요
- 기기 Root 비밀번호는 반드시 `elgo2580*`로 설정

## 🚀 시스템 구성
ELGO Client QT는 다음과 같은 프로세스들로 구성되어 있습니다:

- **ELGO_Main**: 시스템의 메인 프로세스
- **ELGO_Control**: 시스템 제어 프로세스
- **ELGO_Viewer**: 콘텐츠 표시 프로세스
- **ELGO_Update**: 시스템 업데이트 프로세스
- **ELGO_Remote**: 원격 제어 프로세스

## 💫 주요 기능 및 프로세스 흐름

### 1. 시스템 부팅
시스템 시작 시 프로세스 초기화 및 부팅 과정
![Start_Flow](/document/ReadMe-Flow-Chart/Start_Process_Flow.jpg)

### 2. ELGO_Main 초기화
메인 프로세스의 초기화 과정
<img src="/document/ReadMe-Flow-Chart/Main_Initialize.jpg" width="300px" height="800px" alt="Main_Init"></img>

### 3. 프로세스 시작
> 주의: Viewer가 Process Ready 신호를 Main으로 전송한 후 Control이 StartProcess()를 실행하도록 변경됨
<img src="/document/ReadMe-Flow-Chart/Start_Client.jpg" width="1200px" height="1000px" alt="Start Flow"></img>

### 4. 콘텐츠 서버 연결
<img src="/document/ReadMe-Flow-Chart/ContentServer_Connection.jpg" width="1200px" height="1000px" alt="connect2Server"></img>

## 📺 콘텐츠 관련 기능

### 1. 단일 재생
<img src="/document/ReadMe-Flow-Chart/SignlePlay_Event.jpg" width="1200px" height="1000px" alt="singlePlay"></img>

### 2. 스케줄 재생
<img src="/document/ReadMe-Flow-Chart/PlaySchedule_Event.jpg" width="1200px" height="1000px" alt="playSchedule"></img>

### 3. 전원 스케줄
<img src="/document/ReadMe-Flow-Chart/PowerSchedule_Event.jpg" width="1200px" height="1000px" alt="powerSchedule"></img>

### 4. 화면 캡처
<img src="/document/ReadMe-Flow-Chart/Screen-Capture-Event.jpg" width="1200px" height="1000px" alt="screenCapture"></img>
