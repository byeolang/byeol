# core 모듈 - 언어 구현의 핵심 {#ai-architecture-core}

@ref core 모듈은 Byeol 프로그래밍 언어의 핵심 구현을 담당합니다. <b>Interpreter 패턴</b>과 <b>Tree-walking interpreter</b> 방식을 사용하여 AST(Abstract Syntax Tree) 구조, 파서, 검증기, 실행기 등 언어의 모든 핵심 기능이 이 모듈에 집중되어 있습니다.

<!-- TODO: Tree-walking interpreter의 AST 직접 실행 구조를 보여주는 다이어그램 추가 -->

@ref core 모듈의 가장 큰 특징은 <b>Interpreter 패턴을 사용하여 AST를 직접 실행</b>한다는 점이에요. 일반적인 언어와 달리, Byeol 언어는 AST 구조를 유지한 채로 프로그램을 실행합니다. 따라서 타 언어의 AST는 말그대로 문법 구조를 트리로 표현한 중간결과물에 지나지 않지만, Byeol에는 실행가능한 최종 output을 AST가 담당합니다.

---

## AST 기본 구조

Byeol의 AST는 실행 가능한 프로그램 트리입니다. 일반적인 AST와 달리 평가(evaluation), 타입 추론(type inference), 프레임 관리 등 실행에 필요한 모든 기능을 포함합니다.


### node 클래스 - AST의 기반

@ref by::node "node" 클래스는 AST의 가장 기본이 되는 클래스예요. AST 구조와 관련된 API와 해당 @ref by::node "node" 를 평가하는 `eval()`, 자식 @ref by::node "node" 를 탐색하는 API, 그리고 해당 @ref by::node "node" 객체를 다른 타입으로 형변환하는 API 등을 제공합니다.

<b>AST 탐색</b>

<!-- TODO: Composite 패턴을 사용한 AST 트리 구조를 보여주는 다이어그램 추가 -->

AST 특성상, @ref by::node "node" 는 또 다른 @ref by::node "node" 의 파생클래스의 객체도 가지고 있을 수 있어야 합니다. 때문에 마치 DOM tree처럼 <b>Composite 패턴</b>을 사용해서 설계되어있으며 이 tree를 탐색하는 함수 또한 다양하게 지원하고 있습니다.

이 tree를 byeol 언어에서는 @ref by::scope "scope" 라고 표현하고 있으며 @ref by::scope "scope" 은 map을 기반으로 합니다. @ref by::blockExpr "blockExpr" 같은 것은 @ref by::node "node" 이면서도 내부에 statement 뭉치를 array로 가지고 있습니다. 따라서 AST는 전체적으로 보면 <b>map과 array가 혼합된 구조</b>로 구성되어 있음을 알 수 있습니다.

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

<b>중복 symbol 처리</b>

byeol 언어는 동일한 scope내 중복 symbol을 허용하지 않습니다. 하지만 이 말은 동일한 key로 2개의 pair가 들어갈 수 없다는 뜻은 아닙니다.

함수의 경우는 이름이 같을 지라도 파라메터의 갯수나 타입이 다르면 다른 symbol이 되기 때문입니다. 이는 <b>함수 오버로딩(Function Overloading)</b>을 지원하기 위한 설계입니다. 따라서 단순히 string 비교만으로 중복여부를 판단할 수 없기에 @ref by::scope "scope" 클래스는 map 기반이 아니라 <b>multimap 기반</b>으로 되어 있어요. 같은 이름의 함수들을 모두 저장한 후, 호출 시점에 인자 타입을 기준으로 가장 적합한 함수를 선택하는 <b>Overload Resolution 알고리즘</b>을 사용합니다.

<b>eval() - 평가와 실행</b>

@ref by::node "node" 는 함수일수도 있고, 객체일 수도 있고, 표현식일 수도 있습니다. `eval(const args&)`은 이러한 @ref by::node "node" 를 실행한 결과를 반환합니다. (byeol에는 클래스란 개념이 없고, 객체가 이를 대신합니다. 그러니 객체 또한 함수처럼 실행할 수 있으며, 이는 생성자가 호출되는 것과 같습니다.)

<b>infer() - 타입 추론</b>

`infer()`는 <b>Type Inference 알고리즘</b>을 수행합니다. 런타임의 정확한 값을 반환하는 것이 아니라 verification 단계에서 확정되는 타입을 객체로 반환합니다. 이는 실제 값 계산 없이 타입만 판단하는 알고리즘으로, `eval()`보다 훨씬 빠르게 동작합니다.

```
@style: language-cpp verified
// byeol 언어에서 `2 + 3.5` 표현하면 아래와 같다.
FBOExpr e = FBOExpr(FBOExpr::SYMBOL_ADD, *new nInt(2), *nFlt(3.5));

str infered = e.infer(); // int인 2와 flt인 3.5의 덧셈했을때의 타입은 뭐가 되어야 할까?
infered->cast<nFlt>() != nullptr; // true: 정답은 flt
```

int + flt는 type promotion에 의해 flt이 되므로 infered에는 nFlt 객체가 들어갑니다. 특정 표현식에 대해 타입을 유추하려면 해당 표현식이 가지고 있는 자식 node들에게 재귀적으로 타입유추를 호출해서 계산해야 합니다.

단, Type Inference 알고리즘은 어디까지나 타입을 빨리 아는 데 집중하므로 값의 계산은 수행하지 않습니다. 값을 정확하게 알고 싶다면 `eval()`을 해야만 합니다.

<b>eval() vs infer() 비교 - 두 가지 평가 전략</b>

| 항목 | eval() | infer() |
|------|--------|---------|
| <b>목적</b> | 실제 값을 계산하고 반환 | 결과 타입을 추론하여 반환 |
| <b>실행 시점</b> | 런타임 | Verification 단계 |
| <b>반환값</b> | 실제 계산된 값 (str) | 타입 객체 (str) |
| <b>값 계산</b> | O | X |
| <b>성능</b> | 느림 (실제 연산 수행) | 빠름 (타입만 판단) |
| <b>사용 사례</b> | 프로그램 실행, 실제 결과 필요 | 타입 체크, 컴파일 타임 검증 |

예시:
```
@style: language-cpp verified
FBOExpr e = FBOExpr(FBOExpr::SYMBOL_ADD, *new nInt(2), *nFlt(3.5));

str evaluated = e.eval();  // 5.5를 담은 nFlt 객체 반환 (실제 계산 수행)
str infered = e.infer();   // nFlt 타입 객체만 반환 (값 계산 안함, 타입만 판단)
```

<b>타입 변환 - as()와 is()</b>

@ref by::node "node" 는 명시적 타입 변환을 위한 `as()`와 타입 변환이 가능한지 체크하는 `is()`를 제공합니다.

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
    tstr<nFlt> converted = val OR.ret(); // Early Return Pattern
    doSomething(converted->get());
}
```

이는 <b>Early Return Pattern</b>을 사용하여 변환 실패 시 즉시 함수를 종료하는 방식입니다.

<b>Native vs Managed 타입 변환</b>

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
형변환이 동작하는 흐름을 알고 싶다면 @ref by::ases "ases" 와 @ref by::asable "asable" 을 참조하세요.

<b>묵시적 형변환과 명시적 형변환</b>

묵시적 형변환은 `impliAs()`, `impliIs()`로 제공됩니다. 이를 외부에서 직접 호출하는 일은
적습니다. 대부분은 `expr`이나 `baseFunc` 등 AST 클래스들에 의해서 호출됩니다.

참고로, byeol 사용자가 정의한 타입 변환은 당연히 묵시적 형변환에 포함되지 않습니다.
그래서 이 둘을 구분하는 것입니다.

<b>Side Funcs</b>

@ref by::node "node" 는 byeol 프로젝트의 핵심 모듈 중에서도 핵심이 되는 기본 클래스이기 때문에 매우
다양한 side func을 제공합니다. 대부분의 기능이 @ref by::node "node" 클래스를 통해 접근 가능하도록
설계되어 있습니다.

<b>Visitable Class - Visitor 패턴</b>

<!-- TODO: Visitor 패턴의 Double Dispatch 메커니즘을 보여주는 다이어그램 추가 -->

AST를 순회하는 동작 자체는 매우 다양한 목적으로 발생합니다. 단순히 `eval()`을 위해서만
아니라 디버깅 정보를 위해 AST를 출력할때도 사용됩니다.

순회 방식과 @ref by::node "node" 에 대한 처리를 분리시키기 위해 <b>Visitor 패턴</b>이 @ref by::node "node" 에 반드시 적용되어야
합니다. `accept()`는 바로 이 @ref by::visitor "visitor" 클래스에 의해 사용되며, <b>Double Dispatch</b> 기법을 통해 컴파일 타임에는 알 수 없는 구체 타입을 런타임에 올바르게 처리합니다. 각 클래스는 헤더에
이를 위해 VISIT 매크로가 추가로 필요합니다.

<b>Frame Interaction</b>

@ref by::frame "frame" 은 현재 실행 중인 scope와 안에 등록된 symbol 등을 관리합니다. @ref by::node "node" 는 AST의
기본을 구성하므로 @ref by::node "node" 가 객체이거나 함수일 경우, @ref by::frame "frame" 에 자신이 소유한 함수나 property
등을 등록하거나 해제하는 작업을 해줘야 합니다.

`inFrame()`과 `outFrame()`이 이를 담당하며, 자세한 내용은 @ref by::frame "frame" 을 참조하세요.

<b>Message Priority - 함수 우선순위</b>

byeol은 함수 오버로딩을 지원하므로 어떤 객체에는 같은 이름을 가졌으되, 파라메터가
다른 함수가 여럿 있을 수 있습니다. 어떤 함수는 인자에 대해서 정확하게 일치할 것이고,
어떤 함수는 약간의 타입변환이 필요할 지도 모릅니다.

<b>Overload Resolution 알고리즘</b>

@ref by::node "node" 는 자식 @ref by::node "node" 들에 대해서 주어진 args에 대해 얼마나 잘 일치하는 가를 `prioritize()`
함수를 통해 판단결과를 받습니다. 이 알고리즘은 C++의 Overload Resolution과 유사하게 동작하며, 다음 우선순위로 함수를 선택합니다:

1. <b>EXACT_MATCH</b>: 인자 타입이 정확히 일치
2. <b>IMPLICIT_MATCH</b>: 묵시적 타입 변환으로 일치 가능
3. <b>NO_MATCH</b>: 일치하지 않음

각 @ref by::node "node" 의 파생클래스는 args가 정확하게 일치하면
EXACT_MATCH로 반환하며, 일치하지 않으면 NO_MATCH로 반환됩니다.

정확한 판단기준이나 알고리즘은 @ref by::tprior "tprior" 과 @ref by::priorType "priorType" 을 참고하세요.

<b>prioritize() 사용 예제</b>

```
@style: language-cpp verified
// 함수 오버로딩 시나리오
// def print(val int) void
// def print(val str) void

obj myObj;
myObj.subs().add("print", new funcInt());   // print(int) 함수
myObj.subs().add("print", new funcStr());   // print(str) 함수

// int 인자로 호출
args intArgs;
intArgs.add(new nInt(42));

// 각 함수의 우선순위 계산
priorType prior1 = funcInt.prioritize(intArgs);   // EXACT_MATCH
priorType prior2 = funcStr.prioritize(intArgs);   // IMPLICIT_MATCH (타입 변환 필요)

// EXACT_MATCH가 더 높은 우선순위를 가지므로 funcInt가 선택됨
```


### src, srcFile 클래스 - 소스 위치 정보

@ref by::srcFile "srcFile" 은 AST의 출처가 되는 소스코드 파일에 대한 정보를 담습니다. @ref by::src "src" 는 생성된 AST @ref by::node "node" 가 어떠한 @ref by::srcFile "srcFile" 안의 어느 위치에서 생성된 것인지를 담고있습니다.

이 정보는 byeol 언어의 Context 기반 REPL의 핵심 기능에 사용됩니다. @ref by::src "src" 정보는 managed 환경에 국한하며, native에서 생성된 객체는 @ref by::src "src" 에 더미 값이 들어갑니다.


### args 클래스 - 함수 인자

함수나 객체 `eval()`을 위해 전달하는 인자를 표현합니다. @ref by::tnarr "narr" 을 상속하므로 @ref by::tnarr "narr" 의 모든 API를 사용할 수 있습니다.

<b>me 객체</b>

args는 `me` 객체가 포함되어 전달되는 게 특징입니다. 함수는 현재 런타임의 어떤 객체로부터 `eval()` 요청이 왔는지 기본적으로 알 수 없습니다. 별도로 `setMe()`를 통해 args를 사용하는 함수나 property는 어떤 객체로부터 이 args가 전달되었는지를 알 수 있습니다.

참고로, @ref by::baseObj "baseObj" 는 `eval(name, args)`를 호출받으면 `setMe(this)`를 호출해 자신을 args에 넣습니다.

<b>args 사용 예제</b>

```
@style: language-cpp verified
// 함수 호출을 위한 args 생성
args myArgs;

