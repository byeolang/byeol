# frontend 모듈 - CLI 인터페이스 {#aj-architecture-frontend}

@ref frontend모듈은 Byeol 언어의 명령줄 인터페이스(CLI)를 제공합니다. 이 모듈은 @ref core모듈의 기능들을 조합하여 사용자가 Byeol 프로그램을 실행할 수 있도록 합니다.

<b>사용된 디자인 패턴:</b> <b>Facade 패턴</b> (cli가 core 모듈 조합), <b>Template Method 패턴</b> (flag의 take 구조), <b>Strategy 패턴</b> (flag별 다른 동작), <b>Chain of Responsibility 패턴</b> (flag 매칭 시도), <b>Command 패턴</b> (각 flag가 command)

---

## cli 클래스 - Frontend의 핵심

@ref by::cli "cli" 클래스는 @ref frontend모듈의 핵심 클래스로, @ref core모듈의 @ref by::interpreter "interprete" 를 사용해서 코드를 파싱하고 검증하고 평가하는 일련의 과정들을 위해 적절한 클래스를 호출해서 제어합니다.

즉, @ref by::cli "cli" 는 뭔가 알고리즘을 만들어서 동작하는 클래스가 아니라 <b>이미 잘 짜여진 클래스들을 조합하는 역할</b>을 해요.

파라메터로 @ref by::cli "cli" 프로그램에 사용자가 입력한 인자를 받으며, 이를 적절히 파싱해서 추가로 명령을 수행합니다. 자세한 내용은 `flags` 폴더를 참조하세요.

이는 <b>Facade 패턴</b>의 전형적인 구현입니다. cli는 복잡한 core 모듈의 여러 클래스들(interpreter, errReport, starter 등)을 조합하여 단순한 인터페이스로 제공합니다. 사용자는 cli만 호출하면 되고, 내부의 복잡한 과정은 알 필요가 없습니다.

**핵심 알고리즘**

사용자가 파라메터로 flagArgs를 줘서 eval()을 호출하면 다음과 같이 실행됩니다:

1. `interpreter`, `errReport`, `starter` 객체를 생성한다.
2. interpreter, starter에 flag를 set한다. 이 `tworker`의 flag를 말하는 것으로, cli의
   `flag`와 다른 것이다. 자세한 내용은 @ref by::tworker "tworker" 를 참조.
3. flagArgs를 파싱해서 사전 작업 수행 - flag 객체에 `take()` 함수를 호출해서 수행한다.
4. @ref by::interpreter "interpreter" 를 수행한다.
5. 인터프리트 결과를 체크한다. 이상이 있으면 결과를 내보내고 종료한다.
6. 이상이 없으면 @ref by::starter "starter" 에 검증된 AST를 넣고 실행한다.
7. @ref by::starter "starter" 의 결과를 반환한다.

@startuml
actor 사용자
participant "cli" as cli
participant "flagArgs" as flags
participant "interpreter" as ip
participant "errReport" as report
participant "starter" as st

사용자 -> cli : eval(flagArgs)
activate cli

cli -> ip : new interpreter()
cli -> report : new errReport()
cli -> st : new starter()

cli -> ip : setFlag(tworker 플래그)
cli -> st : setFlag(tworker 플래그)

cli -> flags : 파싱 및 take()
activate flags
loop 각 플래그마다
    flags -> flags : 정규식 패턴 매칭
    alt 패턴 매칭됨
        flags -> cli : _onTake() 실행
        note right
          사전 작업 수행
          (예: bufSupply 추가)
        end note
    end
end
deactivate flags

cli -> ip : work()
activate ip
ip -> ip : 소스 파싱
ip -> ip : 타입 확장
ip -> ip : AST 검증
ip --> cli : 반환
deactivate ip

alt 검증 실패
    cli -> report : 에러 덤프
    cli --> 사용자 : 에러와 함께 종료
