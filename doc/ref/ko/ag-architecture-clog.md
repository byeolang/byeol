# clog 모듈 - 로깅 시스템 {#ag-architecture-clog}

@ref by::clog "clog" 모듈은 여러 출력 스트림과 필터링 기능을 갖춘 경량화된 C++ 로깅 프레임워크를 제공합니다.
이 모듈은 아키텍처 상 indep 모듈을 제외하고 상위 모듈에 대한 의존성이 없습니다.
Facade 패턴, Composite 패턴, Chain of Responsibility 패턴 등을 활용하여 유연하고 확장 가능한 로깅 시스템으로 구현되어있고,
richLog 기능, Scope Logging 기능을 통해 사용성이 뛰어납니다.

<b>clog 모듈의 주요 클래스:</b>

@startuml
class logger {
    - _streams : map<string, stream*>
    - _filters : filters
    + {static} get() : logger&
    + logBypass(level, msg) : void
    + getStream(name) : stream*
    + setFilters(filters&) : void
    + setEnable(bool) : void
    --
    <b>Facade Pattern</b>
    <b>Singleton Pattern</b>
    Logging system entry point
}

abstract class stream {
    - _enable : nbool
    - _state : State
    + dump(level, msg) : void
    + setEnable(bool) : void
    + rel() : void
    --
    <b>State Machine</b>
    Destination for logging messages
}

class consoleStream {
    + dump(level, msg) : void
    --
    Console output stream
    Uses stdout/stderr
}

class fileLogStream {
    - _file : FILE*
    + dump(level, msg) : void
    --
    File output stream
    Writes to log file
}

abstract class filterable {
    + {abstract} filt(Log&) : nbool
    --
    <b>Filter Interface</b>
}

class filters {
    - _container : vector<filterable*>
    + filt(Log&) : nbool
    + add(filterable*) : void
    --
    <b>Composite Pattern</b>
    Manages multiple filters
}

class errPassFilter {
    + filt(Log&) : nbool
    --
    Passes only ERR level
}

class warnPassFilter {
    + filt(Log&) : nbool
    --
    Passes only WARN level
}

logger "1" *-- "0..*" stream
logger *-- filters

stream <|-- consoleStream
stream <|-- fileLogStream

filterable <|-- filters
filterable <|-- errPassFilter
filterable <|-- warnPassFilter

filters "1" o-- "0..*" filterable

@enduml

---

## 로깅 시스템 아키텍처

@ref by::clog "clog" 모듈의 핵심은 @ref by::stream "stream" 입니다. @ref by::stream "stream" 은 쉽게 말해 로깅이 출력되는 스트림, 즉 목적지를
표현합니다. 현재는 @ref by::consoleStream "consoleStream" 과 @ref by::fileLogStream "fileLogStream" 2가지가 존재합니다. 모든 @ref by::stream "stream" 은 기본적으로
@ref by::logger "logger" 클래스가 처음부터 소유하고 있습니다.

각 stream은 byeol의 핵심 클래스들과 마찬가지로 다음과 같은 State Machine 패턴을 사용한 상태 전이 도식을 갖습니다:

```
@style: language-txt verified
RELEASED ---init()---> INITIALIZED
RELEASED <--rel()----- INITIALIZED
```

객체를 처음 생성하면 RELEASED 상태에서 시작하지만, 생성자 내부에서 자동으로 `init()`을 호출하여
INITIALIZED 상태로 진입합니다. 단, 특정 stream 인스턴스를 명시적으로 `rel()`한 경우,
다시 명시적으로 `init()`을 호출하지 않으면 정상적으로 동작하지 않아요.

stream은 또한 enable 여부를 관리합니다. setEnable(false)를 통해 특정 stream을 disable 시키면 해당
stream은 동작하지 않아요.

