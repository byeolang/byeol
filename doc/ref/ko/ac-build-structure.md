# 프로젝트 구조 및 빌드 산출물 {#ac-build-structure}

이 문서는 Byeol 프로젝트의 디렉토리 구조와 빌드 과정에서 생성되는 산출물들에 대해 설명합니다.

---

## 프로젝트 디렉토리 구조

Byeol 프로젝트는 다음과 같은 디렉토리 구조를 가지고 있습니다:

```
@style: language-txt verified
byeol/
├── module/           # 소스 코드 모듈들
│   ├── indep/        # 플랫폼 추상화 계층
│   ├── clog/         # 로깅 서브시스템
│   ├── meta/         # 타입 메타데이터 관리
│   ├── memlite/      # 경량 메모리 관리 계층
│   ├── stela/        # 설정 파싱 계층
│   ├── core/         # 핵심 언어 구현 (AST, Parser, Verifier)
│   ├── frontend/     # CLI 프론트엔드 (byeol 실행 파일)
│   ├── bundle/       # 기본 번들 팩 (sys)
│   └── test/         # 테스트 케이스
│
├── bin/              # 빌드 산출물 (자동 생성)
│   ├── byeol         # 메인 실행 파일
│   ├── test          # 테스트 실행 파일
│   ├── lib*.{so,dylib,dll}  # 동적 라이브러리
│   ├── pod/         # 빌드된 팩 파일들
│   └── testdata/     # 테스트 데이터 (복사됨)
│
├── build/            # 빌드 시스템 및 중간 산출물
│   ├── builder.py    # 통합 빌드 스크립트
│   ├── CMakeLists.txt # CMake 설정 파일
│   ├── doxygen/      # Doxygen 커스터마이징 파일
│   └── module/       # CMake 중간 산출물 (*.o, *.a 등)
│
├── doc/              # 문서
│   ├── guide/        # 가이드 문서 (언어 사용법)
│   │   ├── en/       # 영문 가이드
│   │   └── ko/       # 한글 가이드
│   └── ref/          # 레퍼런스 문서 (개발자용)
│       ├── en/       # 영문 레퍼런스
│       └── ko/       # 한글 레퍼런스
│
└── external/         # 외부 라이브러리 (자동 생성, 수정 금지)
    └── googletest/   # Google Test 프레임워크
```

---

## 디렉토리 설명

### module/ - 소스 코드

Byeol 프로젝트의 모든 C++ 소스 코드가 위치합니다. 각 하위 디렉토리는 독립적인 모듈이며 계층형 의존성을 가집니다. 계층 구조는 indep → clog → meta → memlite → stela → core → frontend 순서이며, 상위 계층은 하위 계층에만 의존할 수 있습니다.

---

### bin/ - 빌드 산출물

<b>자동 생성 디렉토리</b>로, 빌드 과정에서 자동으로 생성됩니다. 모든 실행 파일과 동적 라이브러리가 이곳에 배치됩니다. 빌드 후 `byeol` 실행 파일을 직접 실행할 수 있으며, `builder.py clean` 명령으로 정리됩니다.

---

### build/ - 빌드 시스템

CMake 설정 파일과 빌드 스크립트가 위치합니다. `builder.py`를 통해 모든 빌드 작업을 수행하며, CMake 중간 산출물(오브젝트 파일, 정적 라이브러리 등)이 `build/module/` 하위에 생성됩니다.

---

### doc/ - 문서

가이드 문서는 Byeol 언어의 사용법을 설명하는 문서이고, 레퍼런스 문서는 C++ 코드베이스 개발을 위한 문서입니다. 각 문서는 영어(en)와 한국어(ko) 버전으로 제공됩니다. `builder.py doc` 명령으로 HTML 문서를 생성합니다.

---

### external/ - 외부 라이브러리

<b>자동 생성 디렉토리</b>로, CMake가 자동으로 외부 라이브러리를 다운로드하고 빌드합니다. <b>수동 수정 금지</b>이므로 이 디렉토리의 내용을 직접 수정하지 마세요. 현재 Google Test만 사용 중입니다.

---

## 빌드 산출물

빌드 과정에서 생성되는 산출물들은 모두 `bin/` 디렉토리에 배치됩니다.

### 실행 파일

