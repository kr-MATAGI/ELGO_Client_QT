## Used Command line List
 - 매우 기본적인 명령어는 제외

### elgo_update -> UpdateWindow.cpp
   1. tar -xvf [.tarPath] -C [destPath]
   2. cd [elgo_remote path] && npm start
   
### elgo_main -> WifiManager.cpp
  1. /bin/sh -c "iw dev | grep Interface" - 무선랜 인터페이스 이름을 얻는다.
  2. /bin/sh -c "echo [password] | sudo -S iw [wlanName] scan | egrep 'SSID:|freq:|signal:|RSN:'" - 무선랜 리스트 검색
  3. /bin/sh -c "nmcli dev wifi con [ssid] or <br>
     /bin/sh -c "nmcli dev con [ssid] password [password] - 무선랜 연결
  4. /bin/sh -c "echo [password] | sudo -S ip link set [wlanName] up" - 무선랜 인터페이스 ON (RF-Kill 오류가 뜨면 block 직접 해제해야됨)

### elgo_main -> DeviceManager.cpp
  1. /user/bin/xset -display :0.0 dpms force [on|off] - 절전 모드 설정
  2. /bin/sh -c "sleep 5 && reboot" - 5초 이후에 시스템 재시작
  3. /bin/sh -c "amixer -q -D pulse sset Master [on|off] - 음소거
  4. /bin/sh -c "xrandr -o [normal|right|inverted|left] - 모니터 윗 방향을 기준으로 옵션에 따라 회전.