또하나 중요한 점은 @ref by::logger "logger" 클래스 자체도 @ref by::stream "stream" 에서 상속하기 때문에 @ref by::stream "stream" 과 동일한 API를 제공한다는
점입니다. @ref by::logger "logger" 는 각 API에 대해 소유한 모든 @ref by::stream "stream" 들에 대해 redirection 하는 구성으로 구현되어
있습니다. 이는 Composite 패턴의 전형적인 구조로, logger가 여러 stream을 하나의 트리 구조로 관리하면서
동일한 인터페이스를 제공합니다. 예를들어 `logger::get().setEnable(false)`를 하게 되면, 모든 @ref by::stream "stream" 이 disable 됩니다.

@ref by::stream "stream" 은 logBypass(const nchar*) 라는 함수를 제공하는데, 이것은 어떠한 가공도 없이 문자열을 그대로
지정한 @ref by::stream "stream" 으로 로그 메시지를 보냅니다.

<b>stream 제어 예제</b>

```
@style: language-cpp verified
logger& log = logger::get();

// 특정 stream 가져오기
stream* console = log.getStream("consoleStream");
stream* fileLog = log.getStream("fileLogStream");

// stream별로 enable/disable 제어
console->setEnable(true);   // console 출력 활성화
fileLog->setEnable(false);  // 파일 로깅 비활성화

// 로깅 - console에만 출력됨
BY_I("This goes to console only");

// 모든 stream disable
log.setEnable(false);
BY_I("This won't be logged anywhere");
```

---

## 로깅의 기본 사용법

@ref by::logger "logger" 클래스는 @ref by::stream "stream" 이라고 불리는 복수의 로깅 경로를 통해 체계적으로 로깅이 가능한 경량화된
C++ 로깅 프레임워크의 일종의 facade입니다.

일반적으로는 동봉되는 매크로를 통해, 다음과 같이 사용합니다.

```
@style: language-cpp verified
BY_I("slot[%s] origins loaded.", getName());
```

위 코드는 다음과 같이 출력되는데요,

```
@style: language-txt verified
Oct 22 2025 21:26:13   I    cppPodLoa   <_loadLibs#49>    slot[cpp] origins loaded.
          (1)         (2)     (3)           (4)                    (5)
```

위 메시지는 다음과 같은 정보를 보여줍니다:

1. 로깅한 날짜(Oct 22 2025)와 시간(21:26:13)
2. 로그 레벨(I, Info). 로그 레벨은 ERR, WARN, INFO 총 3개가 존재합니다.
3. 로깅한 클래스명(cppPodLoading)
4. 로깅한 함수(_loadLibs)와 해당 파일내 라인번호(49)
5. 로그 메시지(slot[cpp] origins loaded)

기본적인 사용법은 여기까지만 알아도 충분해요.
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

---

### Scope 로깅 매크로

함수나 블록의 <b>진입과 탈출을 시각화</b>하고 싶을 때는 `BY_I_SCOPE`, `BY_E_SCOPE`, `BY_W_SCOPE` 매크로를 사용합니다.
이 매크로들은 <b>RAII 패턴</b>으로 구현되어 있어서, 스코프에 진입할 때와 벗어날 때 자동으로 로그를 남깁니다.

다음은 기본적인 사용 예시입니다:

```
@style: language-cpp verified
void processData(int id) {
    BY_I_SCOPE("processData id=%d", id);
    // 함수 본문...
    BY_I("processing...");
    foo();
}
void foo() {
    BY_I_SCOPE("inside of foo");
    BY_I("hello foo!");
}
```

위 코드는 다음과 같이 출력됩니다:

```
@style: language-txt verified
I <processData#130> ▶  processData id=123
I <processData#131> ┣ processing...
I <processData#130> ┣▶  inside of foo
I <processData#131> ┃┣ hello foo!
I <processData#130> ┣◀  inside of foo
I <processData#130> ◀  processData id=123
```

`▶` 기호는 스코프 진입을, `◀` 기호는 스코프 탈출을 나타냅니다. 스코프 안쪽의 로그는 `┣` 기호로
시각적인 깊이를 표현하며, 내부적으로 @ref by::line::incLv() "line::incLv()" 를 통해 그래프 깊이가 자동으로
증가/감소합니다.