<b>byeol</b>

경로는 `bin/byeol`(Linux/macOS) 또는 `bin/byeol.exe`(Windows)입니다. Byeol 언어의 메인 인터프리터 실행 파일로, Byeol 스크립트 파일을 실행하거나 REPL 모드로 동작해요. 실행 예시는 다음과 같습니다:
```
@style: language-txt verified
./bin/byeol script.by          # 스크립트 실행
./bin/byeol                    # REPL 모드
./bin/byeol --help             # 도움말 출력
```

---

<b>test</b>

경로는 `bin/test`입니다. Google Test 기반의 테스트 실행 파일로, 모든 모듈의 유닛 테스트 및 통합 테스트를 실행합니다. Linux와 macOS만 지원하며 Windows는 미지원이에요. 실행 예시는 다음과 같습니다:
```
@style: language-txt verified
./bin/test                               # 모든 테스트 실행
./bin/test verbose                       # 상세 로그 출력
./bin/test --gtest_filter="clogTest.*"   # 특정 테스트만 실행
```

---

## 동적 라이브러리

각 모듈은 독립적인 동적 라이브러리로 빌드됩니다. 라이브러리 확장자는 플랫폼에 따라 다릅니다. <b>Linux</b>에서는 `.so`(Shared Object), <b>macOS</b>에서는 `.dylib`(Dynamic Library), <b>Windows</b>에서는 `.dll`(Dynamic Link Library)을 사용해요.

### libindep

경로는 `bin/libindep.{so,dylib,dll}`입니다. 플랫폼 추상화 계층 라이브러리로, OS별 파일 시스템, 시간, 스레드 등의 플랫폼 종속 기능을 추상화합니다.

---

### libclog

경로는 `bin/libclog.{so,dylib,dll}`입니다. 로깅 서브시스템 라이브러리로, 설정 가능한 로그 레벨, 포맷, 스레드 안전 로깅을 제공합니다.

---

### libmeta

경로는 `bin/libmeta.{so,dylib,dll}`입니다. 타입 메타데이터 관리 라이브러리로, 런타임 타입 정보(RTTI) 및 리플렉션 기능을 제공합니다.

---

### libmemlite

경로는 `bin/libmemlite.{so,dylib,dll}`입니다. 경량 메모리 관리 라이브러리로, 메모리 풀 기반 할당으로 성능을 최적화합니다.

---

### libstela

경로는 `bin/libstela.{so,dylib,dll}`입니다. 설정 파싱 계층 라이브러리로, `manifest.stela` 파일 파싱 및 설정 관리를 담당합니다.

---

### libcore

경로는 `bin/libcore.{so,dylib,dll}`입니다. 핵심 언어 구현 라이브러리로, AST, Parser, Verifier, 패키지 로딩 등 언어의 핵심 기능을 담당합니다. 가장 크고 복잡한 라이브러리입니다.

## 기타 산출물

### pod/ - 팩 파일

경로는 `bin/pod/`입니다. Byeol 언어의 번들 팩들이 빌드되어 배치되는 디렉토리로, `sys` 번들 팩(기본 타입, 함수 등)이 포함됩니다. 확장자는 `.pod`입니다.

---

### testdata/ - 테스트 데이터

경로는 `bin/testdata/`입니다. 테스트 실행에 필요한 데이터 파일들로, `module/test/testdata/`의 내용이 빌드 시 복사됩니다.

---

## 배포 트리와 실행 {#ac-build-structure-deployment}

앞 절들이 <b>소스 트리</b>와 <b>빌드 산출물</b>을 설명했다면, 이 절은 산출물이 사용자 머신에
배치된 뒤에 실제로 무슨 일이 벌어지는지를 다룹니다. 저장소들이 어떤 관계로 엮여 있는지는
@ref ae-architecture-overview 의 저장소 구조 섹션에서 다루므로, 여기서는 빌드가 끝난 다음의
이야기에 집중합니다.

이 절을 관통하는 원칙은 하나입니다. <b>사용자는 byeol 하나만 본다</b>는 것이죠. 내부적으로
아무리 여러 실행파일이 협력하더라도, 사용자가 알아야 하는 이름은 `byeol` 뿐이어야 합니다.
아래의 모든 결정은 이 원칙에서 파생됩니다.

