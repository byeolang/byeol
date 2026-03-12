# 프로젝트 구조 및 빌드 산출물 {#build-structure}

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
│   ├── pack/         # 빌드된 팩 파일들
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

### 디렉토리 설명

#### module/ - 소스 코드
* Byeol 프로젝트의 모든 C++ 소스 코드가 위치합니다
* 각 하위 디렉토리는 독립적인 모듈이며, 계층형 의존성을 가집니다
* 계층 구조: indep → clog → meta → memlite → stela → core → frontend
* 상위 계층은 하위 계층에만 의존할 수 있습니다

#### bin/ - 빌드 산출물
* **자동 생성 디렉토리**: 빌드 과정에서 자동으로 생성됩니다
* 모든 실행 파일과 동적 라이브러리가 이곳에 배치됩니다
* 빌드 후 `byeol` 실행 파일을 직접 실행할 수 있습니다
* `builder.py clean` 명령으로 정리됩니다

#### build/ - 빌드 시스템
* CMake 설정 파일과 빌드 스크립트가 위치합니다
* `builder.py`를 통해 모든 빌드 작업을 수행합니다
* CMake 중간 산출물(오브젝트 파일, 정적 라이브러리 등)이 `build/module/` 하위에 생성됩니다

#### doc/ - 문서
* 가이드 문서: Byeol 언어의 사용법을 설명하는 문서
* 레퍼런스 문서: C++ 코드베이스 개발을 위한 문서
* 각 문서는 영어(en)와 한국어(ko) 버전으로 제공됩니다
* `builder.py doc` 명령으로 HTML 문서를 생성합니다

#### external/ - 외부 라이브러리
* **자동 생성 디렉토리**: CMake가 자동으로 외부 라이브러리를 다운로드하고 빌드합니다
* **수동 수정 금지**: 이 디렉토리의 내용을 직접 수정하지 마세요
* 현재 Google Test만 사용 중입니다

---

## 빌드 산출물

빌드 과정에서 생성되는 산출물들은 모두 `bin/` 디렉토리에 배치됩니다.

### 실행 파일

#### byeol
* **경로**: `bin/byeol` (Linux/macOS) 또는 `bin/byeol.exe` (Windows)
* **설명**: Byeol 언어의 메인 인터프리터 실행 파일
* **용도**: Byeol 스크립트 파일을 실행하거나 REPL 모드로 동작
* **실행 예시**:
```
@style: language-txt verified
./bin/byeol script.by          # 스크립트 실행
./bin/byeol                    # REPL 모드
./bin/byeol --help             # 도움말 출력
```

#### test
* **경로**: `bin/test`
* **설명**: Google Test 기반의 테스트 실행 파일
* **용도**: 모든 모듈의 유닛 테스트 및 통합 테스트 실행
* **플랫폼**: Linux, macOS만 지원 (Windows 미지원)
* **실행 예시**:
```
@style: language-txt verified
./bin/test                               # 모든 테스트 실행
./bin/test verbose                       # 상세 로그 출력
./bin/test --gtest_filter="clogTest.*"   # 특정 테스트만 실행
```

### 동적 라이브러리

각 모듈은 독립적인 동적 라이브러리로 빌드됩니다. 라이브러리 확장자는 플랫폼에 따라 다릅니다:
* **Linux**: `.so` (Shared Object)
* **macOS**: `.dylib` (Dynamic Library)
* **Windows**: `.dll` (Dynamic Link Library)

#### libindep
* **경로**: `bin/libindep.{so,dylib,dll}`
* **설명**: 플랫폼 추상화 계층 라이브러리
* **기능**: OS별 파일 시스템, 시간, 스레드 등의 플랫폼 종속 기능을 추상화

#### libclog
* **경로**: `bin/libclog.{so,dylib,dll}`
* **설명**: 로깅 서브시스템 라이브러리
* **기능**: 설정 가능한 로그 레벨, 포맷, 스레드 안전 로깅 제공

#### libmeta
* **경로**: `bin/libmeta.{so,dylib,dll}`
* **설명**: 타입 메타데이터 관리 라이브러리
* **기능**: 런타임 타입 정보(RTTI) 및 리플렉션 기능 제공