// 인자 추가 (narr 인터페이스 사용)
myArgs.add(new nInt(10));
myArgs.add(new nStr("hello"));
myArgs.len();  // 2

// me 객체 설정
obj* caller = new obj();
myArgs.setMe(*caller);

// 함수 호출 시 args 전달
func myFunc(*new modifier(), typeMaker::make<func>(params(), new nInt()));
str result = myFunc.eval(myArgs);

// args는 narr을 상속하므로 배열 접근 가능
node& firstArg = myArgs[0];  // nInt(10)
node& secondArg = myArgs[1];  // nStr("hello")
```


### param 클래스 - 파라메터 정의

param은 말 그대로 파라메터를 표현합니다. 이름과 타입을 표현하기 위한 origin 객체에 대한 참조가 있습니다.

<b>params 사용 예제</b>

```
@style: language-cpp verified
// 함수 타입 정의에 params 사용
// int add(x int, y int) int

// 파라메터 정의
params funcParams;
funcParams.add(new param("x", new nInt()));  // x: int
funcParams.add(new param("y", new nInt()));  // y: int

// 함수 타입 생성
const type& funcType = typeMaker::make<func>(funcParams, new nInt());

// 파라메터 정보 확인
funcParams.len();  // 2
param& firstParam = funcParams[0];
firstParam.getName();  // "x"
firstParam.getOrigin().getType();  // ttype<nInt>()
```

---

## 객체 모델

Byeol의 객체 모델은 native(C++)와 managed(Byeol 언어) 환경을 모두 지원하도록 설계되었습니다. 두 환경의 객체가 동일한 인터페이스로 사용될 수 있으며, 이를 통해 seamless한 상호작용이 가능합니다.

<b>사용된 디자인 패턴:</b> <b>Polymorphism</b> (native/managed 객체를 baseObj로 통일), <b>Prototype 패턴</b> (origin 객체를 clone하여 인스턴스 생성), <b>Adapter/Bridge 패턴</b> (tbridger를 통한 native-managed 브리징), <b>Flyweight 패턴</b> (shares/owns 구분으로 메모리 효율성)

TODO: 객체 모델 구조를 보여주는 클래스 다이어그램 추가 필요


### baseObj 클래스 - 객체의 기반

@ref by::baseObj "baseObj" 는 byeol의 객체를 표현하는 데 있어서 기반 클래스입니다. @ref core 모듈은 byeol 언어로 작성된 managed 환경에서의 객체(@ref by::obj "obj")와 C++ 코드를 사용해서 작성된 native 환경에서의 객체(@ref by::baseObj "baseObj") 모두를 구분하지 않고 사용합니다.

둘은 모두 @ref by::node "node" 임과 동시에 @ref by::baseObj "baseObj" 일 뿐입니다. 이를 위해서 @ref by::obj "obj" 를 @ref by::baseObj "baseObj" 에서 상속받게 함으로써 둘을 같은 클래스 계통으로 묶었습니다. 따라서 @ref by::baseObj "baseObj" 는 native 객체와 managed 객체의 <b>공통된 기능만</b> 갖습니다.

이는 <b>Polymorphism</b>의 전형적인 예시입니다. native 객체든 managed 객체든 상관없이 baseObj 타입으로 동일하게 다룰 수 있습니다.

<b>origin 객체</b>

byeol 언어로 사용자가 정의한, 원본이 되는 타입을 @ref by::origin "origin" 객체라고 합니다. 생성자 호출을 하면 모두 @ref by::origin "origin" 객체를 복사 생성해서 만들어집니다.

<b>getOrigin() override</b>

C++ 코드로 새로운 @ref by::baseObj "baseObj" 를 정의해서 byeol 코드로 사용하고 싶다면 @ref by::baseObj "baseObj" 를 상속한 C++ 클래스를 만들고 `getOrigin()`을 override해서 적절한 @ref by::baseObj "baseObj" 의 @ref by::origin "origin" 객체를 반환하면 됩니다.

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

<b>tbridger 활용 예시</b>

@ref by::tbridger "tbridger" 를 사용하면 native 함수를 매우 간단하게 managed로 노출시킬 수 있습니다.
@ref by::nStr "nStr" 에 좋은 예시가 있습니다:

이는 <b>Adapter 패턴</b> 또는 <b>Bridge 패턴</b>의 실제 활용 사례입니다. C++ native 인터페이스를 byeol managed 인터페이스로 변환하여 두 환경을 연결합니다.

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
사용할 수 있습니다! 어떻게 pack으로 내보내는지는 @ref by::packLoading "packLoading" 이나 @ref by::autoslot "autoslot" 등을
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

이처럼 @ref by::tbridger "tbridger" 는 native로 작성된 함수를 매우 간단하게 managed로 노출시킬 수 있습니다.
좀 더 자세한 @ref by::tbridger "tbridger" 사용 방법은 해당 클래스를 참조하세요.


### obj 클래스 - Managed 객체

@ref by::obj "obj" 클래스는 managed 환경에서의 객체를 표현합니다. @ref by::baseObj "baseObj" 의 기능을 확장해서 managed 환경에서만 필요한 scope에 대한 처리나 shares, owns에 대한 개념을 추가합니다.

<b>타입으로써의 obj</b>

C++에서 타입은 클래스로 표현되지만 byeol은 클래스란 존재하지 않아요. 객체와 클래스의 구분이 없으므로 <b>@ref by::obj "obj" 자체가 타입</b>인 셈입니다.

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

<b>shares, owns</b>

@ref by::baseObj "baseObj" 의 경우는 C++ native 클래스 기반이므로 `new`와 생성자를 통해 만들어집니다. 하지만 byeol 코드로 작성하는 managed 환경은 다릅니다.

managed 환경에서 객체를 정의하는 것은 @ref by::origin "origin" 객체로부터 객체를 복제하는 행위입니다. 이때 함수는 시스템 내 한 개만 있으면 되므로 복사할 필요가 없지만, property는 인스턴스마다 다른 값이 들어가야 하므로 복사가 되어야 합니다.

이를 효율적으로 하기 위해, 같은 타입의 @ref by::obj "obj" 끼리 공유되는 부분들을 <b>shares</b>, 복사가 되는 부분들을 <b>owns</b>로 구분합니다.

obj의 clone()이 발생하면:
- shares: 원본 origin에서 참조만 가져옴
- owns: 실제로 clone 수행

이는 <b>Flyweight 패턴</b>의 응용으로, 여러 인스턴스 간에 공유 가능한 데이터(함수)와 고유한 데이터(property)를 분리하여 메모리 효율성을 극대화합니다.

<b>shares vs owns 예제</b>

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

<b>immutable type</b>

str, int 등 scalar 타입은 모두 immutable 타입입니다. 이는 byeol의 호출 전략이 `by object`를 따르기 때문입니다 (java, C# 등과 유사: 객체는 얇은 복사, raw 타입은 깊은 복사).
객체에 할당을 시도할 경우 이를 복사해야하는지, 아니면 참조만 변경하면 되는지는 @ref by::immutableTactic "immutableTactic" 을 통해서 판단합니다.


### origin 클래스 - 타입의 원본

byeol 언어로 사용자가 정의한, 원본이 되는 타입을 @ref by::origin "origin" 객체라고 합니다. 사용자가 byeol 코드로 객체를 생성하면, 내부적으로 해당 타입의 @ref by::origin "origin" 객체를 복사 생성(clone)하여 새 인스턴스를 만듭니다.

이는 <b>Prototype 패턴</b>의 전형적인 구현입니다. 원본(prototype) 객체를 복제하여 새로운 인스턴스를 생성하므로, 매번 처음부터 객체를 구성할 필요가 없습니다.

<b>중요: origin에 대해 meta 타입 사용 금지</b>

@ref by::origin "origin" 객체는 @ref by::obj "obj" 에서 상속받았으며 생성시점을 제외하고는 @ref by::obj "obj" 타입으로써 사용되는 것을 전제로 작성되어 있습니다. 따라서 함부로 @ref meta 모듈을 사용하면 안 됩니다:

```
@style: language-cpp verified
origin* new1 = new origin(...);  // 생성시에 origin*로 참조하는 건 괜찮지만
new1->getType(); // origin 클래스의 특징을 이해하지 않은 상태에서,
                 // 이런식으로 `meta` 모듈을 사용해서 타입정보를 가져오는 건 권장하지 않는다.
                 // 자칫하면 아래와 같이 쓸 수 있기 때문이다.

new1->cast<origin>();  // nullptr 반환! 위험함
```

<b>왜 cast<origin>()은 nullptr를 반환하는가?</b>

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

이를 해결하기 위해 C++ @ref by::obj "obj" 클래스는 멤버변수로 @ref by::mgdType "mgdType" 변수를 갖습니다. @ref by::mgdType "mgdType" 은
managed 환경에서의 byeol 타입에 대한 타입정보입니다. 자세한 내용은 @ref by::mgdType "mgdType" 을 참조하세요.
아무튼 중요한 점은, 이 @ref by::mgdType "mgdType" 은 기존 native type의 @ref meta 정보 트리의 구조에 확장되는
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

따라서 핵심은 C++의 @ref by::obj "obj" 클래스를 기점으로 기존의 C++ 클래스별로 메타 타입을 제공하던
것이, C++ 인스턴스별로 메타타입을 제공하는 @ref by::mgdType "mgdType" 으로 전환된다는 것입니다.

그러니 @ref by::origin "origin" 객체에 대해서 함부로 @ref meta 모듈을 사용하려고 하면 안됩니다. @ref by::origin "origin" 클래스의
getType()은 C++의 @ref by::origin "origin" 타입과 아무런 관련이 없는 타입을 반환합니다. 하지만 이때의
@ref by::mgdType "mgdType" 은 부모클래스로 `ttype<obj>`가 되도록 parser가 연관을 지어서 생성합니다.

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

<b>중요한 유의사항</b>

origin 자체가 obj 타입으로 사용될 것을 전제로 한 것이기에 거의 모든
public 함수는 obj 타입의 API와 동일한 것입니다. 대부분은 origin 타입으로 써야할 필요가
없습니다.

호출 자체는 아무런 C++ 컴파일러 제약이 없습니다. dynamic_cast를 사용한다면 안전하게
형변환이 가능하며, 위의 제약사항을 잘 이해하고 있다면 얼마든지 origin으로부터 getType()
호출해서 cast나 isSub등을 사용해도 상관없습니다. 요약하면, 의도를 잘 이해를 하고
사용하자는 것입니다.


### tbaseObjOrigin 클래스 - baseObj Origin 템플릿

@ref by::baseObj "baseObj" 에 대한 @ref by::origin "origin" 객체를 쉽게 정의하기 위해서 사용하는 클래스 템플릿입니다.

먼저 @ref by::origin "origin" 와 @ref by::baseObj "baseObj" 를 보고 오는 것이 좋습니다. 구조나 컨셉은 @ref by::origin "origin" 클래스와 동일합니다.
자신의 타입을 shadow하며, @ref by::baseObj "baseObj" 타입으로 다뤄지도록 의도합니다.

단 사용방법에서 차이가 발생합니다. 이는 @ref by::baseObj "baseObj" 클래스의 컨셉이 @ref by::obj "obj" 와 다르기 때문입니다.

<b>obj와 달리 baseObj의 origin 객체는 static으로 존재한다</b>

@ref by::obj "obj" 는 byeol 언어로 작성된 객체를 표현합니다. 이 중에서도 `def` 키워드로 작성된 @ref by::origin "origin"
객체는 @ref by::origin "origin" 클래스의 인스턴스로 표현됩니다.

반면 @ref baseObj 는 그 자체로 사용할 수 없으며 이를 상속한 C++의 클래스가 존재합니다. 둘의
차이를 잘 이해해보겠습니다.

C++ 코드로 봤을때 @ref by::baseObj "baseObj" 의 @ref by::origin "origin" 은 <b>정적</b>이지만 @ref by::obj "obj" 의 @ref by::origin "origin" 은 <b>동적</b>이라는
얘기입니다.

<b>baseObj의 origin - static으로 선언 가능</b>

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

MyObj 자체를 byeol에서는 @ref by::origin "origin" 객체라고 하며 이는 @ref by::origin "origin" 클래스의 인스턴스로 표현됩니다:

```
@style: language-cpp verified
// parser가 런타임에 생성
origin org(typeMaker::make<obj>(name)); // name == "MyObj"
// 런타임에 동적으로 만들어지기에 static이 될 수 없다.
```


### modifier 클래스 - 접근 제한자

타입에 속한 함수나 property에 대해 접근제한자를 표현합니다. byeol 언어에는 public/protected/override 3종류의 modifier만 존재합니다.


### defaultCopyCtor 클래스 - 기본 복사 생성자

managed 환경에서 byeol 타입에 대해 사용자가 복사 생성자를 정의하지 않은 경우 parser에 의해 추가되는 기본 복사생성자입니다. 복사 생성자 호출 시 @ref by::args "args" 의 첫 번째 인자로 복사 대상 객체가 전달되며, 이 객체의 property들을 얇은 복사를 수행합니다.

참고로 scalar는 immutable 객체이므로 얇은 복사를 시도해도 깊은 복사처럼 동작합니다.

<b>defaultCopyCtor 동작 예제</b>

```
@style: language-cpp verified
// origin 객체 생성
origin myObj(typeMaker::make<obj>("myObj"));
myObj.setCallComplete(new evalExpr(&myObj, args()));
myObj.getOwns().add("age", new nInt(23));

