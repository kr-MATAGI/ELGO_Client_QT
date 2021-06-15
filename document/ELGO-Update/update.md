### S3 링크
[배포를 위한 S3 사이트](https://s3.console.aws.amazon.com/s3/buckets/elgo-common?region=ap-northeast-2&prefix=update/&showversions=false)

### 버전 확인 및 업데이트 과정
  1. https://[app | demo].elgo.co.kr/client/version/ubuntu/main 을 curl로 요청하면 아래와 같은 json data가 전달된다.
  <pre><code>
    { "version": 1.0 }
  </code></pre>

  2. elgo_update 실행시 config.xml에서 읽었던 version 과 비교한다. <br>
    서버 및 config.xml의 version 데이터는 무조건 소수형태여야 한다.
  3. 만약 서버 버전이 더 높아 업데이트가 필요하다면 아래 주소에서 각각 다운로드한다.
   <pre><code>
    https://cloud.elgo.co.kr/ubuntu/[elgo_client.tar | elgo_remote.tar]
   </code></pre>
  5. 다운로드가 다되면 폴더에 tar 압축 파일이 2개 생기고 QProcess 명령어를 통해 압축을 해제하고 기존 바이너리 파일이 교체된다.

### 서버에 바이너리 압축파일을 업로드 하는 과정
  * 아래 주소에 있는 가이드를 참조한다. <br>
   [업로드 가이드](https://github.com/JohnnyJuheonShin/document/blob/main/%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8%20%EB%B0%B0%ED%8F%AC.md)
