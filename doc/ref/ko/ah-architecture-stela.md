# stela 모듈 - 경량 설정 언어 {#ah-architecture-stela}

@ref stela 모듈은 byeol 언어의 경량화된 버전으로, manifest나 옵션과 같은 특수 목적용 언어입니다. 트리 구조로
설정 데이터를 표현하며, @ref core 모듈의 @ref by::node "node" 와 유사한 구조를 가집니다.

**참고**: @ref stela 언어는 byeol 언어보다 덜 복잡하므로, **@ref core 모듈의 byeol 파서를 보기 전에 @ref stela 파서
코드를 먼저 살펴볼 것을 권장합니다**. 파서의 기본 구조와 동작 방식을 이해하는데 더 적합합니다.

---

## stela 언어의 기본 기능

### stela 클래스

@ref by::stela "stela" 클래스는 @ref stela 모듈의 가장 기본 단위가 되는 클래스로, 다음의 기능을 제공합니다:

1. **타입 변환 API**: asInt(), asChar(), asStr(), asBool() 등 기본 타입으로 변환을 시도합니다.
2. **값이 있는 경우**: @ref by::valStela "valStela" 의 API가 실행되며, 적절한 값으로 변환됩니다. 예를들어 `verStela(22)`의
   경우 `asStr()`을 하면 `std::string("22")`가 반환됩니다.
3. **값이 없는 경우**: @ref by::nulStela "nulStela" 로 표현됩니다. 이 경우 어떠한 타입변환 시도에도 기본값(빈 문자열 혹은
   0)이 반환됩니다.
4. **버전 타입**: major, minor, patch 버전을 가지고 있으며, 범위로도 표현이 가능합니다.
5. **트리 구조**: @ref by::stela "stela" 는 또다른 @ref by::stela "stela" 를 자식으로 둘 수 있습니다. 각 @ref by::stela "stela" 객체마다 이름이 존재하므로,
   자식을 찾을 때는 이름으로 검색하거나 순회합니다. 주어진 이름에 맞는 자식이 없는 경우 @ref by::nulStela "nulStela" 가
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

### nulStela 클래스 - Null Object 패턴

@ref by::nulStela "nulStela" 는 @ref by::stela "stela" 로 자식 객체를 가져올 때 보통 이름으로 가져오는데, 해당 이름을 가진 자식 객체가 없을
경우 대신 반환됩니다.

@ref by::nulStela "nulStela" 는 <b>null object 패턴</b>을 구현한 것으로, 해당 객체에 대해 값 변환을 요청할 경우 항상 해당
타입의 기본값이 반환됩니다. @ref by::stela "stela" 객체가 @ref by::nulStela "nulStela" 인지 확인하려면 `isExist()` 혹은
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

### valStela 클래스 - Scalar 값 표현

@ref by::valStela "valStela" 는 @ref by::nulStela "nulStela" 와 달리 int, float, string 등 <b>scalar 타입</b>을 가지고 있는 @ref by::stela "stela" 입니다.
내부적으로는 기본 문자열로 값을 가지고 있는 상태이며, `asInt()`와 같은 타입변환 요청에 따라서 적절한
타입으로 변경할 수 있습니다.

만약 타입변환에 실패한다면, 예외가 발생하니 주의하세요.

### verStela 클래스 - 버전 타입

@ref by::verStela "verStela" 는 @ref by::valStela "valStela" 와 비슷하게 <b>version</b>이라는 타입의 값을 가지고 있는 @ref by::stela "stela" 입니다.

**version 타입**

@ref by::stela "stela" 언어는 byeol 언어의 경량화된 언어로, manifest나 옵션과 같은 특수 목적용 언어입니다. version
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

---

## stela 파서 구조

### stelaParser 클래스 - 파싱 진입점

@ref by::stelaParser "stelaParser" 클래스는 stela 파싱 컴포넌트의 진입점 역할을 합니다. `parse()` 나 `parseFromFile()`을
통해서 스크립트를 지정하면 파싱된 결과가 @ref by::stela "stela" 구조로 반환됩니다.

**byeol 파서와 유사한 구조**

@ref stela 언어 자체가 byeol의 특화된 언어이므로 파서 또한 byeol 언어의 파서를 기반으로 하고 있습니다.
byeol 파서 대비 덜 복잡하여 파서의 기본 동작을 이해하기 좋습니다.

**scanner - bison - stelaParser 구조**

flex와 bison을 사용하고 있으며 flex는 lowscanner로, bison은 lowparser로 각각 명명합니다. 이
lowlevel scanner, parser는 parser 컴포넌트 안에만 존재하는 것으로 외부에서는 일절 노출 되지 않습니다.

@ref by::stelaParser::parse() "parse()" 가 실행되면 lowscanner를 실행시키고, lowscanner는 토큰을 뜯어서 lowparser에게
넘기고, lowparser는 받은 토큰에 대해 rule이 match 되면 그 이벤트를 다시 @ref by::stelaParser "stelaParser" 에게 넘깁니다.

그러므로 @ref by::stelaParser "stelaParser" 의 <b>`on`으로 시작하는 함수들</b>은 그러한 이벤트를 handling 하는 함수로, 실제로
어떻게 node를 생성해서 ast를 구축하는지를 정의합니다.

---

## Indentation 처리

### indentation rule

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

### tokenDispatcher

flex는 yyin 이라는 별도로 지정된 stream을 통해서 글자를 가져와 token으로 정의합니다. 위의 예제를
보다시피, 파싱 도중에 토큰을 추가한다는 것은 이 stream에 특정 문자를 추가하는 것을 의미합니다.

기본적으로 이런 경우는 unput을 사용하나, 여러개를 unput 하거나 뒤가 아니라 앞에 push 하는 경우 등에
유연하게 대응하기 위해, stelaLowscanner는 내부적으로 @ref by::stelaTokenDispatcher "stelaTokenDispatcher" 를 사용합니다.

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

### stelaTokenScan 클래스 - 스캔 모드 전략

@ref by::stelaParser "stelaParser" 는 indentation을 정밀하게 측정하기 위해서 <b>scan mode를 동적으로 변경</b>해야 합니다.
tokenScan은 그러한 스캔 모드 전략 1개를 담당합니다.

**tokenScan의 동적 전환**

앞서서 indentation을 탐지하는 게 얼마나 중요한지 설명했습니다. 정확한 공백의 갯수를 셈해야 하기 때문에
`normalScan`과 `indentScan` 2개의 scan 객체를 전환해가며 사용합니다.

개행이 탐지되면 indentScan으로 교체해서 정확하게 공백을 count해서 scope를 결정하고, 이후에는
normalScan으로 교체해서 평상시처럼 공백을 다 무시합니다.

**indentation 검사**


**명령 token**

token 중에는 `SCAN_AGAIN`, `SCAN_EXIT` 등 scanner나 parser에 명령을 주는 토큰들이 존재합니다.
자세한 내용은 @ref by::stelaParser "stelaParser" 의 zztokenType enum을 참조하세요.

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

### stelaSmartDedent 클래스 - Scope 관리

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

그래서 @ref by::stelaSmartDedent "stelaSmartDedent" 가 나옵니다. 위와 같이 inline block을 블록을 사용하되, 콤마로 끝나는
경우는 개행을 추가해주는 아주 단순하지만 parser의 rule의 난이도를 낮추는 역할을 합니다.

---

**다음 문서**: @ref ai-architecture-core