// 기본 복사 생성자 생성
defaultCopyCtor copyCtor(myObj);

// 복사할 새 객체
obj cloned;

// 복사 생성자 호출 - args에 복사 대상을 넣음
args a{&cloned, narr{myObj}};
str result = copyCtor.eval(a);

// 복사 완료
cloned.getOwns().len();  // 1 - property가 복사됨
int value = cloned.getOwns()["age"].cast<int>();  // 23 - 얇은 복사
```


### immutableTactic 클래스 - Immutable 타입 처리

str, int 등 scalar타입은 모두 immutable 타입입니다. 이는 byeol의 호출 전략이 `by object`를 따르기 때문입니다.

단 이를 책임져야 하는 부분은 @ref by::node "node" 나 @ref by::obj "obj" 와 같이 컨테이너를 소유한 객체가 아니라 @ref by::tnmap "tnmap" 과 같은 컨테이너 클래스 자체가 되어야 합니다.

@ref by::obj "obj" 입장에서는 @ref by::node "node" 는 그냥 @ref by::node "node" 일 뿐이며, 자신이 소유한 container에 넣어야할 새로운 @ref by::node "node" 가 immutable 타입인지 아닌지를 알아야할 필요가 있어서는 안됩니다. 이게 깨지는 순간 다형성도 같이 깨집니다.

그래서 객체가 들어왔는데 이를 복사해야하는지, 아니면 참조만 가리키면 되는지를 @ref by::immutableTactic "immutableTactic" 을 통해서 판단합니다.

<b>알고리즘</b>

판단은 단순하게 @ref by::ntype "ntype" 에 정의된 `isImmutable()` 함수로 판단합니다. scalar type을 구현한 @ref by::nInt "nInt", @ref by::nStr "nStr" 등은 자신의 타입을 내보낼때 isImmutable()이 true로 나오는 타입으로 내보냅니다. immutable 타입으로 판단되면 인자를 바로 넣지 않고 `clone()`을 호출합니다.


### manifest 클래스 - Pack 메타데이터

@ref stela 언어로 작성된, pack을 로딩하기 위한 기본정보를 담고 있는 객체입니다.
`manifest.stela` 파일로부터 @ref stela 모듈을 사용해 파싱됩니다.


### tmock 클래스 - Proxy 객체

주어진 타입 파라메터 T에 대한 일종의 proxy 클래스입니다.

만약 생성시 `T*`로 원본이 될 인스턴스를 넣지 않으면 아무런 동작을 하지 않는 dummy 객체로
동작합니다. 최초에는 검증 시간을 최적화 하기 위해 고안되었습니다.

예를들어 @ref by::verifier "verifier" 가 특정 symbol이 scope에 있는지 없는지 코드를 검증하려면 실제로 그
객체를 만들기 보다 타입 정보만 redirection하는 @ref by::tmock "tmock" 을 대신 생성하기 위해서였습니다.

현재는 해당 기능은 사용되지 않으며 dummy 객체로 대신 사용중입니다.

---

## 표현식 (Expression) 시스템

Byeol은 대부분이 표현식으로 구성된 언어입니다. 블록문조차도 표현식이며, 마지막 줄의 evaluation 결과를 반환합니다. 이 섹션에서는 Byeol의 다양한 표현식 클래스들을 살펴봅니다.

<b>사용된 디자인 패턴:</b> <b>RAII 패턴</b> (blockExpr의 local scope 관리), <b>Early Exit 패턴</b> (retStateExpr의 블록 종료 메커니즘)

TODO: 표현식 클래스 계층도 추가 필요


### FBOExpr, FUOExpr 클래스 - 연산자 표현식

`FBOExpr`는 Binary Operator 표현식을 담당합니다. lhs, rhs에 각각 피연산자 표현식이 들어가며, 둘은 모두 scalar 타입이어야 합니다.

`FUOExpr`는 Unary Operator 표현식을 담당합니다. args에 피연산자가 1개 들어가며 scalar 타입이어야 합니다.


### assignExpr 클래스 - 할당 표현식

할당 표현식을 담당합니다.

<b>중요한 주의사항:</b> 이 표현식은 <b>scope에 등록된 참조를 바꾸는 것</b>이지 객체 자체에 대해 `operator=()`를 호출하는 것이 아닙니다. `obj::operator=()`를 호출하게 되면 일종의 깊은 복사처럼 동작하니 주의해야 합니다.

<b>예제로 이해하기</b>

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


### blockExpr 클래스 - 블록 표현식

블록 표현식을 담당합니다. @ref by::frameInteractable "frameInteractable" 하며, 외부에서 @ref by::frame "frame" 에 등록을 요청하면 scope를 하나 생성해 등록합니다. 이 scope는 이 블록문에서만 유효한 것으로, 흔히들 말하는 local scope입니다.

이는 <b>RAII 패턴</b> (Resource Acquisition Is Initialization)을 따릅니다. scope가 블록의 lifetime과 함께 생성되고 소멸되므로, 블록 종료시 자동으로 리소스(scope)가 정리됩니다.

<b>최적화:</b> @ref by::blockExpr "blockExpr" 은 `eval()` 함수 안에서 local scope를 생성하지 않습니다. 이는 특정 context에서는 불필요한 scope 생성을 막아 최적화하려는 것입니다.

<b>Block문은 표현식이다</b>

byeol 언어는 대부분 표현식으로 구성되며, block문도 예외가 아닙니다. block문은 <b>마지막 줄의 evaluation 결과를 반환</b>합니다.


### defArrayExpr 클래스 - 배열 리터럴

배열 리터럴 표현식을 담당합니다. 어떠한 타입의 배열인지는 배열의 원소로 적은 리터럴 상수들의 타입들을 type promotion을 통해 추론됩니다. Type promotion은 여러 타입 중 가장 넓은 범위의 타입으로 승격하는 과정입니다 (예: `[1, 2.5, 3]`이라는 배열이 있다면 int와 flt 타입이 섞여있으므로 flt 배열로 승격됩니다).

@ref by::tnarr "tnarr" 은 native 환경에서 배열을 담당하는 클래스입니다. @ref by::arr "arr" 은 @ref by::tnarr "tnarr" 을 managed 환경에 맞게 확장한 것입니다. 이렇게 함으로써 C++에서도 byeol에서도 서로 유사한 API를 사용 가능하게 됩니다.

<b>tnarr 사용 예제</b>

```
@style: language-cpp verified
// native 배열 생성
tnarr<node> arr1;
arr1.add(new nInt(10));
arr1.add(new nInt(20));
arr1.add(new nInt(30));

// 배열 접근
arr1.len();  // 3
node& first = arr1[0];  // nInt(10)
node& last = arr1[2];   // nInt(30)

// 배열 순회
for(auto& elem : arr1) {
    int value = elem.cast<nInt>()->get();
    // value: 10, 20, 30
}

