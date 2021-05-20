# EFC (ELGO Framework Container)
 * 각 Process 는 EFC를 기반으로 구현되었다.
 * 현재 EFC 폴더는 아래와 같은 Tree 구조를 가진다.
 
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

