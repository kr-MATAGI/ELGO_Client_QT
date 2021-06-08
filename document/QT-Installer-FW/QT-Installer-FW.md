# QT Installer Framework
## 아래에서는 QT Installer Framework를 사용하는 방법에 대해 간랸히 설명하고자 한다.
  * 참고 URL : [Youtube](https://www.youtube.com/watch?v=lYlLavZw10o)
  
___
### 사용 방법
  1) [download.qt.io](https://download.qt.io/official_releases/qt-installer-framework/) 에서 최신버전의 linux용 *.run 파일을 받는다.
  2) 위 .run 파일을 설치 후 임의의 위치에 ELGO-Client의 Installer가 생성될 폴더를 만든다.
  3) 생성 후 QT-Installer-FW가 설치된 폴더에서 examples/tutorial 폴더를 복사 한다. (/home/[user]/Qt/QtIFW-4.1.1/examples/tutorial)
  4) tutorial 폴더 내 config/config.xml 파일에서 Name, Version, Title, Publisher는 적당히 변경하고 <br> 
  TargetDir의 경우 Installer의 결과물이 생성될 경로를 입력한다.(기본경로, Installer 에서 변경 가능)
  5) tutorial 폴더 내 packages/com.vendor.product/meta/package.xml 파일에서 Script, UserInterface 부분을 제거한다. <br>
  그 다음 Default의 value를 true로 바꿔준다. <br>
  DisplayName, Description, Version, ReleaseDate는 적당히 변경한다. <br>
  Lincense의 경우 license.txt의 내용을 읽어 생성될 Installer에서 표시해준다. <br>
  installscript.qs와 page.ui는 사용하지 않도록 변경했으니 삭제해도 된다.
  6) packages/data 폴더에 빌드된 바이너리를 넣는다.
  7) /home/[user]/Qt/QtIFW-4.1.1/bin 경로의 binarycreator를 tutorial에 복사하여 config 및 packages 폴더와 경로를 같게 한다.
  8) 아래 명령어를 입력하면 installer가 생성된다 (*.run 파일) <br>
    <pre><code>
    /home/jaehoon/Qt/QtIFW-4.1.1/bin/binarycreator -c config/config.xml -p packages ELGO-Client-Installer.run
    </code></pre>
    
### 참고
  * tutorial/packages 내 com.vendor.product의 폴더명은 변경해도 된다. (e.g. com.elgo.client)
  * installer를 생성하는 과정 중 어느 경로에라도 '한글'이 포함된 경로가 있으면 오류를 유발하기 쉽다.
  * packages/data 폴더에 config.xml(설정 정보) 파일을 같이 넣어주면 /opt/ELGO 에 같이 설치된다.
  * ELGO_Remote의 build 폴더 또한 함께 packages/data 폴더 내에 넣어주면 같이 설치된다.
