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
<img src="/document/EFC/Flow-Char/Event-Process.jpg" width="1200px" height="1000px" alt="EventProcess"></img></br>