else 검증 성공
    cli -> st : setTask(검증된 AST)
    cli -> st : work()
    activate st
    st -> st : main() 실행
    st --> cli : 결과 반환
    deactivate st

    cli --> 사용자 : 결과 반환
end

deactivate cli
@enduml

---

## flag 클래스 - 명령줄 플래그 처리

shell 기반 프로그램에서 흔히 볼 수 있는 플래그들을 처리하는 클래스예요.

한가지 착각하기 쉬운 포인트는 이 클래스는 `--version`과 같은 플래그를 표현하는 클래스가
아니라 <b>그러한 플래그가 존재하는지, 존재한다면 어떤 동작을 해야 하는지를 담당</b>한다는
것입니다.

이는 여러 디자인 패턴의 조합입니다:
- <b>Template Method 패턴</b>: `take()` 메서드가 `_getRegExpr()`로 패턴을 얻고 `_onTake()`를 호출하는 고정된 알고리즘 구조
- <b>Strategy 패턴</b>: 각 flag 파생 클래스가 `_onTake()`를 다르게 구현하여 서로 다른 동작 정의
- <b>Command 패턴</b>: 각 flag가 하나의 독립적인 command로 동작하며, 실행 시점에 필요한 모든 정보를 캡슐화

**Flag의 설명**

각 @ref by::flag "flag" 는 자신의 이름 뿐만 아니라 어떠한 기능인지 description을 정의해두고 있습니다.
이들은 `helpFlag`에 의해서 사용됩니다.

**정규식에 의한 패턴매칭**

각 @ref by::flag "flag" 클래스는, 자신이 찾고자 하는 패턴이 정해져 있습니다.

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

이 동작은 @ref by::flag "flag" 의 `take()`가 호출되면 파생클래스의 `_getRegExpr()`에 정의해둔 정규식 표현으로 각 @ref by::flag "flag" 객체가 원하는 패턴을 찾는 형태로 동작합니다. 정규식으로 찾기 때문에
flag 간 순서는 무시됩니다.

여러 flag들이 순차적으로 매칭을 시도하는 것은 <b>Chain of Responsibility 패턴</b>입니다. 각 flag는 자신이 처리할 수 있는 패턴인지 확인하고, 해당되지 않으면 다음 flag로 넘어갑니다.

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

위에서 예로 들었던 @ref by::verFlag "verFlag" 를 다시 봅시다. `--version`이라는 문자열이 프로그램 인자에 있는 경우, 버전을 출력합니다. 단, 이때 또 다시 @ref by::verFlag "verFlag" 가 동작하지 않도록, 동작을 마치면 `--version`이라는 문자열을 @ref by::flagArgs "flagArgs" 에서 제거합니다.

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

이처럼 어떤 @ref by::flag "flag" 는 패턴과 일치하는 부분만을 consume하지 않습니다. `--script`을 찾으면
거기서 추가적으로 1개의 인자를 더 뜯어내서 `bufferSrcFlag::_onTake()`로 전달해 함수의
본문이 _onTake()로 전달되도록 해야 합니다.

각 @ref by::flag "flag" 의 파생클래스는 원할 경우, `getArgCount()`를 오버라이드해서 몇개의 인자를 더 뜯어낼 것인지를 명시합니다:

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

@ref by::bufferSrcFlag "bufferSrcFlag" 는 프로그램 시작 전에 사전 작업을 필요로 하는 @ref by::flag "flag" 입니다. 반면 @ref by::verFlag "verFlag" 같은 경우는 일단 매칭이 되면 어떠한 인터프리팅도 하지 않고 그대로 버전을 출력하고 종료합니다. (대다수 프로그램이 이렇게 동작한다는 걸 알고 있을 것입니다.)

이처럼 @ref by::flag "flag" 의 패턴이 매칭이 되면 동작을 하고 바로 종료하고 싶을 때는, `_onTake()`를 오버라이딩할 때 반환값을 <b>EXIT_PROGRAM</b>으로 줍니다. @ref by::bufferSrcFlag "bufferSrcFlag" 처럼 계속 동작을 하는 경우에는 <b>MATCH</b>로 반환합니다.