### 두 개의 실행파일

배포된 byeol에는 실행파일이 두 개 있습니다. 이름이 똑같이 `byeol`이라서 혼란스러울 수 있지만,
역할은 전혀 다릅니다.

첫 번째는 <b>launcher</b>입니다. 사용자의 PATH에 잡히는 진입점이며, 실제로 언어를 실행하지는
않습니다. 어느 버전의 인터프리터를 써야 하는지 결정하고, 업데이트를 처리하고, 패키지 매니저
명령을 전달하는 얇은 프록시입니다.

두 번째는 <b>인터프리터 본체</b>입니다. 파싱, 검증, 실행 등 언어의 모든 일을 실제로 수행하는
프로그램이며, @ref ae-architecture-overview 에서 설명하는 계층 구조를 담고 있는 것이 바로 이
실행파일입니다.

launcher를 따로 둔 이유는 <b>버전을 자유롭게 바꾸기 위해서</b>입니다. rbenv나 pyenv가 하는 일과
같습니다. 사용자가 치는 명령은 `ruby`이지 `rbenv`가 아니듯, byeol에서도 사용자가 치는 명령은
언제나 `byeol`이고 그 뒤에서 어떤 버전이 선택되는지는 launcher가 알아서 처리합니다.

### 빌드 이름과 배포 이름

두 실행파일의 이름이 모두 `byeol`이면 개발자가 빌드 디렉토리에서 둘을 구분할 수 없습니다.
그래서 빌드할 때와 배포할 때 이름을 다르게 씁니다.

빌드 단계에서는 각자 고유한 타깃 이름을 가집니다. launcher 저장소의 CMake 타깃은 `launcher`이고,
byeol 저장소의 타깃은 `byeol`입니다. 개발자는 빌드 디렉토리에서 이름만 보고 둘을 구분할 수 있죠.

배포 단계에서는 둘 다 최종 파일명이 `byeol`이 됩니다. launcher 저장소의 CMakeLists.txt에는 이
의도가 그대로 드러나 있습니다.

```
@style: verified language-txt
set(exeName    "launcher")   # 프로젝트/타깃 이름 (내부 식별자)
set(outputName "byeol")      # 사용자에게 보이는 파일 이름

set_target_properties(${exeName} PROPERTIES OUTPUT_NAME ${outputName})
```

`launcher`라는 이름은 저장소와 빌드와 문서에서만 쓰이는 <b>내부 식별자</b>이며, 사용자 시야에는
절대 닿지 않습니다. launcher에 그럴싸한 브랜드명을 붙이지 않는 것도 같은 이유입니다. 사용자가
가장 먼저 만지는 프로그램에 독립된 이름을 붙이면 그게 언어 이름이라고 오해하게 되니까요.

### 배포 폴더 구조

이름이 같은 두 실행파일은 <b>경로로 구분</b>됩니다.

```
@style: verified language-txt
<root>/
  byeol                    ← launcher (PATH에 잡히는 진입점)
  toolchains/
    v0.1.1/
      byeol                ← 인터프리터 본체
      extraFiles...
    v0.1.4/
      byeol                ← 인터프리터 본체
      extraFiles...
```

규칙은 단순합니다. 루트의 `byeol`은 절대로 버전 폴더 안으로 들어가지 않고, 인터프리터는 절대로
루트에 나오지 않습니다. 그래서 경로만 보면 어느 쪽인지 즉시 알 수 있죠. 디버깅을 하거나 로그를
읽을 때 "이 byeol이 대체 뭐냐"는 질문에 절대경로가 바로 답을 줍니다. 루트에 있으면 launcher,
`toolchains/<버전>/` 아래에 있으면 인터프리터입니다.

여러 버전이 `toolchains/` 아래에 나란히 설치될 수 있다는 점도 중요합니다. 버전을 바꾼다는 것은
새로 설치하고 덮어쓰는 일이 아니라, 이미 있는 폴더 중 어느 것을 쓸지 고르는 일입니다.

### 실행 흐름

사용자가 `byeol hello.by`를 입력하면 다음 순서로 일이 진행됩니다.

