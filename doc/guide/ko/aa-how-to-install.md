# 설치 방법 {#aa-how-to-install}

Byeol은 64비트 운영체제에서만 동작합니다. Mac OS의 경우 Apple Silicon만 정식으로 지원하며, Intel 기반 Mac을 포함하여 타 OS는 직접 빌드해야 합니다.
간단한 코드를 테스트하고 싶다면 설치 없이 <a href="https://byeol.io/play" target="_blank">웹상에서 실행</a>해볼 수 있습니다.

---

## 다운로드

<a href="https://github.com/byeolang/byeol/releases" target="_blank">릴리즈 페이지</a>에서 가장 위에 있는 버전이 최신 버전입니다. Assets에서 운영체제에 맞는 파일을 다운로드하면 됩니다.

제공되는 파일은 다음과 같습니다. `-macos-arm64.pkg`은 Apple Silicon용 64비트 Mac OS 설치 pkg이며, `-ubuntu-x64.deb`는 64비트 우분투용 패키지 파일입니다. `-wasm.zip`은 WASM 바이너리로 WASM과 WASM을 읽기 위한 json 파일만 제공됩니다. Windows 사용자는 `-win-x64.msi` 설치 파일을 사용하는 것이 권장되며, 설치가 필요 없는 portable 버전인 `-win-x64-portable.zip`도 제공됩니다.

---

## Windows

`.msi` 설치 파일을 실행하여 설치를 진행합니다. 설치가 완료되면 환경변수에 byeol 프로그램이 자동으로 추가됩니다. 터미널을 열어 `byeol --version` 명령을 실행해서 설치가 정상적으로 완료되었는지 확인할 수 있습니다. `-portable.zip` 버전의 경우에는 환경변수 등록과 같은 절차가 수행되지 않으므로, 압축을 푼 폴더에서 직접 실행해야 합니다.

---

## ubuntu

`.deb` 파일을 다운 받아 다음 shell 명령어로 설치합니다.

```
@style: language-txt verified
# (필요시 sudo) dpkg -i <다운-받은-byeol-deb-파일>
$ sudo dpkg -i byeol-ubuntu-x64.deb
```

설치 완료 후 `byeol --version`을 입력해서 정상적으로 설치되었는지 확인합니다.

---

## Mac OS

`macos-arm64.pkg` 로 설치를 진행하세요.<br/>
설치가 차단된다면 당황하지 마세요! '시스템 설정 > 개인정보 보호 및 보안'에서 <b>그래도 허용</b>을 누르면 설치가 진행됩니다.<br/>

<img src="../../../assets/images/macos-install1.png" style="width: 32%; height: auto; border-radius: 8px;">
<img src="../../../assets/images/macos-install2.png" style="width: 32%; height: auto; border-radius: 8px;">
<img src="../../../assets/images/macos-install3.png" style="width: 32%; height: auto; border-radius: 8px;">

매년 맥 개발자 등록비용을 내지 않은 경우, pkg에 서명하지 않아 뜨는 메시지일뿐 악성코드는 없습니다.
이후 터미널을 켜서 `byeol --version`을 입력해 정상적으로 실행되는지 확인합니다.

---

## WASM

WASM 버전을 사용하려면 추가적인 코드가 필요합니다. 실제로 <a href="https://github.com/byeolang/website/tree/main/play" target="_blank">website</a>를 만들 때 사용하고 있으므로, 예시가 필요하다면 해당 코드를 참고하세요.

---
**다음 문서**: @ref ab-basic-usage
