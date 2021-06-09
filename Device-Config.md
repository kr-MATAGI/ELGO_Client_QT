# 기기 설정 방법
* 아래에서는 ELGO 디지털 사이니지 솔루션을 위한 기기 설정 방법을 설명한다.
* 사용되는 Directory Path
  - DB : /opt/ELGO/DB (없으면 자동 생성)
  - Resource : /opt/ELGO/resource (없으면 자동 생성)
  - ELGO_Remote : /opt/ELGO/Remote/build

## 검은 배경화면
  * 검은색 이미지 사용

## 마우스 커서 보이지 않게 설정
  * http://seorenn.blogspot.com/2011/03/ubuntulinux.html
  1. sudo apt-get install unclutter
  2. unclutter -display :0.0 -idle 5 <br>
   > 5초이상 움직이지 않으면 마우스 커서를 숨김

## 최상단 표시줄 지우기
  * https://m.blog.naver.com/jamg123123/221768857065 <br>
   > 전체화면시 최상단 표시줄을 없애준다.

## Docker 숨기기
  * 우분투 설정 - 모양 - 자동으로 독 숨기기

## 절전모드 끄기
  * 우분투 설정 - 전원 - 절전
  
## 부팅로고 변경 (Splash 변경)
  * http://work6.mrds.kr/bbs/board.php?bo_table=MyTips&wr_id=91&page=2
  * http://no1linux.org/hottips/27817 <br>
  * https://askubuntu.com/questions/2007/how-do-i-change-the-plymouth-bootscreen <br>
  * https://www.gnome-look.org/browse/cat/108/order/latest/ <br>
   > 우분투에는 plymouth가 기본으로 설치되어 있는 듯 한데, Progress Bar가 있는 테마를 <br>
   다운받아 .png 이미지만 교체하면 될 것으로 보인다.
   
## 터미널 오픈 단축키 변경
  * 우분투 설정 - 키보드 바로가기

## 시작 프로그램 설정
  * http://choesin.com/ubuntu-linux%EC%97%90%EC%84%9C-%EC%8B%9C%EC%9E%91-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%A8%EC%9D%84-%EA%B4%80%EB%A6%AC%ED%95%98%EB%8A%94-%EB%B0%A9%EB%B2%95
  * Alt + F2
  * gnome-session-properties
    <br>만약 설치가 안되었다면 sudo apt install gnome-startup-applications
  * 시작 프로그램에 'elgo_update' 바이너리 추가 <br>
  * 기존 설정을 실수로 지웠을 시 <br>
   (기존 설정되어있는 값을 제거시 사용자에게 확인하는 절차가 없음) <br>
   
   - SSH Key Agent <br>
   /usr/bin/gnome-keyring-daemon --start --components=ssh <br>
   GNOME Keyring: SSH Agent <br>

   - im-launch <br>
   sh -c 'if [ "x$XDG_SESSION_TYPE" = "xwayland" ] ; then exec env IM_CONFIG_CHECK_ENV=1 im-launch true; fi'

## 자동 업데이트 끄기
 * https://hiseon.me/linux/ubuntu/ubuntu-disable-automatic-upgrades/ <br>
 * 우분투 Software & updates - Updates - Automatically check for updates 부분을 Never로 설정.

## 우분투 오류보고창 끄기
 * https://lute3r.tistory.com/63
 * sudo gedit /etc/default/apport
   <br>enabled=1 > enabled=0 변경
 * sudo service apport stop

## 바탕화면 Icon 숨기기
 * https://www.omgubuntu.co.uk/2020/03/remove-trash-from-desktop-ubuntu
 * GNOME Tweaks를 설치한다.
 * Alt + F2를 입력후 gnome-tweaks를 실행
 * 확장 탭을 클릭하면 Desktop Icons가 있다. on/off 옆 설정버튼을 누르고 휴지통을 비롯한 icon의 숨길 수 있다.

## Wifi 연결 과정에서 아래와 같은 pop-up을 방지
![image](https://user-images.githubusercontent.com/30927066/120747099-6bf6a280-c53b-11eb-8e52-a2f82c7c26ca.png)
  
  * https://askubuntu.com/questions/67500/how-to-disable-notification-from-network-manager
  <code><pre>
      TODO....
  </code></pre>

## 중요 - 동적 링크
 * QT를 상업용으로 배포할려면 상업용 라이센스를 구매하거나 LGPL 라이센스의 제약을 따라야한다.
 * 상업용 라이센스는 잠시 찾아보면 개발자당 부과하며 연간 요금을 부과하는 것으로 보인다.
 * 따라서 LGPL 라이센스를 따라야하며 배포시 같이 *.so 파일들을 배포한다.
 * https://jacking75.github.io/Linux_lib_setting/
 * https://blog.kesuskim.com/2018/06/spl-cpp-3/
 * ldd 명령어로 각 실행파일마다 필요한 so파일들을 확인하고
 * /etc/ld.so.conf 에 동적 라이브러리 정보를 설정 후 ldconfig 명령어를 사용해 /etc/ld.so.cache 파일로 만들어 주거나
 * 리눅스 커널 2.6 이후부터는 /etc/ld.so.conf.d 폴더에 'xxx.conf'로 설정할 수 있다.
 * 보통  캐시로 설정하는 것이 빠르다고 한다.
 * 아래의 주소들에서 간단히 deploy 하는 방법이 있다. <br>
  [Blog](https://yonglimlee.tistory.com/entry/QT-linux-deploy-%EC%9A%B0%EB%B6%84%ED%88%AC%EC%97%90%EC%84%9C-%EB%B0%B0%ED%8F%AC%ED%95%98%EA%B8%B0-2004LTS-64bit) <br>
  [git-release](https://github.com/QuasarApp/CQtDeployer/wiki/Build-and-Install) <br>
 