먼저 PATH에 잡힌 launcher가 실행됩니다. launcher는 인자를 해석해서 이것이 자기가 직접 처리할
명령인지, 인터프리터에게 넘길 명령인지 판단합니다. `toolchain` 관련 명령이나 `self update`는
launcher가 직접 처리하고, 코드를 실행하는 명령은 인터프리터에게 넘깁니다.

넘길 명령이라면 launcher는 활성 버전을 해소합니다. 버전 설정 파일을 읽어서 어느 toolchain을
쓸지 결정하고, 그에 해당하는 `toolchains/<버전>/byeol` 경로를 얻습니다.

그다음 launcher는 그 인터프리터를 <b>자식 프로세스로 spawn</b>하면서 자신의 표준 입출력을
<b>그대로 물려줍니다</b>. 그리고 자식이 끝날 때까지 부모로 남아 기다립니다. 자식이 종료되면
그 종료 코드를 그대로 반환하고 자신도 종료합니다.

여기서 핵심은 <b>launcher가 데이터를 중계하지 않는다</b>는 점입니다. 표준 입출력을 상속시켰기
때문에 터미널이나 IDE는 인터프리터와 직접 이야기하며, launcher는 오가는 바이트를 건드리지
않습니다. 그래서 launcher가 중간에 살아 있어도 속도에 아무런 영향이 없습니다. 기다리는 동안
launcher는 잠들어 있으므로 비용도 사실상 없죠.

그런데도 launcher를 종료시키지 않고 남겨두는 이유는 <b>세션 생명주기 관리</b> 때문입니다.
인터프리터가 끝나면 launcher의 대기가 풀리면서 세션이 깔끔하게 닫히고, IDE 입장에서는 추적해야
할 프로세스가 명확해집니다.

### 디버깅 연결

IDE에서 byeol 코드를 디버깅할 때는 DAP(Debug Adapter Protocol) 방식으로 통신합니다. JSON 기반의
메시지를 주고받으며, 전송 수단으로는 소켓이 아니라 <b>파이프(표준 입출력)</b>를 사용합니다.

파이프를 고른 이유는 앞서 설명한 프로세스 모델과 자연스럽게 맞물리기 때문입니다. launcher가 이미
표준 입출력을 인터프리터에게 물려주고 있으므로, IDE는 추가 설정 없이 인터프리터와 직접 대화할 수
있습니다. 소켓을 쓰면 포트를 협상하고, 충돌을 처리하고, 보안 바인딩을 신경 써야 하는데 그 고민을
통째로 피할 수 있죠. 대부분의 DAP 어댑터가 표준 입출력을 기본으로 쓰는 것도 같은 이유입니다.

IDE는 어느 버전의 인터프리터가 실제로 실행되는지 알 필요가 없습니다. IDE는 launcher만 바라보고,
버전 해소는 launcher가 책임집니다. 이것이 launcher를 버전 스위처로 둔 설계가 디버깅까지 일관되게
이어지는 지점입니다.

---

## 저장소간 릴리스 {#ac-build-structure-release}

byeol은 6개의 저장소로 나뉘어 있고, 상위 저장소는 하위 저장소를 <b>태그로 pin</b>해서
가져옵니다. 새 기능이나 수정이 하위 저장소에 담긴다면, 상위 저장소가 그 변경을 실제로
쓰기 위해서는 두 단계가 필요합니다. 하위 저장소가 먼저 태그를 붙여 릴리스하고, 그다음
상위 저장소가 pin을 그 태그로 갱신해야 합니다. 이 순서가 뒤집힐 수 없는 이유는 단순합니다.
pin이 태그를 가리키고 있으므로, 태그가 존재하기 전까지 상위 저장소는 그 변경을 참조할
방법이 없습니다.

### pin이 걸린 위치

각 pin은 상위 저장소의 CMake 파일 안에 `FetchContent_Declare(...)` 블록의 `GIT_TAG`
줄로 존재합니다. 갱신은 그 줄의 태그 이름을 바꾸는 커밋 하나면 됩니다.

| 상위 → 하위 | 파일 |
|-------------|------|
| byeol → stela | `module/core/CMakeLists.txt` |
| byeol → flagStacker | `module/frontend/CMakeLists.txt` |
| launcher → indep | `module/launcher/CMakeLists.txt` |
| launcher → flagStacker | `module/launcher/CMakeLists.txt` |
| stela → indep | `module/clog/CMakeLists.txt` |