<b>주의사항: 인자가 두 번 평가됩니다</b>

`BY_I_SCOPE` 매크로는 내부적으로 @ref by::BY_END "BY_END" 와 @ref by::scopeLog "scopeLog" 를 사용하는 RAII 패턴으로 구현되어 있습니다.
이때 <b>format string의 인자가 두 번 평가</b>되므로, 부수 효과가 있는 표현식을 사용하면 안 됩니다:

```
@style: language-cpp verified
// ✅ 올바른 사용
void good(int id) {
    BY_I_SCOPE("good id=%d", id);  // OK: id는 여러 번 평가해도 안전
    // ... 작업 ...
}

// ❌ 잘못된 사용
void bad() {
    BY_I_SCOPE("count=%d", counter++);  // WRONG: counter가 두 번 증가함!
}
```

<b>주의사항: if 블록에서 사용할 때 조심하세요</b>

`BY_I_SCOPE` 매크로를 if 문의 블록문으로 사용하면 예상과 다르게 동작할 수 있습니다. 매크로가 expand
되면서 여러 개의 statement로 펼쳐지기 때문이에요. 다음 예제를 보죠.

```
@style: language-cpp verified
void foo() {
    if(isVerbose) BY_I_SCOPE("okay, verbose mode is on.");
    BY_I("hello");
}
```

위 코드는 다음과 같이 expand 됩니다:

```
@style: language-cpp verified
void foo() {
    if(isVerbose)
        BY_I("▶  okay, verbose mode is on.");
    ::by::line::incLv();
    BY_END(scopeLog, ...., "◀  okay, verbose mode is on.");
    BY_I("hello");
}
```

보시다시피 if 조건에 걸리는 건 시작 로그뿐이며, `incLv()`와 `BY_END`는 항상 실행됩니다. 이는 의도한
동작이 아니죠. 이런 경우에는 <b>반드시 중괄호로 블록을 명시</b>해야 합니다:

```
@style: language-cpp verified
// ✅ 올바른 사용
void foo() {
    if(isVerbose) {
        BY_I_SCOPE("okay, verbose mode is on.");
    }
    BY_I("hello");
}
```

---

## richLog - 다형성 로깅

@ref by::richLog "richLog" 는 서식문자에 입력된 argument를 다형성을 활용해서 적절한 타입으로 변환해서 로깅하는
강력한 기능입니다. 함수 오버로딩을 응용하여, 각 타입에 맞는 변환 함수를 자동으로 선택합니다.
쉽게, 예를들어 보죠.

```
@style: language-cpp verified
struct person {
    std::string name;
    int age;
};
struct address {
    std::string state;
    std::string city;
};

person p{"Harland D. Sanders", 59};
address a{"Henryville", "Indiana"};

BY_I("Hello I'm %s and %d years old. I live in %s, %s", p.name, p.age, a.city, a.state);
// 출력: Hello I'm Harland D. Sanders and 59 years old. I live in Henryville, Indiana
```

richLog를 사용한다면, 위의 로깅은 다음과 같이 작성할 수 있게 됩니다.

```
BY_I("Hello I'm %s. I live in %s", p, a);
// 출력: 위와 동일.
```

즉, 각 가변 인자의 타입을 보고 적절한 로깅 메시지를 생성하여 대신 전달하는 기능입니다.
물론 공짜는 없어요. 어딘가에는 Person타입에 대해 어떠한 메시지를 생성하겠다는 걸 추가해줘야 합니다.
하지만 그렇다곤 해도, 어떻게 이런 게 가능할까요.

### 핵심 알고리즘

@startuml
package "richLog System" {
    abstract class "convert(T)" as convert {
        + {static} convert(tstr<obj>&) : strWrap
        + {static} convert(int) : noWrap<int>
        + {static} convert(void*) : strWrap
        --
        <b>Compile-time Dispatching</b>
        Overloaded for each type
    }

    class strWrap {
        - _val : std::string
        + unwrap() : const char*
        --
        String Wrapper
        Returns c_str()
    }

    class "noWrap<T>" as noWrap {
        - _val : T
        + unwrap() : T
        --
        Passes value as is
        For scalar types
    }
}