#### libmemlite
* **경로**: `bin/libmemlite.{so,dylib,dll}`
* **설명**: 경량 메모리 관리 라이브러리
* **기능**: 메모리 풀 기반 할당으로 성능 최적화

#### libstela
* **경로**: `bin/libstela.{so,dylib,dll}`
* **설명**: 설정 파싱 계층 라이브러리
* **기능**: `manifest.stela` 파일 파싱 및 설정 관리

#### libcore
* **경로**: `bin/libcore.{so,dylib,dll}`
* **설명**: 핵심 언어 구현 라이브러리
* **기능**: AST, Parser, Verifier, 패키지 로딩 등 언어의 핵심 기능
* **특징**: 가장 크고 복잡한 라이브러리

### 기타 산출물

#### pack/ - 팩 파일
* **경로**: `bin/pack/`
* **설명**: Byeol 언어의 번들 팩들이 빌드되어 배치되는 디렉토리
* **내용**: `sys` 번들 팩 (기본 타입, 함수 등)
* **확장자**: `.pack`

#### testdata/ - 테스트 데이터
* **경로**: `bin/testdata/`
* **설명**: 테스트 실행에 필요한 데이터 파일들
* **생성 방식**: `module/test/testdata/`의 내용이 빌드 시 복사됨

---

## 빌드 타겟

`builder.py`는 여러 빌드 타겟을 지원합니다. 각 타겟은 최적화 수준과 디버깅 정보 포함 여부가 다릅니다.

### Debug 빌드 (dbg)

```
@style: language-txt verified
./build/builder.py dbg
```

* **최적화**: 없음 (`-O0`)
* **디버깅 심볼**: 포함 (`-g`)
* **특수 플래그**: `-DBY_DEBUG`, `-D_GLIBCXX_DEBUG`
* **용도**: 개발 중 디버깅, 상세한 에러 메시지 확인
* **특징**:
  - 실행 속도가 느립니다
  - 바이너리 크기가 큽니다
  - 디버거(gdb, lldb)로 편리하게 디버깅 가능
  - 추가 런타임 검증 활성화 (STL 디버그 모드)

### Release 빌드 (rel)

```
@style: language-txt verified
./build/builder.py rel
```

* **최적화**: 최대 (`-O2`)
* **디버깅 심볼**: 미포함
* **용도**: 배포용 빌드
* **특징**:
  - 실행 속도가 빠릅니다
  - 바이너리 크기가 작습니다
  - 디버깅이 어렵습니다

### Release with Debug Info 빌드 (reldbg)

```
@style: language-txt verified
./build/builder.py reldbg
```

* **최적화**: 최대 (`-O2`)
* **디버깅 심볼**: 포함 (`-g`)
* **용도**: Release 빌드에서만 발생하는 버그를 디버깅할 때
* **특징**:
  - Release 수준의 최적화 적용
  - 디버거 사용 가능 (단, 최적화로 인해 일부 변수는 추적 불가)
  - 바이너리 크기가 Debug보다는 작지만 Release보다는 큼

### WebAssembly 빌드 (wasm)

```
@style: language-txt verified
./build/builder.py wasm
```

* **최적화**: 웹 환경 최적화 (`-O2`)
* **디버깅 심볼**: 미포함
* **용도**: 웹 브라우저에서 Byeol 실행
* **산출물**:
  - `.wasm` 파일: WebAssembly 바이너리
  - `.js` 파일: WebAssembly를 로드하고 실행하는 JavaScript 래퍼
* **특징**:
  - Emscripten 컴파일러 사용
  - 동적 라이브러리 대신 정적 라이브러리(`.a`) 생성
  - `test` 모듈 빌드 제외
  - `bundle` 모듈 빌드 제외

### Coverage 빌드 (cov)

```
@style: language-txt verified
./build/builder.py cov
```

* **최적화**: Debug 수준
* **디버깅 심볼**: 포함
* **용도**: 코드 커버리지 측정
* **특수 플래그**:
  - Clang: `-fprofile-instr-generate -fcoverage-mapping`
  - GCC: `-fprofile-arcs -ftest-coverage`
