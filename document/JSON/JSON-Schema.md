# JSON Schema

## ELGO_Control <-> ELGO_Remote
### 1. Request (ELGO_Remote -> ELGO_Control)
* Request <br>
  + 'action' 에 따라 각 Object가 JSON 데이터에 삽입된다.
<pre><code>
{
  "action" : String,
  "deviceLogin" : {
    "id" : String,
    "pw" : String
  },
  "manageDevice" : {
    "oldPassword": String,
    "newPassword" : String,
  },
  "rotateDisplay": {
    "newHeading" : String
  },
  "deviceOptions" : {
    "displayOnOff" : boolean,
    "deviceMute" : boolean,
    "contentPause" : boolean
  },
  "wifi" : {
    "ssid" : String,
    "password" : String,
    "encryption" : boolean
}
</code></pre>

* Action
  + 'action'은 아래와 같은 종류로 구분된다.
<pre><code>
"action" : [
  "NONE_ACTION",
  "DEVICE_LOGIN",
  "UPDATE_WIFI_LIST",
  "CONNECT_WIFI",
  "MANAGE_DEVICE",
  "ROTATE_DISPLAY",
  "DEVICE_OPTIONS",
  "USER_LOGIN"
]
</code></pre>

* Heading
  + 'heading'은 기기 회전을 위한 Value로써 아래와 같이 정의된다.
<pre><code>
"DIRECTION" : [
  "HEAD_TOP",
  "HEAD_RIGHT",
  "HEAD_BOTTOM",
  "HEAD_LEFT"
]
</code></pre>

### 2. Response (ELGO_Control -> ELGO_Remote)
* Date
  + 각 Response에는 'date'를 추가하여 JSON 데이터를 전달한다.
<pre><code>
date : yyyy-mm-dd_hh:mm:ss.msec"
</code></pre>
<br>

* 대부분의 Response의 데이터가 동일한 부분이 많지만 <br>
  코드상에서는 Log의 출력, 추후 기능 추가 등을 고려해 함수를 분리시켰다. <br>
  함수명만 다를 뿐이며 코드 내부에서 공통적인 함수를 Call 한다.
  본 문서에서도 추후 기능 추가를 위해 'action' 별로 구분한다.
<br>  

* Action.DEVICE_LOGIN
<pre><code>
{
  "date" :  DateTime,
  "action" : String,
  "result" : boolean
}
</code></pre>

* Action.MANAGE_DEVICE
<pre><code>
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean
}
</code></pre>

* Action.ROTATE_DISPLAY
<pre><code>
action.RotateDisplay
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean
}
</code></pre>

* Action.DEVICE_OPTIONS
<pre><code>
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean
}
</code></pre>

* Action.UPDATE_WIFI_LIST
<pre><code>
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean,
  "wifiList" : [{
    "ssid" : String,
    "freq" : int,
    "signal" : int,
    "encryption" : boolean
  }]
}
</code></pre>

* Action.CONNECT_WIFI
<pre><code>
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean,
}
</code></pre>

* Action.USER_LOGIN
<pre><code>
{
  "date" : DateTime,
  "action" : String,
  "result" : boolean,
  "os" : String,
  "udid" : String
}
</code></pre>
