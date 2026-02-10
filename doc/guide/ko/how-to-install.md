# 설치 방법
* 64비트 운영체제만 지원합니다.
* Mac OS는 Apple Silicon 만 지원합니다.
* 간단한 코드라면 설치 없이 [웹상에서 실행](https://byeol.io/play)해볼 수도 있습니다.


## 다운로드
* [릴리즈 페이지](https://github.com/byeolang/byeol/releases) 에서 가장 위에 있는 버전이 최신 버전입니다.
* Assets에서 OS에 맞게 다운로드 받으면 됩니다.
* 다음과 같이 구성되어 있습니다.
    * `-macos-arm64.zip`: Apple silicon 용 64비트 Mac OS 용 바이너리
    * `-ubuntu-x64.deb`: 64비트 우분투 용 패키지 파일
    * `-wasm.zip`: WASM 바이너리. WASM과 WASM을 읽기 위한 json 파일만
    제공됩니다.
    * `-win-x64.msi`: Windows 64비트 설치 파일 (권장)
    * `-win-x64-portable.zip`: Windows 64비트 portable 버전 (설치 불필요)


## Windows
* `.msi` 설치 파일을 설치합니다.
* 설치가 완료되면 **환경변수에 byeol** 프로그램이 추가 됩니다.
* 터미널을 열어 **byeol --version** 명령을 실행해서 설치가 잘 되었는지 확인합니다.
* `-portable.zip`의 경우에는 당연히 환경변수 등록과 같은 절차가 수행되지
않습니다.


## ubuntu
* `.deb` 파일을 다운 받아, 다음 shell 명령어로 설치합니다.
```
@style: language-txt
# (필요시 sudo) dpkg -i <다운-받은-byeol-deb-파일>
$ sudo dpkg -i byeol-ubuntu-x64.deb
```

* 설치 완료 후, `byeol --version` 을 입력해서 완료되었는지 확인합니다.


## Mac OS
* `arm64.zip` 파일을 다운 받습니다.
* 적당한 곳에 압축을 풀고, 해당 폴더에 들어가 `byeol --version`을 입력해 실행을
확인합니다.
* 아직 설치버전을 제공하지 않으므로, 환경변수에 `byeol`이 등록되지는 않습니다.


## WASM
* 사용을 위해서는 추가적인 코드가 필요합니다.
* 실제로 [website](https://github.com/byeolang/website/tree/main/play) 를 만들때 사용하고 있습니다. 예시가 필요하다면 해당 코드를 참고하세요.


## 다음은,
* 설치가 완료되었으면 @ref basic-usage "사용"을 해봐야겠죠.
