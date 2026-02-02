# 아키텍쳐와 설계 {#architecture-and-class-design}

## 소개

이 문서는 Byeol 프로그래밍 언어의 구현을 이해하기 위한 가이드입니다. Byeol은 추상 구문 트리(AST)를
직접 실행하는 강타입 인터프리터 언어로, 계층형 아키텍처를 기반으로 설계되었습니다.

이 가이드는 각 모듈의 설계, 알고리즘, 클래스들이 어떻게 상호작용하는지를 중점적으로 다룹니다.
byeol 언어 문법을 알고있는, 코드베이스를 처음 접하는 C++개발자가 체계적으로 이해할 수 있도록 구성되었습니다.
byeol 언어의 문법에 대해 이해하고 있다는 전제로 시작하기 때문에 필요하다면 먼저 [언어 가이드](https://byeol.io/guide/)
를 참고하세요.


## 아키텍처 개요

Byeol 프로젝트는 엄격한 계층형 아키텍처를 따릅니다. 각 계층은 하위 계층에만 의존할 수 있으며,
@ref indep 모듈 위로는 플랫폼 독립성이 유지됩니다.

### 계층 구조

```
@style: language-txt verified
┌───────────────────┐
│         frontend (CLI)               │  ← 사용자 인터페이스
├───────────────────┤
│         core (언어 구현)             │  ← AST, Parser, Verifier
├───────────────────┤
│         stela (설정 파싱)            │  ← Manifest 파싱
├───────────────────┤
│         memlite (메모리 관리)        │  ← 커스텀 메모리 관리
├───────────────────┤
│         meta (타입 시스템)           │  ← 런타임 타입 정보
├───────────────────┤
│         clog (로깅)                  │  ← 로깅 프레임워크
├───────────────────┤
│         indep (플랫폼 추상화)        │  ← 플랫폼 종속 코드
└───────────────────┘
```

각 모듈은 하위 계층의 모듈에만 접근할 수 있습니다. 예를 들어, @ref core 는 @ref stela, @ref memlite, @ref meta,
@ref clog, @ref indep 에 의존하지만, @ref frontend 나 다른 상위 모듈에는 의존하지 않습니다.

이러한 아키텍처는 플랫폼 독립성, 테스트 용이성, 명확한 의존성 관리를 제공합니다. 플랫폼 종속 코드는
@ref indep 모듈에만 격리되어 있어, 다른 모듈들은 플랫폼에 관계없이 동일한 방식으로 동작합니다.


## indep 모듈 - 플랫폼 추상화 계층

@ref indep 모듈은 Byeol 프로젝트의 최하위 계층으로, 플랫폼 종속적인 기능을 추상화합니다. 이 모듈의
핵심 목표는 Windows, POSIX 계열 운영체제 등 다양한 플랫폼에서 동일한 API를 제공하는 것입니다.

Byeol의 아키텍처 규칙에 따라, 플랫폼 종속적인 코드(`#ifdef` 조건부 컴파일 등)는 반드시 @ref indep
모듈에만 존재해야 합니다. @ref indep 보다 상위의 모듈에서는 OS에 대한 조건부 컴파일이나 플랫폼별 분기를
사용하지 않습니다.


### Early-return 패턴과 에러 처리

#### WHEN 매크로

Byeol은 프로젝트 전체적으로 early-return 패턴을 적용하고 있습니다. 이는 코드의 흐름을 알기 쉽게
해주며 block 문 depth를 줄여주는 이점이 있습니다.

단점이라면 `if(checks) return res;`와 같은 식으로 사용하기 때문에 `if`만 보고 이 코드가
branching을 위한 if인지 아니면 return하려고 하는 if인지 구분이 되지 않는다는 점과 `if(checks)` 뒤에
블록문이 오게 되는 경우 가독성이 떨어진다는 점입니다.

다음과 같은 코드를 보면 좀 체감이 될 것입니다.

```
@style: language-cpp verified
str me::eval(const args& a) {
    std::string key = _makeKey(a);
    if(key.empty()) {
        BY_E("key is empty");
        return tstr<obj>();
    }
    if(_isSelfMaking(key)) {
        BY_E("error: you tried to clone self generic object.");
        return tstr<obj>();
    }

    if(!_cache.count(key))
        _makeGeneric(key, params::make(_paramNames, a));

    return _cache[key];
}
```

@ref by::WHEN "WHEN" 매크로는 이 부분을 해결하는 것으로 @ref by::WHEN "WHEN"은 early-return 패턴 시에만 사용됩니다. 또한 90%
이상의 early-return은 에러 발견시 로그를 찍고 에러 값을 내보내는 것 뿐이라는 것에 착안해서 그 2가지
과정을 한 줄에 표현할 수 있도록 chaining을 지원합니다.

결과 다음과 같이 코드가 간략해지고 if의 목적이 명확해집니다.

```
@style: language-cpp verified
str me::eval(const args& a) {
    std::string key = _makeKey(a);
    WHEN(key.empty()).err("key is empty").ret(tstr<obj>());
    WHEN(_isSelfMaking(key)).err("error: you tried to clone self generic object.").ret(tstr<obj>());

    if(!_cache.count(key)) _makeGeneric(key, params::make(_paramNames, a));
    return _cache[key];
}
```

**WHEN 매크로의 동작 원리**

@ref by::WHEN "WHEN" 매크로는 조건이 참일 때 체이닝 가능한 헬퍼 객체를 반환합니다. 이 객체는 다음 메서드들을
제공합니다:
- `.err(fmt, ...)`: 에러 로그를 출력하고 자기 자신을 반환 (체이닝 가능)
- `.ret(value)`: 주어진 값을 반환하며 함수를 종료

따라서 `WHEN(condition).err("msg").ret(value)`는 condition이 참일 때만 에러를 로깅하고
value를 반환합니다. 조건이 거짓이면 아무 동작도 하지 않고 다음 코드로 진행됩니다.

@ref by::WHEN "WHEN" 매크로는 프로젝트 내에서 아주 빈번하게 사용되므로 잘 파악해 두는 게 좋습니다.


#### tmay 클래스

@ref by::tmay "tmay" 클래스는 값으로 반환하는 함수에 대해 속도가 느린 exception을 사용하지 않고도 에러임을
알려주는 클래스입니다. `std::optional<T>`과 거의 동일합니다. API look을 프로젝트 컨벤션에 맞춘
것에 가깝습니다.

정상동작일 경우 @ref by::tmay "tmay"의 생성자로 값을 T&로 넘기면 됩니다.
에러상황일 경우 기본 생성자 `tmay<T>()`를 사용하면 됩니다. 내부적으로 @ref by::tmedium "tmedium" 클래스를 사용하여
값의 유무를 추적합니다. @ref by::tmedium "tmedium"은 값을 저장하는 중간 계층으로, T&와 nullptr 모두를 받을 수 있는
유연한 인터페이스를 제공합니다.

`has()` 나 `get()`, `rel()`, `set()` 함수를 제공하는데, 이는 프로젝트 전반적으로 많이 사용되는
네이밍 컨벤션입니다.

**사용 예시**

@ref by::tmay "tmay"는 에러를 반환할 수 있는 함수의 반환 타입으로 사용됩니다:

```
@style: language-cpp verified
tmay<int> divide(int a, int b) {
    if (b == 0)
        return tmay<int>(); // 에러 상황
    return tmay<int>(a / b); // 정상 값 반환
}

// 사용 시:
auto result = divide(10, 2);
if (result.has()) {
    int value = result.get(); // 5
} else {
    // 에러 처리
}
```


#### tres 클래스

@ref by::tres "tres" 클래스는 @ref by::tmay "tmay"와 동일하나, 에러일 경우, 원하는 에러 타입을 갖도록 정의할 수 있습니다.
예를들어 tmay<A>는 에러인지 아닌지만 알 수 있지만, tres<A, std::string>으로 정의하면 에러일 경우,
어떤 에러인지 메시지도 알 수 있도록 만들 수 있습니다.

**사용 예시**

@ref by::tres "tres" 는 @ref by::tmay "tmay"와 달리 에러 정보도 함께 반환할 수 있습니다:

```
@style: language-cpp verified
tres<int, std::string> parseNumber(const std::string& str) {
    if (str.empty())
        return tres<int, std::string>("입력이 비어있습니다");

    try {
        int num = std::stoi(str);
        return tres<int, std::string>(num);
    } catch(...) {
        return tres<int, std::string>("숫자로 변환할 수 없습니다");
    }
}

// 사용 시:
auto result = parseNumber("123");
if (result.has()) {
    int value = result.get();
} else {
    std::string error = result.getErr(); // 에러 메시지 확인
}
```


### 플랫폼 추상화

#### platformAPI 클래스

@ref by::platformAPI "platformAPI" 클래스는 단발성으로 호출되는, 플랫폼 종속적인 API들을 독립적으로 제공하는 일종의
완충작용을 합니다.

예를들면 텍스트 출력시 색깔을 입히려면 posix 계열 플랫폼에서는 ANSI escape sequence를 사용하지만
윈도우에서는 WINAPI를 사용해야 합니다. 이때 @ref by::platformAPI::foreColor() "foreColor()" 를 사용하면,

```
@style: language-cpp verified
cout << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(rightName)
     << foreColor(LIGHTGRAY) << ")";
```

와 같이 사용하면 플랫폼 독립적인 코드를 작성하게 됩니다.


#### buildFeature 클래스

@ref by::buildFeature "buildFeature" 클래스는 CMake에 의해서 자동으로 생성되는 buildInformation.hpp 에 정의된 정보를
반환하는 클래스입니다. 절대 수동으로 값을 변경해서는 안됩니다.

빌드 일시, 버전, 빌드한 OS, 바이너리 타입에 대한 정보를 갖습니다. buildInformation.hpp에 값의
원본이 #define으로 정의되어 있으므로, 필요하다면 이를 사용해도 됩니다. #ifdef시에도 종종
사용되니 참고하세요.

예:

```
@style: language-cpp verified
if(buildFeature::config::isDbg())
    platformAPI::unlimitCoreDump();

// 혹은,

#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
...
#endif
```

앞서 언급했듯 Byeol의 아키텍처는 플랫폼 종속적인 코드는 반드시 @ref indep 모듈에 속해야 합니다. 따라서
@ref indep 보다 상위의 모듈에서 함부로 OS에 대한 #ifdef의 조건부 컴파일이나
@ref by::buildFeature::platform::getName() "getName()" 을 사용해서 코드를 branch 하는 건 권장하지 않습니다.


### 파일 시스템

#### fsystem 클래스

@ref by::fsystem "fsystem" 클래스는 지정한 폴더에서 파일을 재귀적으로 탐색하는 간단한 클래스입니다. 윈도우와 posix
계열 운영체제에서 모두 사용가능한 플랫폼 독립적인 API를 제공합니다. 핵심 API는 iterator 클래스를
통해 이뤄집니다.

사용 예제는 아래와 같습니다.

```
@style: language-cpp verified
auto e = fsystem::find("../your/path");
while(e.next()) { // 모든 파일을 탐색하면 false를 반환한다.
    const std::string& path = *e; // 찾은 파일의 경로
    if(*e == "../your/path/child/helloWorld.cpp") // 항상 상대경로를 사용한다.
        doSomething(e->getDir()); // 찾은 파일의 folder 경로를 반환한다.
}
```

**항상 파일만을 iterate 합니다**. 빈 폴더가 있다면 해당 폴더는 iterate시 skip 됩니다.


### 문자열 처리

#### cpIter 클래스

@ref by::cpIter "cpIter" 클래스는 문자열에 대해서 codepoint 기반의 iteration을 담당합니다. 주로 @ref by::nStr "nStr"에서 UTF8
unicode와 같은 multibyte 문자열을 순회할 때 사용합니다.

일반적인 iterator 답게, 전위 증가 및 후위증가연산자, 역참조 연산자, bool 형변환 연산자 등을
지원합니다. @ref by::cpIter "cpIter"를 생성할때 순회할 문자열과 함께 기본 iteration의 방향을 지정할 수 있습니다.

@ref by::cpIter "cpIter" 생성시 입력한 방향은 iter 자체의 기본 방향과 초기 위치를 정의한 것입니다. reverse = true로
argument로 주면 @ref by::cpIter "cpIter"는 문자열 끝에서 역방향으로 출발합니다. 그러나 이때 stepBackward()나
stepForward()를 명시적으로 호출하면, 이 기본 방향과 관계없이 해당 방향으로 전진합니다.

STL과 마찬가지로 begin은 첫번째 원소를 가리키지만, end는 마지막 원소의 다음 위치를 가리킵니다.
(past-the-end) 따라서 역방향 순회시에는 먼저 next()를 해야만 합니다.

**기본 사용 예제 (정방향 순회)**

먼저 간단한 정방향 순회 예제입니다:

```
@style: language-cpp verified
std::string src = "abc🏁"; // UTF8에서 🏁는 4바이트로 표현되지만 1개의 codepoint다.
cpIter e(src); // 기본은 정방향, 첫 번째 문자를 가리킨다.

while(e) { // iterator가 유효한 동안
    std::cout << *e << " "; // 현재 codepoint 출력
    ++e; // 다음 codepoint로 이동
}
// 출력: a b c 🏁
```

**역방향 순회 예제**

좀 더 복잡한 역방향 순회 예제입니다:

```
@style: language-cpp verified
std::string src = "abcd🏁efg"; // UTF8에서 🏁는 3개의 character로 표현된다.
cpIter e4(src, true); // 기본 iteration 을 역방향으로 정의한다. 이때 마지막 원소의 다음 위치를 가리킨다.
while(*e4 != "") // 현재 end 위치에 있으므로, 이때는 ""가 반환된다.
    ++e4; // e4.next()를 호출한 것과 같다. 이때 기본 iteration 방향인 역방향으로 순회한다.
          // 하지만 실제로 이 코드는 한번도 실행되지 않는다.
ASSERT_EQ(*e4, "");

std::string expects[] = {"g", "f", "e", "🏁", "d", "c", "b", "a"};
for(int n = 0; n < 8; n++) {
    e4.stepBackward(1); // 명시적으로 방향을 지정해서 iterator를 순회한다. e4의 방향과 관계없이 무조건 역방향으로 순회한다.
    // 역방향시에는 이처럼 step을 먼저해야한다.
    ASSERT_EQ(*e4, expects[n]);
}
```


### 동적 라이브러리 로딩

#### dlib 클래스

@ref by::dlib "dlib" 클래스는 dynamic loading for library의 약자입니다. 플랫폼 독립적인 동적 로딩을 담당합니다.
라이브러리의 메모리 적재, 원하는 함수을 찾아 함수포인터로 변환할 수 있습니다.

다음과 같이 사용합니다:
1. @ref by::dlib "dlib" 객체를 생성한다.
2. 로딩할 라이브러리의 위치를 지정한다.
3. 함수명을 통해 원하는 함수를 찾아 함수포인터로 받는다.

@ref by::tmay "tmay"를 사용하므로 @ref by::tmay "tmay"를 사전에 익혀두는 걸 권장합니다.

@ref by::dlib "dlib"은 RAII idiom으로 구현되어 있습니다. 해당 인스턴스가 소멸될때 외부로 반환된 함수포인터는 사용할
수 없게 됩니다.

**사용예제**

다음 예제는 동적 라이브러리를 로드하고 함수를 호출하는 전체 과정을 보여줍니다.
코드에서 `rel()`은 리소스를 명시적으로 해제하는 함수로, 에러 발생 시 dlib 객체를 정리한 후
반환하기 위해 comma 연산자 `(rel(), false)`를 사용합니다:

```
@style: language-cpp verified
dlib lib = dlib(path); // 1번과 2번을 동시에 한다.
auto res = lib.load(); // `res` evaluated as true when it has an error.
WHEN(res) .err("couldn't open %s slot: %d", path, res.get()).ret((rel(), false));
// rel()로 리소스를 먼저 해제하고, comma 연산자로 false를 반환

typedef void (*entrypointFunc)(bicontainable*);
constexpr const nchar* ENTRYPOINT_NAME = "byeol_bridge_cpp_entrypoint";
auto info = lib.accessFunc<entrypointFunc>(ENTRYPOINT_NAME); // 결과는 tmay로 받는다.
WHEN(!info.has()) // tmay의 has()로 결과 체크 중
    .err("couldn't access entrypoint of %s slot: %d", path, info.getErr()).ret((rel(), false));

(*info)(&tray); // 정상적으로 함수를 가져오면, 호출이 가능하다.

// lib이 소멸되면서 자동으로 메모리가 해제된다.
```


### 기타 유틸리티

#### end 클래스

@ref by::end "end" 클래스는 코드 실행을 지연시킵니다. 다른 언어에서 `defer`와 같은 키워드와 같은 역할입니다.

**사용 예시**

RAII 패턴을 활용하여 스코프 종료 시 특정 코드를 실행합니다:

```
@style: language-cpp verified
void processFile(const std::string& path) {
    FILE* fp = fopen(path.c_str(), "r");
    end cleanup([&]() {
        if (fp) fclose(fp);  // 스코프 종료 시 자동으로 파일 닫기
    });

    // 파일 처리 작업...
    // 함수가 어떤 경로로 종료되든 cleanup이 실행됨
}
```


## clog 모듈 - 로깅 시스템

@ref by::clog "clog" 모듈은 여러 출력 스트림과 필터링 기능을 갖춘 경량화된 C++ 로깅 프레임워크를 제공합니다.
이 모듈은 아키텍처 상 하위 계층에 위치하므로, 상위 모듈에 대한 의존성이 없습니다.


### 로깅의 기본 사용법

#### logger 클래스

@ref by::logger "logger" 클래스는 @ref by::stream "stream" 이라고 불리는 복수의 로깅 경로를 통해 체계적으로 로깅이 가능한 경량화된
C++ 로깅 프레임워크의 일종의 facade입니다.

일반적으로는 동봉되는 매크로를 통해, 다음과 같이 사용합니다.

```
@style: language-cpp verified
BY_I("slot[%s] origins loaded.", getName());
```

위 코드는 다음과 같이 출력됩니다.

```
@style: language-txt verified
Oct 22 2025  21:26:13 I cppPackLo <_loadLibs#49> slot[cpp] origins loaded.
```

위 로그는 다음과 같은 정보를 보여줍니다:

1. 로깅한 날짜(Oct 22 2025)와 시간(21:26:13)
2. 로그 레벨(I, Info). 로그 레벨은 ERR, WARN, INFO 총 3개가 존재합니다.
3. 로깅한 클래스명(cppPackLoading)
4. 로깅한 함수(_loadLibs)와 해당 파일내 라인번호(49)
5. 로그 메시지(slot[cpp] origins loaded)

기본적인 사용법은 여기까지만 알아도 충분합니다.


### 로깅 시스템 아키텍처

#### stream 클래스

@ref by::clog "clog" 모듈의 핵심은 @ref by::stream "stream"입니다. @ref by::stream "stream"은 쉽게 말해 로깅이 출력되는 스트림, 즉 목적지를
표현합니다. 현재는 @ref by::consoleStream "consoleStream" 과 @ref by::fileLogStream "fileLogStream" 2가지가 존재합니다. 모든 @ref by::stream "stream" 은 기본적으로
@ref by::logger "logger" 클래스가 처음부터 소유하고 있습니다.

각 stream은 byeol의 핵심 클래스들과 마찬가지로 다음과 같은 상태 전이 도식을 갖습니다:

```
@style: language-txt verified
RELEASED ---init()---> INITIALIZED
RELEASED <--rel()----- INITIALIZED
```

객체를 처음 생성하면 RELEASED 상태에서 시작하지만, 생성자 내부에서 자동으로 `init()`을 호출하여
INITIALIZED 상태로 진입합니다. 단, 특정 stream 인스턴스를 명시적으로 `rel()`한 경우,
다시 명시적으로 `init()`을 호출하지 않으면 정상적으로 동작하지 않습니다.

stream은 또한 enable 여부를 관리합니다. setEnable(false)를 통해 특정 stream을 disable 시키면 해당
stream은 동작하지 않습니다.

또하나 중요한 점은 @ref by::logger "logger" 클래스 자체도 @ref by::stream "stream" 에서 상속하기 때문에 @ref by::stream "stream" 과 동일한 API를 제공한다는
점입니다. @ref by::logger "logger"는 각 API에 대해 소유한 모든 @ref by::stream "stream" 들에 대해 redirection 하는 구성으로 구현되어
있습니다. 예를들어 `logger::get().setEnable(false)`를 하게 되면, 모든 @ref by::stream "stream" 이 disable 됩니다.

@ref by::stream "stream" 은 logBypass(const nchar*) 라는 함수를 제공하는데, 이것은 어떠한 가공도 없이 문자열을 그대로
지정한 @ref by::stream "stream" 으로 로그 메시지를 보냅니다.


### 로깅 매크로 시스템

앞서 언급한 `logByPass()`를 통해서 직접 로깅을 해도 되지만, 파일명이나 함수명, 라인번호 등 좀 더
고급스러운 로깅을 하고 싶다면 로깅 매크로를 사용하면 됩니다.

다음과 같이 사용합니다.

```
@style: language-cpp verified
BY_I("just message.")
```

Byeol 매크로 컨벤션에 따라 매크로는 항상 `BY_` prefix로 시작합니다. 뒤에는 로그 레벨인 `I`가 나오며,
매크로 안쪽에는 로깅할 메시지가 들어갑니다. 위와 같이 로깅할 경우 다음과 같이 출력됩니다.

```
@style: language-txt verified
Oct 22 2025  21:26:13 I cppPackLo <_loadLibs#49> just message.
```

로그 레벨은 Error, Warning, Info, 3개가 존재하며, 각각 매크로도 3개가 존재합니다. 또한 만약 디버그
바이너리에서만 로그를 출력하고 싶다면 레벨 앞에 `D`를 붙입니다. 이를테면 다음과 같습니다.

```
@style: language-cpp verified
BY_DE("leaf: ERR: %s", e);
```

위 코드는 printf에서 익숙하게 봤을 서식문자를 사용하고 있습니다. 이렇게 동적인 값을 집어넣는 것도
가능합니다. 결과는 다음과 같이 출력됩니다.

```
@style: language-txt verified
Oct 22 2025  21:26:13 E leafPars <_finalize#263> leaf: ERR: src is empty
```

해당 macro의 구현은, 출력할 문자열 앞에 날짜나 시간 등을 붙여서 logBypass()에 전달하도록 expand 하는
것이긴 하나, 그외에도 한가지 중요한 기능이 더 있습니다.


#### 간략화된 주소값

@ref by::richLog "richLog" 로 void*를 넘기게 되면 @ref indep 모듈에 있는 @ref by::platformAPI "platformAPI" 를 사용해서 `toAddr()`를 호출합니다.
이 함수는 void*를 마지막 4 hex값을 문자열로 변환하는 함수이며, 프로젝트 내에서 주로 `@` 뒤에 적는
스타일입니다. 예를 들면 다음과 같이 로깅 됩니다.

```
@style: language-txt verified
Nov 18 2025  20:02:13 I verifier  <onLeave#87> '' assignExpr@9a50: step#1 --> set evalType
```

위 로그에서 assignExpr@9a50은 assignExpr 객체가 heap 주소의 끝자리가 9a50인 곳에 있는 인스턴스라는
얘기입니다. 이는 동일한 타입에 대해 서로 다른 인스턴스가 같은 시점에 로깅되는 경우 좀 더 수월하게
디버깅 하도록 돕습니다.


### richLog - 다형성 로깅

@ref by::richLog "richLog" 는 서식문자에 입력된 argument를 다형성을 활용해서 적절한 타입으로 변환해서 로깅하는
기능입니다.

@ref clog 모듈은 architecture 상 아랫부분에 위치하기 때문에 @ref clog 에 종속하는 클래스가 뭐가 있는지 알아서는
안됩니다. 그렇기 때문에 @ref by::richLog "richLog" 는 각 모듈마다 정의되어 있으며, 해당 모듈에 포함된 클래스를 어떻게
문자열로 변환할지를 정의해두고 있습니다.

이를 사용하면 다음과 같은 코드가 가능해집니다.

```
@style: language-cpp verified
BY_I("make a closure for %s.%s", meObj, cast.getSrc().getName());
```

결과는 다음과 같이 나올 수 있습니다.

```
@style: language-txt verified
Oct 22 2025 22:01:12 I closure <_make#73> make a closure for obj.foo
```

meObj은 tstr<obj>라는 타입이고, getName()은 std::string을 반환하지만 양쪽 모두 적절하게 문자열로
변환해서 로깅이 됩니다. 단 주의할 점은, `%d` 인지 `%s`를 써야 하는지 타입마다 다를 수 있다는
점입니다. (하지만 대부분 scalar type을 제외하고는 %s를 사용합니다.)

@ref by::richLog "richLog" 는 크게 __convert__ 부분과 wrap 2 부분으로 나뉘어져 있습니다.

#### convert()

각 모듈에 속한 클래스를 어떻게 로깅하기 쉬운 타입으로 변경할지를 정의합니다. 호출자는
__convert__()에 자신이 받은 구체타입을 넣을 뿐이며, 오버로딩에 의해서 가장 적절한 타입에 대한
__convert__가 호출됩니다.

그 말은, 범용적인 __convert__()와 특정 타입에 특화된 __convert__()를 동시에 정의할 수 있다는
말입니다. 대표적으로는 __convert__(void*)가 있습니다. 어떠한 T*에도 매칭 되지 않으면 이
__convert__가 대신 호출됩니다.

#### wrap

wrap이 필요한 이유는 richLog() 안쪽에서 __convert__()를 호출하고 이때 전달받은 값들을 가변인자를
통과할 수 있도록 풀어주는 역할을 합니다.

가변인자 함수는 scalar type이나 T*만 넘길 수 있기 때문에 값으로는 넘길 수 없습니다. 일부
__convert__()는 안에서 새로운 값을 만들어 값으로 넘겨야 하는 상황도 있을 수 있기 때문에
@ref by::logger::log() "log()" 가 가변인자로 구성되어 있는 한은 @ref by::wrap "wrap"이 꼭 필요합니다.

__convert__() 함수들은 크게 `strWrap` 혹은 `noWrap<T>` 2가지 중 하나를 반환형으로 정의하는데
noWrap은 아무런 가공없이 받은 걸 그대로 반환하지만 strWrap은 std::string::c_str()를 내부적으로
호출합니다.

기본적으로는 @ref by::strWrap "strWrap"을 사용하면 되지만, richLog 사용 시 format string에 `%d`, `%f` 등 사용자가
직접 타입을 명시하는 서식문자를 사용하고 싶을 때는 noWrap을 사용합니다. 예를 들어:

```
@style: language-cpp verified
// strWrap: 자동으로 문자열로 변환 (%s 사용)
strWrap __convert__(const myClass& obj) { return strWrap(obj.toString()); }

// noWrap: 원본 타입 유지 (%d 등 명시적 서식문자 사용 가능)
noWrap<int> __convert__(int val) { return noWrap<int>(val); }
```

#### richLog 확장 예제

각 모듈은 자신의 타입을 로깅할 수 있도록 `__convert__()` 함수를 추가로 정의합니다.
예를 들어 @ref meta 모듈에서는 다음과 같이 정의합니다:

```
@style: language-cpp verified
// module/meta/common/richLog.hpp
namespace by {
    class type;
    class typeProvidable;

    // type 클래스를 로깅할 수 있도록 __convert__ 구현
    _nout strWrap __convert__(const type& rhs);
    _nout strWrap __convert__(const type* rhs);
    _nout strWrap __convert__(const typeProvidable& rhs);
    _nout strWrap __convert__(const typeProvidable* rhs);
}
```

이렇게 정의하면 로깅 매크로에서 해당 타입을 직접 사용할 수 있습니다:

```
@style: language-cpp verified
type& t = ttype<myClass>();
BY_I("type is %s", t);  // __convert__(const type&)가 호출되어 적절히 문자열로 변환됨
```


### 스트림 제어

#### enablesZone 클래스

@ref by::logger "logger" 클래스는 여러개의 @ref by::stream "stream"을 가지고 있고 특정 @ref by::stream "stream" 을 disable 혹은 enable 함으로써
출력되는 경로를 제어할 수 있습니다.

여기서 문제는 특정 코드 블록 혹은 함수에서만 @ref by::stream "stream"을 제어 한 후, 블록을 벗어날 때는 원래 상태로
되돌리고자 하는 경우가 매우 자주 일어납니다.

@ref by::enablesZone "enablesZone" 은 이럴때 사용하는 것으로, RAII idiom으로 구현되어 있어서, 객체 생성과 동시에 @ref by::stream "stream"의
 enable 상태를 기록했다가 @ref by::enablesZone "enablesZone" 이 소멸될때 각 @ref by::stream "stream" 의 enable 여부를 초기상태로 되돌립니다.

보통은 다음과 같이 사용합니다.

```
@style: language-cpp verified
// let's assume that all streams in logger are enabled.

{
    enablesZone zone;
    logger& log = logger::get();
    log.getStream(0).setEnable(false); // 0번째 stream을 disable.
    logger::getStream("consoleStream").setEnable(false); // console로 logging 하지 않도록 disable.
    BY_E("error message1"); // 이 로그는 0번 stream과 console로는 로깅되지 않는다.

    logger::setEnable(false); // 모든 stream을 disable.
    BY_E("this message won't log on any stream");

}

BY_E("this message will definitely be log on entire stream");
```


### 필터링 시스템

#### filterable 클래스

@ref by::filterable "filterable"클래스는 @ref by::logger "logger"클래스가 특정한 조건에 해당하는 logging은 필터링 할 수 있게 해줍니다.
`filt()` 함수를 제공하며, parameter로 주어진 Log 정보에 대해 true를 반환할 경우 해당 Log는 @ref by::stream "stream"
에 올려지지 않습니다.

각 @ref by::stream "stream"에 전달될 로그 메시지를, 특정한 조건으로 필터링 할 수 있게 해줍니다. @ref by::filterable "filterable"은
`filt()` 함수를 통해, 메시지의 적합성을 판단해 필터링할 것인지를 결정합니다. @ref by::logger "logger" 클래스의
`setFilters(const filters&)`를 통해서 @ref by::filterable "filterable"을 추가할 수 있습니다.

예를들어 @ref by::errPassFilter "errPassFilter"는 `errLv`이 ERR일때만 통과시키는 필터입니다. 다음과 같이 사용할 수
있습니다.

```
@style: language-cpp verified
const filters& prevFilters = logger::get().getFilters();
filters fs(new errPassFilter());
logger::get().setFilters(fs);

// doSomething..

logger::get().setFilters(prevFilters);
```

#### filters 클래스

@ref by::filters "filters"클래스는 @ref by::logger "logger"클래스에서 등록된 @ref by::filterable "filterable"클래스들을 관리합니다. @ref by::filterable "filterable"과
동일한 API를 가지며, 해당 API를 호출하면 소유한 모든 @ref by::filterable "filterable"에 해당 API를 호출합니다.

#### errPassFilter 클래스

말그대로 err만 통과시키는 @ref by::filterable "filterable"클래스입니다. 이 filter를 @ref by::logger "logger"에 등록하면 warning이나
info는 출력되지 않습니다. 직접 사용하지 않으며, 객체 생성하여 @ref by::logger "logger"에 add 하는 용도로 사용합니다.


## meta 모듈 - 런타임 타입 시스템

@ref meta 모듈은 런타임 타입 정보(RTTI)와 리플렉션 기능을 제공합니다. C++의 기본 RTTI보다 더 강력하고
효율적인 타입 시스템을 구현하고 있으며, Byeol 언어의 타입 시스템 기반이 됩니다.


### 메타 시스템 개요

메타 시스템은 프로그램 실행 중에 타입 정보를 조회하고 조작할 수 있게 해줍니다. 이는 다음과 같은
목적을 위해 사용됩니다:

1. 타입 계층 구조 탐색 (상속 관계 확인)
2. 타입 정보 조회 (이름, 추상 클래스 여부, 템플릿 여부 등)
3. 동적 인스턴스 생성
4. 효율적인 타입 캐스팅 (dynamic_cast보다 빠름)

메타 시스템의 핵심은 Monostate 패턴을 사용하여 매번 객체를 생성해도 내부 상태는 공유된다는 점입니다.
따라서 `ttype<MyClass>()`를 여러 번 호출해도 추가 비용이 들지 않습니다.


### 타입 정보 사용하기

#### ttype<T> 클래스

@ref by::ttype "ttype"클래스는 사용자가 메타 정보를 다루고자할때 진입점이 되는 클래스입니다. @ref by::ttype "ttype"를
사용할때는 매번 객체를 생성해서 사용해야 합니다. 전체적으로 monostate 패턴으로 설계 되어있어서 매번
객체를 만들어 사용하더라도 값은 공유하기 때문에 추가비용은 들지 않습니다.

@ref meta 모듈의 전체적인 설계에 대해 파악하고자 한다면 핵심이 되는 @ref by::type "type"을 먼저 살펴보세요.


### type 클래스의 기능

@ref by::type "type"클래스는 @ref meta 모듈의 핵심이 되는 클래스입니다. @ref by::type "type"에 대한 다음의 기본적인 API를 제공합니다.

#### 기본 타입 식별

다음 함수들로 타입의 기본 정보를 얻을 수 있습니다:

- `isTemplate()`: 템플릿 클래스 여부를 반환합니다.
- `isAbstract()`: 추상 클래스 여부를 반환합니다.
- `getName()`: 클래스명을 반환합니다. 이름은 demangle 되어 반환됩니다.

#### 클래스 계층 관련 정보

**getSupers()**

super class들을 리스트에 담아 반환합니다. 이 클래스의 직접적인 부모 클래스는 리스트 마지막 원소에,
보다 더 super class일 수록 리스트 앞에 담겨있습니다.

예를들어 다음과 같은 코드를 보세요.

```
@style: language-cpp verified
const auto& supers = getType().getSupers();

// 가장 첫번째 클래스는 항상 adam 이 된다. adam 클래스 참조.
supers[0].getName() // "adam"

supers.last()->getName() // this 클래스의 부모클래스 이름이 나온다.
```

**getSubs()**

sub class들을 리스트에 담아 반환합니다. getSupers()와 마찬가지 방식으로, 가까운 sub 일 수록 리스트
앞에 담겨있습니다.

**isSuper(const rhs& type)**

this class가 rhs보다 super 클래스 인지 체크합니다. dynamic_cast과 동일한 기능이지만 알고리즘
복잡도가 더 뛰어납니다.

일반적으로 dynamic_cast는 vtable을 순회해서 체크합니다. 반면 meta는 메타 정보 생성시 super 클래스
계층도를 통해서 자신이 몇 tier의 자손인지를 바로 알 수 있습니다. 이 tier값과 char*의 주소값을
비교해서 타입정보를 비교합니다.

**isSub(const rhs& type)**

isSuper와 반대로 동작합니다.

#### 인스턴스 생성

**make()**

이 type의 기본생성자로 인스턴스를 생성합니다. 만약, 기본생성자가 없다면 nullptr가 반환되니
주의하세요.

#### 메타 타입 정보 관리

`init()`으로 초기화를, `rel()`로 해제를 합니다. 이는 `BY_INIT_META` 매크로를 통해서 자동으로
처리가 되는 부분이라 외부에서 직접 호출할 경우는 드뭅니다.


### 메타 정보의 생성과 관리

#### 메타 정보가 어떻게 생성되나

`isTemplate()` 이나 `isAbstract()`, `getName()` 같은 타입정보는 @ref by::ttypeBase "ttypeBase"에서
메타프로그래밍을 통해 채워줍니다. 따라서 @ref by::type::init() "init()"이 존재하는 이유는 클래스 계층을 구성하기
위해서입니다.

그리고 그 계층을 구성하는 핵심은 **모든 클래스는 typedef로 super를 정의해야 한다**라는 제약사항으로
해결합니다. 모든 클래스에 `super`가 존재한다면, @ref by::ttype::init() "init()"도 호출할 수 있기 때문에
다음과 같은 간단한 코드로 클래스 계층을 재귀적으로 구성할 수 있게 됩니다.

**참고**: 부모 클래스가 없는 최상위 클래스는 `typedef adam super;`로 정의하여 @ref by::adam "adam"에 연결됩니다.
이를 통해 모든 타입이 단일 계층 구조를 이루게 됩니다.

```
@style: language-cpp verified
// 주의: 다음은 실제 코드 동작을 이해하기 쉽게 간략화 한 것이다.
nbool me::init() {
    if(_isInit) return false;
    _isInit = true;

    // 위 코드로 인해, 여기는 딱 1번만 실행된다.
    type& super = (type&) getSuper(); // getSuper()는 ttype<typename T::super>::get() 를 반환한다.
    super.init(); // 재귀적으로 부모의 타입을 계속 호출하는 과정이 반복된다.
                  // 최종적으로는 adam 클래스까지 올라가게 되며, adam 은 부모가 없기 때문에 취소된다.

    types& mySupers = getSupers();
    mySupers = super.getSupers();
    mySupers.push_back(&super);
    ...
}
```

실제 코드는 이와 크게 다르지 않습니다. 실제로 몇 줄의 간단한 코드만으로도 클래스 계층을 구성합니다.
이 과정은 재귀를 사용하긴 하지만, 프로그램 실행 후 1번만 발생하기 때문에 비용도 비교적 적습니다.

#### 메타 정보 자동 생성

앞서 얘기한 대로 각 type 클래스들의 정보는 `init()` 함수를 호출하면 생성되고 채워집니다. 문제는 각
클래스들에 대한 타입 객체를 일일이 사용자가 생성하고 각각을 명시적으로 `init()`을 호출하는 것은
굉장히 비효율적이라는 점입니다.

이걸 해결하고자 BY_INIT_META 매크로를 사용합니다. BY_INIT_META 는 BY_INITIATOR 매크로를
응용하는데, 이 매크로는 static 객체에 람다함수를 끼워넣음으로써 원하는 동작을 main() 함수가 시작되기
전에 실행하는 매크로입니다.

이 매크로를 응용하면 init() 함수를 main() 함수가 호출되기 직전에 실행하는 게 가능해집니다. 한가지
제약사항으로는 각 클래스 선언시에 BY_INIT_META (MyClass)를 추가해야 한다는 점입니다.

**주의**: static 초기화 순서를 사용하므로, 여러 translation unit 간의 초기화 순서는 보장되지
않습니다. 다만, 각 type의 `init()`은 재귀적으로 부모를 먼저 초기화하는 구조이므로, 타입 계층 구조
내에서는 부모에서 자식으로 가는 초기화 순서가 보장됩니다.

이러한 메타 DSL 형태의 매크로들은 BY 매크로에 의해서 실행되도록 컨벤션이 정해져있습니다. 그리고 @ref core
모듈에서도 추가로 정의해야할 메타 DSL 매크로가 있기 때문에 직접 BY_INIT_META 를 호출하기 보다는
`BY(CLASS())` 나 `BY(ADT())`를 통해서 한번에 정의합니다.


### adam - 최상위 타입

@ref by::adam "adam"클래스는 모든 타입 계층의 루트입니다. @ref by::type "type"의 클래스 계층구조 상 아무런 부모도 없는
메타타입은 범용적으로 다루기가 어렵습니다.

예를들어 모든 메타타입에 대해서 동작하는 함수를 작성하고자 한다면, 다음과 같은 코드를 작성할 수
있어야 합니다.

```
@style: language-cpp verified
const auto& types = how_to_get_all_meta_types() // ?
for(const auto& t : types)
    cout << t.getName() << "\n";

vector<type> how_to_get_all_meta_types() {
    // 모든 타입들을 순회해서 가져올 방법이 없다.
    // 그러니 다음과 같이 해야한다?
    vector<type> ret;
    ret.push_back(ttype<A>());
    ret.push_back(ttype<B>());
    ...

    return ret;
}
```

이를 해결하고자 메타 정보를 구성할때 어떠한 부모클래스도 없는 클래스라면 부모를 @ref by::adam "adam"으로 정의합니다.
이제 다음과 같이 작성할 수 있게 됩니다.

```
@style: language-cpp verified
for(const auto& t : ttype<adam>().getSubs())
    cout << t.getName() << "\n";
```


### 메타 타입 확장하기

@ref by::type "type"은 기본적으로도 비교적 많은 타입 정보를 제공하지만, byeol 처럼 언어를 다루는 경우에는 parameter나
return type과 같이 추가적인 정보를 담고 싶은 경우가 있습니다.

이때 쉽게 생각하면 @ref by::type "type"을 상속받은 클래스를 만들면 되는거 아닌가라는 아이디어를 떠올리기 쉽지만
`ttype<T>`에서 언급한 것처럼, 항상 사용자의 최종 진입점은 `ttype<T>`에 접근하면서 시작되어야
합니다. 문제는 @ref by::ttype "ttype"의 코드를 @ref meta 모듈을 종속하는 쪽에서 수정할 수는 없기 때문에 상속으로는
불가능하고 메타 타입을 주입하는 형태로 문제를 해결합니다.

핵심 코드는 @ref by::ttypeBase "ttypeBase"에 있습니다.

```
@style: language-cpp verified
template <typename T, typename S = typename tmetaTypeDef<T>::is>
class ttypeBase: public S {
    ....
}

template <typename T, nbool hasMeta = tifHasMetaTypeDef<T>::is>
struct tmetaTypeDef {
    using is = type;
};
template <typename T>
struct tmetaTypeDef<T, true> {
    using is = typename T::metaType;
};
```

tmetaTypeDef는 T에 typedef metaType이 있을 경우에는 해당타입을 반환하고, 없으면 @ref by::type "type"을 반환합니다.
@ref by::ttype "ttype"은 @ref by::ttypeBase "ttypeBase"를 상속하며, @ref by::ttypeBase "ttypeBase"는 바로 tmetaTypeDef<T>::is를 상속합니다.

이걸 통해서 만약 class T에 대해 ttype<T>를 호출하는 순간, class T의 개발자가 `typedef metaType
MyType;` 처럼 새로운 MyType 클래스를 선언하여 추가하면 해당 ttype<T>()로 객체를 만들었을때 MyType을
기반으로 해서 만들어지게 됩니다.

실제로 이 기능은 @ref core 모듈에서 @ref by::ntype "ntype"을 주입하기 위해 사용합니다. 자세한 내용은 @ref by::ntype "ntype"을 참조하세요.


## memlite 모듈 - 커스텀 메모리 관리

@ref memlite 모듈은 참조 카운팅을 갖춘 커스텀 메모리 풀 시스템을 제공하여 자동 메모리 관리를 유지합니다.

@ref memlite 의 궁극적인 목적은, byeol managed 환경을 실행할 수 있는 경량화된 C++ 메모리 관리에 있습니다.
따라서 GC 등 추가적인 메모리 관리가 필요로 해지며, 이는 자체 메모리 풀을 가지고 있으며, 인스턴스의
라이프사이클을 관리해야 한다는 것을 시사합니다.


### 바인딩 시스템 사용하기

#### binder 클래스

@ref by::binder "binder"클래스는 일반화된 바인딩 클래스로 @ref instance 클래스를 상속한 클래스로부터 생성된 모든 객체를
바인딩할 수 있습니다. reference counting으로 적절한 시점에 객체를 소멸시키며, 표준 라이브러리에 잘
정의된 std::weak_ptr과 같은 기능을 @ref by::tweak "tweak"가, std::shared_ptr은 @ref by::tstr "tstr"이 각 담당합니다.

shared_ptr를 이미 잘 알고 있다면 아래와 같이 사용할 수 있다는 걸 쉽게 이해할 수 있을 것입니다.

#### 기본 사용법

객체를 바인딩하는 `bind()`와 `isBind()`, `get()`을 주로 사용하게 될 것입니다. byeol은 기본적으로
setter 형 API에 대해 T*와 T&를 모두 준비해두는 경우가 많으니 참고하세요.

```
@style: language-cpp verified
class A : public instance {}; // instance를 상속했으니 바인딩 가능하다.
A* a = new A();

{
    tstr<A> strBinder;
    strBinder.bind(a);
    // strBinder에 의해 a의 `life`는 count가 1이 된다.

    strBinder.isBind(); // true
    a == strBinder.get(); // true
} // 이때 strBinder가 소멸되면서 life의 count를 0으로 만들고 결과 a가 자동으로 소멸된다.

*a; // 에러: 소멸된 객체를 사용하려 한다.
```

위는 아주 기본적인 API만 사용한 지나치게 정석적인 예제입니다. 실제로는 이보다는 더 간략하게 쓰는
편입니다.

```
@style: language-cpp verified
class shell : public instance {
public:
    int age;
};

tstr<shell> foo() {
    tstr<shell> ptr(new shell()); // 생성과 동시에 바인딩한다.
    ptr->age = 57; // operator->를 지원한다.

    tweak<shell> weak = ptr; // 같은 shell에 대한 바인더 끼리도 호환된다.
    callShell(*weak); // operator*() 도 당연히 지원한다.

    return ptr; // tstr을 값으로 반환하므로 count는 유지된다.
                // 따라서 foo() 안에서 new 로 생성한 shell객체는 소멸되지 않는다.
}
```


### std::shared_ptr 대비 장점

이쯤되면, 아마도 왜 shared_ptr를 사용하지 않고 굳이 tstr을 만들었는가에 대해 의문을 가질 것입니다.
shared_ptr이 제공하지 못하는 몇가지 장점이 있기 때문입니다.

#### reference counting 블록은 인스턴스 자체에 붙어있다

shared_ptr은 생성시 내부적으로 reference counting을 위한 `Control block`이라는 걸 heap에 만들어서
관리한다는 건 이미 잘 알고 있을 것입니다. 그래서 shared_ptr 사용시 다음과 같은 사용은 매우 위험합니다.

```
@style: language-cpp verified
Foo* raw = new Foo();
shared_ptr<Foo> foo1(raw);
    .....
shared_ptr<Foo> foo2(raw); // foo1과 foo2 각각 control block 이 생성되므로
                           // double delete가 발생한다.
```

그리고 이 문제는 바로 프로그램이 종료하지 않기 때문에 디버깅이 아주 어렵습니다.

byeol에서는 reference counting을 위한 클래스를 @ref by::life "life"라고 하며, 이는 @ref by::watcher "watcher"에 의해 인스턴스마다
별도로 제공됩니다. @ref by::watcher "watcher"는 내부에 @ref by::life "life"객체들을 배열로 미리 대량 할당해둔 풀(pool)을 관리합니다.
새로운 인스턴스가 바인딩될 때 사용 가능한 @ref by::life "life"를 할당하고, 인스턴스가 소멸되면 해당 @ref by::life "life"를
사용 가능(available) 상태로 표시하여 나중에 재사용합니다. 동일한 인스턴스에 대해서는 항상 같은
@ref by::life "life"가 할당되므로 이중 해제 문제가 발생하지 않습니다.

#### ADT 제공

@ref by::tstr "tstr"과 @ref by::tweak "tweak"는 같은 @ref by::binder "binder"기반클래스를 갖기 때문에 @ref by::binder "binder"타입으로 범용적인 로직을 구현할 수 있습니다.

```
@style: language-cpp verified
void me::rel(binder& me) { // me가 tstr인지 tweak인지 상관없다.
    WHEN(!me.isBind()) .ret();

    life* l = me._getBindTag();
    if(l) l->_onStrong(-1);
}
```

@ref by::binder "binder"는 abstract 하므로 객체 생성이 불가능합니다. @ref by::tstr "tstr"이나 @ref by::tweak "tweak"로 이미 생성된 바인더들을 범용적인
로직을 작성할때만 의의를 갖습니다.

#### 동적 타입 체킹

@ref by::binder "binder"는 ADT이며 클래스 템플릿 조차 아닙니다. 따라서 binder::bind() 함수는 parameter가 @ref by::instance "instance"
타입으로 되어있습니다. 이 말은 tstr<A>라고 할지라도 bind(new B()); 컴파일 에러가 발생하지 않는다는
걸 의미합니다.

bind() 안쪽에서 @ref meta 모듈을 사용하여 동적으로 타입을 검사해서 올바른 경우만 인스턴스가 바인딩 됩니다.
타입이 일치하지 않을 경우, bind()는 `false`를 반환하고 바인딩을 수행하지 않습니다. 따라서 타입
안정성이 필요한 경우 bind()의 반환값을 확인해야 합니다.

#### 자체 메모리 풀 사용

인스턴스 할당시 모든 작업은 @ref by::instancer "instancer"를 시작으로 이뤄집니다. 자체 메모리 풀을 사용함으로써 heap보다
빠른 할당/해제가 가능합니다.

#### 속도 개선

shared_ptr의 알고리즘은 같은 shared_ptr 끼리 공유되는 reference counting 정보를 heap에 보관하고
공유하는 것입니다. heap보다 빠른 자체 메모리 풀을 사용하고, 바인딩 속도를 조금이나마 최적화한다면 속도를
개선할 여지가 있습니다.

참고로, binding은 byeol에서 가장 많은 퍼포먼스 비용을 차지하는 핫스팟중에 하나입니다.

#### 추가 정보 제공

shared_ptr은 heap에 reference counting 정보를 보관하는 객체를 생성하고 이를 공유합니다. 반면 @ref memlite
모듈은 @ref by::watcher "watcher"클래스를 통해서 이미 메모리는 할당된, 빈 @ref by::life "life"하나를 내어주고, 그곳을 해당
@ref by::instance "instance"의 reference counting 공간으로 활용합니다.

만약 이후, GC와 같은 기능이 추가되면 인스턴스마다 추가적으로 생명주기와 관련된 정보를 필요로 할 여지가
있습니다. shared_ptr와 달리 각 인스턴스의 생명주기 정보 또한 자체적으로 관리하고 있기 때문에 그런
요구사항에도 적절하게 대응할 수 있습니다.


### 메모리 풀 아키텍처

@ref memlite 의 메모리 관리는 여러 계층으로 구성되어 있습니다. 각 계층은 특정한 역할을 담당하며, 하위
계층부터 이해하는 것이 전체 구조를 파악하는데 도움이 됩니다.

```
@style: language-txt verified
instancer (관리자)
├── pool (저수준 할당자)
│      └── chunks (블록 관리자)
│               └── chunk (고정 크기 블록)
└── watcher (생명주기 관리자)
         └── life (참조 카운터)
```

#### chunk - 최소 할당 단위

@ref by::chunk "chunk"클래스는 @ref memlite 에서 메모리를 실제로 할당 가능한 최소 단위 클래스입니다. 모든 메모리 관리는
@ref by::chunk "chunk"들을 엮어서 수행합니다.

@ref by::chunk "chunk"는 메모리가 flexible하게 늘어나도록 하는 _resize() 함수가 있지만, @ref memlite 의 컨셉상 이를 public으로
공개하지 않습니다. 결과적으로 @ref by::chunk "chunk"의 메모리는 객체 생성시 고정되며, 추가 메모리가 필요하다면 @ref by::chunk "chunk"
객체를 더 생성해서 운영해야 합니다.

**Block size**

@ref chunk 는 생성시 block size와 size 2개를 입력받습니다. blockSize는 메모리에 인스턴스 하나가 차지하게 될
최소 단위 크기입니다. 반면 size는 그러한 인스턴스가 몇개 까지 들어갈 지를 정합니다.

예를들어 만약 int64만 100개 담는 chunk를 만든다고 한다면, 다음과 같이 됩니다.

```
@style: language-cpp verified
// int64 크기(8바이트"바이")의 블록 100개를 담는 chunk 생성
chunk myChunk(sizeof(int64), 100);

// 메모리 할당
void* ptr1 = myChunk.new1();  // 첫 번째 블록 할당
void* ptr2 = myChunk.new1();  // 두 번째 블록 할당

// 할당된 메모리 사용
int64* data = static_cast<int64*>(ptr1);
*data = 42;

// 메모리 해제
// 참고: chunk::del()은 2번째 파라메터로 size가 있긴 하지만, 사용하지 않음
myChunk.del(ptr1, 0);  // 첫 번째 블록 메모리 해제
myChunk.del(ptr2, 0);  // 두 번째 블록 메모리 해제

// 상태 확인
myChunk.len();   // 현재 할당된 블록 수
myChunk.size();  // 전체 블록 수 (100)
```

**real block size**

실제 메모리 할당시에는 block size 대신 real block size를 사용하는데, 이는 최적화에 따른 것입니다. CPU
연산시 1이나 2바이트 등 작은 단위로 메모리 할당해서 계산하는 것보다 CPU 아키텍처에 맞게 정렬(padding)하는 것이
더 효율적입니다. 예를 들어, 64비트 CPU에서는 8바이트 단위로 정렬되며, 3바이트를 요청해도 실제로는
8바이트가 할당됩니다. 이는 메모리 접근 속도를 최적화하기 위한 것입니다.

**ArrayList 구현**

@ref by::chunk "chunk"는 배열 기반 리스트(ArrayList)로 직접 구현되어 있습니다. 크기가 고정되어 있지만 크기 내에서는
List처럼 추가 삭제가 자유로우며 임의접근 속도는 Array처럼 빠릅니다.

알고리즘은 다음과 같습니다:

1. 각 원소의 byte 크기는 real block size 이상이어야 한다는 전제조건을 갖습니다. 이는 앞서 설명한 real block size로
   인한 것으로, CPU 아키텍처에 따라 1바이트를 할당하더라도 실제로는 4바이트 이상으로 정렬되기 때문입니다.

2. 각 원소들은 모두 동일한 byte 크기를 갖습니다.

3. @ref by::chunk "chunk"는 각 블록을 기본적으로는 void*로 취급하지만 값이 없을 경우에는 int타입으로 취급합니다.
   이 int타입은 다음에 참조해야할 빈 인덱스를 가리킵니다.

4. 배열을 초기화시 사용자로부터 size를 받아 n번째 원소에 n+1를 표현하는 정수값을 넣어둡니다.
> 예: size=4의 경우, [1, 2, 3, 4]

5. _head는 가장 최근에 add된 원소의 인덱스를 표현하며, 0으로 초기화되며, _heap은 heap에 할당된 메모리를
   가리킵니다.

6. new1으로 외부에서 메모리 할당을 요청하면 _head를 현재 _head번째 원소의 int로 값으로 할당합니다.
> 예: new1() 경우, _head는 이제부터 _heap[0]에 담긴 `1` 값이 할당됩니다. 이는 다음 new1()을 했을때
   _heap[_head]인 _heap[1]를 할당가능한 유력한 빈 원소로 간주한다는 얘기입니다.

7. 방금 가져온 원소에 할당된 메모리 주소를 반환합니다.
> _head = 1, [사용중, 2, 3, 4]

8. 메모리 해제가 발생하면, parameter로 해제할 메모리 주소를 void*로 받습니다.
> 예: del(used); // 이때 used = _heap[0]의 주소

9. 해당 메모리에 현재 _head 값을 저장합니다. (여기에 오기 전에 이미 소멸자가 호출되었다고 전제합니다.)
> *used = _head  // [1, 2, 3, 4]

10. _head값을 현재 해제중인 메모리의 인덱스로 업데이트합니다. 이때 인덱스는 used 주소가 _heap으로부터
   얼마나 떨어져 있는지를 pointer 연산으로 계산합니다.
> 예: _head = (used - _heap) / blockSize
> 위 상황에서는 used가 _heap[0]의 주소이므로, (used - _heap) / blockSize = 0
> _head = 0, [1, 2, 3, 4]

#### chunks - 다중 chunk 관리

@ref by::chunks "chunks"객체는 여러개의 @ref by::chunk "chunk"의 인스턴스 관리를 담당합니다. @ref by::chunk "chunk"는 생성시 고정된 크기만 메모리를
활용하기 때문에 @ref by::chunks "chunks"가 여러개의 @ref by::chunk "chunk"를 추가/삭제 함으로써 유동적으로 메모리를 관리합니다.

**chunks 역시 고정된 메모리만 제공한다**

@ref by::chunks "chunks"는 @ref by::chunk "chunk"들을 추가하거나 삭제하므로, @ref by::chunk "chunk"가 각 셀마다 고정된 크기만을 사용하기 때문에 @ref by::chunks "chunks"또한
고정된 크기의 메모리만 할당할 수 있습니다.
만약 length를 넘게되면 `resize()` 를 자동 수행합니다.

```
@style: language-cpp verified
// 16바이트 블록을 관리하는 chunks 생성
chunks myChunks(16);  // blockSize = 16 bytes

// 메모리 할당 - 내부적으로 적절한 chunk를 찾거나 생성
void* ptr1 = myChunks.new1();
void* ptr2 = myChunks.new1();
// ... 계속 할당하면 자동으로 새 chunk가 추가됨

// 메모리 해제
myChunks.del(ptr1, 16);
myChunks.del(ptr2, 16);

// 전체 chunks에서 할당 가능한 메모리가 없으면
// 내부적으로 새로운 chunk를 자동 생성하여 추가
```

**pool과의 연계**

최초 메모리 요청을 받는 곳은 @ref by::pool "pool"클래스입니다. 해당 객체에서 할당해야할 memory size를 받으면, 해당
memory size를 처리할 수 있는 @ref by::chunks "chunks"인스턴스를 lazy로 가져와, new1()를 요청하게 됩니다.
@ref by::chunks::new1() "new1()"에서는 메모리 할당이 가능한 @ref by::chunk "chunk"를 찾고, 없을 경우는 추가로 @ref by::chunk "chunk"를 생성합니다.

**가용 chunk 검색 알고리즘**

가장 최근에 메모리를 할당한 @ref by::chunk "chunk"가 추가로 할당 할 가능성이 가장 높습니다. 멤버변수 `_s`는 바로 최근에
할당한 @ref by::chunk "chunk"의 인덱스를 가지고 있습니다.

만약 _chunks[_s]에 가용 메모리가 없을 경우 _s를 ++ 합니다. 이후 마치 원형배열처럼, _chunks의 끝은
처음과 이어져 있다고 보면 됩니다. 그래서 다시 _s가 순회직전의 _s로 값이 같아질 때까지도 가용 메모리가
없다면, @ref by::chunks "chunks"전체에 가용 메모리가 없는 상태이므로 resize()에 들어갑니다.

**vector를 쓰면 안된다**

당연한 건데, vector는 heap으로 관리되므로 자체 메모리 풀을 만든다면서 vector를 사용해서는 안됩니다.
차후 수정 예정입니다.

#### pool - 크기별 메모리 관리

@ref by::pool "pool"클래스는 외부로부터 메모리 할당 요청시 가장 최초로 처리하는 클래스입니다. 내부적으로 @ref by::chunks "chunks"에
대한 배열을 가지고 있으며, @ref by::chunks "chunks"는 @ref by::chunk "chunk"를 가지고 있으므로, 사실상 로우레벨의 메모리 관련 클래스를
모두 관리하는 셈입니다.

**pool은 할당 가능한 size 별로 lazy하게 chunks를 가진다**

자체 메모리 풀을 만들때 중요한 포인트는, 같은 사이즈의 메모리를 한 곳에 나열함으로써 속도를 높이는
것입니다. @ref by::chunks "chunks"는 블록이라는 개념이 있어서 각 블록은 미리 지정된 크기의 메모리만 할당/해제 될 수
있습니다.

@ref by::pool "pool"은 @ref by::chunks "chunks"를 만들때 블록의 크기를 고정해서 생성하며, 외부에 의해서 특정 사이즈의 메모리 할당을
요청받으면, 해당 크기의 블록을 담당하는 @ref by::chunks "chunks"를 찾습니다. 없을 경우 lazy 하게 생성합니다.

```
@style: language-cpp verified
// pool 객체를 직접 사용하는 예제
pool myPool;

// 16바이트 크기의 메모리가 필요한 경우
ncnt smallSize = 16;
chunks* smallChunks = myPool.get(smallSize);  // 16바이트용 chunks를 가져옴 (없으면 생성)

// chunks를 통해 메모리 할당
void* ptr1 = smallChunks->new1();  // 16바이트 메모리 할당
void* ptr2 = smallChunks->new1();  // 같은 chunks에서 또 할당 (빠름)

// 64바이트 크기의 메모리가 필요한 경우
ncnt mediumSize = 64;
chunks* mediumChunks = myPool.get(mediumSize);  // 64바이트용 chunks를 가져옴

void* ptr3 = mediumChunks->new1();  // 64바이트 메모리 할당

// 메모리 해제
smallChunks->del(ptr1, smallSize);   // 16바이트 메모리 반환
smallChunks->del(ptr2, smallSize);
mediumChunks->del(ptr3, mediumSize); // 64바이트 메모리 반환

// 같은 크기를 다시 요청하면 기존 chunks를 재사용
chunks* sameChunks = myPool.get(16);  // smallChunks와 동일한 객체 반환
void* ptr4 = sameChunks->new1();      // 빠른 재할당

// 핵심:
// - pool.get(size)는 해당 크기를 담당하는 chunks를 lazy 생성
// - 같은 크기는 항상 같은 chunks에서 관리 (메모리 지역성 향상)
// - chunks 내부에서 빠른 할당/해제 수행
```

#### instancer - 메모리 관리 조정자

@ref by::instancer "instancer"클래스는 low level로 메모리를 관리하는 @ref by::pool "pool"클래스와, @ref by::instance "instance"들의 라이프사이클을
관리하는 @ref by::watcher "watcher"를 가지고 있습니다.

이 둘을 잘 제어해서 인스턴스의 생명 관리(할당/소멸)를 하는 것이 목적입니다. 사실상 @ref memlite 에서 핵심
작업을 수행하기 위해 각 제어클래스들에게 작업을 분배하거나 명령을 내리는 진입점을 담당합니다.

```
@style: language-cpp verified
// instancer를 통한 인스턴스 생성
class MyClass : public instance {
public:
    int value;
    MyClass(int v) : value(v) {}
};

// 인스턴스 생성 - instancer가 자동으로 관여
MyClass* obj = new MyClass(42);
// 내부적으로:
// 1. instancer가 pool에게 메모리 요청
// 2. watcher가 life 객체 할당하여 참조 카운팅 준비
// 3. id 부여

// binder를 통한 바인딩
tstr<MyClass> ptr(obj);
// 내부적으로:
// 1. watcher의 life가 strong count 증가
// 2. 참조 카운팅 시작

// 소멸
ptr.rel();  // 또는 ptr이 스코프를 벗어남
// 내부적으로:
// 1. watcher의 life가 strong count 감소
// 2. count가 0이 되면 instancer가 pool에게 메모리 반환 요청
// 3. 소멸자 호출 후 메모리 블록 반환
```

### 인스턴스 생명주기 관리

#### instance 클래스

@ref by::instance "instance"클래스는 @ref memlite 모듈의 자체 memory pool에 의해서 관리되는 객체의 기반 클래스입니다.
@ref by::instance "instance"클래스를 상속해야만 @ref by::binder "binder를 통해 weak pointer나 strong pointer로 참조 할 수 있습니다.
@ref by::instance "instance"의 식별은 `id`를 통해서 이뤄집니다.

**id 부여 알고리즘**

@ref memlite 에서 가장 취약한 부분을 고르라면 바로 이 id 부여 알고리즘입니다. 인스턴스 생성은 memory pool을
관리하는 @ref by::instancer "instancer"에 의해서 이뤄집니다. 이때 @ref by::instancer "instancer"는 `vault`라고 하는 @ref by::instance "instance"내부의 클래스에
@ref by::instance "instance"주소와 id를 map에 push 합니다. instance::operator new()가 불리면 안쪽에서는 vault에게
`map[this]`와 같은 코드로 id값을 가져오는 방식입니다.

얼핏 괜찮아 보이지만 단점이 많습니다:

1. 생성자 단계에서 vault에 접근해서 값을 가져오는 방식 자체가 안전하다고 보긴 어렵습니다.
2. 느립니다.

최초 구현은 vector로만 되어있었으며 FIFO로 관리했었으나, 생성자 안에서 다른 객체를 생성하는 경우에는
추가되는 id의 순서가 FIFO가 아니게 되면서 ID가 꼬이는 문제가 있었습니다.

**속도에 있어서 instance 클래스의 중요성**

byeol에서 가장 빈번히 하는 작업은 객체를 생성하면서 id를 부여하거나 binding을 하는 작업입니다. 이 부분은
개선 예정이며, 더 나은 알고리즘에 대한 아이디어를 환영합니다.

#### id 클래스

@ref by::id "id"클래스는 64bit integer로 되어있는 @ref by::instance "instance"식별자입니다. tagN은 @ref by::life "life"를 식별하며, chkN은
몇번째 @ref by::chunk "chunk"인지를 나타내며 serial은 객체 검증에 사용됩니다.

**serial은 프로세스 실행 도중 instance 객체의 생성횟수다**

@ref by::pool "pool"과 @ref by::chunk "chunk"를 먼저 봤다면 알겠지만, 자체 메모리 풀을 사용하기 때문에 메모리가 해제 될때는 소멸자만
호출할 뿐, 모든 메모리를 초기화 하지 않습니다.

그러니 이전에 할당해서 사용후 소멸된 데이터가 그대로 남아있으며, 심지어 이 데이터에 접근도 가능합니다.
(이미 사용한 데이터에 접근시 exception이나 UB가 된다면 weak pointer나 strong pointer를 구현한 @ref by::binder "binder"를
구현할 수 없었을 것입니다)

@ref by::binder "binder"에서는 이렇게 해서 가져온 데이터가 정말로 유효한 데이터인지 구분하기 위해서 serial을 추가로
비교합니다.

**tagN은 life 객체에 접근할때 사용한다**

@ref by::watcher "watcher"는 자신의 배열에서 tagN 번째 @ref by::life "life"객체를 가져올때 이 값을 사용합니다.

**chkN은 chunk 객체를 가져올 때 사용한다**

@ref by::pool "pool"은 먼저 id와 매핑된 @ref by::instance "instance"의 size를 계산해 @ref by::chunks "chunks"를 가져옵니다. 그리고 @ref by::chunks "chunks"는 자신의 chkN
번째 원소인 메모리블록을 반환합니다. 외부에서는 전달 받은 메모리 주소와 serial 값을 비교해서 같은
인스턴스인지를 검증합니다.

#### life 클래스

@ref by::pool "pool"클래스가 로우레벨 관점에서 블록 단위로 메모리를 관리하는 클래스라면, @ref by::watcher "watcher"컴포넌트는 각
블록의 정보를 유기적으로 관리하는 클래스입니다.

@ref by::life "life"는 @ref by::pool "pool"에 할당되어있는 주소값(_pt)와 reference counting을 위한 값들을 갖습니다. _strong은
reference counting을 위한 값이며, _pt는 @ref by::pool "pool"에 할당받은 인스턴스를 직접 가리킵니다. _id는 객체를
식별하기 위한 값으로 자세한 내용은 @ref by::id "id"를 참고하세요.

#### watcher 클래스

@ref by::watcher "watcher"클래스는 메모리 관리의 한 축을 담당하는 클래스로, 생성된 객체의 라이프사이클을 관리합니다.
@ref by::instance "instance"가 생성될때마다 @ref by::watcher "watcher"는 @ref by::life "life"객체를 추가로 할당해 reference counting으로 객체의
소멸시점을 판별합니다.

**reference counting**

@ref by::binder "binder"에 의해서 @ref by::instance "instance"가 바인딩 될때마다 @ref by::life "life"가 count 하는 strong 값을 1 증가시킵니다. @ref by::binder "binder"가
@ref by::instance "instance"를 rel() 할때 count를 1 감소하며, 0이 되는 순간 delete로 메모리에서 해제합니다. @ref by::instance "instance"는
`operator delete()`를 통해 @ref by::instancer "instancer"에게 메모리 해제 작업을 실행하도록 합니다.


### 메모리 관리 인터페이스

#### memoryHaver 클래스

@ref by::memoryHaver "memoryHaver"클래스는 memory pool에서 일정 메모리를 직접 혹은 간접적으로 소유하고 있으며, 그 메모리를
READ 가능한 클래스들의 기본 인터페이스를 정의합니다. 그래서 메모리의 크기나, 상태 등을 알 수 있는
인터페이스로 정의되어 있습니다.

**간접적으로 소유하다?**

해당 객체가 직접 메모리를 할당받아 사용하는 것이 아니라, 내부에 멤버변수로 있는 다른 객체들이 담당하는
경우가 있습니다. 그리고 메모리의 할당은 내부 멤버변수들을 통해 직접해야 한다면, 그 클래스는 @ref by::memoryHaver "memoryHaver"만
상속받아야 합니다. 만약 할당도 가능하다면 @ref by::allocator "allocator"를 상속하면 됩니다.

**len과 size**

할당 가능한 메모리의 크기는 size로 표현하며, 그 중에서 할당한 메모리는 len으로 표현됩니다. void* 및 byte
단위로만 제어하는 것을 전제로 합니다.

@ref by::memoryHaver "memoryHaver"의 파생클래스들은 자신들이 담당하는 메모리의 사이즈가 제각기 다르다는 것에 주의하세요.

#### allocator 클래스

@ref by::allocator "allocator"클래스는 @ref by::memoryHaver "memoryHaver"를 상속하고 있다는 점에서 알다시피 관리하는 메모리의 상태나 크기를
측정할 수 있으면서, 추가적으로 메모리를 할당/소멸 할 수 있는 클래스입니다.

모든 메모리는 void* 및 바이트 관점에서만 바라본다는 @ref memlite 컨셉에 맞게, new(), del()의 파라메터는
void*만 제공합니다.

**memlite 전용의 공통 인터페이스의 네이밍 컨벤션**

할당은 new1() (new one 이라는 뜻입니다.), 해제는 del()를 사용합니다. 이 네이밍은 @ref memlite 뿐만 아니라
byeol 프로젝트 내부에서 자주 사용됩니다.


## stela 모듈 - 경량 설정 언어

@ref stela 모듈은 byeol 언어의 경량화된 버전으로, manifest나 옵션과 같은 특수 목적용 언어입니다. 트리 구조로
설정 데이터를 표현하며, @ref core 모듈의 @ref by::node "node"와 유사한 구조를 가집니다.

**참고**: @ref stela 언어는 byeol 언어보다 덜 복잡하므로, **@ref core 모듈의 byeol 파서를 보기 전에 @ref stela 파서
코드를 먼저 살펴볼 것을 권장합니다**. 파서의 기본 구조와 동작 방식을 이해하는데 더 적합합니다.

### stela 언어의 기본 기능

#### stela 클래스

@ref by::stela "stela"클래스는 @ref stela 모듈의 가장 기본 단위가 되는 클래스로, 다음의 기능을 제공합니다:

1. **타입 변환 API**: asInt(), asChar(), asStr(), asBool() 등 기본 타입으로 변환을 시도합니다.
2. **값이 있는 경우**: @ref by::valStela "valStela"의 API가 실행되며, 적절한 값으로 변환됩니다. 예를들어 `verStela(22)`의
   경우 `asStr()`을 하면 `std::string("22")`가 반환됩니다.
3. **값이 없는 경우**: @ref by::nulStela "nulStela"로 표현됩니다. 이 경우 어떠한 타입변환 시도에도 기본값(빈 문자열 혹은
   0)이 반환됩니다.
4. **버전 타입**: major, minor, patch 버전을 가지고 있으며, 범위로도 표현이 가능합니다.
5. **트리 구조**: @ref by::stela "stela"는 또다른 @ref by::stela "stela"를 자식으로 둘 수 있습니다. 각 @ref by::stela "stela"객체마다 이름이 존재하므로,
   자식을 찾을 때는 이름으로 검색하거나 순회합니다. 주어진 이름에 맞는 자식이 없는 경우 @ref by::nulStela "nulStela"가
   반환됩니다.

**사용 예제**

```
@style: language-cpp verified
const std::string script = R"SRC(
    def man
        dummy := 5
        name := "dark souls"
        ver := 1.0.8
)SRC";

root = stelaParser().parse(script);
ASSERT_TRUE(root);

stela& man = root->sub("man");
stela& name = man["name"];
ASSERT_TRUE(name);

ASSERT_STREQ(name.asStr().c_str(), "dark souls");

verStela& ver = man["ver"].cast<verStela>() OR_ASSERT(ver);
ASSERT_STREQ(ver.asStr().c_str(), "1.0.8");
ASSERT_EQ(ver.asMajor(), 1);
ASSERT_EQ(ver.asMinor(), 0);
ASSERT_EQ(ver.asFix(), 8);
```

#### nulStela 클래스 - Null Object 패턴

@ref by::nulStela "nulStela"는 @ref by::stela "stela"로 자식 객체를 가져올 때 보통 이름으로 가져오는데, 해당 이름을 가진 자식 객체가 없을
경우 대신 반환됩니다.

@ref by::nulStela "nulStela"는 <b>null object 패턴</b>을 구현한 것으로, 해당 객체에 대해 값 변환을 요청할 경우 항상 해당
타입의 기본값이 반환됩니다. @ref by::stela "stela"객체가 @ref by::nulStela "nulStela"인지 확인하려면 `isExist()` 혹은
`operator bool()`이 false인지 확인하면 됩니다.

**사용 예제**

```
@style: language-cpp verified
stela& config = root->sub("config");
stela& device = config["device"];
stela& notExist = config["notExistKey"];  // nulStela 반환

// Null Object 패턴으로 안전하게 처리
if(notExist) {  // operator bool() - false 반환
    // 이 블록은 실행되지 않음
}

// 또는
if(!notExist.isExist()) {  // true
    std::cout << "Key does not exist" << std::endl;
}

// 값 변환 시도 - 기본값 반환
std::string str = notExist.asStr();  // "" (빈 문자열)
int val = notExist.asInt();          // 0
```

#### valStela 클래스 - Scalar 값 표현

@ref by::valStela "valStela"는 @ref by::nulStela "nulStela"와 달리 int, float, string 등 <b>scalar 타입</b>을 가지고 있는 @ref by::stela "stela"입니다.
내부적으로는 기본 문자열로 값을 가지고 있는 상태이며, `asInt()`와 같은 타입변환 요청에 따라서 적절한
타입으로 변경할 수 있습니다.

만약 타입변환에 실패한다면, 예외가 발생하니 주의하세요.

#### verStela 클래스 - 버전 타입

@ref by::verStela "verStela"는 @ref by::valStela "valStela"와 비슷하게 <b>version</b>이라는 타입의 값을 가지고 있는 @ref by::stela "stela"입니다.

**version 타입**

@ref by::stela "stela"언어는 byeol 언어의 경량화된 언어로, manifest나 옵션과 같은 특수 목적용 언어입니다. version
타입은 `major.minor.fix`의 3가지 변수를 가지고 있으며 범위 표현 또한 가능한 타입입니다.

**사용 예제**

```
@style: language-cpp verified
const std::string script = R"SRC(
    def package
        name := "mylib"
        version := 2.1.5
        minVersion := 1.0.0
        maxVersion := 3.0.0
)SRC";

stela& root = stelaParser().parse(script);
stela& pkg = root->sub("package");

// 버전 정보 추출
verStela& ver = pkg["version"].cast<verStela>();
ASSERT_EQ(ver.asMajor(), 2);
ASSERT_EQ(ver.asMinor(), 1);
ASSERT_EQ(ver.asFix(), 5);
ASSERT_STREQ(ver.asStr().c_str(), "2.1.5");

// 버전 범위 체크 (범위 표현 가능)
verStela& minVer = pkg["minVersion"].cast<verStela>();
verStela& maxVer = pkg["maxVersion"].cast<verStela>();
// 1.0.0 <= 2.1.5 <= 3.0.0 범위 확인 가능
```

### stela 파서 구조

#### stelaParser 클래스 - 파싱 진입점

@ref by::stelaParser "stelaParser"클래스는 stela 파싱 컴포넌트의 진입점 역할을 합니다. `parse()` 나 `parseFromFile()`을
통해서 스크립트를 지정하면 파싱된 결과가 @ref by::stela "stela"구조로 반환됩니다.

**byeol 파서와 유사한 구조**

@ref stela 언어 자체가 byeol의 특화된 언어이므로 파서 또한 byeol 언어의 파서를 기반으로 하고 있습니다.
byeol 파서 대비 덜 복잡하여 파서의 기본 동작을 이해하기 좋습니다.

**scanner - bison - stelaParser 구조**

flex와 bison을 사용하고 있으며 flex는 lowscanner로, bison은 lowparser로 각각 명명합니다. 이
lowlevel scanner, parser는 parser 컴포넌트 안에만 존재하는 것으로 외부에서는 일절 노출 되지 않습니다.

@ref by::stelaParser::parse() "parse()" 가 실행되면 lowscanner를 실행시키고, lowscanner는 토큰을 뜯어서 lowparser에게
넘기고, lowparser는 받은 토큰에 대해 rule이 match 되면 그 이벤트를 다시 @ref by::stelaParser "stelaParser"에게 넘깁니다.

그러므로 @ref by::stelaParser "stelaParser"의 <b>`on`으로 시작하는 함수들</b>은 그러한 이벤트를 handling 하는 함수로, 실제로
어떻게 node를 생성해서 ast를 구축하는지를 정의합니다.

### Indentation 처리

#### indentation rule

stela 언어는 byeol 언어와 마찬가지로 <b>offside rule</b>을 적용하므로, indentation에 매우 민감합니다.
일반적인 언어와 달리, 개행직후로 몇개의 공백이 있는가를 count 할 수 있어야 하며, indentation이
확정되어 해당 코드라인이 어느 scope에 속한 것인지가 확정되면 이후로는 공백을 무시해야 합니다.

다음 byeol 언어의 예제를 보고, 이 문제에 대해 더 생각해봅시다:

```
@style: language-byeol verified
def A
    foo(val int) void
        if val > 0
          if val < 5
                  print("0 < val < 5")
        print("end of func")  // 이 라인의 indentation 판단이 중요
```

파서가 `print("0 < val < 5")`를 파싱하고 나서 다음 줄인 `print("end of func")`를 파싱할때 앞에
공백이 몇개 있는지 세야 합니다. 그리고 지금까지 각 scope별 공백 갯수와 비교해서 해당 코드라인이 어느
scope에 속한 것인지를 판단해야 합니다.

위 예제를 보면, 우리는 직관적으로 `print("end of func")`이 `if val > 0`과 같은 공백을 갖고 있다는
점에서, foo() 본문 바로 안에 속한 코드라는 걸 알 수 있습니다.

그렇다는 건, 이 시점에서 `if val > 0` 안쪽의 scope와 `if val < 5` 안쪽의 scope 2개 모두 종료되었으므로
lowscanner는 lowparser가 `print("end of func")`를 인식하기 전에 scope의 종료를 의미하는 <b>`DEDENT`
토큰을 2개 먼저 인식</b> 할 수 있도록 만들어야 합니다.

예를들어 다음의 stela 코드를 파싱한다고 합시다:

```
@style: language-byeol verified
def config
    def device
      name := "my device"
      // 현재 파싱 위치
```

만약 마지막 위치에서 파서가 파싱중일때, parser의 indents 객체는
> [0, 4, 6]

으로 내부 배열 값이 구성되어 있습니다. 각각은 scope이 몇 번의 공백으로 구분되고
있는지를 나타냅니다. 즉 가장 바깥의 scope의 공백의 수는 0 이 되며, 가장 안쪽의 scope인
device의 scope임을 증명하는 공백의 수는
> smartDedent[smartDedent.len() - 1] = 6

으로 개행 직후 6개의 공백이 나와야 한다는 걸 의미합니다.

#### tokenDispatcher

flex는 yyin 이라는 별도로 지정된 stream을 통해서 글자를 가져와 token으로 정의합니다. 위의 예제를
보다시피, 파싱 도중에 토큰을 추가한다는 것은 이 stream에 특정 문자를 추가하는 것을 의미합니다.

기본적으로 이런 경우는 unput을 사용하나, 여러개를 unput 하거나 뒤가 아니라 앞에 push 하는 경우 등에
유연하게 대응하기 위해, stelaLowscanner는 내부적으로 @ref by::stelaTokenDispatcher "stelaTokenDispatcher"를 사용합니다.

**사용 예제**

```
@style: language-cpp verified
// tokenDispatcher를 통한 토큰 관리
stelaTokenDispatcher dispatcher;

// 1. DEDENT 토큰 2개를 앞에 추가해야 하는 경우
// (scope 2개가 종료되었을 때)
dispatcher.pushFront(DEDENT);
dispatcher.pushFront(DEDENT);

// 2. 현재 토큰을 다시 읽어야 하는 경우 (unput 대신)
dispatcher.pushFront(currentToken);

// 3. 여러 토큰을 순서대로 추가
dispatcher.pushFront(END_SCOPE);
dispatcher.pushFront(DEDENT);
dispatcher.pushFront(NEWLINE);

// 토큰 읽기
while(!dispatcher.isEmpty()) {
    Token token = dispatcher.pop();
    // 토큰 처리
}

// unput과의 차이:
// - unput: 한번에 하나씩만, 역순으로 처리됨
// - tokenDispatcher: 여러개를 순서대로, 앞/뒤 모두 push 가능
```

#### stelaTokenScan 클래스 - 스캔 모드 전략

@ref by::stelaParser "stelaParser"는 indentation을 정밀하게 측정하기 위해서 <b>scan mode를 동적으로 변경</b>해야 합니다.
tokenScan은 그러한 스캔 모드 전략 1개를 담당합니다.

**tokenScan의 동적 전환**

앞서서 indentation을 탐지하는 게 얼마나 중요한지 설명했습니다. 정확한 공백의 갯수를 셈해야 하기 때문에
`normalScan`과 `indentScan` 2개의 scan 객체를 전환해가며 사용합니다.

개행이 탐지되면 indentScan으로 교체해서 정확하게 공백을 count해서 scope를 결정하고, 이후에는
normalScan으로 교체해서 평상시처럼 공백을 다 무시합니다.

**indentation 검사**


**명령 token**

token 중에는 `SCAN_AGAIN`, `SCAN_EXIT` 등 scanner나 parser에 명령을 주는 토큰들이 존재합니다.
자세한 내용은 @ref by::stelaParser "stelaParser"의 zztokenType enum을 참조하세요.

**isBypass**

IndentScan의 경우 대부분의 token을 무시하며 오직 공백이 몇개인지 갯수를 세는 데 집중합니다.
그리고 indentation이 가장 최근 scope의 indentation과 차이가 발생한 경우 DEDENT 혹은
INDENT token을 dispatcher에 추가합니다.

하지만 이전 라인에서 여러 token을 push 해둔 상황이라면 내부적으로 bypass 모드로 동작합니다.
이때는 indentation 갯수를 세는 동작을 skip 하고 넣어둔 token을 그대로 읽어서 반환합니다.

**tokenScan 사용 예제**

```
@style: language-cpp verified
// tokenScan의 사용법과 교체되는 방식을 보여주기 위한 예제입니다.
// 실제 코드와는 아주 다릅니다.
class stelaParser {
    normalScan* _normalScan;
    indentScan* _indentScan;
    stelaTokenScan* _currentScan;

public:
    int getNextToken() {
        // 현재 스캔 모드에서 토큰 읽기
        return _currentScan->onScan();
    }
};

nint indentScan::onScan(parser& ps, .....) {
    // indentScan도 마찬가지로 공백을 무시한다.
    // 즉 여기에 온 tok 값은 개행후 가장 먼저 발견된 공백이 아닌 token이라는 뜻이다.
    nint tok = super::onScan(ps.....);

    // 공백 계산은 뒤에서 할 것이다. 일단은 scan을 일반모드로 교체한다.
    ps.setScan<normalScan>();

    ncnt cur = loc->start.col; // 현재 token의 column이 공백의 갯수가 된다.
    std::vector<ncnt>& ind = ps.getIndents();
    ncnt prev = ind.back(); // 가장 최근의 indentation level

    if(cur > prev) return ps.onIndent(cur, tok);
    else if(cur < prev) return ps.onDedent(cur, tok);

    return tok;
}

```

#### stelaSmartDedent 클래스 - Scope 관리

offside-rule에 의해 byeol은 개행이 있는지 여부가 매우 중요하다고 말씀드렸죠.
byeol은 표현식 기반 문법을 가지고 있기 때문에 `for`나 `if` 문이 함수 인자로
오는 것도 가능해야 합니다.
이때 사용자는 간단하게 사용하기 위해 보통은 `:`을 사용하는 inline block 문법을 사용하게 되는데,
위의 3가지가 한번에 적용되기 시작하면 예외사항이 나오기 마련이죠.

```
@style: language-byeol verified
onEvent(e event) void
foo(listener onEvent, e event) void
    e.process()
    listener(e)

main() void
    boo() void: print("") # 1) 이렇게 inline block을 만드는 건 아무런 문제가 없습니다.

    foo((e) void
        onAfterRender()
    , RENDER_UI) # 2) 이렇게 lambda를 써도 문제가 없습니다만,

    foo((e) void: onAfterRender(), RENDER_UI)
    # 3) 위처럼 inline block과 람다를 한줄에 쓰면 문제가 됩니다.
    # 왜 문제일까요?
```

함수 뒤에는 블록문이 옵니다. 블록문은 `<표현식> <개행>` 이 매치되면 구문으로 인식되고
이러한 구문들만 블록문에 들어올 수 있습니다.
1번의 경우처럼 inline block을 쓸 때도 뒤에 <개행>이 온다는 점은 유지되어야 합니다.
하지만 3번을 보세요. inline block 뒤에 콤마가 나오는데, 이 경우 inline block의 <개행>은
없습니다.
그렇다고 단순히 `inline block은 개행이 있을 수도 있고 없을 수도 있다` 라고 규칙을
정해버리면 모호한 경우가 너무 많이 생깁니다.

그래서 @ref by::stelaSmartDedent "stelaSmartDedent"가 나옵니다. 위와 같이 inline block을 블록을 사용하되, 콤마로 끝나는
경우는 개행을 추가해주는 아주 단순하지만 parser의 rule의 난이도를 낮추는 역할을 합니다.



## core 모듈 - 언어 구현의 핵심

@ref core 모듈은 Byeol 프로그래밍 언어의 핵심 구현을 담당합니다. AST(Abstract Syntax Tree) 구조, 파서, 검증기, 실행기 등 언어의 모든 핵심 기능이 이 모듈에 집중되어 있습니다.

@ref core 모듈의 가장 큰 특징은 <b>AST를 직접 실행</b>한다는 점입니다. 일반적인 언어와 달리, Byeol 언어는 AST 구조를 유지한 채로 프로그램을 실행합니다. 따라서 타 언어의 AST는 말그대로 문법 구조를 트리로 표현한 중간결과물에 지나지 않지만, Byeol에는 실행가능한 최종 output을 AST가 담당합니다.


### AST 기본 구조

Byeol의 AST는 실행 가능한 프로그램 트리입니다. 일반적인 AST와 달리 평가(evaluation), 타입 추론(type inference), 프레임 관리 등 실행에 필요한 모든 기능을 포함합니다.


#### node 클래스 - AST의 기반

@ref by::node "node"클래스는 AST의 가장 기본이 되는 클래스입니다. AST 구조와 관련된 API와 해당 @ref by::node "node"를 평가하는 `eval()`, 자식 @ref by::node "node"를 탐색하는 API, 그리고 해당 @ref by::node "node"객체를 다른 타입으로 형변환하는 API 등을 제공합니다.

**AST 탐색**

AST 특성상, @ref by::node "node"는 또 다른 @ref by::node "node"의 파생클래스의 객체도 가지고 있을 수 있어야 합니다. 때문에 마치 DOM tree처럼 composition 패턴을 사용해서 설계되어있으며 이 tree를 탐색하는 함수 또한 다양하게 지원하고 있습니다.

이 tree를 byeol 언어에서는 @ref by::scope "scope"라고 표현하고 있으며 @ref by::scope "scope"은 map을 기반으로 합니다. @ref by::blockExpr "blockExpr"같은 것은 @ref by::node "node"이면서도 내부에 statement 뭉치를 array로 가지고 있습니다. 따라서 AST는 전체적으로 보면 <b>map과 array가 혼합된 구조</b>로 구성되어 있음을 알 수 있습니다.

AST 탐색을 위해 주로 사용하는 함수는 `operator[], sub(), subs(), in(), subAll()`입니다.

```
@style: language-cpp verified
const node& root = getRoot();
root.sub("name1"); // name1이라는 이름을 가진 node를 root에서 찾는다.
                   // 이 name1이 함수인지 객체인지 모른다.
root.sub<func>("name1") // name1 이름을 가진 symbol이 함수일때만 가져온다.

// 이름이 name2이며 int 인자 하나를 받는 node를 모두 찾아서 반환한다.
tnarr found = root.subAll("name2", args(narr(*new nInt())));

const scope& subs = root.subs(); // subs는 tbicontainable의 모든 API를 활용할 수 있다.
subs.len(); // root가 몇 개의 자식 node를 가지고 있는지 반환.
```

**중복 symbol 처리**

byeol 언어는 동일한 scope내 중복 symbol을 허용하지 않습니다. 하지만 이 말은 동일한 key로 2개의 pair가 들어갈 수 없다는 뜻은 아닙니다.

함수의 경우는 이름이 같을 지라도 파라메터의 갯수나 타입이 다르면 다른 symbol이 되기 때문입니다. 따라서 단순히 string 비교만으로 중복여부를 판단할 수 없기에 @ref by::scope "scope"클래스는 map 기반이 아니라 <b>multimap 기반</b>으로 되어 있습니다.

**eval() - 평가와 실행**

@ref by::node "node"는 함수일수도 있고, 객체일 수도 있고, 표현식일 수도 있습니다. `eval(const args&)`은 이러한 @ref by::node "node"를 실행한 결과를 반환합니다. (byeol에는 클래스란 개념이 없고, 객체가 이를 대신합니다. 그러니 객체 또한 함수처럼 실행할 수 있으며, 이는 생성자가 호출되는 것과 같습니다.)

**infer() - 타입 추론**

`infer()`는 type inference를 수행합니다. 런타임의 정확한 값을 반환하는 것이 아니라 verification 단계에서 확정되는 타입을 객체로 반환합니다.

```
@style: language-cpp verified
// byeol 언어에서 `2 + 3.5` 표현하면 아래와 같다.
FBOExpr e = FBOExpr(FBOExpr::SYMBOL_ADD, *new nInt(2), *nFlt(3.5));

str infered = e.infer(); // int인 2와 flt인 3.5의 덧셈했을때의 타입은 뭐가 되어야 할까?
infered->cast<nFlt>() != nullptr; // true: 정답은 flt
```

int + flt는 type promotion에 의해 flt이 되므로 infered에는 nFlt 객체가 들어갑니다. 특정 표현식에 대해 타입을 유추하려면 해당 표현식이 가지고 있는 자식 node들에게 재귀적으로 타입유추를 호출해서 계산해야 합니다.

단, type inference는 어디까지나 타입을 빨리 아는 데 집중하므로 값의 계산은 수행하지 않습니다. 값을 정확하게 알고 싶다면 `eval()`을 해야만 합니다.

**eval() vs infer() 비교**

| 항목 | eval() | infer() |
|------|--------|---------|
| **목적** | 실제 값을 계산하고 반환 | 결과 타입을 추론하여 반환 |
| **실행 시점** | 런타임 | Verification 단계 |
| **반환값** | 실제 계산된 값 (str) | 타입 객체 (str) |
| **값 계산** | O | X |
| **성능** | 느림 (실제 연산 수행) | 빠름 (타입만 판단) |
| **사용 사례** | 프로그램 실행, 실제 결과 필요 | 타입 체크, 컴파일 타임 검증 |

예시:
```
@style: language-cpp verified
FBOExpr e = FBOExpr(FBOExpr::SYMBOL_ADD, *new nInt(2), *nFlt(3.5));

str evaluated = e.eval();  // 5.5를 담은 nFlt 객체 반환 (실제 계산 수행)
str infered = e.infer();   // nFlt 타입 객체만 반환 (값 계산 안함, 타입만 판단)
```

**타입 변환 - as()와 is()**

@ref by::node "node"는 명시적 타입 변환을 위한 `as()`와 타입 변환이 가능한지 체크하는 `is()`를 제공합니다.

```
@style: language-cpp verified
// 다음의 byeol 코드를 c++로 옮긴 것이다:
//  foo(val int) void
//      if val is flt
//          doSomething(val as flt)

void foo(const nInt& val) {
    if(val.is<nFlt>()) {
        str isFlt = val.as<nFlt>();
        nflt converted = isFlt->get();
        doSomething(converted);
    }
}
```

실제 코드에서는 WHEN 매크로를 사용해서 더 간결하게 작성됩니다:

```
@style: language-cpp verified
void foo(const nInt& val) {
    tstr<nFlt> converted = val OR.ret(); // early-return pattern
    doSomething(converted->get());
}
```

**Native vs Managed 타입 변환**

앞서 설명한 `as()`, `is()`는 byeol 언어 환경에서의 타입 변환입니다. 이와 별도로 C++ native 환경에서의 타입 변환은 `cast()`가 담당합니다.

```
@style: language-cpp verified
// 상속관계 간단 표현
class nFlt : public obj {};
class nInt : public obj {};

nInt val1;
nFlt val2;

// Native 타입 변환:
nFlt* cast1 = val1.cast<nFlt>(); // nullptr: 허용되지 않는 변환
node* isNode = val1.cast<node>(); // != nullptr: upcasting은 가능

// Managed 타입 변환:
val1.is<nFlt>(); // true: byeol 언어에서 int <-> flt의 명시적 형변환을 지원
```

이처럼 @ref core 모듈에는 native와 managed 별로 동일한 개념을 각각 구현한 경우가 종종 있습니다.
형변환이 동작하는 흐름을 알고 싶다면 @ref by::ases "ases"와 @ref by::asable "asable"을 참조하세요.

**묵시적 형변환과 명시적 형변환**

묵시적 형변환은 `impliAs()`, `impliIs()`로 제공됩니다. 이를 외부에서 직접 호출하는 일은
적습니다. 대부분은 `expr`이나 `baseFunc` 등 AST 클래스들에 의해서 호출됩니다.

참고로, byeol 사용자가 정의한 타입 변환은 당연히 묵시적 형변환에 포함되지 않습니다.
그래서 이 둘을 구분하는 것입니다.

**Side Funcs**

@ref by::node "node"는 byeol 프로젝트의 핵심 모듈 중에서도 핵심이 되는 기본 클래스이기 때문에 매우
다양한 side func을 제공합니다. 대부분의 기능이 @ref by::node "node"클래스를 통해 접근 가능하도록
설계되어 있습니다.

**Visitable Class - Visitor 패턴**

AST를 순회하는 동작 자체는 매우 다양한 목적으로 발생합니다. 단순히 `eval()`을 위해서만
아니라 디버깅 정보를 위해 AST를 출력할때도 사용됩니다.

순회 방식과 @ref by::node "node"에 대한 처리를 분리시키기 위해 visitor 패턴이 @ref by::node "node"에 반드시 적용되어야
합니다. `accept()`는 바로 이 @ref by::visitor "visitor"클래스에 의해 사용되며, 각 클래스는 헤더에
이를 위해 VISIT 매크로가 추가로 필요합니다.

**Frame Interaction**

@ref by::frame "frame"은 현재 실행 중인 scope와 안에 등록된 symbol 등을 관리합니다. @ref by::node "node"는 AST의
기본을 구성하므로 @ref by::node "node"가 객체이거나 함수일 경우, @ref by::frame "frame"에 자신이 소유한 함수나 property
등을 등록하거나 해제하는 작업을 해줘야 합니다.

`inFrame()`과 `outFrame()`이 이를 담당하며, 자세한 내용은 @ref by::frame "frame"을 참조하세요.

**Message Priority - 함수 우선순위**

byeol은 함수 오버로딩을 지원하므로 어떤 객체에는 같은 이름을 가졌으되, 파라메터가
다른 함수가 여럿 있을 수 있습니다. 어떤 함수는 인자에 대해서 정확하게 일치할 것이고,
어떤 함수는 약간의 타입변환이 필요할 지도 모릅니다.

@ref by::node "node"는 자식 @ref by::node "node"들에 대해서 주어진 args에 대해 얼마나 잘 일치하는 가를 `prioritize()`
함수를 통해 판단결과를 받습니다. 이때 각 @ref by::node "node"의 파생클래스는 args가 정확하게 일치하면
EXACT_MATCH로 반환하며, 일치하지 않으면 NO_MATCH로 반환됩니다.

정확한 판단기준이나 알고리즘은 @ref by::tprior "tprior"과 @ref by::priorType "priorType"을 참고하세요.


#### src, srcFile 클래스 - 소스 위치 정보

@ref by::srcFile "srcFile"은 AST의 출처가 되는 소스코드 파일에 대한 정보를 담습니다. @ref by::src "src"는 생성된 AST @ref by::node "node"가 어떠한 @ref by::srcFile "srcFile"안의 어느 위치에서 생성된 것인지를 담고있습니다.

이 정보는 byeol 언어의 Context 기반 REPL의 핵심 기능에 사용됩니다. @ref by::src "src"정보는 managed 환경에 국한하며, native에서 생성된 객체는 @ref by::src "src"에 더미 값이 들어갑니다.


#### args 클래스 - 함수 인자

함수나 객체 `eval()`을 위해 전달하는 인자를 표현합니다. @ref by::tnarr "narr"을 상속하므로 @ref by::tnarr "narr"의 모든 API를 사용할 수 있습니다.

**me 객체**

args는 `me` 객체가 포함되어 전달되는 게 특징입니다. 함수는 현재 런타임의 어떤 객체로부터 `eval()` 요청이 왔는지 기본적으로 알 수 없습니다. 별도로 `setMe()`를 통해 args를 사용하는 함수나 property는 어떤 객체로부터 이 args가 전달되었는지를 알 수 있습니다.

참고로, @ref by::baseObj "baseObj"는 `eval(name, args)`를 호출받으면 `setMe(this)`를 호출해 자신을 args에 넣습니다.


#### param 클래스 - 파라메터 정의

param은 말 그대로 파라메터를 표현합니다. 이름과 타입을 표현하기 위한 origin 객체에 대한 참조가 있습니다.



### 객체 모델

Byeol의 객체 모델은 native(C++)와 managed(Byeol 언어) 환경을 모두 지원하도록 설계되었습니다. 두 환경의 객체가 동일한 인터페이스로 사용될 수 있으며, 이를 통해 seamless한 상호작용이 가능합니다.


#### baseObj 클래스 - 객체의 기반

@ref by::baseObj "baseObj"는 byeol의 객체를 표현하는 데 있어서 기반 클래스입니다. @ref core 모듈은 byeol 언어로 작성된 managed 환경에서의 객체(@ref by::obj "obj")와 C++ 코드를 사용해서 작성된 native 환경에서의 객체(@ref by::baseObj "baseObj") 모두를 구분하지 않고 사용합니다.

둘은 모두 @ref by::node "node"임과 동시에 @ref by::baseObj "baseObj"일 뿐입니다. 이를 위해서 @ref by::obj "obj"를 @ref by::baseObj "baseObj"에서 상속받게 함으로써 둘을 같은 클래스 계통으로 묶었습니다. 따라서 @ref by::baseObj "baseObj"는 native 객체와 managed 객체의 **공통된 기능만** 갖습니다.

**origin 객체**

byeol 언어로 사용자가 정의한, 원본이 되는 타입을 @ref by::origin "origin"객체라고 합니다. 생성자 호출을 하면 모두 @ref by::origin "origin"객체를 복사 생성해서 만들어집니다.

**getOrigin() override**

C++ 코드로 새로운 @ref by::baseObj "baseObj"를 정의해서 byeol 코드로 사용하고 싶다면 @ref by::baseObj "baseObj"를 상속한 C++ 클래스를 만들고 `getOrigin()`을 override해서 적절한 @ref by::baseObj "baseObj"의 @ref by::origin "origin"객체를 반환하면 됩니다.

```
@style: language-cpp verified
// integer를 표현하는 nInt 클래스
const baseObj& nInt::getOrigin() const {
    // tbaseObjOrigin을 사용해서 nInt 클래스에 기반한 origin 객체를 쉽게 만들 수 있다.
    static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
    const baseObj& supers = super::getOrigin();
    return &supers == this ? org : supers;
}
```

**tbridger 활용 예시**

@ref by::tbridger "tbridger"를 사용하면 native 함수를 매우 간단하게 managed로 노출시킬 수 있습니다.
@ref by::nStr "nStr"에 좋은 예시가 있습니다:

```
@style: language-cpp verified
// 예시를 위해 실제코드에서 일부를 생략한다.
class nStr : baseObj {
    // 아래 함수들을 managed 쪽으로도 노출할 거다.
    nint len() const;
    nchar get(nidx n) const;

    // 그리고 `get`이라는 이름으로 seq를 넣으면, 위의 get(nidx)와 다른 동작을 하도록
    // managed에서만 추가로 노출해보자.
    // 예를들어 `get(1..3)`을 하면 원 문자열에 대해 index [1, 3)까지를 짤라서 반환하는
    // 함수를 추가해보자.
};

typedef nStr me;

// origin 객체를 내보내자.
const baseObj& me::getOrigin() const {
    // baseObj라는 정적타입의 origin이므로 static이 가능하다.
    static tbaseObjOrigin<me> org(
        // 여기서부터 tbridger를 사용해서 C++의 함수를 본따 managed 함수를 생성한다.
        tbridger<me>::ctor().ctor<nStr>()
            .func("len", &me::len)  // nStr::len() const는 이 한줄로 managed에서도 사용 가능
            .func<nchar, nidx>("get", &me::get)  // 타입 파라메터로 파라메터형과 반환형을 명시하면
                                                 // C++의 오버로딩 함수가 있어도 문제없이 노출 가능
            .func("get", new getSeqFunc())       // 이것은 `get`이라는 이름으로 `getSeqFunc`
                                                 // 함수를 C++로 넣는다.
            .subs());
    return org;
}


// 참고로 getSeqFunc은 아래와 같다:
//  func 부분이므로, 굳이 모든 코드를 이해 못해도 이 클래스와는 관계없다.
class getSeqFunc: public baseFunc {
    ...
    str eval(const args& a) override {
        // 여기서부터 함수의 동작 부분이다. 가장 먼저 인자에서 seq를 얻어와야 한다.
        WHEN(a.len() != 1) .ret(str());  // 사실 인자 체크가 검증되지 않으면 이 함수는 호출되지 않는다.
                                         // 그래도 혹시 모르니까.
        nStr& me = a.getMe() TO(template cast<nStr>()) OR.ret(str());
        tstr<seq> s = a[0].as<seq>() OR.ret(str());

        nint start = (*s).get().getStart().get();
        nint end = (*s).get().getEnd().get();
        return me.substr(start, end);
    }

    const ntype& getType() const override {
        // 이 함수는 seq 타입을 1개 받는다고 parameter를 정의한다.
        // 반환형은 nStr, 즉, 문자열로 정의한다.
        static mgdType inner("get", ttype<baseFunc>::get(),
            params(*new param("range", new seq(nInt(0), nInt(1)))), false, new nStr());
        return inner;
    }
    ...
};
```

이제 위 클래스를 pack으로써 노출하게 되면 byeol 코드에서도 @ref by::nStr::len() "len()" 이나 getSeqFunc을
사용할 수 있습니다! 어떻게 pack으로 내보내는지는 @ref by::packLoading "packLoading"이나 @ref by::autoslot "autoslot"등을
참조하세요.

byeol 코드에서는 다음과 같이 사용할 수 있게 됩니다:

```
@style: language-byeol verified
# nStr의 이름은 byeol 환경에서는 `str`로 변경해서 내보냈다.
foo(got str) void
    got.len()  # 5 반환. 내부적으로 nStr::len()이 호출된다
    got.get(1)  # 'e' 반환. nStr::get(nidx)는 n번째 인덱스의 문자를 반환한다.
    got.get(2..4)  # 'll' 반환. sequence를 넣었으므로 getSeqFunc 함수가 실행된다.

main() void
    foo("hello")
```

이처럼 @ref by::tbridger "tbridger"는 native로 작성된 함수를 매우 간단하게 managed로 노출시킬 수 있습니다.
좀 더 자세한 @ref by::tbridger "tbridger"사용 방법은 해당 클래스를 참조하세요.


#### obj 클래스 - Managed 객체

@ref by::obj "obj"클래스는 managed 환경에서의 객체를 표현합니다. @ref by::baseObj "baseObj"의 기능을 확장해서 managed 환경에서만 필요한 scope에 대한 처리나 shares, owns에 대한 개념을 추가합니다.

**타입으로써의 obj**

C++에서 타입은 클래스로 표현되지만 byeol은 클래스란 존재하지 않습니다. 객체와 클래스의 구분이 없으므로 **@ref by::obj "obj"자체가 타입**인 셈입니다.

```
@style: language-cpp verified
// C++:
class A {};  // 클래스 = 타입
A* a = new A();  // 객체와 클래스는 구분됨

// Byeol 코드를 C++로 표현:
//  def myObj
//      name str
//      foo() void: ...
//
//  myObj2 myObj

obj myObj = new obj(....);  // byeol에서 `def`로 만든 경우
obj* myObj2 = myObj.clone();  // byeol에서 `myObj2 myObj`로 객체를 만든 경우
```

managed 환경에서는 `def로 정의된 obj`와 `obj로부터 복제된 obj` 2가지 경로로만 생성되며, 이 둘은 모두 타입으로써 사용되므로 구분이 무의미합니다.

**shares, owns**

@ref by::baseObj "baseObj"의 경우는 C++ native 클래스 기반이므로 `new`와 생성자를 통해 만들어집니다. 하지만 byeol 코드로 작성하는 managed 환경은 다릅니다.

managed 환경에서 객체를 정의하는 것은 @ref by::origin "origin"객체로부터 객체를 복제하는 행위입니다. 이때 함수는 시스템 내 한 개만 있으면 되므로 복사할 필요가 없지만, property는 인스턴스마다 다른 값이 들어가야 하므로 복사가 되어야 합니다.

이를 효율적으로 하기 위해, 같은 타입의 @ref by::obj "obj"끼리 공유되는 부분들을 **shares**, 복사가 되는 부분들을 <b>owns</b>로 구분합니다.

obj의 clone()이 발생하면:
- shares: 원본 origin에서 참조만 가져옴
- owns: 실제로 clone 수행

**shares vs owns 예제**

```
@style: language-cpp verified
// byeol 코드:
//  def Person
//      name str       // property (인스턴스마다 다른 값) → owns
//      age int        // property → owns
//      greet() void   // 함수 (모든 인스턴스가 공유) → shares
//          print("Hello, my name is " + name)

// C++ 표현:
origin personOrigin;
personOrigin.shares().add("greet", new greetFunc());  // 함수는 shares에 추가
personOrigin.owns().add("name", new nStr(""));        // property는 owns에 추가
personOrigin.owns().add("age", new nInt(0));

// 객체 복제 시:
obj* person1 = personOrigin.clone();
person1->subs()["name"] = new nStr("Alice");  // 각 인스턴스는 자신만의 name을 가짐
person1->subs()["age"] = new nInt(25);

obj* person2 = personOrigin.clone();
person2->subs()["name"] = new nStr("Bob");    // person2도 자신만의 name
person2->subs()["age"] = new nInt(30);

// greet 함수는 두 인스턴스가 공유:
// person1->subs()["greet"] == person2->subs()["greet"]  // true (같은 함수 객체)
// 하지만 name은 각자 다름:
// person1->subs()["name"] != person2->subs()["name"]    // true (다른 문자열 객체)
```

이러한 구조 덕분에 메모리 효율성이 크게 향상됩니다. 수천 개의 인스턴스가 생성되더라도 함수는
단 하나만 메모리에 존재하게 됩니다.

**immutable type**

str, int 등 scalar 타입은 모두 immutable 타입입니다. 이는 byeol의 호출 전략이 `by object`를 따르기 때문입니다 (java, C# 등과 유사: 객체는 얇은 복사, raw 타입은 깊은 복사).
객체에 할당을 시도할 경우 이를 복사해야하는지, 아니면 참조만 변경하면 되는지는 @ref by::immutableTactic "immutableTactic"을 통해서 판단합니다.


#### origin 클래스 - 타입의 원본

byeol 언어로 사용자가 정의한, 원본이 되는 타입을 @ref by::origin "origin"객체라고 합니다. 사용자가 byeol 코드로 객체를 생성하면, 내부적으로 해당 타입의 @ref by::origin "origin"객체를 복사 생성(clone)하여 새 인스턴스를 만듭니다.

**중요: origin에 대해 meta 타입 사용 금지**

@ref by::origin "origin"객체는 @ref by::obj "obj"에서 상속받았으며 생성시점을 제외하고는 @ref by::obj "obj"타입으로써 사용되는 것을 전제로 작성되어 있습니다. 따라서 함부로 @ref meta 모듈을 사용하면 안 됩니다:

```
@style: language-cpp verified
origin* new1 = new origin(...);  // 생성시에 origin*로 참조하는 건 괜찮지만
new1->getType(); // origin 클래스의 특징을 이해하지 않은 상태에서,
                 // 이런식으로 `meta` 모듈을 사용해서 타입정보를 가져오는 건 권장하지 않는다.
                 // 자칫하면 아래와 같이 쓸 수 있기 때문이다.

new1->cast<origin>();  // nullptr 반환! 위험함
```

**왜 cast<origin>()은 nullptr를 반환하는가?**

사용자가 byeol 코드로 다음과 같이 작성했다고 가정합시다:

```
@style: language-byeol verified
def MyObj
    foo() void
        print("hello")

def YourObj
    boo() void
        doSomething()
```

`MyObj`는 AST의 일부이므로 C++에서는 `obj`의 인스턴스로 생성됩니다. 만약 별다른 추가 코드
없이 `MyObj`를 표현하는 obj 인스턴스에 `getType()`을 하면 어떻게 될까요? 당연히
`ttype<obj>()`가 나오게 됩니다.

문제는 `YourObj`를 표현하는 obj 인스턴스에 호출시에도 같은 `ttype<obj>()`를 반환할 것이라는
점입니다. 결과, 프로그램을 실행하는 `starter`는 YourObj와 MyObj를 같은 타입이라고 생각하게
됩니다.

이를 해결하기 위해 C++ @ref by::obj "obj"클래스는 멤버변수로 @ref by::mgdType "mgdType"변수를 갖습니다. @ref by::mgdType "mgdType"은
managed 환경에서의 byeol 타입에 대한 타입정보입니다. 자세한 내용은 @ref by::mgdType "mgdType"을 참조하세요.
아무튼 중요한 점은, 이 @ref by::mgdType "mgdType"은 기존 native type의 @ref meta 정보 트리의 구조에 확장되는
형태로 구성되기 때문에 `ttype<obj>`를 상속한 것으로 표현된다는 점입니다.

그래서 C++의 MyObj 객체에 대해 getType()을 하면 mgdType이 나오고, 이 타입은 부모클래스가
`obj` 타입이며 YourObj와는 다른 타입이라고 구분할 수 있게 됩니다.

만약 이런 방식을 하지 않고, getType()과 getMgdType() 2개를 구분한다면, type을 사용하는
모든 함수들에서, `node*`를 받았을때 이게 native 객체인가 managed 객체인가를 구분하고
managed에서 생성된 객체라면 getMgdType()을 호출해서 처리를 해야 했을 것입니다.

눈치가 빠른 사람이라면 이제 왜 `cast<origin>()`을 하면 안되는지 알았을지도 모릅니다.
아무런 처리를 하지 않는다면 origin 클래스의 getType()은 `ttype<origin>()`을 반환하려고
할 것입니다. 하지만 위의 예제처럼 byeol 코드의 MyObj를 표현하기 위해서 생성된 obj
인스턴스는 mgdType을 반환해야 합니다.

그리고 당연히 `ttype<origin>`은 방금 생성된 mgdType 인스턴스와 아무런 상속 관계가 아니므로
cast<origin>을 호출하면 내부에서 타입체킹에 실패하게 됩니다.

따라서 핵심은 C++의 @ref by::obj "obj"클래스를 기점으로 기존의 C++ 클래스별로 메타 타입을 제공하던
것이, C++ 인스턴스별로 메타타입을 제공하는 @ref by::mgdType "mgdType"으로 전환된다는 것입니다.

그러니 @ref by::origin "origin"객체에 대해서 함부로 @ref meta 모듈을 사용하려고 하면 안됩니다. @ref by::origin "origin"클래스의
getType()은 C++의 @ref by::origin "origin"타입과 아무런 관련이 없는 타입을 반환합니다. 하지만 이때의
@ref by::mgdType "mgdType"은 부모클래스로 `ttype<obj>`가 되도록 parser가 연관을 지어서 생성합니다.

그러니 obj 타입으로의 형변환이나 `cast<obj>()` 같은 것은 아무런 문제가 되지 않습니다.

```
@style: language-cpp verified
origin* makeOrigin() {
    mgdType t = typeMaker::make<obj>("MyObj");  // 부모가 obj, 이름은 MyObj인 타입
    return new origin(t);
}

void main() {
    auto* org = makeOrigin();
    org->setCallComplete(....);

    tstr<obj> bind1(org);  // 성공: obj는 origin의 부모 클래스
    str bind2(org);  // 성공

    obj* cast = bind2->cast<obj>();  // 성공
    cast->getType().isSub<obj>();  // true
}
```

**중요한 유의사항**

origin 자체가 obj 타입으로 사용될 것을 전제로 한 것이기에 거의 모든
public 함수는 obj 타입의 API와 동일한 것입니다. 대부분은 origin 타입으로 써야할 필요가
없습니다.

호출 자체는 아무런 C++ 컴파일러 제약이 없습니다. dynamic_cast를 사용한다면 안전하게
형변환이 가능하며, 위의 제약사항을 잘 이해하고 있다면 얼마든지 origin으로부터 getType()
호출해서 cast나 isSub등을 사용해도 상관없습니다. 요약하면, 의도를 잘 이해를 하고
사용하자는 것입니다.


#### tbaseObjOrigin 클래스 - baseObj Origin 템플릿

@ref by::baseObj "baseObj"에 대한 @ref by::origin "origin"객체를 쉽게 정의하기 위해서 사용하는 클래스 템플릿입니다.

먼저 @ref by::origin "origin"와 @ref by::baseObj "baseObj"를 보고 오는 것이 좋습니다. 구조나 컨셉은 @ref by::origin "origin"클래스와 동일합니다.
자신의 타입을 shadow하며, @ref by::baseObj "baseObj"타입으로 다뤄지도록 의도합니다.

단 사용방법에서 차이가 발생합니다. 이는 @ref by::baseObj "baseObj"클래스의 컨셉이 @ref by::obj "obj"와 다르기 때문입니다.

**obj와 달리 baseObj의 origin 객체는 static으로 존재한다**

@ref by::obj "obj"는 byeol 언어로 작성된 객체를 표현합니다. 이 중에서도 `def` 키워드로 작성된 @ref by::origin
객체는 @ref origin 클래스의 인스턴스로 표현됩니다.

반면 @ref baseObj 는 그 자체로 사용할 수 없으며 이를 상속한 C++의 클래스가 존재합니다. 둘의
차이를 잘 이해해보겠습니다.

C++ 코드로 봤을때 @ref baseObj 의 @ref origin 은 <b>정적</b>이지만 @ref obj 의 @ref origin 은 <b>동적</b>이라는
얘기입니다.

**baseObj의 origin - static으로 선언 가능**

C++ 클래스는 컴파일 타임에 정의되므로 origin 객체를 static으로 선언할 수 있습니다.
반면, 사용자가 byeol 코드로 다음과 같이 작성했다고 해보겠습니다:

```
@style: language-byeol verified
def MyObj
    foo() void
        "void
       print("hello")
```

MyObj라는 타입은 C++에서 봤을 때는 동적입니다. 런타임에 parser가 이 코드를 읽기 전까지는
`class MyObj`라는 것을 컴파일 타임에 만들 수 없습니다.

MyObj 자체를 byeol에서는 @ref by::origin "origin"객체라고 하며 이는 @ref by::origin "origin"클래스의 인스턴스로 표현됩니다:

```
@style: language-cpp verified
// parser가 런타임에 생성
origin org(typeMaker::make<obj>(name)); // name == "MyObj"
// 런타임에 동적으로 만들어지기에 static이 될 수 없다.
```


#### modifier 클래스 - 접근 제한자

타입에 속한 함수나 property에 대해 접근제한자를 표현합니다. byeol 언어에는 public/protected/override 3종류의 modifier만 존재합니다.


#### defaultCopyCtor 클래스 - 기본 복사 생성자

managed 환경에서 byeol 타입에 대해 사용자가 복사 생성자를 정의하지 않은 경우 parser에 의해 추가되는 기본 복사생성자입니다. 복사 생성자 호출 시 @ref by::args "args"의 첫 번째 인자로 복사 대상 객체가 전달되며, 이 객체의 property들을 얇은 복사를 수행합니다.

참고로 scalar는 immutable 객체이므로 얇은 복사를 시도해도 깊은 복사처럼 동작합니다.


#### immutableTactic 클래스 - Immutable 타입 처리

str, int 등 scalar타입은 모두 immutable 타입입니다. 이는 byeol의 호출 전략이 `by object`를 따르기 때문입니다.

단 이를 책임져야 하는 부분은 @ref by::node "node"나 @ref by::obj "obj"와 같이 컨테이너를 소유한 객체가 아니라 @ref by::tnmap "tnmap"과 같은 컨테이너 클래스 자체가 되어야 합니다.

@ref by::obj "obj"입장에서는 @ref by::node "node"는 그냥 @ref by::node "node"일 뿐이며, 자신이 소유한 container에 넣어야할 새로운 @ref by::node "node"가 immutable 타입인지 아닌지를 알아야할 필요가 있어서는 안됩니다. 이게 깨지는 순간 다형성도 같이 깨집니다.

그래서 객체가 들어왔는데 이를 복사해야하는지, 아니면 참조만 가리키면 되는지를 @ref by::immutableTactic "immutableTactic"을 통해서 판단합니다.

**알고리즘**

판단은 단순하게 @ref by::ntype "ntype"에 정의된 `isImmutable()` 함수로 판단합니다. scalar type을 구현한 @ref by::nInt"nIn", @ref by::nStr "nStr"등은 자신의 타입을 내보낼때 isImmutable()이 true로 나오는 타입으로 내보냅니다. immutable 타입으로 판단되면 인자를 바로 넣지 않고 `clone()`을 호출합니다.


#### manifest 클래스 - Pack 메타데이터

@ref stela 언어로 작성된, pack을 로딩하기 위한 기본정보를 담고 있는 객체입니다.
`manifest.stela` 파일로부터 @ref stela 모듈을 사용해 파싱됩니다.


#### tmock 클래스 - Proxy 객체

주어진 타입 파라메터 T에 대한 일종의 proxy 클래스입니다.

만약 생성시 `T*`로 원본이 될 인스턴스를 넣지 않으면 아무런 동작을 하지 않는 dummy 객체로
동작합니다. 최초에는 검증 시간을 최적화 하기 위해 고안되었습니다.

예를들어 @ref by::verifier "verifier"가 특정 symbol이 scope에 있는지 없는지 코드를 검증하려면 실제로 그
객체를 만들기 보다 타입 정보만 redirection하는 @ref by::tmock "tmock"을 대신 생성하기 위해서였습니다.

현재는 해당 기능은 사용되지 않으며 dummy 객체로 대신 사용중입니다.



### 표현식 (Expression) 시스템

Byeol은 대부분이 표현식으로 구성된 언어입니다. 블록문조차도 표현식이며, 마지막 줄의 evaluation 결과를 반환합니다. 이 섹션에서는 Byeol의 다양한 표현식 클래스들을 살펴봅니다.


#### FBOExpr, FUOExpr 클래스 - 연산자 표현식

`FBOExpr`는 Binary Operator 표현식을 담당합니다. lhs, rhs에 각각 피연산자 표현식이 들어가며, 둘은 모두 scalar 타입이어야 합니다.

`FUOExpr`는 Unary Operator 표현식을 담당합니다. args에 피연산자가 1개 들어가며 scalar 타입이어야 합니다.


#### assignExpr 클래스 - 할당 표현식

할당 표현식을 담당합니다.

**중요한 주의사항:** 이 표현식은 <b>scope에 등록된 참조를 바꾸는 것</b>이지 객체 자체에 대해 `operator=()`를 호출하는 것이 아닙니다. `obj::operator=()`를 호출하게 되면 일종의 깊은 복사처럼 동작하니 주의해야 합니다.

**예제로 이해하기**

```
@style: language-cpp verified
// byeol 코드:
//   a := 5
//   a = 10

// C++로 표현하면:
obj* aObj = new nInt(5);
obj* newObj = new nInt(10);

scope["a"] = aObj;  // := 연산자: scope에 새로운 참조 등록
scope["a"] = newObj;  // = 연산자: scope의 참조를 다른 객체로 변경
                      // 이것은 aObj 자체를 수정하는 게 아니라
                      // scope에 등록된 "a"라는 이름의 참조를 newObj로 바꾸는 것

// 만약 obj::operator=()를 호출했다면:
// *aObj = *newObj;  // 이렇게 되어 깊은 복사가 발생
// 하지만 byeol의 assignExpr은 이렇게 동작하지 않는다!
```


#### blockExpr 클래스 - 블록 표현식

블록 표현식을 담당합니다. @ref by::frameInteractable "frameInteractable"하며, 외부에서 @ref by::frame "frame"에 등록을 요청하면 scope를 하나 생성해 등록합니다. 이 scope는 이 블록문에서만 유효한 것으로, 흔히들 말하는 local scope입니다.

**최적화:** @ref by::blockExpr "blockExpr"은 `eval()` 함수 안에서 local scope를 생성하지 않습니다. 이는 특정 context에서는 불필요한 scope 생성을 막아 최적화하려는 것입니다.

**Block문은 표현식이다**

byeol 언어는 대부분 표현식으로 구성되며, block문도 예외가 아닙니다. block문은 <b>마지막 줄의 evaluation 결과를 반환</b>합니다.


#### defArrayExpr 클래스 - 배열 리터럴

배열 리터럴 표현식을 담당합니다. 어떠한 타입의 배열인지는 배열의 원소로 적은 리터럴 상수들의 타입들을 type promotion을 통해 추론됩니다. Type promotion은 여러 타입 중 가장 넓은 범위의 타입으로 승격하는 과정입니다 (예: `[1, 2.5, 3]`이라는 배열이 있다면 int와 flt 타입이 섞여있으므로 flt 배열로 승격됩니다).

@ref by::tnarr "tnarr"은 native 환경에서 배열을 담당하는 클래스입니다. @ref by::arr "arr"은 @ref by::tnarr "tnarr"을 managed 환경에 맞게 확장한 것입니다. 이렇게 함으로써 C++에서도 byeol에서도 서로 유사한 API를 사용 가능하게 됩니다.


#### defNestedFuncExpr 클래스 - 중첩 함수 정의

byeol 언어의 중첩 함수인 @ref by::nestedFunc "nestedFunc"을 생성합니다. 중첩함수는 정의할때는 이름을 생략할 수 있으며, `closure`로도 활용될 수 있습니다.


#### defSeqExpr 클래스 - 시퀀스 표현식

시퀀스(범위) 표현식을 담당합니다.

@ref by::nseq "nseq"은 native 환경에서 시퀀스를 담당하는 클래스입니다. @ref by::seq "seq"은 @ref by::nseq "nseq"를 managed 환경에 맞게 확장한 것입니다. 이렇게 함으로써 C++에서도 byeol에서도 서로 유사한 API를 사용 가능하게 됩니다.


#### endExpr 클래스 - End 키워드

`end` 키워드를 구현합니다. endExpr은 단순히 블록문을 가지고 있을 뿐입니다. `func`이 end를 실행시킵니다.


#### retStateExpr 클래스 - 블록 종료 키워드

`blockExpr`에서 벗어나는 키워드들의 기반클래스입니다.

**블록문을 바로 종료하는 알고리즘**

블록문 안에서 `ret` 등을 하여 바로 블록문을 종료해야 하는 경우, `thread`에 `setRet()`로 값을 넣어야 합니다. 각 blockExpr은 구문을 한줄 한줄 실행하다가 thread에 setRet()에 일정한 값이 발견될 경우, 모든 동작을 중단하고 해당 값을 블록문의 호출자에게 올려보냅니다.



### 컨테이너 시스템

Byeol의 컨테이너 시스템은 native(C++)와 managed(Byeol 언어) 환경에서 동일한 API를 제공하도록 설계되었습니다. 특히 `tnchain`은 AST 데이터를 보관하는 가장 중요한 컨테이너입니다.


#### tucontainable, tbicontainable 클래스 - 컨테이너 인터페이스

byeol 언어에는 기본적으로 array, map, seq를 지원합니다.

`tucontainable` 클래스는 array와 같은 **index를 입력받아 임의의 타입 T를 반환**하는 container 클래스의 기본 인터페이스를 정의합니다.

`tbicontainable` 클래스는 map과 같은 **key를 입력받아 임의의 타입 T를 반환**하는 container 클래스의 기본 인터페이스를 정의합니다.

**Native 타입을 managed 타입이 상속한다**

byeol 언어는 AST를 그대로 프로그램 실행으로 이용하는 구조이기 때문에 C++ native 클래스를 그대로 managed 타입으로 노출시키면, native API와 managed API가 동일해지므로 이점이 있습니다.

이를 원활하게 하기 위해서, 기본 인터페이스를 tucontainable로 정의하고, 이걸 구현한 C++ native 클래스를 정의합니다. byeol에 사용할 managed 타입은 이 native 클래스를 상속한 후, bridger를 사용해 쉽게 노출시키는 전략을 사용합니다.

**Native 타입에는 이름 앞에 `n`이 붙는다**

`n`은 `native`의 첫글자를 딴 것입니다. 컨테이너 클래스는 native 클래스와 그걸 상속한 managed 클래스가 나오게 됩니다. native 클래스는 임의의 타입에 대해서도 동작해야 하므로 클래스 템플릿으로 정의됩니다.

그래서 native 환경에서 사용하는 `tnarr`과 managed 환경에서 사용할 `arr`과 같이 하나의 컨테이너에 대해 2벌씩 클래스가 나옵니다.

**`t` prefix**

컨벤션 규칙에 따라, 클래스 템플릿인 경우는 prefix `t`가 붙습니다. 그리고 `t` 빠진 타입을 typedef로 선언하는 경우가 있는데 이는 해당 클래스 템플릿의 기본 파라메터를 넣은 것을 의미합니다.

예를들어 클래스 템플릿인 `tnarr`의 기본 타입 파라메터인 `node`를 넣은걸 `narr`이라고 합니다. `tstr`도 마찬가지로 `tstr<node>`를 `str`이라고 선언하고 있습니다.

**컨테이너 기본 제공 API**

모든 컨테이너는 다음 API를 제공합니다:
- 원소 추가: `add()`
- 삭제: `del()`
- 반복자: `begin(), end(), iterate()`
- 역방향 반복자: `rend(), rbegin(), riterate()`
- 원소 대체: `set()`
- 존재 검사: `in()`

**R과 RSquare**

tucontainable은 본래 index에 대해 타입파라메터 T를 원소로 반환하는 컨테이너입니다.
그러니 타입 파라메터가 T만 있어도 충분할거라 생각하기 쉽지만 실제로는 R과 RSquare라는
타입을 추가로 갖고 있습니다.

- **R**: Reference의 약자로 대부분의 타입 T에 대한 참조를 내보내야 하는 경우에 사용됩니다.
  일반적인 경우는 `T*`가 될 것입니다.

- **RSquare**: non nullable 참조자를 내보내는 경우에 사용되는 타입입니다. 만약 R이 non
  nullable이라면 R이 그대로 사용됩니다. 일반적인 경우에는 `T&`가 될 것입니다.

이렇게 2가지 타입을 추가로 정의하는 이유는 `nseq` 때문입니다. nseq는 그 기본 형태가
index를 기반으로 하는 int 배열과 유사합니다.

다만 반환 타입은 참조가 될 수 없습니다. seq의 범위가 몇 억에 가깝다면 사용하지도 않을
몇 억개의 int 원소를 다 가지고 있을 필요가 없습니다.

그래서 nseq는 99% API가 ucontainable과 동일하나 반환형이 `nInt*`가 아니라 `nInt` 여야
한다는 차이만 있을 뿐입니다.


#### tnchain 클래스 - Chain으로 연결되는 컨테이너

`node`가 AST의 근간을 이루는 가장 중요한 클래스라고 한다면, <b>tnchain은 AST의 데이터를 보관하는 컨테이너로써 가장 중요한 클래스</b>라 할 수 있습니다.

**목적**

tnchain은 말그대로 <b>컨테이너를 chain하면서 관리</b>하는 클래스로, 여러 원소들을 하나의 그룹으로 묶어서 참조하거나, 순회하거나, 다른 곳에 있는 컨테이너를 여기에 참조만 chain해서 겉으로는 마치 하나의 flatten된 map처럼 보이도록 하는 기능을 가지고 있습니다.

쉽게 비유하면 <b>linked list의 각 node를 배열로 구성한 컨테이너</b>를 떠올리면 좋습니다.

**tbicontainable로도 대부분의 기능을 사용할 수 있다**

tnchain의 가장 중요한 컨셉으로, 외부 사용자는 bicontainer를 하나 받았을 뿐, 이것이 실체가 `tnmap`인지, tnchain인지는 몰라도 상관없어야 합니다.

물론 이것은 tnchain을 bicontainable로써 `add(), del(), len()` 등을 할때의 얘기이며, `link()`와 같이 tnchain의 고유 기능을 사용하려면 tnchain이라는 걸 알아야만 합니다.

**link() - tnchain의 존재의의**

tnchain은 내부적으로는 타입 파라메터로 선언한 defaultContainer를 한개 가지고 있으며, next, prev라고 하는 tnchain에 대한 참조를 갖는 일종의 linked list의 node와 비슷한 구조로 되어 있습니다.

`link(const tnchain<T>&)`로 새로운 chain 객체를 넘기면 this의 container의 next에 새로운 chain이 연결됩니다. 이때 탐색을 하면 tnchain은 자신의 defaultContainer를 모두 탐색한 후 바로 다음 next chain의 defaultContainer로 순회합니다.

따라서 **외부에서 봤을때는 this container에 next chain의 원소들이 복사되어 들어간 것처럼** 코드를 작성할 수 있습니다.

```
@style: language-cpp verified
nchain chn1;  // defaultContainer를 명시하지 않으면 tnmap이 사용됨
chn1.add("0", new myNode(0));
chn1.add("1", new myNode(1));
// chn1은 {0, 1} 상태

nchain chn2;
chn2.add("6", new myNode(6));
chn2.add("5", new myNode(5));
// chn2는 {6, 5} 상태

nchain chn3;
chn3.add("2", new myNode(2));
chn3.add("3", new myNode(3));
// chn3은 {2, 3} 상태

chn1.getNext();  // nullptr, 아직 연결 안됨
chn1.link(chn2);  // chn1 -> chn2
chn2.link(chn3);  // chn1 -> chn2 -> chn3
```

이제 chn1을 순회하면 {0, 1, 6, 5, 2, 3} 순서로 모든 원소에 접근할 수 있습니다. 하지만 **실제로는 어떠한 복사도 일어나지 않았습니다**. 단지 참조를 chain으로 연결했을 뿐입니다.

**순회 예제**

```
@style: language-cpp verified
// chn1을 순회하면 chain으로 연결된 모든 원소에 접근 가능
for(auto& pair : chn1) {
    std::cout << pair.first << ": " << pair.second->getValue() << std::endl;
}
// 출력:
// 0: 0
// 1: 1
// 6: 6
// 5: 5
// 2: 2
// 3: 3

// len()도 chain 전체를 고려
chn1.len();  // 6 반환 (chn1의 2개 + chn2의 2개 + chn3의 2개)

// 특정 key 검색도 chain 전체에서 수행
chn1["2"];  // chn3에 있는 원소를 반환
```


#### arr 클래스 - Managed 배열

**Native API를 byeol 환경에서도 그대로 사용**

byeol 언어는 AST를 그대로 프로그램 실행으로 이용하는 구조이기 때문에 C++ native 클래스를 그대로 managed 타입으로 노출시키면, native API와 managed API가 동일해지므로 이점이 있습니다.

노출할때는 `tbridger`를 사용해서 몇 줄만으로도 간단하게 노출시킬 수 있습니다.


#### nseq 클래스 - 범위형 컨테이너

`seq`의 기반이 되는 `ucontainable`를 구현한 클래스로, int를 받아서 적절한 int를 반환하는 범위형 컨테이너입니다.

예를들어 byeol 코드로 `3..5`라고 정의한 seq가 있다면 해당 seq[0]은 3, seq.len()은 2가 나오게 됩니다.


#### smultimap 클래스 - 삽입 순서를 기억하는 Multimap

byeol에서는 AST에서 node를 구성할때 단순하게 map을 사용할 순 없으며 multimap을 사용해야 합니다. (함수 오버로딩 때문입니다. 자세한 내용은 `node`를 참조하세요.)

다만 여기서 중요한 점은 원소의 <b>삽입 순서가 scope의 검색 우선순위를 결정</b>한다는 것입니다. scope에서 같은 이름의 여러 심볼(예: 오버로딩된 함수들)을 찾을 때, 먼저 삽입된 것을 우선적으로 검색하고 매칭을 시도합니다. 따라서 삽입된 순서를 기억하고 유지할 필요가 있어 삽입 순서를 기억하는 multimap을 별도로 구현해 사용하고 있습니다.

**API는 STL 라이브러리를 최대한 비슷하게 구성**

이는 `tnarr`또한 내부적으로는 vector를 사용하므로 같은 상황에 같은 컨벤션이 되도록 의도적으로 구성한 것입니다.

**삽입 순서를 기억해야 하는 또 다른 이유: Closure의 Scope Capture**

중첩 함수가 closure로 export될 때, 그 시점의 local scope를 capture하고 유지해야 하는 요구사항이 있습니다. 하지만 capture 시점 이후에 발생한 local 변수들은 중첩 함수와 아무런 관련이 없어야 합니다.

이러한 이유로 closure는 전체 scope를 capture하지 않고, closure가 생성되는 시점까지의 local scope에 속한 변수들만 capture합니다. 이를 구현하려면 scope의 얕은 복사가 필요하다고 생각하기 쉽지만, `tnchain`과 `link()`를 사용해서 복사 없이 이 문제를 해결합니다.

하지만 이를 위해서는 tnchain이 반복자가 삽입 순서대로 순회하는 것을 보장해야 하므로, STL의 multimap을 직접 사용할 수 없어서 smultimap을 별도로 구현했습니다.



### Generic 시스템

Byeol은 C++의 템플릿과 유사한 generic 타입 시스템을 제공합니다. 하지만 C++ 템플릿과 달리, byeol의 generic은 **지연 인스턴스화(lazy instantiation)** 방식으로 동작합니다. 즉, generic 타입이 실제로 사용될 때만 해당 타입의 구체화된 인스턴스가 생성됩니다.


#### getGenericExpr 클래스 - Generic 타입 참조의 진입점

generic 클래스의 참조를 담당하며 generic 타입 생성의 진입점을 담당합니다. 사용자가 `SomeGeneric<MyObj>()`처럼 generic 타입을 사용하면, 이 표현식이 `getGenericExpr`로 표현됩니다.

내부적으로는 `genericOrigin`에게 generic 타입을 줄 것을 요청하며, genericOrigin은 해당 타입에 대한 generic이 있다면 그걸 바로 주고, 없다면 `generalizer`를 통해 generic 타입을 생성 후, 그 새로운 인스턴스를 반환합니다.

**설계는 Lazy, 실제는 Eager**

genericOrigin은 설계상 lazy instantiation을 지원합니다. 즉, `eval()` 호출 시점에 주어진 type parameter로 generic 타입을 생성합니다. 하지만 `verifier`가 프로그램 실행 전에 모든 타입을 검증하면서 필요한 generic 인스턴스를 미리 생성하므로, 실제로는 eager instantiation처럼 동작합니다.


#### genericOrigin 클래스 - Generic 타입의 생성과 관리

기본적으로 `origin`과 같은 역할을 수행하는 클래스이지만, generic을 지원한다는 점과 이 인스턴스 자체가 origin을 담당하는 게 아니라 <b>필요에 의해 origin을 생성/관리</b>한다는 점이 다릅니다.

**동작 방식**

genericOrigin은 내부적으로 map을 사용해서 타입 파라메터별로 구체화된 origin 객체들을 관리합니다. 예를 들어, `SomeGeneric<MyObj>`가 요청되면:

1. map에서 `MyObj`를 key로 찾습니다
2. 없다면, 원본 origin 객체(`_org`)를 deepClone합니다
3. `generalizer`를 사용해서 타입 파라메터 `T`를 모두 `MyObj`로 교체합니다
4. 교체된 origin을 map에 저장하고 반환합니다
5. 이후 같은 타입이 요청되면 map에서 바로 반환합니다

다음은 generic 타입이 어떻게 관리되는지 보여주는 AST 덤프 예시입니다:

```
@style: language-txt verified
SomeGeneric<T @incomplete>@21d0 
   ┣━[myObj] SomeGeneric<T myObj>@3970    // myObj로 구체화된 origin
   ┃  ┣━[0] @9068 boo() int
   ┃  ┃  ┗━[0] codes blockExpr@e190
   ┃  ┃     ┗━[0]  int = 3
   ┃  ┣━[1] @5a40 @ctor() myObj
   ┃  ┗━[2] @5c08 @ctor(rhs myObj) myObj
```

가장 중요한 부분은 첫 줄로, SomeGeneric이라는 genericOrigin 인스턴스 안에 `myObj`라는 key로 SomeGeneric<myObj>이라는 origin 객체가 들어있다는 것입니다.


#### generalizer 클래스 - Generic 타입의 구체화

`visitor` 기반으로 동작하며, generic 타입을 구체화하는 실질적인 작업을 담당합니다. 동작은 매우 단순합니다:

**AST를 순회하면서 타입 파라메터를 찾아 교체합니다.**

예를 들어, `SomeGeneric<T>`에서 `SomeGeneric<myObj>`를 만들 때:
- AST 전체를 방문하면서
- `getExpr(args = T)` 형태의 노드를 찾으면
- 이를 `getExpr(args = myObj)`로 교체합니다

이 과정이 모든 node에 걸쳐서 수행되면, 원본 generic 타입의 복사본에서 타입 파라메터가 모두 구체 타입으로 교체된 새로운 origin이 완성됩니다.

**Generic 타입 생성 흐름**

전체 흐름을 정리하면 다음과 같습니다:

1. 사용자 코드: `SomeGeneric<MyObj>().foo()`
2. `getGenericExpr`가 `genericOrigin`에게 `MyObj` 타입 요청
3. `genericOrigin`이 map에서 `MyObj` 검색
4. 없으면 원본을 deepClone하고 `generalizer` 실행
5. `generalizer`가 AST를 순회하며 `T`를 `MyObj`로 교체
6. 완성된 origin을 map에 저장하고 반환
7. 이후 같은 타입 요청시 4-5번 과정 없이 바로 반환



### Native-Managed 브리징 시스템

Byeol은 C++로 작성된 native 코드와 byeol 언어로 작성된 managed 코드가 서로 상호작용할 수 있는 bridge 시스템을 제공합니다. 이 시스템의 핵심은 <b>C++ 클래스와 함수를 간단한 선언만으로 byeol 언어에서 사용 가능하도록 노출</b>하는 것입니다.


#### tbridger 클래스 - Bridge 컴포넌트의 진입점

`tbridger`는 bridge 시스템의 facade 역할을 합니다. C++ 클래스를 타입 파라메터로 받는 클래스 템플릿이며, <b>monostate 패턴</b>으로 설계되어 있습니다.

**기본 사용법**

다음은 C++ 구조체를 byeol에서 사용 가능하도록 만드는 예시입니다:

```
@style: language-cpp verified
// C++ 코드에 이런 구조체가 있다고 가정
struct window {
    int getX() { return 5; }
    int getY() { return _y; }
    void setY(int newY) { _y = newY; }
    
    window& new1(int newY) {
        window& ret = *new window();
        ret._y = newY;
        return ret;
    }
    
    int _y;
};

struct openGL {
    int init(window* win) { return win->getY() + win->getX(); }
};

// tbridger를 사용해서 byeol에 노출
tbridger<window>::ctor()
    .ctor<window>()
    .func("new1", &window::new1)
    .func("getX", &window::getX)
    .func("getY", &window::getY)
    .func("setY", &window::setY);

tbridger<openGL>::ctor()
    .ctor<openGL>()
    .func("init", &openGL::init);

// managed 객체 생성
node* winBridge = tbridger<window>::make(new window());
node* glBridge = tbridger<openGL>::make(new openGL());

// C++ 코드에서 managed 객체 사용
winBridge->eval("setY", args(narr(*new nInt(20))));
str res = glBridge->eval("init", args(narr(*winBridge)));
res->cast<int>(); // 25
```

위 코드가 pack으로 배포되면, byeol 코드에서는 다음과 같이 사용할 수 있습니다:

```
@style: language-byeol verified
win := window()
win.setY(20)

res := openGL().init(win)  # res == 25
```

**설계 구조**

@ref by::tbridger"tbridge"는 다음 컴포넌트들의 조합으로 동작합니다:

- **tbridger**: Facade 패턴, `func()` 등으로 함수를 static subs()에 저장
- **tbridge**: @ref by::tbridger"tbridge"에 등록된 subs를 @ref by::origin"origi"으로 삼아 생성된 @ref by::baseObj "baseObj"
- **tbridgeFunc**: C++ 멤버 함수 포인터를 redirect하는 managed 함수
- **tbridgeCtor**: 생성자를 표현 (객체 생성과 생성자 호출을 모두 처리)
- **tbridgeClosure**: C++ 람다를 byeol 함수로 노출
- **tmarshaling**: native 타입과 managed 타입 간 변환 처리

**동작 흐름**

사용자가 `tbridger<window>::ctor().func("setY", &window::setY)`를 호출하면:

1. `tbridger<window>::ctor()`이 호출됩니다
2. `tbridger<T>`는 모든 public API에서 `_get()` 함수를 호출합니다
3. `_get()`은 static variable을 반환하는 싱글톤 함수입니다 (monostate 패턴)
4. `func()`는 `new tbridgeFunc<...>()`를 통해 함수 객체를 만들어 `_get().subs().add()` 합니다

이후 `winBridge->eval("setY", args(...))`를 호출하면:

1. `tbridge<window>::eval(name, args)` 호출
2. `node::eval()`이 subs에서 name과 args를 처리할 sub를 찾음
3. `window::setY`로 만들어진 `tbridgeFunc`이 매치됨
4. `baseObj::_onEvalSub()`이 args에 `setMe(this)` 주입
5. `tbridgeFunc::eval(args)` 호출
6. `tmarshaling`을 통해 `nInt` → `int` 변환 (marshaling)
7. 변환된 native 타입으로 실제 C++ 함수 포인터 실행
8. 반환값을 다시 managed 타입으로 변환해서 반환


#### tbridge 클래스 - Native 클래스의 Managed 표현

@ref by::tbridge"tbridg"는 @ref by::tbridger"tbridge"에 등록된 정보를 바탕으로 생성되는 @ref by::baseObj"baseOb"입니다. 내부적으로 `tbaseObjOrigin<tbridger<T>>` 형태로 @ref by::origin"origi"을 정의하고 있어서, @ref by::tbridger"tbridge"를 통해 채워진 subs()를 자신의 origin으로 사용합니다.

따라서 @ref by::tbridge "tbridge"객체는 @ref by::tbridger"tbridge"에 등록된 함수들을 마치 자신의 멤버 함수처럼 소유하게 됩니다.


#### tmock 클래스 - Proxy와 Dummy 객체

`tmock`은 주어진 타입 파라메터 `T`에 대한 proxy 클래스입니다. 생성시 원본 인스턴스를 넣지 않으면 아무 동작도 하지 않는 dummy 객체로 동작합니다.

원래는 `verifier`가 symbol이 scope에 있는지 검증할 때, 실제 객체를 만들지 않고 타입 정보만 redirect하는 용도로 고안되었습니다. 현재는 해당 기능은 사용되지 않으며, 주로 dummy 객체 용도로 사용됩니다.



### 스코프와 실행 컨텍스트

Byeol의 코드 실행은 @ref by::scope"scop", @ref by::frame"fram", @ref by::frames"frame", @ref by::thread "thread"가 유기적으로 협력하여 이루어집니다. 이 시스템은 symbol 탐색, 함수 호출, 실행 흐름 관리를 담당합니다.


#### scope 클래스 - Symbol 저장소

`scope`는 `tnchain`을 기반으로 설계되어 있습니다. byeol에서 scope는 만든 주체에 따라 5가지로 분류됩니다:

1. **local scope**: `blockExpr`에 의해 생성, 블록 종료시 소멸
2. **func scope**: 함수가 소유한 symbol 저장 (nested func, static variable 등)
3. **obj scope**: 객체가 소유한 함수와 변수들이 속한 scope
4. **file scope**: 소스 파일 단위의 scope, pack보다 위에 선언된 symbol
5. **pack scope**: pack에 속한 symbol들, 외부에서 접근 가능

**Scope 분류 예시**

```
@style: language-byeol verified
VERSION := "1.0"  # file scope - pack 선언 전

pack myPack

PI := 3.14  # pack scope - pack 내부

def Calculator  # obj scope 생성
    result := 0  # obj scope의 property

    add(val int) void  # func scope 생성
        temp := val * 2  # local scope - 함수 내 local 변수
        if temp > 10
            doubled := temp  # local scope - if 블록 내 local 변수
        result = result + val  # obj scope의 property 참조

main() void
    calc := Calculator()
    calc.add(5)
```

위 예시에서:
- `VERSION`: file scope
- `PI`, `Calculator`: pack scope
- `result`, `add`: obj scope (Calculator 객체의 멤버)
- `temp`: func scope 내 local scope (add 함수의 local 변수)
- `doubled`: if 블록의 local scope

**각 scope의 특징**

<b>Local scope</b>는 함수 내 블록문이 실행될 때 생성됩니다. 최적화를 위해 `blockExpr`이 직접 생성/해제하지 않고, `frameInteract`를 통해 생성됩니다:

```
@style: language-cpp verified
str me::_interactFrame(node& meObj, scope& s, nidx exN) {
    ...
    frameInteract f3(*_blk); // 여기서 local scope이 생성되어 frame에 들어감
    return _run(exN);
    // RAII로 C++ 블록 종료와 함께 local scope도 해제됨
}
```

<b>Func scope</b>는 함수가 소유한 symbol들이 저장됩니다. 주의할 점은 매 함수 호출마다 새로 생성되는 게 아니라, <b>시스템 내 유일하게 존재하는 @ref by::func "func"객체가 소유한 sub node들</b>이라는 것입니다.

<b>File scope와 pack scope</b>는 밀접한 관계가 있습니다:

```
@style: language-byeol verified
IS_DBG := false  # file scope

####### 여기서부터 pack scope #######
pack test

def yourObj
    age := 3

IS_DBG := true  # pack scope (경고: 중복 이름)

main() void
    print(IS_DBG)  # false. file scope이 pack scope보다 우선됨
```

IS_DBG는 file scope과 pack scope에 각각 1개씩 정의됩니다. 중요한 점은 <b>file scope는 @ref by::parser "parser"에 의해 항상 pack scope를 chain</b>한다는 것입니다. Symbol을 찾을 때 file scope를 먼저 검색하고, 없을 경우 pack scope를 검색하므로, file scope에 선언된 값이 우선됩니다.


#### frame 클래스 - Scope들의 동적 연결

`frame`은 현재 실행중인 코드 블록에서 접근 가능한 symbol을 관리합니다. 여러 개의 `scope`를 동적으로 chain하는 방식으로 최적화된 symbol 탐색을 제공합니다.

**Frame 생성 과정**

`baseFunc`이 `eval()` 되면 다음 순서로 frame이 구성됩니다:

1. `baseObj`: 새 frame 객체를 만들어 frames에 추가, subs()와 `me` 참조 등록
2. `obj`: baseObj 동작에 더해 현재 file scope 추가 (pack scope도 함께)
3. `func`: 자신의 subs()와 args를 frame에 등록
4. `blockExpr`: local scope으로 사용할 빈 scope 생성

이 과정을 거쳐 함수 호출시 최소 5개의 scope가 frame에 등록됩니다.

**실제 예시: yourObj.foo(10) 호출 시 frame 구조**

앞서 본 Calculator 예시에서 `calc.add(5)` 호출 시 생성되는 frame은 다음과 같은 순서로 scope가 적층됩니다:

```
@style: language-txt verified
frame (calc.add(5) 실행 중)
├─ [1] local scope (add 함수 내부 local 변수: temp 등)
├─ [2] func scope (add 함수 자체의 scope)
├─ [3] args scope (add의 인자: val=5)
├─ [4] obj scope (Calculator 객체의 멤버: result, add)
├─ [5] file scope (VERSION 등)
└─ [6] pack scope (PI, Calculator 등)
```

Symbol 탐색은 위에서 아래로 순차 진행됩니다:
- `temp` 검색: [1] local scope에서 발견
- `val` 검색: [3] args scope에서 발견
- `result` 검색: [4] obj scope에서 발견
- `PI` 검색: [6] pack scope에서 발견

**Scope 탐색의 효율성**

`frame`은 `node`를 상속하므로 `subs()`를 제공합니다. 하지만 `subs()`는 `vector<scope>`가 아니라 **여러 scope이 chain으로 연결된 하나의 scope 객체**를 반환합니다. 따라서 symbol을 찾을 때:

```
@style: language-cpp verified
// 마치 하나의 컨테이너를 순회하는 것처럼 간결함
for(auto& elem : frame.subs()) {
    // 내부적으로는 여러 scope이 연결되어 있지만
    // 코드상으로는 하나의 컨테이너처럼 보임
}
```

이는 `scope`이 `tnchain`으로 구현되어 있기 때문에 가능합니다.


#### frames 클래스 - Frame 적층 관리

`frames`는 하나의 `thread`에 속한 여러 `frame`을 관리합니다. byeol에서는 최상위 코드도 암묵적으로 초기화 함수 내에서 실행되며, `main()`도 함수이므로, 모든 코드 실행은 함수 컨텍스트 안에서 이루어집니다. 따라서 코드 실행 = 함수 실행이며, 함수 실행시 frame 객체가 생성되어 적절히 scope를 적층시킵니다.

**Frame 적층의 필요성**

다음 byeol 코드를 봅시다:

```
@style: language-txt verified
IS_DBG := false
name := "kniz"

pack test
age := 57

def yourObj
    age := 3
    foo(newAge int) void
        age = newAge
        print(age)

main() void
    name := "unknown"
    yourObj.foo()
    if IS_DBG
        print("I'm $name, $age yo.")
```

`main()` 안에서 `yourObj.foo()`를 호출하면 frame은 다음과 같이 구성됩니다:

```
@style: language-txt verified
   frame        scope        symbol
            ┌─────┬────────┐
        ▲  │  local   │msg("age=3")    │
        │  ├─────┼────────┤
        │  │ foo(int) │newAge int      │
        │  ├─────┼────────┤
        │  │ yourObj  │foo, age(3)     │
        │  ├─────┼────────┤
frame 2 │  │  file    │IS_DBG, name    │
        │  ├─────┼────────┤
        │  │  pack    │age(57),yourObj │
        ▼  │          │main()          │
            ├─────┼────────┤
        ▲  │  local   │name("unknown") │
frame 1 │  ├─────┼────────┤
        │  │ main()   │empty           │
        ▼  └─────┴────────┘
```

frame #2를 보면 yourObj 아래에 file/pack scope이 다시 나타납니다. 왜 그럴까요?

**Pack scope을 다시 넣어야 하는 이유**

Symbol 탐색은 위에서 아래로 순차 탐색합니다. 만약 frame #2에서 pack/file scope을 제거한다면, `yourObj.foo()` 안에서 `name`을 참조할 때 main()의 local scope에 있는 "unknown"이 먼저 발견됩니다. 이는 잘못된 결과입니다 (file scope의 "kniz"가 나와야 함).

올바른 결과를 위해서는 frame #2에 pack/file scope를 끼워넣어야 합니다. if-else로 scope 탐색 장소를 분리하는 것보다, **하나의 큰 배열을 위에서 아래로 순차 탐색**하는 것이 훨씬 간결합니다.

게다가 `tnchain`의 `link()`를 활용하면, `parser`가 obj를 생성할 때 pack과 file을 미리 chain으로 연결해두어서, obj의 scope를 frames에 추가하는 것만으로도 obj, file, pack scope이 한번에 연결됩니다. 이 과정에서 **어떠한 복사도 일어나지 않습니다**.


#### thread 클래스 - 실행 흐름의 관리자

`thread`는 하나의 프로그램 실행 흐름을 표현합니다 (현재는 단일 스레드만 지원). `thread`는 `frames`와 `errReport`를 소유하여 프로그램 실행시 frame을 구성하고 에러를 수집합니다.

**Builtin pack**

`builtin` pack은 byeol 언어에서 기본 제공하는 타입/함수들입니다. manifest 선언 없이도 항상 접근 가능하며, `int`, `str`, `arr`, `err`, `print()` 등이 여기 속합니다.

standard pack과 비슷하지만 엄연히 구분되는 pack이며, builtin은 전부 C++ native 클래스를 `tbridger`를 활용해 노출한 형태입니다.

**싱글톤 속성**

`thread`는 `static thread& get()`을 제공합니다. `scope`나 `expr`들은 이를 통해 현재 활성화된 thread에 접근합니다.

기본 thread를 사용하지 않고 직접 thread 인스턴스를 만들 때는, thread 객체를 등록하고 종료시 원본으로 교체하는 작업이 필요합니다. 이를 위해 `threadUse`를 사용합니다:

```
@style: language-cpp verified
if(main.canEval(a)) {
    threadUse thr(getReport());
    // 새로운 thread로 doSomething()...
}
// 블록을 빠져나오면 원본 thread로 자동 교체
// 블록 진입 전의 실행 상태로 복원됨
```

`threadUse`는 생성과 동시에 내부에서 thread 객체를 자동 생성까지 해주므로 편리합니다.



### 패키지 시스템

Byeol은 `pack`이라는 단위로 라이브러리를 배포합니다. 패키지 시스템은 pack을 동적으로 lazy 로딩하며, 종속성 관리와 검증을 담당합니다.


#### manifest 클래스 - Pack 메타데이터

stela 언어로 작성된, pack을 로딩하기 위한 기본정보를 담고 있는 객체입니다. `manifest.stela` 파일로부터 @ref stela 모듈을 사용해 파싱됩니다.

@ref by::manifest "manifest"는 pack의 entrypoint, 종속성 정보 등을 포함합니다. entrypoint는 pack이 어떠한 종류의 라이브러리를 포함하고 있는지를 나타냅니다 (예: `cpp`, `byeol`).


#### slot 클래스 - Pack의 결과물

byeol 언어는 `pack`이라는 일종의 압축파일 단위로 라이브러리를 배포하는데, pack 파일에는 최상위 `obj` 객체와 @ref by::manifest"manifes", 종속하는 pack 목록이 포함됩니다.

(byeol 언어에서 내 코드에서 다른 pack을 사용하려면 @ref by::manifest "manifest"에 종속관계에 있다는 걸 선언해야 합니다)

**@ref by::slot "slot"은 pack 파일로부터 만들어지는 결과물**이지, pack을 불러오는 걸 담당하지 않습니다. pack 로딩에 대해서는 `packLoading`이나 `slotLoader`를 참조하세요.


#### autoslot 클래스 - Lazy Pack 로딩

byeol 언어는 pack을 **lazy하게 동적으로** 불러옵니다. @ref by::autoslot "autoslot"은 이 기능을 구현한 것으로, `slotLoader`가 pack 파일을 찾으면 `packLoading` 객체를 적절히 생성해서 @ref by::autoslot "autoslot"에 넣어둡니다.

이후 @ref by::autoslot "autoslot"에 접근해서 안에 포함된 symbol을 가져오려는 시도를 하면 lazy하게 @ref by::packLoading "packLoading"이 동작해 symbol을 파일로부터 불러옵니다.

**복수의 packLoading**

@ref by::packLoading "packLoading"은 native 환경에서 가져올 수도 있고(dll 혹은 so 파일), managed 환경에서 가져올 수도 있습니다(.byeol 파일). 또는 2개가 모두 하나의 pack에 있는 경우도 있을 수 있습니다.

따라서 @ref by::autoslot "autoslot"은 항상 1개의 @ref by::packLoading "packLoading"만 가지지 않고, 배열로 처리합니다.

**autoslot 상태 (State)**

총 4개의 상태를 가지며 다음과 같은 흐름으로 로딩 파이프라인을 갖습니다:

```
@style: language-txt verified
┌────────┐
│Make an instance│
└───┬────┘
        │
    ┌─▼──┐
    │RELEASED│
    └─┬──┘
     ┌─▼─┐
     │PARSED│
     └─┬─┘
     ┌─▼──┐
     │VERIFIED│
     └─┬──┘
     ┌─▼─┐
     │LINKED│
     └───┘
```

- **Slot 생성**: @ref by::slotLoader "slotLoader"가 @ref by::slot "slot"객체를 생성해 시스템에 추가, dependencies도 기록
- **RELEASED**: 초기 상태로, 어떠한 메모리도 점유하지 않음. 사용하지 않는 대부분의 @ref by::slot "slot"은 여기에 속함
- **PARSED**: @ref by::autoslot "autoslot"에 접근이 이뤄진 경우, 본격적으로 사용하기 위해 코드를 파싱. 파싱 단계가 필요없는 경우 (optimized pack 또는 native pack)에는 LINKED 상태로 바로 건너뜀
- **VERIFIED**: 파싱 이후, 코드의 정합성을 검증. 검증에 실패했다면 isValid값을 false로 설정
- **LINKED**: 자신이 검증에 실패한 상태라면, 자신을 참조하는 모든 dependents에게 자신이 검증에 실패했다는 사실을 전파

왜 이런 상태 관리가 필요할까요?

**동적 검증과 의존성 문제**

모든 pack이 검증이 완료된, 완전무결한 상태라고 전제하고 그냥 로딩만 해서는 안될 수 있습니다. 때로는 pack이 올바른지 한번 더 검증할 필요가 있기에, @ref by::autoslot "autoslot"중 일부는 symbol을 불러올때 사전에 파싱이나 검증을 해야 합니다.

문제는 어떠한 pack은 다른 pack에 종속되는 경우가 매우 많이 발생한다는 점으로, 종속한 pack이 검증에 실패하게 되면, 그 사실을 전파해서 종속된 pack들도 모두 사용이 불가능해야 합니다. 이를 위의 4가지 상태를 제어하는 알고리즘으로 해결합니다.

**재귀적 로딩**

pack이 다른 pack에 종속되는 경우는 부지기수로 많습니다. @ref by::autoslot "autoslot"이 lazy하게 동작하기 때문에 어떠한 @ref by::slot "slot"을 loading하다가 다른 @ref by::autoslot "autoslot"에 접근함으로써 해당 @ref by::autoslot "autoslot"도 재귀적으로 로딩 시퀸스에 들어가는 일도 많습니다.

이때 중복으로 초기화 되거나 아직 완전히 파이프라인을 통과하지 않는지 체크합니다.


**RAII**
@ref by::autoslot "autoslot"은 @ref by::slot "slot"에 정의된 _pack 객체를 가리킵니다. 이 객체는 외부 파일인 `pack` 을 로딩함으로써 읽어온 심볼들인데, @ref by::autoslot "autoslot"은 @ref by::packLoading "packLoading"을 통한 pack의 symbol 생성을 책임지므로, pack의 소멸 또한 책임집니다.
그래서 RAII를 사용해, @ref by::autoslot "autoslot"객체가 소멸될때 모든 심볼을 먼저 없애고 @ref by::packLoading "packLoading"객체 또한 없앰으로써 so 파일을 close 하는 등의 작업을 수행합니다.
자세한 내용은 @ref by::packLoading "packLoading"을 참조하세요.


#### slotLoader 클래스 - Pack 로더

@ref by::slotLoader "slotLoader"는 외부 pack을 로딩하는 역할을 담당합니다. `addPath()`로 탐색 경로를 추가하고, `load()`를 호출하면 pack을 불러올 수 있습니다.

**기본 사용법**

```
@style: language-cpp verified
nmap ret;
errReport report;

slotLoader()
    .setReport(report)  // report를 넣지 않으면 dummyReport가 대신 사용됨
    .setBaseSlots(*ret)
    .addPath("pack/")
    .load();

ret.len();  // 로딩된 slot 개수 반환
```

**manifest와 entrypoint**

pack 로딩 중에는 필연적으로 @ref by::manifest "manifest"를 파싱합니다. manifest를 통해 가장 중요한 정보는 **entrypoint**입니다. entrypoint는 pack이 어떠한 종류의 라이브러리를 포함하고 있는지를 나타냅니다.

예를들어 pack에 C++ 동적 라이브러리가 포함되어 있다면 entrypoint는 `cpp`가 되며, byeol 라이브러리라면 `byeol`이 됩니다.

@ref by::slotLoader "slotLoader"는 manifest를 읽은 후 @ref by::autoslot "autoslot"을 생성하고 entrypoint에 따라 적절한 @ref by::packLoading "packLoading"인스턴스를 autoslot에 추가합니다.

**주의사항:**
- 하나의 pack 라이브러리는 여러개의 entrypoint를 가질 수 있습니다
- 경로는 항상 재귀적으로 탐색됩니다 (하위 디렉토리의 pack들도 모두 로딩)
- 파일 탐색, 동적 라이브러리 로딩 등 플랫폼 종속적인 기능들은 indep 모듈에 위임합니다


#### packLoading 클래스 - Pack 로딩 추상 클래스

`slotLoader`에 의해 패키지를 로딩할 때 사용되는 추상 클래스입니다. @ref by::packMakable "packMakable"인터페이스가 핵심 API를 제공합니다.

packLoading은 `rel(), parse(), verify()` 함수를 제공하며, 이는 `autoslot`의 상태와 깊은 관련이 있습니다.


#### cppPackLoading 클래스 - C++ Pack 로더

`slotLoader`에 의해 cpp 패키지를 로딩할 때 사용되는 `packLoading` 중 하나입니다. 이름 그대로 C++ pack을 동적 라이브러리 파일에서 로딩하는 역할을 합니다.

**Entrypoint**

@ref indep 모듈의 @ref by::dlib "dlib"을 사용해서 동적 라이브러리를 로딩합니다. entrypoint는 이미 정의되어 있는 @ref by::ENTRYPOINT_NAME "ENTRYPOINT_NAME"의 값을 사용합니다.



### Visitor 패턴 및 AST 순회

Byeol에서는 AST를 중점적으로 다루기 때문에 @ref by::visitor "visitor"를 자주 사용하게 됩니다. 순회하는 방법과 순회시 `node`를 visit했을 때의 동작을 서로 분리하기 위해 @ref by::visitor "visitor"가 적극적으로 활용됩니다.


#### visitor 클래스 - AST 순회의 핵심

**순회**

@ref by::visitor "visitor"는 항상 <b>전위 순회(pre-order traversal)</b>를 따릅니다. 후위 순회를 하도록 변경은 불가능합니다.

`visit()`은 다음 3개의 단계로 이뤄져 있습니다:
1. 현재 찾은 node를 방문 (`onVisit()`)
2. 다음 하위 node를 순회 (`onTraverse()`)
3. 현재 찾은 node를 떠남 (`onLeave()`)

**accept를 통한 Downcasting**

@ref by::visitor "visitor"는 `onVisit(T&)`과 같이 많은 구체 타입에 대한 방문을 표현하는 virtual function을 다수 가지고 있습니다.

반면 onTraverse에서 탐색시에는 주로 `node`의 `subs()`를 통해서 `tbicontainable` 인터페이스를 사용하기 때문에 node 타입을 사용하게 됩니다. 그래서 어디선가는 node 타입을 `nInt`나 `defNestedFuncExpr`과 같이 구체타입으로 downcasting을 해야합니다.

이를 위해 @ref by::node "node"의 `accept()`라는 virtual 함수를 호출합니다:

```
@style: language-cpp verified
void defNestedFuncExpr::accept(const visitInfo& i, visitor& v) {
    v.visit(i, *this);  // visitor::visit(const visitInfo&, defNestedFuncExpr&) 호출
}
```

가상함수 accept()가 호출되면 안에서 *this를 통해 구체타입으로써 역으로 visitor의 visit()을 호출하는 식입니다.

이를 위해 visitation에 참여하는 모든 @ref by::node "node"의 파생클래스는 `accept()`라는 virtual 함수를 override 해야 하는데, 이 과정을 쉽게 하기 위해서 <b>VISIT 매크로</b>를 사용합니다:

```
@style: language-cpp verified
class _nout slot: public node {
    BY(CLASS(slot, node), VISIT())  // <--- VISIT 매크로

public:
```

만약 @ref by::node "node"파생클래스 임에도 `accept()`를 override 하지 않았다면, `onTraverse(node&)`가 대신 사용되며, 이걸로도 충분한 경우에 해당됩니다.

**중복 방문 제거**

AST는 참조가 서로 순환하는 경우도 종종 발생합니다 (예: A가 B를 참조하고 B가 다시 A를 참조). 이 경우 아무런 예외처리 없이 순회하면 이미 방문했던 @ref by::node "node"를 다시 방문하면서 <b>무한 순회</b>에 빠집니다.

@ref by::visitor "visitor"는 `_visited`라는 map을 소유하고 있습니다. 이를 통해서 `visit()`이 호출 되었을 때 이미 방문한 @ref by::node "node"인지를 판단해서 예외처리를 해주고 있습니다.

이 방문 기록 정보는 매번 @ref by::visitor "visitor"가 방문을 시작하기 직전에 초기화됩니다. 만약 재방문이 가능하도록 하고 싶다면 `setReturnable(true)`로 값을 변경하면 됩니다.


#### graphVisitor 클래스 - AST 로깅

`visitor` 기반의 AST 로깅 클래스로, 주어진 root `node`를 순회하면서 상세 정보를 모두 로깅합니다.

다음과 같은 상황에서 사용됩니다:
- `interpreter`에 의해서 디버그 빌드에서 사용
- `defaultSigZone`에 의해서 crash가 발생하면 출력
- `logStructureFlag`에 의해서 `interpreter`나 `starter`로 하여금 @ref by::graphVisitor "graphVisitor"를 사용하도록 flag가 set됨

@ref by::graphVisitor "graphVisitor"는 AST 구조를 트리 형태로 시각화하여 출력하므로, 디버깅시 AST의 구조를 파악하는 데 매우 유용합니다.



### 파싱 시스템

byeol 언어의 파싱 시스템은 <b>Flex</b>와 <b>Bison</b>을 사용하는 전통적인 파서 구조를 따르지만,
byeol 언어의 특성인 <b>offside rule</b>을 지원하기 위해 정교한 메커니즘을 갖추고 있습니다.

#### parser 클래스 - 파싱의 진입점

`parser`는 byeol 파싱 컴포넌트의 진입점 역할을 하며 `worker`를 상속합니다. `work()`을
통해서 파싱된 결과가 `slot`으로 반환됩니다.

**Scanner - Bison - Parser 구조**

Flex와 Bison을 사용하고 있으며 flex는 `lowscanner`로, bison은 `lowparser`로 각각
명명합니다. 이 low level인 scanner, @ref by::parser "parser"는 @ref by::parser "parser"컴포넌트 안에만 존재하는 것으로
외부에서는 일절 노출되지 않습니다.

`parser::work()`가 실행되면 다음과 같은 흐름으로 동작합니다:

1. lowscanner를 실행
2. lowscanner는 토큰을 뜯어서 lowparser에게 넘김
3. lowparser는 받은 토큰에 대해 rule이 match되면 그 이벤트를 다시 parser에게 넘김

그러므로 @ref by::parser "parser"의 `on`으로 시작하는 함수들(예: `onPack()`, `onCompilationUnit()`)은 그러한 이벤트를 handling하는 함수로, 실제로 어떻게 @ref by::node "node"를 생성해서 AST를 구축하는지를 정의합니다.

**lowparser.y의 Lifecycle 관리**

각 rule에서 `parser::onXXXX()` 함수들을 호출하면, 해당 함수 내에서는 `new` 키워드로
새로운 객체를 heap에 생성해서 반환하는 경우가 많습니다. 이렇게 받은 인자를 그대로
`tbicontainer` 등에 직접 넣으면 괜찮지만, 그렇지 않은 경우는 미리 `tstr` 등으로
binding을 하지 않으면 <b>메모리 릭</b>이 발생하기 딱 좋습니다:

```
@style: language-cpp verified
// lowparser.y
pack: PACK name-access NEWLINE {
    $$ = PS.onPack(*$2);  // onPack()은 new pack()을 반환한다.
}

compilation-unit: pack defblock {
    tstr<obj> pak($1);  // 이렇게 tstr로 바인딩하지 않으면,

    PS.onCompilationUnit(pak.get());  // onCompilationUnit() 안에서 주어진 pak의
                                       // 값이 문제가 있어, 동작이 취소된 경우, heap에
                                       // 생성한 pack 객체는 메모리릭이 된다.
}
```

이 점은 `onXXXX()` 같은 함수에 국한되지 않고, 문자열 token을 받는 경우도 동일하게
적용됩니다:

```
@style: language-cpp verified
// lowscanner.l
<stateString>\"  {  // 문자열 scan이 종료되면
    if(!yylval->asStr) yylval->asStr = new std::string();  // string 객체를 new로 생성
    ....
    return STRVAL;
}

// lowparser.y
   | STRVAL {  // STRVAL이 scanner로부터 넘어오면
    $$ = PS.onPrimitive<nStr>(*$1);  // 일단 넘긴다. 프로젝트 내에서 string는 항상
                                      // immutable로 다루므로 항상 객체가 복제되어 들어감
    delete $1;  // 객체가 복제되었으니 원본 객체는 메모리를 해제한다.
}
```

#### smartDedent, tokenScan 클래스 - Indentation 관리

byeol 파서의 `smartDedent`와 `tokenScan`은 @ref by::stela "stela"모듈의 `stelaSmartDedent`, `stelaTokenScan`과 동일한 원리로 동작합니다. Indentation을 배열로 관리하고, scan mode를 동적으로 전환하는 방식은 완전히 같습니다.

상세한 동작 원리와 예제는 @ref stela 모듈의 해당 클래스들을 참조하세요. byeol 파서의 경우
더 복잡한 문법을 가지므로 명령 토큰(`SCAN_AGAIN`, `SCAN_EXIT` 등)이 더 다양합니다

자세한 명령 토큰 목록은 `lowparser.y`의 `// mode:` 단락을 참조하세요.

#### srcSupply 클래스 - 소스 코드 공급 추상화

`parser`에서 사용되는 클래스로, @ref by::parser "parser"에 입력되는 코드를 어떠한 형태로 제공할지를 추상화합니다.

예를 들어 파일로써 제공하는 방법도 있지만, 버퍼에 담아서 줄 수도 있고 stream으로 주는
방법도 있을 것입니다. `srcSupply`는 이러한 다양한 소스 제공 방식을 추상화하여 @ref by::parser "parser"가
일관된 인터페이스로 소스 코드를 읽을 수 있도록 합니다.

#### expander 클래스 - 사전 타입 추론

`parser`에서 명시한 것처럼 현재 사전 타입 추론 알고리즘은 종속성 그래프를 만들지 않고
모든 사전 타입 추론 표현식을 모아둔 후, 반복적으로 타입 추론을 시도하는 방식입니다.

이러한 방식을 사용하기 때문에 반복적으로 표현식을 순회하며 사전 타입 추론을 시도하는
동작을 `verifier` 안에서 한번에 수행하기에는 적합하지 않습니다. 따라서 @ref by::verifier "verifier"로 검증하기 전에 @ref by::expander "expander"로 사전 타입 추론을 완료해둘 필요가 있습니다.

기본 동작은 `visitor`에 기반하므로, 사전에 @ref by::visitor "visitor"를 숙지하고 오는 게 좋습니다.

**defBlock과 Expand**

어떠한 byeol 코드는 타입 추론시 순서를 요구합니다:

```
@style: language-byeol verified
def myObj
    msg := "I'm $name and $age yo."
    name := "little man #$age"
    age := outerModule.defaultValue + 2
```

위와 같은 코드를 타입추론을 위해 위에서부터 아래로 코드를 평가한다고 해봅시다:

- `msg`는 `name`과 `age`를 참조하는데 name과 age의 타입을 모르므로 실패
- `name`은 `age`를 참조하는데 age를 모르므로 실패
- `age`는 심지어 외부 모듈의 값을 참조하므로 해당 모듈에 접근해야 함

만약 해당 모듈에서 값을 가져왔다고 가정한다면 age는 int 타입임을 알 수 있지만 name과
msg는 타입 확정에 실패한 상태로 진행됩니다.

**현재 사용중인 타입 추론 방법**

byeol은 이러한 타입추론 표현식들을 한곳에 모아둔 후, parsing이 종료된 후 <b>1줄이라도 타입추론이 성공한다면 무한루프를 도는 방법</b>을 사용합니다. 가지고 있는 모든 표현식이 타입추론에 실패하거나, 타입추론 표현식이 바닥나면 루프를 종료합니다.

이 방식은 추론 순서를 따지지 않아도 되므로 구현이 간단하지만 속도가 느리므로,
향후 종속성 그래프를 구축하여 타입 추론 순서를 최적화하는 방법으로 개선할 여지가 있습니다.

`defBlock`은 이러한 역할을 돕습니다. @ref by::defBlock "defBlock"에는 expand, common, scope 3종류의 데이터가 있는데:

- **scope**: @ref by::parser "parser"에 의해서 바로 AST가 만들어지는 @ref by::node "node"
- **expand**: 파싱은 성공했지만 타입추론이 필요하여, `verifier` 동작 전에 `expander`로
  넘길 부분
- **common**: 공통 생성자를 위한 부분


### 코드 검증 및 실행

파싱이 완료된 AST는 실행되기 전에 검증 과정을 거쳐야 합니다. byeol 언어는 인터프리터
구조를 띄고 있지만 <b>강형 타입</b>을 사용하며, 컴파일 언어처럼 사전에 에러를 도출합니다.

#### tworker 클래스 - 배치 작업의 기반

`tworker`는 대량의 배치 작업을 수행하는 데 최적화되어 있습니다. `errReport`로 이
과정에서 발생하는 에러를 수집할 수 있으며 다양한 flag들을 지정해서 작업 중 일부 동작을
변경시킬 수도 있습니다.

`verifier`, `visitor`, `parser` 등 큰 작업을 수행하는 클래스들은 @ref by::tworker "tworker"기반으로 돌아갑니다.

**work와 task**

@ref by::tworker "tworker"는 말 그대로 `work()`를 하기 위해 존재합니다. 이때 작업의 대상이 되는 input을
`task`라고 합니다.

작업 전에 `_prepare()`가 호출되며 작업이 종료되면 `_onEndWork()`가 호출됩니다.

**errReport 통합**

대량의 작업을 깊은 함수 depth를 동반하면서 수행해야 하므로, 콜스택 깊은 곳에 있는
에러를 충분히 탐지하기 위해서는 @ref by::errReport "errReport"를 통해서 이를 수집할 수 있어야 합니다.

@ref by::tworker "tworker"는 `setReport()`를 통해서 외부로부터 @ref by::errReport "errReport"객체를 넣을 수 있습니다. @ref by::errReport "errReport"를 할당하지 않을 경우 `dummyErrReport`가 대신 들어갑니다.

**Log Flag**

@ref by::tworker "tworker"는 작업 도중에 로깅을 위한 다양한 flag를 가지고 있습니다:

- `LOG_ON_EX`: 에러 발생시 한줄 로그를 남김
- `DUMP_ON_EX`: 에러 발생시 callstack을 포함한 `err` 객체를 dump
- `GUARD`: 함수가 시작되고 끝날 때 로깅
- `INTERNAL`: 에러나 GUARD를 제외한 로그를 로깅
- `LOG_ON_END`: 작업이 모두 완료되면, 그간 수집한 err를 모두 `log()`로 한줄 로그를 남김
- `DUMP_ON_END`: 작업이 모두 완료되면, 그간 수집한 err를 모두 `dump()`

#### verifier 클래스 - 코드 검증

`visitor` 기반의 코드 검증을 담당합니다. byeol 언어는 인터프리터의 구조를 띄고 있지만
언어 자체는 강형 타입을 사용하고 컴파일을 실행하는 것처럼 사전에 에러를 도출합니다. @ref by::verifier "verifier"는 그 기능의 핵심을 구현합니다.

`parser`와 `expander`로 최종 생성된 AST를 실행하기 전에 에러는 없는지 확인합니다.

**사전 학습 권장 사항**

@ref by::verifier "verifier"는 다양한 모듈을 종합적으로 사용하는 고급 클래스이므로, 다음 개념들을 먼저 학습한 후 접근하는 것을 권장합니다:

- `node`: AST의 기본 구조
- `visitor`: AST 순회 패턴
- `expr`: 표현식 클래스들
- WHEN 매크로: 에러 처리 패턴

**visitor 기반**

onTraverse는 @ref by::visitor "visitor"에서 알아서 채워주므로, @ref by::verifier "verifier"는 onVisit과 onLeave시 어떻게 해야 하는지에 대해서만 정의해두고 있습니다.

**eval과 infer**

프로그램의 실행은 결국 각 `node`의 `eval()`로 이뤄지지만, @ref by::verifier "verifier"는 실행한 런타임 값은 관심이 없습니다. 오직 실행이 가능한가, 지정한 표현식의 결과가 정의된 property의
타입과 묵시적 변환이 허용되는가와 같은 <b>타입 매칭</b>에만 관심이 있습니다.

node의 `infer()`는 타입 추론 기능을 수행하는 것으로 실행하면 값은 모르지만 결과 타입을
`origin` 객체로 반환하는 함수입니다. 값을 계산하지 않으므로 `eval()`보다 더 빠릅니다.

이제 감이 왔겠지만, @ref by::verifier "verifier"는 `eval()`보다는 `infer()`를 중점적으로 사용합니다:

```
@style: language-cpp verified
void me::onLeave(const visitInfo& i, assignExpr& me, nbool) {
    ....
    str left = me.getLeft().infer() OR.myExErr(me, LHS_IS_NUL).ret();
    ....
    str right = me.getRight().infer() OR.myExErr(me, RHS_IS_NUL).ret();
    ....
}
```

위의 예시처럼 `eval()` 대신 `infer()`를 주로 사용합니다.

**많은 코드 라인을 가진 파일**

@ref by::verifier "verifier"는 가장 LOC가 많은 파일 중 하나입니다. 이때 코드들은 크게 3 종류로 분류됩니다:

1. 검증을 위한 사전 작업
2. WHEN 매크로를 사용한 실제 검증 로직
3. 각 단계별 상세 로깅

**단계별 상세 로깅**

@ref by::visitor "visitor"는 타입당 1개의 `onVisit()` 함수만 가질 수 있습니다. 그러나 @ref by::verifier "verifier"는 하나의 타입이 주어졌을 때 검증해야 하는 케이스가 대부분 2개 이상입니다. 그러니 잘못하면
하나의 `onVisit()` 함수는 여러 개의 검증 로직으로 뒤범벅되어 구분이 어려워집니다.

단계별 상세 로깅이란, 이를 도와주는 코드로, 검증 타겟이 되는 객체에 대해 진행하게 될
검증 로직이 무엇인지를 로그를 남기는 역할을 합니다:

```
@style: language-cpp verified
// verifier가 assignExpr 객체에 대해 검증하는 경우:
void me::onLeave(const visitInfo& i, assignExpr& me, nbool) {
    _GUARD("onLeave(assignExpr&)");  // assignExpr에 대한 검증이 시작됨을 로깅

    // 첫번째 step: evalType을 set한다.
    _STEP("set evalType");
    ....

    // 두번째 step: LHS와 RHS의 타입이 매칭되는지 확인
    _STEP("verify type matching");
    ....
}
```

이렇게 단계별로 로깅을 남기면 복잡한 검증 로직을 추적하기가 훨씬 쉬워집니다.

#### starter 클래스 - AST 실행

`worker`의 일종으로, `verifier`에 의해 검증이 완료된 AST를 실행합니다. 실행 결과
`thread`에게 exception이 발생되었다면 로그를 덤프하고 exception을 반환합니다.

**main 함수 Look up**

`starter`가 메인 함수를 찾을 때 사용하는 조건:

- **위치**: root(pack) 바로 밑에 위치해야 함
- **이름**: `main`이어야 함
- **반환형**: `void` 또는 `int`
- **내용**: 최소 1개 이상의 구문을 포함해야 함

@ref by::starter "starter"는 @ref by::interpreter "interpreter"와 함께 사용하면 byeol 코드를 파싱해서 실행할 수 있습니다.

#### sigZone 클래스 - Signal 처리

`signaler`에 RAII를 적용한 클래스입니다.

@ref by::sigZone "sigZone"인스턴스가 정의된 블록문 안의 코드를 실행할 때 signal이 오면 람다를 수행하도록 @ref by::signaler "signaler"를 사용합니다. RAII 패턴을 통해 블록을 벗어나면 자동으로 signal handler가
해제됩니다.


### 에러 처리

byeol 언어는 에러 처리를 위한 정교한 시스템을 갖추고 있습니다. 에러는 단순히 발생하고
끝나는 것이 아니라, callstack 정보와 함께 수집되며, 검증 단계에서부터 실행 단계까지
체계적으로 관리됩니다.

#### baseErr 클래스 - 에러의 기반

byeol 언어에서의 @ref by::err "err"클래스 계통의 가장 base 클래스입니다. 에러 처리에 대한 공통
로직과 인터페이스를 포함합니다.

**byeol Error의 문법적 구분과 내부 구현**

byeol 문법상으로는 에러는 2종류로 구분됩니다:

1. **Known error**: `?`로 표현되며, errorable 타입으로 명시
2. **Exception**: errorable 타입으로 명시하지 않는 상황에서 에러가 반환되는 케이스

하지만 <b>구현상으로는 둘은 완전히 동일한 에러 객체</b>입니다. 에러는 `parser`나 `verifier`가 발생시킨, native 상에서 발생한 `nerr`와 byeol 코드로 인해 발생한 `err`, 2개가 에러의 발생원에 의해 구분되고 있을 뿐입니다.

**@ref by::err "err"는 @ref by::frames "frames"에 대한 강한 참조를 갖는다**

이 @ref by::frame "frame"을 사용해서 callstack 정보를 제공합니다. `frame`에서 설명한 것처럼 @ref by::frame "frame"은 함수 호출시 @ref by::baseObj "baseObj"에 의해서 생성됩니다.

이후 @ref by::frame "frame"은 `frames`에 의해서 `del()` 되는데 이때 객체의 강한 참조를 잃어버릴 뿐
heap에서 바로 삭제가 되진 않습니다. (이 프로젝트에서 memlite 모듈의 `tstr`에 의한
레퍼런스 카운팅 없이 직접 heap에서 new/delete를 하는 경우는 극히 드뭅니다.)

그러므로 <b>@ref by::err "err"가 살아있는 한 참조하는 @ref by::frame "frame"데이터 또한 유지</b>됩니다.

**nerr 생성**

@ref by::nerr "nerr"은 주로 core 모듈의 `__core_when__`에 의해서 만들어집니다. 다음과 같은 코드로
이뤄집니다:

```
@style: language-cpp verified
WHEN_NUL(stmt).exErr(IS_NUL, getReport(), "stmt").ret(blk);
```

위 코드는 `stmt`가 nullptr일 경우 `IS_NUL`이라는 에러가 `getReport()`로 반환되는
`errReport` 객체에 추가되고 blk를 반환하면서 함수가 종료됩니다.

**dumpable과 log**

- `log()`: 해당 에러를 clog 모듈을 사용해서 로깅
- `dump()`: callstack을 포함해서 최대한 많은 정보를 노출

**errCode**

관리를 위해서 @ref by::nerr "nerr"는 @ref by::errCode "errCode"로 관리됩니다. nerr는 주로 `parser`나 `verifier`에 의해서
생성되는데, 이는 문법 에러가 발생했을 때 `Err2203`과 같이 code 2203를 같이 줌으로써
어떤 에러인지 상세 정보를 쉽게 검색하도록 도움을 주기 위해서입니다.

@ref by::errCode "errCode"는 리터럴 상수로 간단하게 description 문자열도 정의하고 있으니 참고하세요.

#### errReport 클래스 - 에러 수집

`baseErr`들을 담아두는 클래스입니다.

시간이 오래 걸리거나 복잡한 동작을 수행하는 로직이 있는 경우 함수의 depth가 매우
깊어집니다. 안쪽에서 특정 함수에서 에러가 발생한 경우, 에러가 발생했다는 사실을
기록하고 작업은 최대한 진행하고 싶을 때 @ref by::errReport "errReport"를 사용해서 에러를 기록합니다.

**컨테이너**

@ref by::errReport "errReport"는 거의 모든 기능이 `baseErr` 객체에 대한 관리이기 때문에 `tucontainable`과 유사한 인터페이스를 가지고 있습니다.

**noisy**

어떠한 @ref by::errReport "errReport"는 새로운 @ref by::err "err"객체가 add될 때마다 자동으로 `log()`를 수행하고 싶을 때가 있습니다. 이때 `setNoisy(true)`를 지정합니다. 이렇게 하면 에러가 추가되는 즉시
로그가 남아 디버깅이 더 쉬워집니다.



## frontend 모듈 - CLI 인터페이스

@ref frontend모듈은 Byeol 언어의 명령줄 인터페이스(CLI)를 제공합니다. 이 모듈은 @ref core모듈의 기능들을 조합하여 사용자가 Byeol 프로그램을 실행할 수 있도록 합니다.


### cli 클래스 - Frontend의 핵심

@ref by::cli "cli"클래스는 @ref frontend모듈의 핵심 클래스로, @ref core모듈의 @ref by::interpreter "interprete"를 사용해서 코드를 파싱하고 검증하고 평가하는 일련의 과정들을 위해 적절한 클래스를 호출해서 제어합니다.

즉, @ref by::cli "cli"는 뭔가 알고리즘을 만들어서 동작하는 클래스가 아니라 <b>이미 잘 짜여진 클래스들을 조합하는 역할</b>을 합니다.

파라메터로 @ref by::cli "cli"프로그램에 사용자가 입력한 인자를 받으며, 이를 적절히 파싱해서 추가로 명령을 수행합니다. 자세한 내용은 `flags` 폴더를 참조하세요.

**핵심 알고리즘**

사용자가 파라메터로 flagArgs를 줘서 eval()을 호출하면 다음과 같이 실행됩니다:

1. `interpreter`, `errReport`, `starter` 객체를 생성한다.
2. interpreter, starter에 flag를 set한다. 이 `tworker`의 flag를 말하는 것으로, cli의
   `flag`와 다른 것이다. 자세한 내용은 @ref by::tworker "tworker"를 참조.
3. flagArgs를 파싱해서 사전 작업 수행 - flag 객체에 `take()` 함수를 호출해서 수행한다.
4. @ref by::interpreter "interpreter"를 수행한다.
5. 인터프리트 결과를 체크한다. 이상이 있으면 결과를 내보내고 종료한다.
6. 이상이 없으면 @ref by::starter "starter"에 검증된 AST를 넣고 실행한다.
7. @ref by::starter "starter"의 결과를 반환한다.


### flag 클래스 - 명령줄 플래그 처리

shell 기반 프로그램에서 흔히 볼 수 있는 플래그들을 처리하는 클래스입니다.

한가지 착각하기 쉬운 포인트는 이 클래스는 `--version`과 같은 플래그를 표현하는 클래스가
아니라 <b>그러한 플래그가 존재하는지, 존재한다면 어떤 동작을 해야 하는지를 담당</b>한다는
것입니다.

**Flag의 설명**

각 @ref by::flag "flag"는 자신의 이름 뿐만 아니라 어떠한 기능인지 description을 정의해두고 있습니다.
이들은 `helpFlag`에 의해서 사용됩니다.

**정규식에 의한 패턴매칭**

각 @ref by::flag "flag"클래스는, 자신이 찾고자 하는 패턴이 정해져 있습니다.

예를들어 `verFlag`는 `--version`이라는 문자열이 프로그램 인자로 들어와 있는지를 찾아,
들어와 있을 경우 `buildFeature`로부터 version 정보를 가져와 출력합니다.

```
@style: language-cpp verified
// -- verFlag.cpp
const strings& verFlag::_getRegExpr() const {
    static strings inner{"^\\--version$"}; // 이 정규식이 매치되면, _onTake()가 실행됩니다.
    return inner;
}

me::res verFlag::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
    std::cout << "🌟 'byeol' " << buildFeature::version::get() << "\n"
              << "Built on " << buildFeature::date::get() << " (build " << buildFeature::version::getBuildCount()
              << ", git " << buildFeature::version::getCommitHash() << ", " << buildFeature::config::getName()
              << ", " << buildFeature::platform::getArchBits() << "-bit)\n"
              << "© 2010-" << buildFeature::date::getYear()
              << " Taehun Lee(kniz) and contributors. All rights reserved.\n";

    return EXIT_PROGRAM;
}
```

이 동작은 @ref by::flag "flag"의 `take()`가 호출되면 파생클래스의 `_getRegExpr()`에 정의해둔 정규식 표현으로 각 @ref by::flag "flag"객체가 원하는 패턴을 찾는 형태로 동작합니다. 정규식으로 찾기 때문에
flag 간 순서는 무시됩니다.

또한 정규식 패턴을 정의할 때는 여러개 패턴을 정의할 수 있습니다:

```
@style: language-cpp verified
// -- logStructureFlag.cpp
const strings& me::_getRegExpr() const {
    static strings inner{"^\\-S$", "^\\--show-structure$"};
    return inner;
}

me::res me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
    ip.addFlag(interpreter::LOG_STRUCTURE);
    s.addFlag(starter::LOG_STRUCTURE);
    return MATCH;
}
```

위는 `-S` 혹은 `--show-structure` 둘 중 하나라도 매칭되면, 파싱된 AST의 구조와 프레임 정보를 로깅하도록 플래그를 설정합니다.

**복수의 flag 인자를 consume하기**

위에서 예로 들었던 @ref by::verFlag "verFlag"를 다시 봅시다. `--version`이라는 문자열이 프로그램 인자에 있는 경우, 버전을 출력합니다. 단, 이때 또 다시 @ref by::verFlag "verFlag"가 동작하지 않도록, 동작을 마치면 `--version`이라는 문자열을 @ref by::flagArgs "flagArgs"에서 제거합니다.

그러면 다음으로 `bufferSrcFlag`를 봅시다. 다음과 같이 사용합니다:

```
@style: language-txt verified
$ byeol --script "main() void: print("wow!)"
```

이 명령은 파일 없이 byeol 코드를 직접 실행합니다. 위 한 줄 코드를 풀어쓰면:

```
@style: language-byeol verified
main() void
    print("wow!")
```

실행 결과는 `wow!`가 화면에 출력됩니다.

이처럼 어떤 @ref by::flag "flag"는 패턴과 일치하는 부분만을 consume하지 않습니다. `--script`을 찾으면
거기서 추가적으로 1개의 인자를 더 뜯어내서 `bufferSrcFlag::_onTake()`로 전달해 함수의
본문이 _onTake()로 전달되도록 해야 합니다.

각 @ref by::flag "flag"의 파생클래스는 원할 경우, `getArgCount()`를 오버라이드해서 몇개의 인자를 더 뜯어낼 것인지를 명시합니다:

```
@style: language-cpp verified
// -- bufferSrcFlag.cpp
ncnt me::getArgCount() const { return 1; } // 1개 더 뜯을 거임.

me::res me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
    // main()의 argc, argv를 잘 알고 있을 것이다. 그것과 비슷하게,
    // 매칭된 문자열이 tray에 담겨있다.
    // 즉 tray[0]은 "--script"
    //    tray[1]은 "main() void: print(\"wow\")"
    // 가 들어있게 된다.
    if(tray.size() < 2) return BY_E("invalid flagArgument size < 2"), EXIT_PROGRAM;

    ip.addSupply(*new bufSupply(tray[1]));
    return MATCH;
}
```

**중단 가능한 flag**

@ref by::bufferSrcFlag "bufferSrcFlag"는 프로그램 시작 전에 사전 작업을 필요로 하는 @ref by::flag "flag"입니다. 반면 @ref by::verFlag "verFlag"같은 경우는 일단 매칭이 되면 어떠한 인터프리팅도 하지 않고 그대로 버전을 출력하고 종료합니다. (대다수 프로그램이 이렇게 동작한다는 걸 알고 있을 것입니다.)

이처럼 @ref by::flag "flag"의 패턴이 매칭이 되면 동작을 하고 바로 종료하고 싶을 때는, `_onTake()`를 오버라이딩할 때 반환값을 <b>EXIT_PROGRAM</b>으로 줍니다. @ref by::bufferSrcFlag "bufferSrcFlag"처럼 계속 동작을 하는 경우에는 <b>MATCH</b>로 반환합니다.


## 다음 단계

지금까지 byeol 언어의 아키텍처, 설계 그리고 핵심 클래스들을 설명했습니다.
가급적 예제를 직접 넣긴 했지만 그래도 완전히 이해하기에는 부족했을 거라 생각되네요.
역시 좀 더 깊이 이해하고 싶다면, `test` 모듈의 unit test 코드들을 직접 읽어보는 것을 강력히 권장합니다.

각 모듈별로 작성된 테스트 케이스들은 실제 사용 예제를 담고 있으며, 특정 기능이 어떻게
동작하는지 가장 명확하게 보여줍니다.
코드에 기여하기 전에 관련 테스트들을 먼저 살펴보면 이해에 큰 도움이 될 것입니다.

-- Nov 2025, kniz