`build-common`은 예외입니다. `FetchContent`가 아니라 <b>git subtree</b>로 byeol
저장소의 `external/build-common/` 아래에 병합되어 있습니다. 그래서 태그로 pin되지
않고, 갱신할 때는 subtree pull을 씁니다.

```
@style: language-txt verified
git subtree pull --prefix=external/build-common \
    https://github.com/byeolang/build-common main --squash
```

### 태그 형식

모든 저장소가 SemVer 형식(`vMAJOR.MINOR.PATCH`)을 씁니다. 예: `v0.1.7`. 판정 기준은
평소의 SemVer 그대로입니다. API를 깨지 않으면 <b>patch</b>, 하위 호환되는 새 기능이면
<b>minor</b>, 하위 호환을 깨는 변경이면 <b>major</b>입니다.

### 릴리스 절차

한 저장소에 변경이 쌓여 릴리스할 준비가 되면 아래 순서를 따릅니다.

먼저 로컬에서 테스트가 통과하는지 확인합니다.

```
@style: language-txt verified
./build/builder.py test
```

그다음 새 태그를 붙여 push합니다.

```
@style: language-txt verified
git tag vX.Y.Z
git push origin vX.Y.Z
```

이 태그가 원격에 올라간 시점부터 상위 저장소가 pin을 이 태그로 올릴 수 있습니다.
상위 저장소에서는 위 표의 `GIT_TAG` 줄을 새 태그 이름으로 바꾸는 PR을 열고, 리뷰와
CI 통과를 거쳐 머지합니다. 커밋 메시지는 `chore: bump <이름> to vX.Y.Z` 형태를 씁니다.

변경이 여러 저장소에 걸쳐 있으면, 항상 가장 아래층부터 이 절차를 반복합니다. 예를 들어
`indep`을 손봤다면 먼저 `indep`에 태그를 붙이고, 그다음 `stela`와 `launcher`가 각자
`GIT_TAG`를 갱신하고 태그를 붙이고, 마지막으로 `byeol`이 갱신됩니다. 이 순서는 저장소
의존 그래프의 역순입니다.

---

# 빌드 타겟

`builder.py`는 여러 빌드 타겟을 지원합니다. 각 타겟은 최적화 수준과 디버깅 정보 포함 여부가 다릅니다.

## Debug 빌드 (dbg)

```
@style: language-txt verified
./build/builder.py dbg
```

<b>최적화</b>는 없음(`-O0`), <b>디버깅 심볼</b>은 포함(`-g`), <b>특수 플래그</b>로는 `-DBY_DEBUG`, `-D_GLIBCXX_DEBUG`를 사용합니다. 개발 중 디버깅과 상세한 에러 메시지 확인을 위해 사용되죠.

실행 속도가 느리고 바이너리 크기가 크지만, 디버거(gdb, lldb)로 편리하게 디버깅할 수 있습니다. 추가 런타임 검증이 활성화됩니다(STL 디버그 모드).

---

## Release 빌드 (rel)

```
@style: language-txt verified
./build/builder.py rel
```

<b>최적화</b>는 최대(`-O2`), <b>디버깅 심볼</b>은 미포함입니다. 배포용 빌드로 사용됩니다.

실행 속도가 빠르고 바이너리 크기가 작지만, 디버깅이 어렵습니다.

---

## Release with Debug Info 빌드 (reldbg)

```
@style: language-txt verified
./build/builder.py reldbg
```

<b>최적화</b>는 최대(`-O2`), <b>디버깅 심볼</b>은 포함(`-g`)합니다. Release 빌드에서만 발생하는 버그를 디버깅할 때 사용됩니다.

Release 수준의 최적화가 적용되어 디버거 사용이 가능하지만, 최적화로 인해 일부 변수는 추적할 수 없습니다. 바이너리 크기는 Debug보다는 작지만 Release보다는 큽니다.

---

## WebAssembly 빌드 (wasm)

```
@style: language-txt verified
./build/builder.py wasm
```

<b>최적화</b>는 웹 환경 최적화(`-O2`), <b>디버깅 심볼</b>은 미포함입니다. 웹 브라우저에서 Byeol을 실행하기 위해 사용됩니다.