* **플랫폼**: Linux, macOS만 지원
* **특징**:
  - 테스트 실행 후 커버리지 리포트 생성
  - `.profraw` 또는 `.gcda` 파일 생성

### 빌드 타겟 비교표

| 타겟 | 최적화 | 디버그 심볼 | 바이너리 크기 | 실행 속도 | 용도 |
|------|--------|-------------|---------------|-----------|------|
| dbg | 없음 (-O0) | ✓ | 가장 큼 | 가장 느림 | 개발/디버깅 |
| rel | 최대 (-O2) | ✗ | 가장 작음 | 가장 빠름 | 배포 |
| reldbg | 최대 (-O2) | ✓ | 중간 | 빠름 | Release 버그 디버깅 |
| wasm | 웹 최적화 (-O2) | ✗ | 작음 | 빠름 | 웹 환경 배포 |
| cov | 없음 (-O0) | ✓ | 큼 | 느림 | 커버리지 측정 |

---

## Git Repository 구조

### 추적되는 파일

Git으로 버전 관리되는 주요 디렉토리와 파일:

* `module/` - 모든 소스 코드
* `build/` - 빌드 스크립트 및 설정 파일
  - `builder.py`
  - `CMakeLists.txt`
  - `doxygen/` (문서 생성 설정)
* `doc/` - 문서 소스 (Markdown)
* `.clang-format` - 코드 포맷팅 규칙
* `.clang-tidy` - 정적 분석 규칙
* `.gitignore` - Git 무시 패턴

### 추적되지 않는 파일

`.gitignore`에 의해 무시되는 디렉토리 (자동 생성):

* `bin/` - 빌드 산출물
* `build/module/` - CMake 중간 산출물
* `build/.cmake/` - CMake 캐시
* `external/` - 외부 라이브러리
* `*.o`, `*.a`, `*.so`, `*.dylib`, `*.dll` - 컴파일 산출물
* `*.profraw`, `*.gcda`, `*.gcno` - 커버리지 데이터

---

## 참고 사항

### 플랫폼별 차이

#### Linux
* 동적 라이브러리 확장자: `.so`
* 모든 빌드 타겟 지원
* 패키지 관리자로 의존성 설치 가능

#### macOS
* 동적 라이브러리 확장자: `.dylib`
* 모든 빌드 타겟 지원
* Apple Silicon (arm64)과 Intel (x64) 모두 지원
* Homebrew로 의존성 설치 권장

#### Windows
* 동적 라이브러리 확장자: `.dll`
* `test` 빌드 미지원 (테스트는 Linux/macOS에서 수행)
* MSVC (Visual Studio 2022 17+) 사용 권장
* Bison/Flex는 [winflexbison](https://github.com/lexxmark/winflexbison) 사용

### 주의사항

1. **`bin/` 디렉토리 수동 수정 금지**
   - 빌드 과정에서 자동으로 생성되므로 직접 수정하지 마세요
   - `builder.py clean` 실행 시 모두 삭제됩니다

2. **`external/` 디렉토리 수동 수정 금지**
   - CMake가 자동으로 관리하므로 직접 수정하지 마세요
   - **단, Google Test 오류 발생 시 예외**: 빌드 중 Google Test 관련 오류가 발생하거나 파일이 없다는 메시지가 나타나면, `external/googletest/` 디렉토리를 수동으로 삭제한 후 다시 빌드하세요
   - `builder.py clean` 명령은 `external/` 디렉토리를 삭제하지 않으므로 수동 삭제가 필요합니다

3. **증분 빌드 주의**
   - 새 파일 추가 시: 전체 빌드 필요 (`builder.py dbg`)
   - 기존 파일 수정 시: 증분 빌드 가능 (`make`)

4. **빌드 카운트**
   - `builder.py`로 빌드 시에만 빌드 카운트가 증가합니다
   - `cmake`나 `make`를 직접 사용 시에는 증가하지 않습니다

---

## 관련 문서

* 개발 환경 설정: @ref dev-env.md "개발환경"
* 코딩 규칙: @ref convention-rules "Byeol 코딩 규칙"
* 아키텍처 상세: @ref architecture-and-class-design "아키텍쳐와 설계"
