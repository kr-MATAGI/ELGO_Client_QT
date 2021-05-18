## DB Schema

## device.db
  * ELGO_Remote에서 Device_Login Action에 필요한 ID와 Password를 저장.
  * MainWindow에서 QR과 함께 보이는 Device Name을 저장.

### device table

|Name|Type|Default Value|Allow Null|Primary key|
|:--:|:--:|:-----------:|:--------:|:---------:|
|id|VARCHAR(30)|root|NO|NO|
|pw|VARCHAR(30)|root|NO|NO|
|name|VARCHAR(30)|elgo_[random]|YES|NO|

<br>

## schedule.db
  * 서버에서 전달된 Play Data, Play Schedule, Power Schedule을 저장한다.
  * 마지막에 재생하고 있는 Play Data의 ID와 Type을 저장한다.

### playData table
  * 'data' 항목에는 서버에서 전달받은 JSON이 저장된다. 

|Name|Type|Default Value|Allow Null|Primary key|
|:--:|:--:|:-----------:|:--------:|:---------:|
|id|VARCAHR(30)|.|NO|YES|
|type|INTEGER|.|NO|YES|
|data|BLOB|.|NO|NO|

### playSchedule table
  * cron에는 playSchedule을 가공한 Cron List가 저장된다.

|Name|Type|Default Value|Allow Null|Primary key|
|:--:|:--:|:-----------:|:--------:|:---------:|
|id|VARCAHR(30)|.|NO|YES|
|startDate|TEXT|.|NO|NO|
|endDate|TEXT|.|NO|NO|
|playDataId|VARCHAR(30)|.|NO|NO|
|playDataType|INTEGER|.|NO|NO|
|cron|BLOB|.|NO|NO|

### powerSchedule table
  * data에는 powerSchedule JSON이 저장된다.

|Name|Type|Default Value|Allow Null|Primary key|
|:--:|:--:|:-----------:|:--------:|:---------:|
|id|VARCAHR(30)|.|NO|YES|
|isOn|BOOLEAN|.|NO|YES|
|startDate|TEXT|.|NO|NO|
|endDate|TEXT|.|NO|NO|
|playDataId|VARCHAR(30)|.|NO|NO|
|playDataType|INTEGER|.|NO|NO|
|cron|BLOB|.|NO|NO|

### playingInfo table
  * 마지막에 재생된 PlayData에 대한 ID와 Type을 저장한다.

|Name|Type|Default Value|Allow Null|Primary key|
|:--:|:--:|:-----------:|:--------:|:---------:|
|id|VARCHAR(30)|.|NO|YES|
|type|INTEGER|.|NO|YES|