convert ..> strWrap : Create
convert ..> noWrap : Create

@enduml

핵심은 <b>paramter pod을 활용한 변환함수 호출</b>과 <b>값 전달</b> 2가지 입니다.
먼저 `BY_I` 와 같은 매크로를 사용하면 다음의 richLog 함수를 호출하는데요.

```
@style: language-cpp verified
template <typename... Ts>
void richLog(errLv::level lv, const std::string& filename, const nchar* func, nint line, const nchar* fmt,
    const Ts&... args) {
    ::by::logger::get().log(lv, filename, func, line, fmt, __convert__((const Ts&) args).unwrap()...);
}
```

위와 같이 각각의 가변인자에 대해 __convert__()를 호출해 적절한 타입(예: 문자열)로 변환 되면 wrap<T> 객체가 반환되는데요,
wrap::unwrap()을 통해 log() 함수의 가변인자로 넣는 거죠.


### convert()

각 모듈에 속한 클래스를 어떻게 로깅하기 쉬운 타입으로 변경할지를 정의합니다. 호출자는
__convert__()에 자신이 받은 구체타입을 넣을 뿐이며, 오버로딩에 의해서 가장 적절한 타입에 대한
__convert__가 호출됩니다.

```
@style: language-cpp verified
noWrap<nflt> __convert__(nflt rhs) { return rhs; }
noWrap<nflt> __convert__(nflt* rhs) { return rhs ? __convert__(*rhs) : 0.0f; }
noWrap<const nchar*> __convert__(const nchar* rhs) { return rhs ? rhs : "null"; }
noWrap<ndbl> __convert__(ndbl* rhs) { return rhs ? __convert__(*rhs) : 0.0; }
strWrap __convert__(nbool rhs) { return strWrap(rhs ? "true" : "false"); }
strWrap __convert__(nbool* rhs) { return rhs ? __convert__(*rhs) : strWrap("null"); }
...
```

만약 앞서 언급한 Person이나 Address에 대해서도 richLog를 가능하게 하려면 어떻게 할까요?

```
strWrap __convert__(person&& p) { return p.name + p.age + " years old"; }
strWrap __convert__(address&& a) { return a.city + ", " + a.state; }
```

생각보다 간단한가요?
위와 같은 설계의 장점은, 범용적인 __convert__()와 특정 타입에 특화된 __convert__()를 동시에 정의할 수 있다는
말입니다. 대표적으로는 __convert__(void*)가 있어요. 어떠한 T*에도 매칭 되지 않으면 이
__convert__가 대신 호출됩니다.

이처럼 __convert__가 필요한 이유는 쉽게 이해가 가능합니다. 근데 왜 wrap을 굳이 사용해야만 할까요?

### wrap

wrap이 필요한 이유는 richLog() 안쪽에서 __convert__()를 호출하고 이때 전달받은 값들을 가변인자를
통과할 수 있도록 풀어주는 역할을 합니다.

c++ 스펙상, 가변인자 함수는 scalar type이나 T*만 넘길 수 있기 때문에 값으로는 넘길 수 없습니다.
문제는 위의 __convert__(person&&)에서 봤듯이 상당수의 __convert__는  안에서 새로운 값을 만들어
값으로 넘겨야 한다는 거예요.
이때 wrap이 등장합니다. __convert__의 반환형을 보면 아시겠지만 wrap*로 넘기지 않죠. 값으로 넘깁니다.
그러니 richLog 함수템플릿 안에선, 임시 객체인 wrap이 생성되며, __convert__안에서 임시로 생성한
문자열 또한 안전하게 `const char* strWrap::unwrap()`을 호출하면서 char*를 가변인자 안으로 넘길 수 있습니다.

즉, 요약하면 @ref by::logger::log() "log()" 가 가변인자이므로 c++ 스펙상 임시객체를 값으로 넘길 수 없기
때문입니다.