// 타입 승격 예시 (byeol 코드)
// [1, 2.5, 3] -> flt 배열로 승격
// int와 flt이 섞여있으면 더 넓은 flt로 승격됨
```


### defNestedFuncExpr 클래스 - 중첩 함수 정의

byeol 언어의 중첩 함수인 @ref by::nestedFunc "nestedFunc" 을 생성합니다. 중첩함수는 정의할때는 이름을 생략할 수 있으며, `closure`로도 활용될 수 있습니다.


### defSeqExpr 클래스 - 시퀀스 표현식

시퀀스(범위) 표현식을 담당합니다.

@ref by::nseq "nseq" 은 native 환경에서 시퀀스를 담당하는 클래스입니다. @ref by::seq "seq" 은 @ref by::nseq "nseq" 를 managed 환경에 맞게 확장한 것입니다. 이렇게 함으로써 C++에서도 byeol에서도 서로 유사한 API를 사용 가능하게 됩니다.


### endExpr 클래스 - End 키워드

`end` 키워드를 구현합니다. endExpr은 단순히 블록문을 가지고 있을 뿐입니다. `func`이 end를 실행시킵니다.


### retStateExpr 클래스 - 블록 종료 키워드

`blockExpr`에서 벗어나는 키워드들의 기반클래스입니다.

<b>블록문을 바로 종료하는 알고리즘</b>

블록문 안에서 `ret` 등을 하여 바로 블록문을 종료해야 하는 경우, `thread`에 `setRet()`로 값을 넣어야 합니다. 각 blockExpr은 구문을 한줄 한줄 실행하다가 thread에 setRet()에 일정한 값이 발견될 경우, 모든 동작을 중단하고 해당 값을 블록문의 호출자에게 올려보냅니다.

이는 <b>Early Exit 패턴</b>의 구현입니다. thread에 상태를 설정하여 중첩된 블록 구조에서도 즉시 탈출할 수 있도록 합니다.

---

## 컨테이너 시스템

Byeol의 컨테이너 시스템은 native(C++)와 managed(Byeol 언어) 환경에서 동일한 API를 제공하도록 설계되었습니다. 특히 `tnchain`은 AST 데이터를 보관하는 가장 중요한 컨테이너입니다.

<b>사용된 디자인 패턴:</b> <b>Iterator 패턴</b> (begin/end/iterate), <b>Linked List 구조</b> (tnchain의 next/prev 연결), <b>Facade 패턴</b> (tnchain을 tbicontainable로 사용)

TODO: 컨테이너 클래스 계층도와 tnchain의 linked list 구조 다이어그램 추가 필요


### tucontainable, tbicontainable 클래스 - 컨테이너 인터페이스

byeol 언어에는 기본적으로 array, map, seq를 지원합니다.

`tucontainable` 클래스는 array와 같은 <b>index를 입력받아 임의의 타입 T를 반환</b>하는 container 클래스의 기본 인터페이스를 정의합니다.

`tbicontainable` 클래스는 map과 같은 <b>key를 입력받아 임의의 타입 T를 반환</b>하는 container 클래스의 기본 인터페이스를 정의합니다.

<b>Native 타입을 managed 타입이 상속한다</b>

byeol 언어는 AST를 그대로 프로그램 실행으로 이용하는 구조이기 때문에 C++ native 클래스를 그대로 managed 타입으로 노출시키면, native API와 managed API가 동일해지므로 이점이 있습니다.

이를 원활하게 하기 위해서, 기본 인터페이스를 tucontainable로 정의하고, 이걸 구현한 C++ native 클래스를 정의합니다. byeol에 사용할 managed 타입은 이 native 클래스를 상속한 후, bridger를 사용해 쉽게 노출시키는 전략을 사용합니다.

<b>Native 타입에는 이름 앞에 `n`이 붙는다</b>

`n`은 `native`의 첫글자를 딴 것입니다. 컨테이너 클래스는 native 클래스와 그걸 상속한 managed 클래스가 나오게 됩니다. native 클래스는 임의의 타입에 대해서도 동작해야 하므로 클래스 템플릿으로 정의됩니다.

그래서 native 환경에서 사용하는 `tnarr`과 managed 환경에서 사용할 `arr`과 같이 하나의 컨테이너에 대해 2벌씩 클래스가 나옵니다.

<b>`t` prefix</b>

컨벤션 규칙에 따라, 클래스 템플릿인 경우는 prefix `t`가 붙습니다. 그리고 `t` 빠진 타입을 typedef로 선언하는 경우가 있는데 이는 해당 클래스 템플릿의 기본 파라메터를 넣은 것을 의미합니다.

예를들어 클래스 템플릿인 `tnarr`의 기본 타입 파라메터인 `node`를 넣은걸 `narr`이라고 합니다. `tstr`도 마찬가지로 `tstr<node>`를 `str`이라고 선언하고 있습니다.

<b>컨테이너 기본 제공 API</b>

모든 컨테이너는 다음 API를 제공합니다:
- 원소 추가: `add()`
- 삭제: `del()`
- 반복자: `begin(), end(), iterate()`
- 역방향 반복자: `rend(), rbegin(), riterate()`
- 원소 대체: `set()`
- 존재 검사: `in()`

이는 <b>Iterator 패턴</b>을 따릅니다. `begin()`, `end()`, `iterate()` 등의 메서드를 통해 컨테이너의 내부 구조를 노출하지 않고 순회할 수 있습니다.

<b>R과 RSquare</b>

tucontainable은 본래 index에 대해 타입파라메터 T를 원소로 반환하는 컨테이너입니다.
그러니 타입 파라메터가 T만 있어도 충분할거라 생각하기 쉽지만 실제로는 R과 RSquare라는
타입을 추가로 갖고 있습니다.

- <b>R</b>: Reference의 약자로 대부분의 타입 T에 대한 참조를 내보내야 하는 경우에 사용됩니다.
  일반적인 경우는 `T*`가 될 것입니다.

- <b>RSquare</b>: non nullable 참조자를 내보내는 경우에 사용되는 타입입니다. 만약 R이 non
  nullable이라면 R이 그대로 사용됩니다. 일반적인 경우에는 `T&`가 될 것입니다.

이렇게 2가지 타입을 추가로 정의하는 이유는 `nseq` 때문입니다. nseq는 그 기본 형태가
index를 기반으로 하는 int 배열과 유사합니다.

다만 반환 타입은 참조가 될 수 없습니다. seq의 범위가 몇 억에 가깝다면 사용하지도 않을
몇 억개의 int 원소를 다 가지고 있을 필요가 없습니다.

그래서 nseq는 99% API가 ucontainable과 동일하나 반환형이 `nInt*`가 아니라 `nInt` 여야
한다는 차이만 있을 뿐입니다.


### tnchain 클래스 - Chain으로 연결되는 컨테이너

`node`가 AST의 근간을 이루는 가장 중요한 클래스라고 한다면, <b>tnchain은 AST의 데이터를 보관하는 컨테이너로써 가장 중요한 클래스</b>라 할 수 있습니다.

<b>목적</b>

tnchain은 말그대로 <b>컨테이너를 chain하면서 관리</b>하는 클래스로, 여러 원소들을 하나의 그룹으로 묶어서 참조하거나, 순회하거나, 다른 곳에 있는 컨테이너를 여기에 참조만 chain해서 겉으로는 마치 하나의 flatten된 map처럼 보이도록 하는 기능을 가지고 있습니다.

쉽게 비유하면 <b>linked list의 각 node를 배열로 구성한 컨테이너</b>를 떠올리면 좋습니다. 이는 <b>Linked List 자료구조</b>를 활용한 설계입니다. next와 prev 포인터로 여러 컨테이너를 연결합니다.

<b>tbicontainable로도 대부분의 기능을 사용할 수 있다</b>

tnchain의 가장 중요한 컨셉으로, 외부 사용자는 bicontainer를 하나 받았을 뿐, 이것이 실체가 `tnmap`인지, tnchain인지는 몰라도 상관없어야 합니다.

물론 이것은 tnchain을 bicontainable로써 `add(), del(), len()` 등을 할때의 얘기이며, `link()`와 같이 tnchain의 고유 기능을 사용하려면 tnchain이라는 걸 알아야만 합니다.

이는 <b>Facade 패턴</b>의 예시입니다. 복잡한 tnchain의 linked list 구조를 단순한 tbicontainable 인터페이스로 감추어, 사용자는 일반 bicontainer처럼 사용할 수 있습니다.

<b>link() - tnchain의 존재의의</b>

tnchain은 내부적으로는 타입 파라메터로 선언한 defaultContainer를 한개 가지고 있으며, next, prev라고 하는 tnchain에 대한 참조를 갖는 일종의 linked list의 node와 비슷한 구조로 되어 있습니다.

`link(const tnchain<T>&)`로 새로운 chain 객체를 넘기면 this의 container의 next에 새로운 chain이 연결됩니다. 이때 탐색을 하면 tnchain은 자신의 defaultContainer를 모두 탐색한 후 바로 다음 next chain의 defaultContainer로 순회합니다.

따라서 <b>외부에서 봤을때는 this container에 next chain의 원소들이 복사되어 들어간 것처럼</b> 코드를 작성할 수 있습니다.

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

이제 chn1을 순회하면 {0, 1, 6, 5, 2, 3} 순서로 모든 원소에 접근할 수 있습니다. 하지만 <b>실제로는 어떠한 복사도 일어나지 않았습니다</b>. 단지 참조를 chain으로 연결했을 뿐입니다.

<b>tnchain link() 실전 사용 예제</b>

```
@style: language-cpp verified
class Item : public node {
public:
    Item(int val) : value(val) {}
    int value;

    scope& subs() override { return dumScope::singleton(); }
    priorType prioritize(const args& a) const override { return NO_MATCH; }
    str eval(const args& a) override { return str(); }
};

// 세 개의 독립적인 컨테이너 생성
tnchain<std::string, Item> localVars;
localVars.add("x", new Item(10));
localVars.add("y", new Item(20));

tnchain<std::string, Item> funcParams;
funcParams.add("arg1", new Item(100));
funcParams.add("arg2", new Item(200));

tnchain<std::string, Item> globalVars;
globalVars.add("PI", new Item(314));
globalVars.add("MAX", new Item(999));

// 체이닝: local -> params -> global
localVars.link(funcParams);
funcParams.link(globalVars);

// localVars를 통해 모든 변수에 접근 가능 (복사 없음!)
localVars.len();  // 6 (2 + 2 + 2)

// "arg1" 검색: local에 없으면 params에서 찾음
Item* found = localVars.get("arg1");
found->value;  // 100

// "PI" 검색: local, params 없으면 global에서 찾음
found = localVars.get("PI");
found->value;  // 314

// 순회: 모든 체인을 자동으로 순회
for(auto& elem : localVars) {
    std::cout << elem.value << " ";
}
// 출력: 10 20 100 200 314 999
```

<b>순회 예제</b>

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


### arr 클래스 - Managed 배열

<b>Native API를 byeol 환경에서도 그대로 사용</b>

byeol 언어는 AST를 그대로 프로그램 실행으로 이용하는 구조이기 때문에 C++ native 클래스를 그대로 managed 타입으로 노출시키면, native API와 managed API가 동일해지므로 이점이 있습니다.

노출할때는 `tbridger`를 사용해서 몇 줄만으로도 간단하게 노출시킬 수 있습니다.


### nseq 클래스 - 범위형 컨테이너

`seq`의 기반이 되는 `ucontainable`를 구현한 클래스로, int를 받아서 적절한 int를 반환하는 범위형 컨테이너입니다.

예를들어 byeol 코드로 `3..5`라고 정의한 seq가 있다면 해당 seq[0]은 3, seq.len()은 2가 나오게 됩니다.

<b>사용 예제</b>

```
@style: language-cpp verified
// 1부터 4까지의 범위 (1, 2, 3)
nseq s(nInt(1), nInt(4));

// 반복자를 통한 순회
auto e = s.begin();
int expects[] = {1, 2, 3};
for(int n = 0; n < 3; n++, ++e) {
    int value = (*e).get();  // 1, 2, 3 순서대로
    // value 사용...
}

// 역방향 반복자
auto re = s.rbegin();  // 3부터 시작
++re;  // 2
++re;  // 1
int value = (*re).get();  // 1
```


### smultimap 클래스 - 삽입 순서를 기억하는 Multimap

byeol에서는 AST에서 node를 구성할때 단순하게 map을 사용할 순 없으며 multimap을 사용해야 합니다. (함수 오버로딩 때문입니다. 자세한 내용은 `node`를 참조하세요.)

다만 여기서 중요한 점은 원소의 <b>삽입 순서가 scope의 검색 우선순위를 결정</b>한다는 것입니다. scope에서 같은 이름의 여러 심볼(예: 오버로딩된 함수들)을 찾을 때, 먼저 삽입된 것을 우선적으로 검색하고 매칭을 시도합니다. 따라서 삽입된 순서를 기억하고 유지할 필요가 있어 삽입 순서를 기억하는 multimap을 별도로 구현해 사용하고 있습니다.

<b>API는 STL 라이브러리를 최대한 비슷하게 구성</b>

이는 `tnarr`또한 내부적으로는 vector를 사용하므로 같은 상황에 같은 컨벤션이 되도록 의도적으로 구성한 것입니다.

<b>삽입 순서를 기억해야 하는 또 다른 이유: Closure의 Scope Capture</b>

중첩 함수가 closure로 export될 때, 그 시점의 local scope를 capture하고 유지해야 하는 요구사항이 있습니다. 하지만 capture 시점 이후에 발생한 local 변수들은 중첩 함수와 아무런 관련이 없어야 합니다.

이러한 이유로 closure는 전체 scope를 capture하지 않고, closure가 생성되는 시점까지의 local scope에 속한 변수들만 capture합니다. 이를 구현하려면 scope의 얕은 복사가 필요하다고 생각하기 쉽지만, `tnchain`과 `link()`를 사용해서 복사 없이 이 문제를 해결합니다.

하지만 이를 위해서는 tnchain이 반복자가 삽입 순서대로 순회하는 것을 보장해야 하므로, STL의 multimap을 직접 사용할 수 없어서 smultimap을 별도로 구현했습니다.

<b>삽입 순서 보장 예제</b>

```
@style: language-cpp verified
smultimap<std::string, str> scope;

// 같은 키에 여러 값을 삽입 - 삽입 순서가 유지됨
for(nint n = 0; n < 10; ++n) {
    scope.insert("apple", *new nInt(n));  // 0, 1, 2, 3, ..., 9 순서대로
    if(n == 5)
        scope.insert("banana", *new nInt(-1));
}

// 삽입 순서대로 순회됨
auto e = scope.begin("apple");
for(nint n = 0; n < 10; n++) {
    int value = *e->get()->cast<nint>();  // 0, 1, 2, ..., 9 (삽입 순서 유지)
    ++e;
}

// 함수 오버로딩 검색 시 삽입 순서가 우선순위를 결정
// 먼저 삽입된 함수가 우선적으로 매칭 시도됨
```

---

## Generic 시스템

Byeol은 C++의 템플릿과 유사한 generic 타입 시스템을 제공합니다. 하지만 C++ 템플릿과 달리, byeol의 generic은 <b>지연 인스턴스화(lazy instantiation)</b> 방식으로 동작합니다. 즉, generic 타입이 실제로 사용될 때만 해당 타입의 구체화된 인스턴스가 생성됩니다.

<b>사용된 디자인 패턴:</b> <b>Lazy Instantiation 패턴</b> (설계상 lazy, 실제로는 verifier가 eager 수행), <b>AST Transformation</b> (generalizer가 타입 파라메터를 구체 타입으로 교체), <b>Cache 패턴</b> (map으로 생성된 origin 재사용)

TODO: Generic 타입 생성 흐름도와 AST transformation 과정 다이어그램 추가 필요


### getGenericExpr 클래스 - Generic 타입 참조의 진입점

generic 클래스의 참조를 담당하며 generic 타입 생성의 진입점을 담당합니다. 사용자가 `SomeGeneric<MyObj>()`처럼 generic 타입을 사용하면, 이 표현식이 `getGenericExpr`로 표현됩니다.

내부적으로는 `genericOrigin`에게 generic 타입을 줄 것을 요청하며, genericOrigin은 해당 타입에 대한 generic이 있다면 그걸 바로 주고, 없다면 `generalizer`를 통해 generic 타입을 생성 후, 그 새로운 인스턴스를 반환합니다.

```
@style: language-cpp verified
// generic origin 생성 (타입 파라메터 T를 가진 Optional 클래스)
origin org(typeMaker::make<obj>("Optional"));
org.getOwns().add("value", *new getExpr("T"));
genericOrigin genericOrg(org, {"T"});  // "T"가 타입 파라메터

