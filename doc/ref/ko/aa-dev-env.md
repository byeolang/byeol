# 개발환경 {#aa-dev-env}

빌드를 비롯해서 테스트, 커버리지 확인, 코드 포맷팅 등 모든 개발 환경은 `build/builder.py`를 통해서 이뤄집니다. `builder.py`는 작업별로 필요로 하는 툴이나 프로그램이 무엇인지, 버전은 만족하는지를 사전에 체크해서 보여주기 때문에 `builder.py`를 다룰 줄만 안다면 기본적인 빌드 환경은 쉽게 구축할 수 있어요.

`builder.py`를 실행하기 위해서는 `git`과 `python3` 3.6.0+이 설치되어야 합니다.

`builder.py help`를 실행하면 지원하는 명령어와 옵션을 자세히 알 수 있어요.

---

## 빌드

빌드만 하고 싶은 경우 다음 프로그램이 추가로 필요합니다: `CMake` 2.6.0+, `Make` 3.0+, `Clang++` 14.0+ 혹은 `gcc` 8.0.0+ 혹은 `MSBuild.exe`(VS2022) 17.0.0+, `Bison` 3.8.0+, `Flex` 2.6.0+, `clang-tidy` 14.0.0+.

설치 전에 반드시 버전을 체크해서 받아야 합니다. OS별로 다음 내용을 참고해서 필요한 프로그램을 설치하면 됩니다.


### Ubuntu

