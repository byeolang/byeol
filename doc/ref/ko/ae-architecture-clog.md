# clog 모듈 - 로깅 시스템 {#ae-architecture-clog}

@ref by::clog "clog" 모듈은 여러 출력 스트림과 필터링 기능을 갖춘 경량화된 C++ 로깅 프레임워크를 제공합니다.
이 모듈은 아키텍처 상 하위 계층에 위치하므로, 상위 모듈에 대한 의존성이 없습니다.

---

## 로깅의 기본 사용법

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

기본적인 사용법은 여기까지만 알아도 충분해요.

---

## 로깅 시스템 아키텍처

@ref by::clog "clog" 모듈의 핵심은 @ref by::stream "stream" 입니다. @ref by::stream "stream" 은 쉽게 말해 로깅이 출력되는 스트림, 즉 목적지를
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
다시 명시적으로 `init()`을 호출하지 않으면 정상적으로 동작하지 않아요.

stream은 또한 enable 여부를 관리합니다. setEnable(false)를 통해 특정 stream을 disable 시키면 해당
stream은 동작하지 않아요.

또하나 중요한 점은 @ref by::logger "logger" 클래스 자체도 @ref by::stream "stream" 에서 상속하기 때문에 @ref by::stream "stream" 과 동일한 API를 제공한다는
점입니다. @ref by::logger "logger" 는 각 API에 대해 소유한 모든 @ref by::stream "stream" 들에 대해 redirection 하는 구성으로 구현되어
있습니다. 예를들어 `logger::get().setEnable(false)`를 하게 되면, 모든 @ref by::stream "stream" 이 disable 됩니다.

@ref by::stream "stream" 은 logBypass(const nchar*) 라는 함수를 제공하는데, 이것은 어떠한 가공도 없이 문자열을 그대로
지정한 @ref by::stream "stream" 으로 로그 메시지를 보냅니다.

**stream 제어 예제**

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

## 로깅 매크로 시스템

앞서 언급한 `logByPass()`를 통해서 직접 로깅을 해도 되지만, 파일명이나 함수명, 라인번호 등 좀 더
고급스러운 로깅을 하고 싶다면 로깅 매크로를 사용하면 되죠.

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

---

### 간략화된 주소값

@ref by::richLog "richLog" 로 void*를 넘기게 되면 @ref indep 모듈에 있는 @ref by::platformAPI "platformAPI" 를 사용해서 `toAddr()`를 호출합니다.
이 함수는 void*를 마지막 4 hex값을 문자열로 변환하는 함수이며, 프로젝트 내에서 주로 `@` 뒤에 적는
스타일입니다. 예를 들면 다음과 같이 로깅 됩니다.

```
@style: language-txt verified
Nov 18 2025  20:02:13 I verifier  <onLeave#87> '' assignExpr@9a50: step#1 --> set evalType
```

위 로그에서 assignExpr@9a50은 assignExpr 객체가 heap 주소의 끝자리가 9a50인 곳에 있는 인스턴스라는
얘기입니다. 이는 동일한 타입에 대해 서로 다른 인스턴스가 같은 시점에 로깅되는 경우 좀 더 수월하게
디버깅 하도록 도와요.

---

## richLog - 다형성 로깅

@ref by::richLog "richLog" 는 서식문자에 입력된 argument를 다형성을 활용해서 적절한 타입으로 변환해서 로깅하는
기능입니다.

@ref clog 모듈은 architecture 상 아랫부분에 위치하기 때문에 @ref clog 에 종속하는 클래스가 뭐가 있는지 알아서는
안됩니다. 그렇기 때문에 @ref by::richLog "richLog" 는 각 모듈마다 정의되어 있으며, 해당 모듈에 포함된 클래스를 어떻게
문자열로 변환할지를 정의해두고 있어요.

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

@ref by::richLog "richLog" 는 크게 __convert__ 부분과 wrap 부분으로 나뉘어져 있습니다.

### convert()

각 모듈에 속한 클래스를 어떻게 로깅하기 쉬운 타입으로 변경할지를 정의합니다. 호출자는
__convert__()에 자신이 받은 구체타입을 넣을 뿐이며, 오버로딩에 의해서 가장 적절한 타입에 대한
__convert__가 호출됩니다.

그 말은, 범용적인 __convert__()와 특정 타입에 특화된 __convert__()를 동시에 정의할 수 있다는
말입니다. 대표적으로는 __convert__(void*)가 있습니다. 어떠한 T*에도 매칭 되지 않으면 이
__convert__가 대신 호출됩니다.

### wrap

wrap이 필요한 이유는 richLog() 안쪽에서 __convert__()를 호출하고 이때 전달받은 값들을 가변인자를
통과할 수 있도록 풀어주는 역할을 합니다.

가변인자 함수는 scalar type이나 T*만 넘길 수 있기 때문에 값으로는 넘길 수 없습니다. 일부
__convert__()는 안에서 새로운 값을 만들어 값으로 넘겨야 하는 상황도 있을 수 있기 때문에
@ref by::logger::log() "log()" 가 가변인자로 구성되어 있는 한은 @ref by::wrap "wrap" 이 꼭 필요합니다.

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

### richLog 확장 예제

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

이렇게 정의하면 로깅 매크로에서 해당 타입을 직접 사용할 수 있어요:

```
@style: language-cpp verified
type& t = ttype<myClass>();
BY_I("type is %s", t);  // __convert__(const type&)가 호출되어 적절히 문자열로 변환됨
```

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

### filterable 클래스

@ref by::filterable "filterable" 클래스는 @ref by::logger "logger" 클래스가 특정한 조건에 해당하는 logging은 필터링 할 수 있게 해줍니다.
`filt()` 함수를 제공하며, parameter로 주어진 Log 정보에 대해 true를 반환할 경우 해당 Log는 @ref by::stream "stream"
에 올려지지 않습니다.

각 @ref by::stream "stream" 에 전달될 로그 메시지를, 특정한 조건으로 필터링 할 수 있게 해줍니다. @ref by::filterable "filterable" 은
`filt()` 함수를 통해, 메시지의 적합성을 판단해 필터링할 것인지를 결정합니다. @ref by::logger "logger" 클래스의
`setFilters(const filters&)`를 통해서 @ref by::filterable "filterable" 을 추가할 수 있습니다.

예를들어 @ref by::errPassFilter "errPassFilter" 는 `errLv`이 ERR일때만 통과시키는 필터입니다. 다음과 같이 사용할 수
있습니다.

```
@style: language-cpp verified
const filters& prevFilters = logger::get().getFilters();
filters fs(new errPassFilter());
logger::get().setFilters(fs);

// doSomething..

logger::get().setFilters(prevFilters);
```

### filters 클래스

@ref by::filters "filters" 클래스는 @ref by::logger "logger" 클래스에서 등록된 @ref by::filterable "filterable" 클래스들을 관리합니다. @ref by::filterable "filterable" 과
동일한 API를 가지며, 해당 API를 호출하면 소유한 모든 @ref by::filterable "filterable" 에 해당 API를 호출합니다.

### errPassFilter 클래스

말 그대로 err만 통과시키는 @ref by::filterable "filterable" 클래스입니다. 이 filter를 @ref by::logger "logger" 에 등록하면 warning이나
info는 출력되지 않습니다. 직접 사용하지 않으며, 객체 생성하여 @ref by::logger "logger" 에 add 하는 용도로 사용합니다.

---

**다음 문서**: @ref af-architecture-meta