// scope에 Optional 등록
obj myScope;
myScope.getOwns().add("Optional", genericOrg);

// Optional<nInt> 타입 생성
myScope.inFrame();
getGenericExpr expr("Optional", *new args{narr{*new nInt()}});
str result = expr.eval();  // Optional<nInt> 타입의 origin 반환
```

<b>설계는 Lazy, 실제는 Eager</b>

genericOrigin은 설계상 lazy instantiation을 지원합니다. 즉, `eval()` 호출 시점에 주어진 type parameter로 generic 타입을 생성합니다. 하지만 `verifier`가 프로그램 실행 전에 모든 타입을 검증하면서 필요한 generic 인스턴스를 미리 생성하므로, 실제로는 eager instantiation처럼 동작합니다.


### genericOrigin 클래스 - Generic 타입의 생성과 관리

기본적으로 `origin`과 같은 역할을 수행하는 클래스이지만, generic을 지원한다는 점과 이 인스턴스 자체가 origin을 담당하는 게 아니라 <b>필요에 의해 origin을 생성/관리</b>한다는 점이 다릅니다.

<b>동작 방식</b>

genericOrigin은 내부적으로 map을 사용해서 타입 파라메터별로 구체화된 origin 객체들을 관리합니다. 예를 들어, `SomeGeneric<MyObj>`가 요청되면:

1. map에서 `MyObj`를 key로 찾습니다
2. 없다면, 원본 origin 객체(`_org`)를 deepClone합니다
3. `generalizer`를 사용해서 타입 파라메터 `T`를 모두 `MyObj`로 교체합니다
4. 교체된 origin을 map에 저장하고 반환합니다
5. 이후 같은 타입이 요청되면 map에서 바로 반환합니다

이는 <b>Lazy Instantiation 패턴</b>과 <b>Cache 패턴</b>의 조합입니다. 요청이 있을 때만 origin을 생성(lazy)하고, 한번 생성된 origin은 map에 저장하여 재사용(cache)합니다.

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


### generalizer 클래스 - Generic 타입의 구체화

`visitor` 기반으로 동작하며, generic 타입을 구체화하는 실질적인 작업을 담당합니다. 동작은 매우 단순합니다:

<b>AST를 순회하면서 타입 파라메터를 찾아 교체합니다.</b>

예를 들어, `SomeGeneric<T>`에서 `SomeGeneric<myObj>`를 만들 때:
- AST 전체를 방문하면서
- `getExpr(args = T)` 형태의 노드를 찾으면
- 이를 `getExpr(args = myObj)`로 교체합니다

이 과정이 모든 node에 걸쳐서 수행되면, 원본 generic 타입의 복사본에서 타입 파라메터가 모두 구체 타입으로 교체된 새로운 origin이 완성됩니다.

이는 <b>AST Transformation</b> 기법입니다. AST를 순회하며 노드를 찾아 수정하여, 컴파일 타임에 코드 구조를 변환합니다.

<b>Generic 타입 생성 흐름</b>

전체 흐름을 정리하면 다음과 같습니다:

1. 사용자 코드: `SomeGeneric<MyObj>().foo()`
2. `getGenericExpr`가 `genericOrigin`에게 `MyObj` 타입 요청
3. `genericOrigin`이 map에서 `MyObj` 검색
4. 없으면 원본을 deepClone하고 `generalizer` 실행
5. `generalizer`가 AST를 순회하며 `T`를 `MyObj`로 교체
6. 완성된 origin을 map에 저장하고 반환
7. 이후 같은 타입 요청시 4-5번 과정 없이 바로 반환

---

## Native-Managed 브리징 시스템

Byeol은 C++로 작성된 native 코드와 byeol 언어로 작성된 managed 코드가 서로 상호작용할 수 있는 bridge 시스템을 제공합니다. 이 시스템의 핵심은 <b>C++ 클래스와 함수를 간단한 선언만으로 byeol 언어에서 사용 가능하도록 노출</b>하는 것입니다.

<b>사용된 디자인 패턴:</b> <b>Adapter/Bridge 패턴</b> (tbridger로 native-managed 연결), <b>Facade 패턴</b> (tbridger가 복잡한 브리징을 단순 API로 제공), <b>Monostate 패턴</b> (tbridger의 static variable), <b>Proxy 패턴</b> (tmock), <b>Marshaling</b> (tmarshaling으로 타입 변환)

TODO: 브리징 시스템 아키텍처 다이어그램과 marshaling 프로세스 다이어그램 추가 필요


### tbridger 클래스 - Bridge 컴포넌트의 진입점

`tbridger`는 bridge 시스템의 facade 역할을 합니다. C++ 클래스를 타입 파라메터로 받는 클래스 템플릿이며, <b>monostate 패턴</b>으로 설계되어 있습니다.

이는 <b>Facade 패턴</b>과 <b>Monostate 패턴</b>을 활용합니다. Facade로서 복잡한 브리징 시스템을 간단한 API(`ctor()`, `func()`)로 감추고, Monostate로 모든 인스턴스가 static variable을 공유하여 동일한 상태를 유지합니다.

<b>기본 사용법</b>

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

<b>설계 구조</b>

@ref by::tbridger "tbridger" 는 다음 컴포넌트들의 조합으로 동작합니다:

- <b>tbridger</b>: Facade 패턴, `func()` 등으로 함수를 static subs()에 저장
- <b>tbridge</b>: @ref by::tbridger "tbridger" 에 등록된 subs를 @ref by::origin "origin" 으로 삼아 생성된 @ref by::baseObj "baseObj"
- <b>tbridgeFunc</b>: C++ 멤버 함수 포인터를 redirect하는 managed 함수
- <b>tbridgeCtor</b>: 생성자를 표현 (객체 생성과 생성자 호출을 모두 처리)
- <b>tbridgeClosure</b>: C++ 람다를 byeol 함수로 노출
- <b>tmarshaling</b>: native 타입과 managed 타입 간 변환 처리

<b>동작 흐름</b>

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

이 과정에서 <b>Marshaling</b> 기법이 핵심입니다. `tmarshaling`이 managed 타입(nInt)을 native 타입(int)으로, 반환값을 다시 managed 타입으로 자동 변환하여 두 환경 간의 타입 불일치를 해결합니다.


### tbridge 클래스 - Native 클래스의 Managed 표현

@ref by::tbridge "tbridge" 는 @ref by::tbridger "tbridger" 에 등록된 정보를 바탕으로 생성되는 @ref by::baseObj "baseObj" 입니다. 내부적으로 `tbaseObjOrigin<tbridger<T>>` 형태로 @ref by::origin "origin" 을 정의하고 있어서, @ref by::tbridger "tbridger" 를 통해 채워진 subs()를 자신의 origin으로 사용합니다.

따라서 @ref by::tbridge "tbridge" 객체는 @ref by::tbridger "tbridger" 에 등록된 함수들을 마치 자신의 멤버 함수처럼 소유하게 됩니다.


### tmock 클래스 - Proxy와 Dummy 객체

`tmock`은 주어진 타입 파라메터 `T`에 대한 proxy 클래스입니다. 생성시 원본 인스턴스를 넣지 않으면 아무 동작도 하지 않는 dummy 객체로 동작합니다.

원래는 `verifier`가 symbol이 scope에 있는지 검증할 때, 실제 객체를 만들지 않고 타입 정보만 redirect하는 용도로 고안되었습니다. 현재는 해당 기능은 사용되지 않으며, 주로 dummy 객체 용도로 사용됩니다.

이는 <b>Proxy 패턴</b>의 구현입니다. 원본 객체에 대한 대리자(proxy) 역할을 하며, 원본이 없을 때는 dummy로 동작합니다.

---

## 스코프와 실행 컨텍스트

Byeol의 코드 실행은 @ref by::scope "scope", @ref by::frame "frame", @ref by::frames "frames", @ref by::thread "thread" 가 유기적으로 협력하여 이루어집니다. 이 시스템은 symbol 탐색, 함수 호출, 실행 흐름 관리를 담당합니다.

<b>사용된 디자인 패턴:</b> <b>Chain of Responsibility 패턴</b> (scope chain을 통한 symbol 탐색), <b>RAII 패턴</b> (scope/frame의 lifetime 관리), <b>Singleton 패턴</b> (dumScope)

TODO: scope/frame/frames/thread의 관계도와 symbol 탐색 프로세스 다이어그램 추가 필요


### scope 클래스 - Symbol 저장소

`scope`는 `tnchain`을 기반으로 설계되어 있습니다. byeol에서 scope는 만든 주체에 따라 5가지로 분류됩니다:

이는 <b>Chain of Responsibility 패턴</b>을 활용합니다. scope들이 chain으로 연결되어, symbol 탐색 요청이 현재 scope에서 처리되지 않으면 다음 scope로 전달됩니다.

1. <b>local scope</b>: `blockExpr`에 의해 생성, 블록 종료시 소멸
2. <b>func scope</b>: 함수가 소유한 symbol 저장 (nested func, static variable 등)
3. <b>obj scope</b>: 객체가 소유한 함수와 변수들이 속한 scope
4. <b>file scope</b>: 소스 파일 단위의 scope, pack보다 위에 선언된 symbol
5. <b>pack scope</b>: pack에 속한 symbol들, 외부에서 접근 가능

<b>Scope 분류 예시</b>

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

<b>각 scope의 특징</b>

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

이는 <b>RAII 패턴</b>의 전형적인 예입니다. `frameInteract` 객체의 생성자가 scope를 frame에 등록하고, 소멸자가 자동으로 정리하므로 리소스 누수를 방지합니다.

<b>Func scope</b>는 함수가 소유한 symbol들이 저장됩니다. 주의할 점은 매 함수 호출마다 새로 생성되는 게 아니라, <b>시스템 내 유일하게 존재하는 @ref by::func "func" 객체가 소유한 sub node들</b>이라는 것입니다.

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

IS_DBG는 file scope과 pack scope에 각각 1개씩 정의됩니다. 중요한 점은 <b>file scope는 @ref by::parser "parser" 에 의해 항상 pack scope를 chain</b>한다는 것입니다. Symbol을 찾을 때 file scope를 먼저 검색하고, 없을 경우 pack scope를 검색하므로, file scope에 선언된 값이 우선됩니다.


### frame 클래스 - Scope들의 동적 연결

`frame`은 현재 실행중인 코드 블록에서 접근 가능한 symbol을 관리합니다. 여러 개의 `scope`를 동적으로 chain하는 방식으로 최적화된 symbol 탐색을 제공합니다.

<b>Frame 생성 과정</b>

`baseFunc`이 `eval()` 되면 다음 순서로 frame이 구성됩니다:

1. `baseObj`: 새 frame 객체를 만들어 frames에 추가, subs()와 `me` 참조 등록
2. `obj`: baseObj 동작에 더해 현재 file scope 추가 (pack scope도 함께)
3. `func`: 자신의 subs()와 args를 frame에 등록
4. `blockExpr`: local scope으로 사용할 빈 scope 생성

이 과정을 거쳐 함수 호출시 최소 5개의 scope가 frame에 등록됩니다.

<b>실제 예시: yourObj.foo(10) 호출 시 frame 구조</b>

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

<b>Scope 탐색의 효율성</b>

`frame`은 `node`를 상속하므로 `subs()`를 제공합니다. 하지만 `subs()`는 `vector<scope>`가 아니라 <b>여러 scope이 chain으로 연결된 하나의 scope 객체</b>를 반환합니다. 따라서 symbol을 찾을 때:

```
@style: language-cpp verified
// 마치 하나의 컨테이너를 순회하는 것처럼 간결함
for(auto& elem : frame.subs()) {
    // 내부적으로는 여러 scope이 연결되어 있지만
    // 코드상으로는 하나의 컨테이너처럼 보임
}
```

이는 `scope`이 `tnchain`으로 구현되어 있기 때문에 가능합니다.


### frames 클래스 - Frame 적층 관리

`frames`는 하나의 `thread`에 속한 여러 `frame`을 관리합니다. byeol에서는 최상위 코드도 암묵적으로 초기화 함수 내에서 실행되며, `main()`도 함수이므로, 모든 코드 실행은 함수 컨텍스트 안에서 이루어집니다. 따라서 코드 실행 = 함수 실행이며, 함수 실행시 frame 객체가 생성되어 적절히 scope를 적층시킵니다.

<b>Frame 적층의 필요성</b>

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

<b>Pack scope을 다시 넣어야 하는 이유</b>

Symbol 탐색은 위에서 아래로 순차 탐색합니다. 만약 frame #2에서 pack/file scope을 제거한다면, `yourObj.foo()` 안에서 `name`을 참조할 때 main()의 local scope에 있는 "unknown"이 먼저 발견됩니다. 이는 잘못된 결과입니다 (file scope의 "kniz"가 나와야 함).

올바른 결과를 위해서는 frame #2에 pack/file scope를 끼워넣어야 합니다. if-else로 scope 탐색 장소를 분리하는 것보다, <b>하나의 큰 배열을 위에서 아래로 순차 탐색</b>하는 것이 훨씬 간결합니다.

게다가 `tnchain`의 `link()`를 활용하면, `parser`가 obj를 생성할 때 pack과 file을 미리 chain으로 연결해두어서, obj의 scope를 frames에 추가하는 것만으로도 obj, file, pack scope이 한번에 연결됩니다. 이 과정에서 <b>어떠한 복사도 일어나지 않습니다</b>.


### thread 클래스 - 실행 흐름의 관리자

`thread`는 하나의 프로그램 실행 흐름을 표현합니다 (현재는 단일 스레드만 지원). `thread`는 `frames`와 `errReport`를 소유하여 프로그램 실행시 frame을 구성하고 에러를 수집합니다.

<b>Builtin pack</b>

`builtin` pack은 byeol 언어에서 기본 제공하는 타입/함수들입니다. manifest 선언 없이도 항상 접근 가능하며, `int`, `str`, `arr`, `err`, `print()` 등이 여기 속합니다.

standard pack과 비슷하지만 엄연히 구분되는 pack이며, builtin은 전부 C++ native 클래스를 `tbridger`를 활용해 노출한 형태입니다.

<b>싱글톤 속성</b>

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

<b>threadUse RAII 패턴 예제</b>

```
@style: language-cpp verified
// 외부 컨텍스트에서 작업 수행
thread& originalThread = thread::get();