Bison과 Flex는 구 버전 Ubuntu에서는 apt 패키지에 포함되지 않을 가능성이 있습니다. 이때는 수동으로 Bison과 Flex를 [소스코드](http://ftp.gnu.org/gnu/bison/)를 다운받아 직접 빌드해야 합니다. 우분투는 기본적으로 gcc를 사용하는데, 현재 프로젝트는 clang++를 더 권장합니다 (물론 gcc 빌드도 지원은 하고 있어요).


### Mac OS

Mac OS는 기본적으로 clang을 사용하기 때문에 비교적 설치가 쉬워요. Apple Silicon(arm64)이나 인텔(x64) 모두 빌드를 지원합니다.


### Windows

윈도우 개발 환경은 크게 Visual Studio, VS Code 등을 사용하는 방법과 WSL을 경유해서 Ubuntu로 개발하는 방법 2가지로 분류됩니다. WSL을 사용하지 않는다면, 개인적으로는 VS Code를 더 권장합니다. 참고로 Bison과 Flex는 [win_flex](https://github.com/lexxmark/winflexbison)를 사용을 권장합니다. 설치 후 환경변수 설정이 필요하며, 이름을 `flex`, `bison`으로 각각 변경해야 합니다.

#### WSL

WSL2를 설치합니다. 이후 작업은 Ubuntu 과정과 동일합니다.


#### MSVC

이후 `CMake`가 설치된 상태에서 `builder.py`를 실행하면 MSVC의 경우 `.sln` 파일이 생성됩니다.


### 빌드 실행하기

각 OS별로 준비가 완료되면 `builder.py`로 빌드를 해볼 수 있어요. `builder.py`는 Debug 빌드와 Release 빌드 그리고 RelDebug를 지원합니다.

Debug는 디버깅 심볼이 포함되어 디버깅이 원활하지만 용량이 크고 속도가 느립니다. `builder.py dbg`로 생성합니다. Release는 최적화가 적용되므로 디버깅이 어렵습니다. 배포시 사용되며 `builder.py rel`로 생성합니다. RelDebug는 최적화가 적용되나 디버깅 심볼은 포함시킵니다. Release 빌드에서만 발생하는 오류를 디버깅할 때 사용되며 `builder.py reldbg`로 생성합니다.

`builder.py`로 빌드시 빌드 카운트가 자동으로 증가되며 빌드 환경에 맞춰서 `buildInformation.hpp.in`의 내용이 갱신됩니다. `builder.py`로 갱신하면 내부적으로 `builder.py clean`을 수행해서 이전 중간산출물을 모두 삭제합니다. 그래서 새로운 파일을 추가하거나 파일을 옮기거나 한 게 아니라면(CMake를 빌드할 필요가 없다면), `make` 등을 사용해서 증분 빌드를 사용하면 되죠.

---

## 디버깅

디버깅은 Debug 빌드나 RelDebug 빌드로 하는 게 좋습니다.

### Ubuntu

Ubuntu에서는 `gdb`나 `lldb` 모두 사용 가능합니다. 여담인데, nvim dap으로도 시도해봤고 상당히 오랫동안 사용해왔으나 속도가 너무 느립니다.


### Windows WSL

WSL을 사용하지 않는 경우, Visual Studio로는 알아서 잘 됩니다. WSL에서 VS Code로 디버깅 하기 위해서는 플러그인 설치와 함께 gdb 혹은 lldb와 연결이 되어야 합니다. VS Code를 위한 개발 환경 파일을 별도로 [마련해두었으니](https://github.com/byeolang/byeol-vscode-env) 참고하세요.


### Mac OS

Ubuntu처럼 `gdb`나 `lldb`를 사용해도 되지만 VS Code를 사용해도 됩니다. VS Code를 위한 개발 환경 파일을 별도로 [마련해두었으니](https://github.com/byeolang/byeol-vscode-env) 참고하세요.

---

## 테스트

`test` 모듈에는 수백개의 unit test, e2e test 등이 정의되어 있으며, 빌드가 완료되면 `test`라는 실행파일이 만들어집니다. Windows는 아직 미지원입니다. 해당 파일을 실행하면 기본 탑재된 UT를 실행해서 에러를 검출합니다.

googletest를 사용해서 테스트가 정의되어 있으며, 다음과 같은 옵션을 자주 사용합니다.

```
@style: language-txt verified
# 모든 테스트 실행
$ ./test

# verbose: 로그를 더 상세히 출력
$ ./test verbose

# Wildcard도 사용이 가능: cliTest 파일에 있는 checkDe로 시작하는 모든 testcase 실행
$ ./test --gtest_filter=cliTest.checkDe*

# 2개 같이 사용
$ ./test --gtest_filter=cliTest.checkDefaultAction verbose
```

---

## 가이드 / 레퍼런스 문서 생성

레퍼런스 문서를 위해 `doxygen`을 사용합니다. 가이드 문서 또한 같은 페이지 내에 레퍼런스 문서와 잘 융합되어 보여져야 하기 때문에 마크다운으로 작성되어 doxygen으로 생성됩니다. 이러한 문서의 생성 또한 `builder.py doc` 명령을 통해 생성됩니다.

다음과 같은 프로그램이 필요합니다: `doxygen 1.10+`, `java runtime (jre) 1.8.0+`, `graphviz(선택 사항)`.


### 문서 생성 알고리즘

가이드 및 레퍼런스 문서는 공식적으로는 웹사이트 상으로만 배포됩니다. 그러니 `builder.py doc` 명령은 해당 문서를 공식 웹사이트 배포용으로 HTML로만 생성하는 명령입니다.

이 과정을 위해서 [website](https://github.com/byeolang/website)을 clone한 후에 doxygen 명령을 수행하여 markdown을 html과 관련 asset으로 생성한 후, 기존 파일에 덮어씁니다. 커밋까지는 하지 않습니다. 생성이 완료되면 `build/html` 폴더 안에 변경된 website git Repository가 들어있음을 알 수 있습니다.

생성된 웹사이트는 [jekyll](https://jekyllrb-ko.github.io/)을 통해 제공됩니다. 사이트 전체를 보려면 jekyll 설정이 필요하며, 생성된 문서만 보고 싶다면 byeol/build/html/guide/generated/ko/index.html과 같이 경로로 확인이 가능합니다.


### 문서의 종류

문서는 크게 가이드 문서와 레퍼런스 문서로 나뉘어집니다. 가이드 문서는 byeol 언어를 위한 가이드 문서로, byeol 언어의 설치부터 문법 소개까지를 다룹니다. 레퍼런스 문서는 byeol C++ 코드를 개발을 위한 문서로, 개발 환경 설정과 실행, 테스트 수행, 코드 레퍼런스 등이 포함됩니다.

자세한 내용은 @ref am-document-convention 를 참조하세요.


### 문서의 doxygen 커스터마이징

생성된 html은 doxygen-awesome(https://jothepro.github.io/doxygen-awesome-css/)을 기반으로 꾸며집니다. 물론 위 코드만 그대로 사용한 건 아니고, 추가적으로 js나 asset들을 추가해서 예외처리나 syntax highlighting을 하고 있습니다. 관련 파일들은 `build/doxygen` 폴더에서 찾을 수 있습니다.

---

## Core dump

`coredump`란 프로그램이 비정상 종료될 때, 강제 종료 후의 native 환경의 메모리/변수 값을 기록한 파일입니다. 이를 디버거를 통해 로드하면 죽을 당시의 환경에서 디버깅이 가능하게 됩니다. 다만, OS는 기본적으로 이 파일을 생성하지 않도록 막아놓았기 때문에, 기능을 사용하려면 OS의 설정을 변경할 필요가 있습니다.


### Prerequisites

coredump는 때로는 용량이 매우 클 수 있으니 주의하세요. 시스템 설정 자체를 변경해야 하니 주의가 필요합니다. `frontend`를 포함하여 모든 모듈을 `dbg` 혹은 `reldbg`로 빌드한 바이너리가 필요합니다.


### Coredump 생성
#### Linux / Mac

OS는 core pattern 파일에 기록된 경로에 의해서 생성한 coredump를 어디에 둘지 정합니다. 다음 명령어로 현재 폴더에 `byeol.3234.coredump`와 같은 파일이 생성되도록 합니다.

```sh
sudo sh -c `echo "%e.%p.coredump" > /proc/sys/kernel/core_pattern`
```

위 명령은 재부팅시마다 입력해야 합니다. 귀찮을 경우 항상 적용하는 방법도 있으니 필요하면 찾아보세요. 이후 `byeol`을 실행하여 앱이 죽으면 coredump가 생성됩니다. 이 파일을 `gdb`, `lldb` 등으로 다음과 같이 확인 가능합니다.

```sh
gdb ./byeol ./byeol.3234.coredump
```


#### Windows

Windows는 `.dmp` 파일(Minidump)을 사용합니다. 이 파일을 생성하려면 반드시 다음과 같이 레지스트리를 수정해야 합니다.

```
HKEY_LOCAL_MACHINE_SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps
    DumpFolder = <원하는 경로>
    DumpType = 2
```

`.dmp` 파일은 `VisualStudio`나 `WinDbg` 등으로 확인 가능합니다.

---
**다음 문서**: @ref ab-convention-rules