__convert__() 함수들은 크게 `strWrap` 혹은 `noWrap<T>` 2가지 중 하나를 반환형으로 정의하는데
noWrap은 아무런 가공없이 받은 걸 그대로 반환하지만 strWrap은 std::string::c_str()를 내부적으로
호출합니다.

기본적으로는 @ref by::strWrap "strWrap" 을 사용하면 되지만, richLog 사용 시 format string에 `%d`, `%f` 등 사용자가
직접 타입을 명시하는 서식문자를 사용하고 싶을 때는 noWrap을 사용합니다. 예를 들어:

```
@style: language-cpp verified
// strWrap: 자동으로 문자열로 변환 (%s 사용)
strWrap __convert__(const myClass& obj) { return strWrap(obj.toString()); }

// noWrap: 원본 타입 유지 (%d 등 명시적 서식문자 사용 가능)
noWrap<int> __convert__(int val) { return noWrap<int>(val); }
```



### richLog 확장

@ref clog 모듈은 architecture 상 아랫부분에 위치하기 때문에 @ref clog 에 종속하는 클래스가 뭐가 있는지 알아서는
안됩니다. 그렇기 때문에 @ref by::richLog "richLog" 는 각 모듈마다 정의되어 있으며, 해당 모듈에 포함된 클래스를 어떻게
문자열로 변환할지를 추가로 정의해두고 있어요.
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

이렇게 정의하면 로깅 매크로에서 해당 타입을 직접 사용할 수 있게 되는거죠.

```
@style: language-cpp verified
type& t = ttype<myClass>();
BY_I("type is %s", t);  // __convert__(const type&)가 호출되어 적절히 문자열로 변환됨
```


### 간략화된 주소값

@ref by::richLog "richLog" 로 void*를 넘기게 되면 @ref indep 모듈에 있는 @ref by::platformAPI "platformAPI" 를 사용해서 `toAddrId()`를 호출합니다.
addrId란 void*의 주소값의 마지막 4 hex값을 문자열로 표현한 값을 말합니다.
프로젝트 내에서 어느 주소에 있는 객체인지를 구분하기 위해 매우 자주 사용되니 꼭 알아두세요.
로깅할때는 항상 `@` 뒤에 addrId를 적는 코딩 컨벤션을 가지고 있습니다. 예를 들면 다음과 같이 로그를 남깁니다.

```
@style: language-txt verified
Nov 18 2025  20:02:13 I verifier  <onLeave#87> '' assignExpr@9a50: step#1 --> set evalType
```

위 로그에서 assignExpr@9a50은 assignExpr 객체가 heap 주소의 끝자리가 9a50인 곳에 있는 인스턴스라는
얘기입니다. 이는 동일한 타입에 대해 서로 다른 인스턴스가 같은 시점에 로깅되는 경우 좀 더 수월하게
디버깅 하도록 도와주는 용도예요.

---

## 스트림 제어

### enablesZone 클래스

@ref by::logger "logger" 클래스는 여러개의 @ref by::stream "stream" 을 가지고 있고 특정 @ref by::stream "stream" 을 disable 혹은 enable 함으로써
출력되는 경로를 제어할 수 있어요.

여기서 문제는 특정 코드 블록 혹은 함수에서만 @ref by::stream "stream" 을 제어 한 후, 블록을 벗어날 때는 원래 상태로
되돌리고자 하는 경우가 매우 자주 일어납니다.

@ref by::enablesZone "enablesZone" 은 이럴때 사용하는 것으로, RAII idiom으로 구현되어 있어서, 객체 생성과 동시에 @ref by::stream "stream" 의
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

---

## 필터링 시스템

로깅은 디버깅에서는 아주 유용한 도구이지만, unittest나 release 빌드에서는 필요없는 정보를 잔뜩 남겨서 혼란스럽게 하기도 합니다.
개발중에는 이처럼, 로깅되는 메시지를 보여줬다가 감췄다가 할 필요가 있는데, 필터링은 각 로그의 상세정보를 조건으로
로그를 걸러주는 역할을 합니다.