{
    // 새로운 thread 컨텍스트 생성
    errReport myReport;
    threadUse newThread(myReport);

    // 이 블록 내에서는 thread::get()이 newThread의 thread를 반환
    thread& currentThread = thread::get();
    // currentThread != originalThread

    // 이 thread에서 코드 검증/실행
    obj myObj;
    myObj.inFrame();
    // frames에 frame 추가됨 (newThread의 frames)

    myObj.outFrame();
}  // RAII: threadUse 소멸 시 자동으로 originalThread로 복원

// 다시 원래 thread로 돌아옴
thread::get();  // originalThread 반환
```

---

## 패키지 시스템

Byeol은 `pack`이라는 단위로 라이브러리를 배포합니다. 패키지 시스템은 pack을 동적으로 lazy 로딩하며, 종속성 관리와 검증을 담당합니다.

<b>사용된 디자인 패턴:</b> <b>Lazy Loading 패턴</b> (autoslot의 lazy pack 로딩), <b>State Machine 패턴</b> (autoslot의 4가지 상태), <b>Builder 패턴</b> (slotLoader의 fluent API), <b>RAII 패턴</b> (autoslot의 리소스 관리)

TODO: 패키지 로딩 파이프라인과 상태 전이 다이어그램 추가 필요


### manifest 클래스 - Pack 메타데이터

stela 언어로 작성된, pack을 로딩하기 위한 기본정보를 담고 있는 객체입니다. `manifest.stela` 파일로부터 @ref stela 모듈을 사용해 파싱됩니다.

@ref by::manifest "manifest" 는 pack의 entrypoint, 종속성 정보 등을 포함합니다. entrypoint는 pack이 어떠한 종류의 라이브러리를 포함하고 있는지를 나타냅니다 (예: `cpp`, `byeol`).


### slot 클래스 - Pack의 결과물

byeol 언어는 `pack`이라는 일종의 압축파일 단위로 라이브러리를 배포하는데, pack 파일에는 최상위 `obj` 객체와 @ref by::manifest "manifest", 종속하는 pack 목록이 포함됩니다.

(byeol 언어에서 내 코드에서 다른 pack을 사용하려면 @ref by::manifest "manifest" 에 종속관계에 있다는 걸 선언해야 합니다)

<b>@ref by::slot "slot" 은 pack 파일로부터 만들어지는 결과물</b>이지, pack을 불러오는 걸 담당하지 않습니다. pack 로딩에 대해서는 `packLoading`이나 `slotLoader`를 참조하세요.


### autoslot 클래스 - Lazy Pack 로딩

byeol 언어는 pack을 <b>lazy하게 동적으로</b> 불러옵니다. @ref by::autoslot "autoslot" 은 이 기능을 구현한 것으로, `slotLoader`가 pack 파일을 찾으면 `packLoading` 객체를 적절히 생성해서 @ref by::autoslot "autoslot" 에 넣어둡니다.

이후 @ref by::autoslot "autoslot" 에 접근해서 안에 포함된 symbol을 가져오려는 시도를 하면 lazy하게 @ref by::packLoading "packLoading" 이 동작해 symbol을 파일로부터 불러옵니다.

이는 <b>Lazy Loading 패턴</b>의 핵심 구현입니다. 필요할 때까지 리소스(pack) 로딩을 미루어 초기 로딩 시간과 메모리 사용량을 최소화합니다.

<b>복수의 packLoading</b>

@ref by::packLoading "packLoading" 은 native 환경에서 가져올 수도 있고(dll 혹은 so 파일), managed 환경에서 가져올 수도 있습니다(.byeol 파일). 또는 2개가 모두 하나의 pack에 있는 경우도 있을 수 있습니다.

따라서 @ref by::autoslot "autoslot" 은 항상 1개의 @ref by::packLoading "packLoading" 만 가지지 않고, 배열로 처리합니다.

<b>autoslot 상태 (State)</b>

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

이는 <b>State Machine 패턴</b>의 명확한 구현입니다. autoslot은 정의된 상태들(RELEASED → PARSED → VERIFIED → LINKED) 사이를 전이하며, 각 상태에서 허용되는 동작이 다릅니다.

- <b>Slot 생성</b>: @ref by::slotLoader "slotLoader" 가 @ref by::slot "slot" 객체를 생성해 시스템에 추가, dependencies도 기록
- <b>RELEASED</b>: 초기 상태로, 어떠한 메모리도 점유하지 않음. 사용하지 않는 대부분의 @ref by::slot "slot" 은 여기에 속함
- <b>PARSED</b>: @ref by::autoslot "autoslot" 에 접근이 이뤄진 경우, 본격적으로 사용하기 위해 코드를 파싱. 파싱 단계가 필요없는 경우 (optimized pack 또는 native pack)에는 LINKED 상태로 바로 건너뜀
- <b>VERIFIED</b>: 파싱 이후, 코드의 정합성을 검증. 검증에 실패했다면 isValid값을 false로 설정
- <b>LINKED</b>: 자신이 검증에 실패한 상태라면, 자신을 참조하는 모든 dependents에게 자신이 검증에 실패했다는 사실을 전파

왜 이런 상태 관리가 필요할까요?

<b>동적 검증과 의존성 문제</b>

모든 pack이 검증이 완료된, 완전무결한 상태라고 전제하고 그냥 로딩만 해서는 안될 수 있습니다. 때로는 pack이 올바른지 한번 더 검증할 필요가 있기에, @ref by::autoslot "autoslot" 중 일부는 symbol을 불러올때 사전에 파싱이나 검증을 해야 합니다.

문제는 어떠한 pack은 다른 pack에 종속되는 경우가 매우 많이 발생한다는 점으로, 종속한 pack이 검증에 실패하게 되면, 그 사실을 전파해서 종속된 pack들도 모두 사용이 불가능해야 합니다. 이를 위의 4가지 상태를 제어하는 알고리즘으로 해결합니다.

<b>재귀적 로딩</b>

pack이 다른 pack에 종속되는 경우는 부지기수로 많습니다. @ref by::autoslot "autoslot" 이 lazy하게 동작하기 때문에 어떠한 @ref by::slot "slot" 을 loading하다가 다른 @ref by::autoslot "autoslot" 에 접근함으로써 해당 @ref by::autoslot "autoslot" 도 재귀적으로 로딩 시퀸스에 들어가는 일도 많습니다.

이때 중복으로 초기화 되거나 아직 완전히 파이프라인을 통과하지 않는지 체크합니다.


<b>RAII</b>
@ref by::autoslot "autoslot" 은 @ref by::slot "slot" 에 정의된 _pack 객체를 가리킵니다. 이 객체는 외부 파일인 `pack` 을 로딩함으로써 읽어온 심볼들인데, @ref by::autoslot "autoslot" 은 @ref by::packLoading "packLoading" 을 통한 pack의 symbol 생성을 책임지므로, pack의 소멸 또한 책임집니다.
그래서 RAII를 사용해, @ref by::autoslot "autoslot" 객체가 소멸될때 모든 심볼을 먼저 없애고 @ref by::packLoading "packLoading" 객체 또한 없앰으로써 so 파일을 close 하는 등의 작업을 수행합니다.
자세한 내용은 @ref by::packLoading "packLoading" 을 참조하세요.

이는 <b>RAII 패턴</b>의 전형적인 활용으로, autoslot 객체의 lifetime에 pack 리소스의 lifetime을 바인딩하여 리소스 누수를 방지합니다.


### slotLoader 클래스 - Pack 로더

@ref by::slotLoader "slotLoader" 는 외부 pack을 로딩하는 역할을 담당합니다. `addPath()`로 탐색 경로를 추가하고, `load()`를 호출하면 pack을 불러올 수 있습니다.

<b>기본 사용법</b>

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

이는 <b>Builder 패턴</b>의 Fluent API 스타일입니다. 메서드 체이닝(`setReport().setBaseSlots().addPath().load()`)으로 객체 구성 과정을 명확하고 읽기 쉽게 표현합니다.

<b>manifest와 entrypoint</b>

pack 로딩 중에는 필연적으로 @ref by::manifest "manifest" 를 파싱합니다. manifest를 통해 가장 중요한 정보는 <b>entrypoint</b>입니다. entrypoint는 pack이 어떠한 종류의 라이브러리를 포함하고 있는지를 나타냅니다.

예를들어 pack에 C++ 동적 라이브러리가 포함되어 있다면 entrypoint는 `cpp`가 되며, byeol 라이브러리라면 `byeol`이 됩니다.

@ref by::slotLoader "slotLoader" 는 manifest를 읽은 후 @ref by::autoslot "autoslot" 을 생성하고 entrypoint에 따라 적절한 @ref by::packLoading "packLoading" 인스턴스를 autoslot에 추가합니다.

<b>주의사항:</b>
- 하나의 pack 라이브러리는 여러개의 entrypoint를 가질 수 있습니다
- 경로는 항상 재귀적으로 탐색됩니다 (하위 디렉토리의 pack들도 모두 로딩)
- 파일 탐색, 동적 라이브러리 로딩 등 플랫폼 종속적인 기능들은 indep 모듈에 위임합니다


### packLoading 클래스 - Pack 로딩 추상 클래스

`slotLoader`에 의해 패키지를 로딩할 때 사용되는 추상 클래스입니다. @ref by::packMakable "packMakable" 인터페이스가 핵심 API를 제공합니다.

packLoading은 `rel(), parse(), verify()` 함수를 제공하며, 이는 `autoslot`의 상태와 깊은 관련이 있습니다.


### cppPackLoading 클래스 - C++ Pack 로더

`slotLoader`에 의해 cpp 패키지를 로딩할 때 사용되는 `packLoading` 중 하나입니다. 이름 그대로 C++ pack을 동적 라이브러리 파일에서 로딩하는 역할을 합니다.

<b>Entrypoint</b>

@ref indep 모듈의 @ref by::dlib "dlib" 을 사용해서 동적 라이브러리를 로딩합니다. entrypoint는 이미 정의되어 있는 @ref by::ENTRYPOINT_NAME "ENTRYPOINT_NAME" 의 값을 사용합니다.

---

## Visitor 패턴 및 AST 순회

Byeol에서는 AST를 중점적으로 다루기 때문에 @ref by::visitor "visitor" 를 자주 사용하게 됩니다. 순회하는 방법과 순회시 `node`를 visit했을 때의 동작을 서로 분리하기 위해 @ref by::visitor "visitor" 가 적극적으로 활용됩니다.

<b>사용된 디자인 패턴:</b> <b>Visitor 패턴</b> (AST 순회와 방문 동작 분리), <b>Double Dispatch</b> (accept와 visit의 조합), <b>Template Method 패턴</b> (visit의 3단계 구조)

TODO: Visitor 패턴의 동작 순서도와 Double Dispatch 메커니즘 다이어그램 추가 필요


### visitor 클래스 - AST 순회의 핵심

<b>순회</b>

@ref by::visitor "visitor" 는 항상 <b>전위 순회(pre-order traversal)</b>를 따릅니다. 후위 순회를 하도록 변경은 불가능합니다.

`visit()`은 다음 3개의 단계로 이뤄져 있습니다:
1. 현재 찾은 node를 방문 (`onVisit()`)
2. 다음 하위 node를 순회 (`onTraverse()`)
3. 현재 찾은 node를 떠남 (`onLeave()`)

이는 <b>Template Method 패턴</b>의 구현입니다. `visit()`이 알고리즘의 골격(3단계 구조)을 정의하고, 각 단계(`onVisit`, `onTraverse`, `onLeave`)는 하위 클래스가 override하여 구체적인 동작을 정의합니다.

<b>accept를 통한 Downcasting</b>

@ref by::visitor "visitor" 는 `onVisit(T&)`과 같이 많은 구체 타입에 대한 방문을 표현하는 virtual function을 다수 가지고 있습니다.

반면 onTraverse에서 탐색시에는 주로 `node`의 `subs()`를 통해서 `tbicontainable` 인터페이스를 사용하기 때문에 node 타입을 사용하게 됩니다. 그래서 어디선가는 node 타입을 `nInt`나 `defNestedFuncExpr`과 같이 구체타입으로 downcasting을 해야합니다.

이를 위해 @ref by::node "node" 의 `accept()`라는 virtual 함수를 호출합니다:

```
@style: language-cpp verified
void defNestedFuncExpr::accept(const visitInfo& i, visitor& v) {
    v.visit(i, *this);  // visitor::visit(const visitInfo&, defNestedFuncExpr&) 호출
}
```

가상함수 accept()가 호출되면 안에서 *this를 통해 구체타입으로써 역으로 visitor의 visit()을 호출하는 식입니다.

이는 <b>Double Dispatch</b> 패턴입니다. 첫 번째 dispatch는 `node::accept()` 호출(동적 바인딩으로 구체 타입의 accept 선택), 두 번째 dispatch는 그 안에서 `visitor::visit(구체타입&)` 호출(오버로드 resolution으로 적절한 visit 선택)입니다. 이를 통해 런타임에 node와 visitor 양쪽의 구체 타입에 따른 동작을 결정합니다.

이를 위해 visitation에 참여하는 모든 @ref by::node "node" 의 파생클래스는 `accept()`라는 virtual 함수를 override 해야 하는데, 이 과정을 쉽게 하기 위해서 <b>VISIT 매크로</b>를 사용합니다:

```
@style: language-cpp verified
class _nout slot: public node {
    BY(CLASS(slot, node), VISIT())  // <--- VISIT 매크로

public:
```

만약 @ref by::node "node" 파생클래스 임에도 `accept()`를 override 하지 않았다면, `onTraverse(node&)`가 대신 사용되며, 이걸로도 충분한 경우에 해당됩니다.

<b>중복 방문 제거</b>

AST는 참조가 서로 순환하는 경우도 종종 발생합니다 (예: A가 B를 참조하고 B가 다시 A를 참조). 이 경우 아무런 예외처리 없이 순회하면 이미 방문했던 @ref by::node "node" 를 다시 방문하면서 <b>무한 순회</b>에 빠집니다.

@ref by::visitor "visitor" 는 `_visited`라는 map을 소유하고 있습니다. 이를 통해서 `visit()`이 호출 되었을 때 이미 방문한 @ref by::node "node" 인지를 판단해서 예외처리를 해주고 있습니다.

이 방문 기록 정보는 매번 @ref by::visitor "visitor" 가 방문을 시작하기 직전에 초기화됩니다. 만약 재방문이 가능하도록 하고 싶다면 `setReturnable(true)`로 값을 변경하면 됩니다.

<b>visitor 구현 예제</b>

```
@style: language-cpp verified
// 커스텀 visitor로 AST를 순회하며 특정 노드 찾기
struct myVisitor: public visitor {
    nbool foundInt;
    nbool foundFunc;