<b>flag 클래스 계층도:</b>

@startuml
abstract class flag {
    + take(flagArgs, cli, interpreter, starter) : res
    # {abstract} _getRegExpr() : strings&
    # {abstract} _onTake(flagArgs, cli, interpreter, starter) : res
    # getArgCount() : ncnt
}

note right of flag
  Template Method 패턴:
  take()가 알고리즘 골격 정의
  _getRegExpr(), _onTake()는
  하위 클래스가 구현
end note

class verFlag {
    # _getRegExpr() : strings&
    # _onTake() : res
    --
    정규식: "^\\\\--version$"
    반환: EXIT_PROGRAM
}

class helpFlag {
    # _getRegExpr() : strings&
    # _onTake() : res
    --
    정규식: "^\\\\-h$", "^\\\\--help$"
    반환: EXIT_PROGRAM
}

class bufferSrcFlag {
    # _getRegExpr() : strings&
    # _onTake() : res
    # getArgCount() : ncnt
    --
    정규식: "^\\\\--script$"
    인자 개수: 1
    반환: MATCH
}

class logStructureFlag {
    # _getRegExpr() : strings&
    # _onTake() : res
    --
    정규식: "^\\\\-S$", "^\\\\--show-structure$"
    반환: MATCH
}

class fileSrcFlag {
    # _getRegExpr() : strings&
    # _onTake() : res
    # getArgCount() : ncnt
    --
    정규식: "^[^\\\\-].*\\\\.byeol$"
    반환: MATCH
}

flag <|-- verFlag
flag <|-- helpFlag
flag <|-- bufferSrcFlag
flag <|-- logStructureFlag
flag <|-- fileSrcFlag

note bottom of verFlag
  버전 정보 출력 후
  프로그램 즉시 종료
end note

note bottom of bufferSrcFlag
  추가 인자 1개 소비
  코드 문자열을 bufSupply로 추가
end note

@enduml

<b>정규식 매칭 프로세스:</b>

@startuml
start

:cli가 flag들에게 take() 호출;

repeat
  :다음 flag 선택;

  :flag._getRegExpr()로\n정규식 패턴 목록 획득;

  repeat
    :다음 정규식 패턴 선택;

    :flagArgs에서\n패턴 매칭 시도;

    if (패턴 매칭?) then (예)
      :매칭된 인자 추출;

      if (getArgCount() > 0?) then (예)
        :추가 인자들도 추출;
      endif

      :flag._onTake(추출된 인자들) 호출;

      :flagArgs에서\n매칭된 인자들 제거;

      if (_onTake() 반환값?) then (EXIT_PROGRAM)
        stop
      else (MATCH)
        :다음 flag로 계속;
      endif
    endif

  repeat while (남은 패턴 있음?)

repeat while (남은 flag 있음?)

:모든 flag 처리 완료;

stop
@enduml

---

# 긴 여정을 함께하느라 고생하셨습니다.

지금까지 byeol 언어의 아키텍처, 설계 그리고 핵심 클래스들을 설명했습니다.
가급적 예제를 직접 넣긴 했지만 그래도 완전히 이해하기에는 많이 부족했을 거라 생각되네요.
역시 좀 더 깊이 이해하고 싶다면, 실제 구현코드와 함께 `test` 모듈의 unit test 코드들을 직접 읽어보는 것을 강력히 권장합니다.

각 모듈별로 작성된 테스트 케이스들은 실제 사용 예제를 담고 있으며, 특정 기능이 어떻게 구체적으로 동작하는지 가장 명확하게 보여줍니다.
코드 기여하기 전에 관련 테스트들을 먼저 살펴보시면 이해에 큰 도움이 될 것입니다.

-- Nov 2025, kniz

---

**다음 문서**: @ref ak-testing-guide