산출물로는 `.wasm` 파일(WebAssembly 바이너리)과 `.js` 파일(WebAssembly를 로드하고 실행하는 JavaScript 래퍼)이 생성됩니다. Emscripten 컴파일러를 사용하며, 동적 라이브러리 대신 정적 라이브러리(`.a`)를 생성합니다. `test` 모듈과 `bundle` 모듈은 빌드에서 제외됩니다.

---

## Coverage 빌드 (cov)

```
@style: language-txt verified
./build/builder.py cov
```

<b>최적화</b>는 Debug 수준, <b>디버깅 심볼</b>은 포함입니다. 코드 커버리지 측정을 위해 사용됩니다.

<b>특수 플래그</b>로는 Clang의 경우 `-fprofile-instr-generate -fcoverage-mapping`을, GCC의 경우 `-fprofile-arcs -ftest-coverage`를 사용합니다. Linux와 macOS만 지원합니다. 테스트 실행 후 커버리지 리포트를 생성하며, `.profraw` 또는 `.gcda` 파일이 생성됩니다.

---

## 빌드 타겟 비교표

| 타겟 | 최적화 | 디버그 심볼 | 바이너리 크기 | 실행 속도 | 용도 |
|------|--------|-------------|---------------|-----------|------|
| dbg | 없음 (-O0) | ✓ | 가장 큼 | 가장 느림 | 개발/디버깅 |
| rel | 최대 (-O2) | ✗ | 가장 작음 | 가장 빠름 | 배포 |
| reldbg | 최대 (-O2) | ✓ | 중간 | 빠름 | Release 버그 디버깅 |
| wasm | 웹 최적화 (-O2) | ✗ | 작음 | 빠름 | 웹 환경 배포 |
| cov | 없음 (-O0) | ✓ | 큼 | 느림 | 커버리지 측정 |

---

# Git Repository 구조

## 추적되는 파일

Git으로 버전 관리되는 주요 디렉토리와 파일은 다음과 같습니다. `module/`은 모든 소스 코드를, `build/`는 빌드 스크립트 및 설정 파일(`builder.py`, `CMakeLists.txt`, `doxygen/` 문서 생성 설정)을 포함합니다. `doc/`은 문서 소스(Markdown)를, `.clang-format`은 코드 포맷팅 규칙을, `.clang-tidy`는 정적 분석 규칙을, `.gitignore`는 Git 무시 패턴을 정의합니다.

---

## 추적되지 않는 파일

`.gitignore`에 의해 무시되는 디렉토리(자동 생성)는 다음과 같습니다. `bin/`은 빌드 산출물을, `build/module/`은 CMake 중간 산출물을, `build/.cmake/`는 CMake 캐시를, `external/`은 외부 라이브러리를 포함합니다. 또한 `*.o`, `*.a`, `*.so`, `*.dylib`, `*.dll`과 같은 컴파일 산출물과 `*.profraw`, `*.gcda`, `*.gcno`와 같은 커버리지 데이터도 무시됩니다.

---

# 참고 사항

<b>`bin/` 디렉토리는 수동으로 수정하지 마세요.</b> 빌드 과정에서 자동으로 생성되며, `builder.py clean` 실행 시 모두 삭제됩니다.

<b>`external/` 디렉토리도 수동으로 수정하지 마세요.</b> CMake가 자동으로 관리합니다. <b>단, Google Test 오류 발생 시는 예외입니다.</b> 빌드 중 Google Test 관련 오류가 발생하거나 파일이 없다는 메시지가 나타나면, `external/googletest/` 디렉토리를 수동으로 삭제한 후 다시 빌드하세요. `builder.py clean` 명령은 `external/` 디렉토리를 삭제하지 않으므로 수동 삭제가 필요합니다.

<b>증분 빌드를 사용할 때는 주의하세요.</b> 새 파일을 추가할 때는 전체 빌드(`builder.py dbg`)가 필요하고, 기존 파일을 수정할 때는 증분 빌드(`make`)가 가능합니다.

<b>빌드 카운트</b>는 `builder.py`로 빌드할 때만 증가합니다. `cmake`나 `make`를 직접 사용하면 증가하지 않습니다.

---

**다음 문서**: @ref ad-convention-rules