    myVisitor(): foundInt(false), foundFunc(false) {}

    // nInt 노드 방문시 호출
    nbool onVisit(const visitInfo& i, nInt& o, nbool) override {
        if(i.name == "value")
            foundInt = true;
        return true;  // 계속 순회
    }

    // func 노드 방문시 호출
    nbool onVisit(const visitInfo& i, func& fun, nbool) override {
        if(i.name == "myFunc") {
            foundFunc = true;
            // 반환 타입 확인
            fun.getRet()->isSub<nFlt>();
        }
        return true;
    }
};

// AST 객체 생성
obj myObj;
myObj.subs().add("myFunc", new func(*new modifier(),
                                     typeMaker::make<func>(params(), new nFlt())));
myObj.subs().add("value", new nInt());

// visitor로 순회
myVisitor v;
v.setTask(myObj).work();  // AST 전체를 전위 순회

v.foundInt;   // true - nInt 노드를 찾음
v.foundFunc;  // true - func 노드를 찾음
```


### graphVisitor 클래스 - AST 로깅

`visitor` 기반의 AST 로깅 클래스로, 주어진 root `node`를 순회하면서 상세 정보를 모두 로깅합니다.

다음과 같은 상황에서 사용됩니다:
- `interpreter`에 의해서 디버그 빌드에서 사용
- `defaultSigZone`에 의해서 crash가 발생하면 출력
- `logStructureFlag`에 의해서 `interpreter`나 `starter`로 하여금 @ref by::graphVisitor "graphVisitor" 를 사용하도록 flag가 set됨

@ref by::graphVisitor "graphVisitor" 는 AST 구조를 트리 형태로 시각화하여 출력하므로, 디버깅시 AST의 구조를 파악하는 데 매우 유용합니다.

---

## 파싱 시스템

byeol 언어의 파싱 시스템은 <b>Flex</b>와 <b>Bison</b>을 사용하는 전통적인 파서 구조를 따르지만,
byeol 언어의 특성인 <b>offside rule</b>을 지원하기 위해 정교한 메커니즘을 갖추고 있습니다.

### parser 클래스 - 파싱의 진입점

`parser`는 byeol 파싱 컴포넌트의 진입점 역할을 하며 `worker`를 상속합니다. `work()`을
통해서 파싱된 결과가 `slot`으로 반환됩니다.

<b>Scanner - Bison - Parser 구조</b>

Flex와 Bison을 사용하고 있으며 flex는 `lowscanner`로, bison은 `lowparser`로 각각
명명합니다. 이 low level인 scanner, @ref by::parser "parser" 는 @ref by::parser "parser" 컴포넌트 안에만 존재하는 것으로
외부에서는 일절 노출되지 않습니다.

`parser::work()`가 실행되면 다음과 같은 흐름으로 동작합니다:

1. lowscanner를 실행
2. lowscanner는 토큰을 뜯어서 lowparser에게 넘김
3. lowparser는 받은 토큰에 대해 rule이 match되면 그 이벤트를 다시 parser에게 넘김

그러므로 @ref by::parser "parser" 의 `on`으로 시작하는 함수들(예: `onPack()`, `onCompilationUnit()`)은 그러한 이벤트를 handling하는 함수로, 실제로 어떻게 @ref by::node "node" 를 생성해서 AST를 구축하는지를 정의합니다.

<b>lowparser.y의 Lifecycle 관리</b>

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
@style: language-txt verified
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

### smartDedent, tokenScan 클래스 - Indentation 관리

byeol 파서의 `smartDedent`와 `tokenScan`은 @ref by::stela "stela" 모듈의 `stelaSmartDedent`, `stelaTokenScan`과 동일한 원리로 동작합니다. Indentation을 배열로 관리하고, scan mode를 동적으로 전환하는 방식은 완전히 같습니다.

상세한 동작 원리와 예제는 @ref stela 모듈의 해당 클래스들을 참조하세요. byeol 파서의 경우
더 복잡한 문법을 가지므로 명령 토큰(`SCAN_AGAIN`, `SCAN_EXIT` 등)이 더 다양합니다

자세한 명령 토큰 목록은 `lowparser.y`의 `// mode:` 단락을 참조하세요.

### srcSupply 클래스 - 소스 코드 공급 추상화

`parser`에서 사용되는 클래스로, @ref by::parser "parser" 에 입력되는 코드를 어떠한 형태로 제공할지를 추상화합니다.

예를 들어 파일로써 제공하는 방법도 있지만, 버퍼에 담아서 줄 수도 있고 stream으로 주는
방법도 있을 것입니다. `srcSupply`는 이러한 다양한 소스 제공 방식을 추상화하여 @ref by::parser "parser" 가
일관된 인터페이스로 소스 코드를 읽을 수 있도록 합니다.

### expander 클래스 - 사전 타입 추론

`parser`에서 명시한 것처럼 현재 사전 타입 추론 알고리즘은 종속성 그래프를 만들지 않고
모든 사전 타입 추론 표현식을 모아둔 후, 반복적으로 타입 추론을 시도하는 방식입니다.

이러한 방식을 사용하기 때문에 반복적으로 표현식을 순회하며 사전 타입 추론을 시도하는
동작을 `verifier` 안에서 한번에 수행하기에는 적합하지 않습니다. 따라서 @ref by::verifier "verifier" 로 검증하기 전에 @ref by::expander "expander" 로 사전 타입 추론을 완료해둘 필요가 있습니다.

기본 동작은 `visitor`에 기반하므로, 사전에 @ref by::visitor "visitor" 를 숙지하고 오는 게 좋습니다.

<b>defBlock과 Expand</b>

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

<b>현재 사용중인 타입 추론 방법</b>

byeol은 이러한 타입추론 표현식들을 한곳에 모아둔 후, parsing이 종료된 후 <b>1줄이라도 타입추론이 성공한다면 무한루프를 도는 방법</b>을 사용합니다. 가지고 있는 모든 표현식이 타입추론에 실패하거나, 타입추론 표현식이 바닥나면 루프를 종료합니다.

이 방식은 추론 순서를 따지지 않아도 되므로 구현이 간단하지만 속도가 느리므로,
향후 종속성 그래프를 구축하여 타입 추론 순서를 최적화하는 방법으로 개선할 여지가 있습니다.

`defBlock`은 이러한 역할을 돕습니다. @ref by::defBlock "defBlock" 에는 expand, common, scope 3종류의 데이터가 있는데:

- <b>scope</b>: @ref by::parser "parser" 에 의해서 바로 AST가 만들어지는 @ref by::node "node"
- <b>expand</b>: 파싱은 성공했지만 타입추론이 필요하여, `verifier` 동작 전에 `expander`로
  넘길 부분
- <b>common</b>: 공통 생성자를 위한 부분

---

## 코드 검증 및 실행

파싱이 완료된 AST는 실행되기 전에 검증 과정을 거쳐야 합니다. byeol 언어는 인터프리터
구조를 띄고 있지만 <b>강형 타입</b>을 사용하며, 컴파일 언어처럼 사전에 에러를 도출합니다.

<b>사용된 디자인 패턴:</b> <b>Template Method 패턴</b> (tworker의 _prepare, work, _onEndWork 구조), <b>Type Inference 알고리즘</b> (verifier의 infer 사용)

TODO: 파싱-검증-실행 파이프라인 다이어그램과 Type Inference 알고리즘 흐름도 추가 필요

### tworker 클래스 - 배치 작업의 기반

`tworker`는 대량의 배치 작업을 수행하는 데 최적화되어 있습니다. `errReport`로 이
과정에서 발생하는 에러를 수집할 수 있으며 다양한 flag들을 지정해서 작업 중 일부 동작을
변경시킬 수도 있습니다.

`verifier`, `visitor`, `parser` 등 큰 작업을 수행하는 클래스들은 @ref by::tworker "tworker" 기반으로 돌아갑니다.

<b>work와 task</b>

@ref by::tworker "tworker" 는 말 그대로 `work()`를 하기 위해 존재합니다. 이때 작업의 대상이 되는 input을
`task`라고 합니다.

작업 전에 `_prepare()`가 호출되며 작업이 종료되면 `_onEndWork()`가 호출됩니다.

<b>errReport 통합</b>

대량의 작업을 깊은 함수 depth를 동반하면서 수행해야 하므로, 콜스택 깊은 곳에 있는
에러를 충분히 탐지하기 위해서는 @ref by::errReport "errReport" 를 통해서 이를 수집할 수 있어야 합니다.

@ref by::tworker "tworker" 는 `setReport()`를 통해서 외부로부터 @ref by::errReport "errReport" 객체를 넣을 수 있습니다. @ref by::errReport "errReport" 를 할당하지 않을 경우 `dummyErrReport`가 대신 들어갑니다.

<b>Log Flag</b>

@ref by::tworker "tworker" 는 작업 도중에 로깅을 위한 다양한 flag를 가지고 있습니다:

- `LOG_ON_EX`: 에러 발생시 한줄 로그를 남김
- `DUMP_ON_EX`: 에러 발생시 callstack을 포함한 `err` 객체를 dump
- `GUARD`: 함수가 시작되고 끝날 때 로깅
- `INTERNAL`: 에러나 GUARD를 제외한 로그를 로깅
- `LOG_ON_END`: 작업이 모두 완료되면, 그간 수집한 err를 모두 `log()`로 한줄 로그를 남김
- `DUMP_ON_END`: 작업이 모두 완료되면, 그간 수집한 err를 모두 `dump()`

### verifier 클래스 - 코드 검증

`visitor` 기반의 코드 검증을 담당합니다. byeol 언어는 인터프리터의 구조를 띄고 있지만
언어 자체는 강형 타입을 사용하고 컴파일을 실행하는 것처럼 사전에 에러를 도출합니다. @ref by::verifier "verifier" 는 그 기능의 핵심을 구현합니다.

`parser`와 `expander`로 최종 생성된 AST를 실행하기 전에 에러는 없는지 확인합니다.

<b>사전 학습 권장 사항</b>

@ref by::verifier "verifier" 는 다양한 모듈을 종합적으로 사용하는 고급 클래스이므로, 다음 개념들을 먼저 학습한 후 접근하는 것을 권장합니다:

- `node`: AST의 기본 구조
- `visitor`: AST 순회 패턴
- `expr`: 표현식 클래스들
- WHEN 매크로: 에러 처리 패턴

<b>visitor 기반</b>

onTraverse는 @ref by::visitor "visitor" 에서 알아서 채워주므로, @ref by::verifier "verifier" 는 onVisit과 onLeave시 어떻게 해야 하는지에 대해서만 정의해두고 있습니다.

<b>eval과 infer</b>

프로그램의 실행은 결국 각 `node`의 `eval()`로 이뤄지지만, @ref by::verifier "verifier" 는 실행한 런타임 값은 관심이 없습니다. 오직 실행이 가능한가, 지정한 표현식의 결과가 정의된 property의
타입과 묵시적 변환이 허용되는가와 같은 <b>타입 매칭</b>에만 관심이 있습니다.

node의 `infer()`는 타입 추론 기능을 수행하는 것으로 실행하면 값은 모르지만 결과 타입을
`origin` 객체로 반환하는 함수입니다. 값을 계산하지 않으므로 `eval()`보다 더 빠릅니다.

이제 감이 왔겠지만, @ref by::verifier "verifier" 는 `eval()`보다는 `infer()`를 중점적으로 사용합니다:

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

<b>많은 코드 라인을 가진 파일</b>

@ref by::verifier "verifier" 는 가장 LOC가 많은 파일 중 하나입니다. 이때 코드들은 크게 3 종류로 분류됩니다:

1. 검증을 위한 사전 작업
2. WHEN 매크로를 사용한 실제 검증 로직
3. 각 단계별 상세 로깅

<b>단계별 상세 로깅</b>

@ref by::visitor "visitor" 는 타입당 1개의 `onVisit()` 함수만 가질 수 있습니다. 그러나 @ref by::verifier "verifier" 는 하나의 타입이 주어졌을 때 검증해야 하는 케이스가 대부분 2개 이상입니다. 그러니 잘못하면
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

### starter 클래스 - AST 실행

`worker`의 일종으로, `verifier`에 의해 검증이 완료된 AST를 실행합니다. 실행 결과
`thread`에게 exception이 발생되었다면 로그를 덤프하고 exception을 반환합니다.

<b>main 함수 Look up</b>

`starter`가 메인 함수를 찾을 때 사용하는 조건:

- <b>위치</b>: root(pack) 바로 밑에 위치해야 함
- <b>이름</b>: `main`이어야 함
- <b>반환형</b>: `void` 또는 `int`
- <b>내용</b>: 최소 1개 이상의 구문을 포함해야 함

@ref by::starter "starter" 는 @ref by::interpreter "interpreter" 와 함께 사용하면 byeol 코드를 파싱해서 실행할 수 있습니다.

<b>interpreter와 starter를 사용한 코드 실행 예제</b>

```
@style: language-cpp verified
// byeol 코드 파싱 및 실행
const char* code = R"(
pack myPack

main() void
    print("Hello from Byeol!")
)";

// 1. interpreter로 파싱 및 검증
errReport report;
interpreter ip;
ip.setTask(*new slot(manifest("myPack")))
  .setReport(report)
  .getParser().addSupply(*new bufSupply(std::string(code)));
ip.work();  // 파싱 및 검증 수행

// 2. 검증 결과 확인
if(!ip.isVerified()) {
    // 에러 출력
    for(const auto& err : report)
        err->dump();
    return;
}

// 3. starter로 실행
starter st;
st.setTask(ip.getTask()).setReport(report);
st.work();  // main() 함수 실행
```

### sigZone 클래스 - Signal 처리

`signaler`에 RAII를 적용한 클래스입니다.

@ref by::sigZone "sigZone" 인스턴스가 정의된 블록문 안의 코드를 실행할 때 signal이 오면 람다를 수행하도록 @ref by::signaler "signaler" 를 사용합니다. RAII 패턴을 통해 블록을 벗어나면 자동으로 signal handler가
해제됩니다.

---

## 에러 처리

byeol 언어는 에러 처리를 위한 정교한 시스템을 갖추고 있습니다. 에러는 단순히 발생하고
끝나는 것이 아니라, callstack 정보와 함께 수집되며, 검증 단계에서부터 실행 단계까지
체계적으로 관리됩니다.

<b>사용된 디자인 패턴:</b> <b>Reference Counting</b> (tstr을 통한 err/frame의 lifetime 관리)

TODO: 에러 전파 메커니즘과 callstack 구성 다이어그램 추가 필요

### baseErr 클래스 - 에러의 기반

byeol 언어에서의 @ref by::err "err" 클래스 계통의 가장 base 클래스입니다. 에러 처리에 대한 공통
로직과 인터페이스를 포함합니다.

<b>byeol Error의 문법적 구분과 내부 구현</b>

byeol 문법상으로는 에러는 2종류로 구분됩니다:

1. <b>Known error</b>: `?`로 표현되며, errorable 타입으로 명시
2. <b>Exception</b>: errorable 타입으로 명시하지 않는 상황에서 에러가 반환되는 케이스

하지만 <b>구현상으로는 둘은 완전히 동일한 에러 객체</b>입니다. 에러는 `parser`나 `verifier`가 발생시킨, native 상에서 발생한 `nerr`와 byeol 코드로 인해 발생한 `err`, 2개가 에러의 발생원에 의해 구분되고 있을 뿐입니다.

<b>@ref by::err "err" 는 @ref by::frames "frames" 에 대한 강한 참조를 갖는다</b>

이 @ref by::frame "frame" 을 사용해서 callstack 정보를 제공합니다. `frame`에서 설명한 것처럼 @ref by::frame "frame" 은 함수 호출시 @ref by::baseObj "baseObj" 에 의해서 생성됩니다.

이후 @ref by::frame "frame" 은 `frames`에 의해서 `del()` 되는데 이때 객체의 강한 참조를 잃어버릴 뿐
heap에서 바로 삭제가 되진 않습니다. (이 프로젝트에서 memlite 모듈의 `tstr`에 의한
레퍼런스 카운팅 없이 직접 heap에서 new/delete를 하는 경우는 극히 드뭅니다.)

그러므로 <b>@ref by::err "err" 가 살아있는 한 참조하는 @ref by::frame "frame" 데이터 또한 유지</b>됩니다.

이는 <b>Reference Counting</b> 메모리 관리 기법입니다. `tstr`이 참조 카운트를 관리하여, err가 frame을 참조하는 동안 frame이 메모리에서 해제되지 않도록 보장합니다. 마지막 참조가 사라질 때 자동으로 메모리가 해제됩니다.

<b>nerr 생성</b>

@ref by::nerr "nerr" 은 주로 core 모듈의 `__core_when__`에 의해서 만들어집니다. 다음과 같은 코드로
이뤄집니다:

```
@style: language-cpp verified
WHEN_NUL(stmt).exErr(IS_NUL, getReport(), "stmt").ret(blk);
```

위 코드는 `stmt`가 nullptr일 경우 `IS_NUL`이라는 에러가 `getReport()`로 반환되는
`errReport` 객체에 추가되고 blk를 반환하면서 함수가 종료됩니다.

<b>dumpable과 log</b>

- `log()`: 해당 에러를 clog 모듈을 사용해서 로깅
- `dump()`: callstack을 포함해서 최대한 많은 정보를 노출

<b>errCode</b>

관리를 위해서 @ref by::nerr "nerr" 는 @ref by::errCode "errCode" 로 관리됩니다. nerr는 주로 `parser`나 `verifier`에 의해서
생성되는데, 이는 문법 에러가 발생했을 때 `Err2203`과 같이 code 2203를 같이 줌으로써
어떤 에러인지 상세 정보를 쉽게 검색하도록 도움을 주기 위해서입니다.

@ref by::errCode "errCode" 는 리터럴 상수로 간단하게 description 문자열도 정의하고 있으니 참고하세요.

### errReport 클래스 - 에러 수집

`baseErr`들을 담아두는 클래스입니다.

시간이 오래 걸리거나 복잡한 동작을 수행하는 로직이 있는 경우 함수의 depth가 매우
깊어집니다. 안쪽에서 특정 함수에서 에러가 발생한 경우, 에러가 발생했다는 사실을
기록하고 작업은 최대한 진행하고 싶을 때 @ref by::errReport "errReport" 를 사용해서 에러를 기록합니다.

<b>컨테이너</b>

@ref by::errReport "errReport" 는 거의 모든 기능이 `baseErr` 객체에 대한 관리이기 때문에 `tucontainable`과 유사한 인터페이스를 가지고 있습니다.

<b>noisy</b>

어떠한 @ref by::errReport "errReport" 는 새로운 @ref by::err "err" 객체가 add될 때마다 자동으로 `log()`를 수행하고 싶을 때가 있습니다. 이때 `setNoisy(true)`를 지정합니다. 이렇게 하면 에러가 추가되는 즉시
로그가 남아 디버깅이 더 쉬워집니다.

<b>사용 예제</b>

```
@style: language-cpp verified
// 여러 파일을 파싱하는 배치 작업
errReport report;
report.setNoisy(true);  // 에러 발생 즉시 로깅

// 여러 파일 파싱 - 에러가 발생해도 계속 진행
for(const auto& file : files) {
    parser p;
    p.setReport(report);  // errReport를 parser에 전달
    p.work(file);  // 파싱 실패 시 report에 에러 추가
}

// 작업 완료 후 에러 확인
if(report.len() > 0) {
    // 발생한 에러들을 순회
    for(const auto& err : report) {
        err->dump();  // 각 에러의 상세 정보 출력 (callstack 포함)
    }
}
```




---

<b>다음 문서</b>: @ref aj-architecture-frontend
