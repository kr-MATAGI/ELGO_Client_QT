# EFC (ELGO Framework Container)
 * 각 Process 는 EFC를 기반으로 구현되었다.
 * 현재 EFC 폴더는 아래와 같은 폴더 구조를 가진다.
 
 ```bash
    ├─EFC
    │  ├─Common
    │  │   ├─Interface
    │  │   │  ├─ContentsPlayDataDef.h
    │  │   │  ├─ContentsPlayDataImpl.cpp
    │  │   │  ├─ContentsPlayDataImpl.h
    │  │   │  ├─ScheduleImpl.cpp
    │  │   │  ├─ScheduleImpl.h
    │  │   │  ├─ScheduleJsonDef.h
    │  │   ├─CommonDef.h
    │  │   ├─DBDefinition.h
    │  │   ├─Definition.h
    │  │   ├─EventList.h
    │  ├─LocalSocketEvent
    │  │   ├─EFCEvent.cpp
    │  │   ├─EFCEvent.h
    │  │   ├─EventState.h
    │  │   ├─LocalSocketServer.cpp
    │  │   ├─LocalSocketServer.h
    │  ├─Logger
    │  │   ├─ELogger.cpp
    │  │   ├─ELogger.h
    │  ├─ShardMem (Not Using)
    │  │   ├─ShmCtrl.cpp
    │  │   ├─ShmCtrl.h
    │  │   ├─ShmDefinition.h
```

<hr />

## 프로세스간 Event 처리
 * 아래를 기본으로 각 프로세스마다 State.cpp에 등록된 <Event, Method> 쌍을 이용해 Event를 처리한다. <br>
<img src="/document/EFC/Flow-Chart/Event-Process.jpg" width="1200px" height="1000px" alt="EventProcess"></img></br>

<hr />

## Logger
 * 각 프로세스마다 ELGO_\[Process\]_Log 의 Logger.cpp 가 존재한다.
 * 각 Logger는 아래와 같은 형식의 Log를 출력한다.
 ```
 [yyyy-m-d_hh:mm:ss.msec][process][FileName:MethodName:line] [Log...]
 ```
 
 <hr />
 
## Common
### Interface
 * Process간 Event를 주고 받을 때, QByteArray를 이용하여 주고받는다. 이를 위해 operator<<, operator>>를 정의를 포함한다.

### Event List
 * Process 마다 수신받을 수 있는 Event를 포함한다.

### DBDefinition
 * device.db, schedule.db 에 사용되는 DB Query를 포함한다.