필터링 시스템은 Strategy 패턴을 사용하여 구현되었습니다. 각 필터는 교체 가능한 알고리즘으로 동작하며, 런타임에 원하는 필터를 선택하여 적용할 수 있습니다.

### filterable 클래스

각 필터는 filterable 인터페이스를 구현해야하는데요, @ref by::filterable "filterable"은 `filt()` 함수를 제공하며,
filt() 함수 안쪽에서 parameter로 주어진 Log 정보에 대해 판단후, true를 반환할 경우 해당 Log는 @ref by::stream "stream"
에 올라가지 않고 무시됩니다.

@ref by::logger "logger" 클래스의 `setFilters(const filters&)`를 통해서 @ref by::filterable "filterable" 을 추가할 수 있습니다.

### filters 클래스

@ref by::filters "filters" 클래스는 @ref by::logger "logger" 클래스에서 등록된 @ref by::filterable "filterable" 클래스들을 관리합니다. @ref by::filterable "filterable" 과
동일한 API를 가지며, 해당 API를 호출하면 소유한 모든 @ref by::filterable "filterable" 에 해당 API를 호출합니다.
이는 Composite 패턴으로 구현되어, 여러 필터를 하나의 필터처럼 다룰 수 있습니다.

### errPassFilter 클래스

말 그대로 err만 통과시키는 @ref by::filterable "filterable" 클래스입니다. 이 filter를 @ref by::logger "logger" 에 등록하면 warning이나
info는 출력되지 않습니다. 직접 사용하지 않으며, 객체 생성하여 @ref by::logger "logger" 에 add 하는 용도로 사용합니다.

동작은 매우 단순한데요, `errLv`이 ERR일때만 통과시키는 필터입니다. 다음과 같이 사용할 수 있습니다.

```
@style: language-cpp verified
const filters& prevFilters = logger::get().getFilters();
filters fs(new errPassFilter());
logger::get().setFilters(fs);

// doSomething..

logger::get().setFilters(prevFilters);
```
---

## 정리

자, 시퀸스 다이어그램으로 지금까지의 내용을 정리해보죠.

@startuml
participant "User Code" as user
participant "BY_I Macro" as macro
participant "richLog" as richLog
participant "convert()\n(Overload)" as convert
participant "wrap" as wrap
participant "logger" as logger

user -> macro : BY_I("obj: %s, val: %d", meObj, count)
activate macro

note right of macro
  Macro Expansion:
  richLog("obj: %s, val: %d",
          meObj, count)
end note

macro -> richLog : richLog(format, meObj, count)
activate richLog

richLog -> convert : convert(meObj)
activate convert

note right of convert
  <b>Calls appropriate overloaded convert():</b>
  Ex: meObj is tstr<obj>* type

  Calls convert(tstr<obj>*).
  (Calls convert(void*) if absent)

  Returns strWrap(obj.getName()).
end note

convert --> richLog : strWrap("obj")
deactivate convert

richLog -> convert : convert(count)
activate convert

note right of convert
  <b>Calls appropriate overloaded convert():</b>

  count is int type.
  Calls convert(int).

  return noWrap<int>(count)
end note

convert --> richLog : noWrap<int>(count)
deactivate convert

richLog -> wrap : strWrap.unwrap()
activate wrap
wrap --> richLog : "obj"
deactivate wrap

richLog -> wrap : noWrap.unwrap()
activate wrap
wrap --> richLog : 3 (value of count)
deactivate wrap

richLog -> logger : log(level, "obj: %s, val: %d", "obj", 3)
activate logger

note right of logger
  Variadic Function Call:
  - All args are scalar or pointer
  - printf style formatting available
end note

logger -> logger : Check filters
logger -> logger : Output to streams

logger --> richLog : void
deactivate logger

richLog --> macro : void
deactivate richLog

macro --> user : void
deactivate macro

note right of user
  <b>Output Result:</b>
  Oct 22 2025  22:01:12 I obj: obj, val: 42
end note

@enduml

---

<b>다음 문서</b>: @ref ah-architecture-meta
