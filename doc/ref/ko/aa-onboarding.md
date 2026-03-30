# 여행 코스 가이드 {#aa-onboarding}

Byeol 프로젝트에 기여하고 싶은 개발자를 위한 가이드입니다.

코드는 C++로 작성되어 있고 다양한 매크로와 독특한 코딩 스타일과 컨벤션 규칙을 가지고 있고,
저 개인적으론 간결한 코드를 항상 지향하기 때문에 라인수는 작지만 독해가 쉬운 편은 아닐꺼예요.

메타프로그래밍이나, 디자인 패턴들(빌더, 전략, 아답터, 방문자 등)나 idium(method chaining, RAII) 여럿 쓰이고 있고,
매크로 오버로딩이나, 메모리풀 등도 가지고 있어서, 해당 개발 경험이 없으시다면 좀 곤란할 수 있습니다.
그리고 bison, flex에 대해서는 따로 설명하지 않고 있으니, 문법을 수정할 거라면 먼저 읽고 오시는 게 좋겠네요.


그래서, 이제부터 많은 문서를 쭉 읽어나가게 되실텐데요.
시작하기에 앞서서, 대량의 문서를 모두 읽기 전에, 원하는 코드를 수정하기 위한 학습 코스를 소개해보려구요.
일단, 가이드 문서는 필수 과정과 시나리오별 과정으로 나눠볼 수 있겠네요.

---

## 필수 과정

공통 과정에는 코드를 독해하기 위한 사전 지식이나 규칙들을 설명합니다.
다음과 같은 순서로 읽어주세요.

- @ref ab-dev-env : buiilder를 통한 개발 환경 세팅 방법을 알려줍니다. (10분)

- @ref ac-build-structure : 이 프로젝트의 각 폴더에 어떤 파일들이 들어있는지 소개합니다. (10분)

- @ref ad-convention-rules : c++ 코드의 컨벤션 규칙을 알려줍니다. (20분)<br/>
byeol 코드는 일반적인 c++ 코드와 코딩 스타일이 많이 다릅니다. 꼭 읽어보시기 바랍니다.

---

## 시나리오별 네비게이션

@ref ae-architecture-overview 에서 자세히 다루겠지만, Byeol 프로젝트는 layered architecture로 설계되었습니다.
간단히 목록을 소개하면 다음과 같아요. Stack diagram 처럼, 밑으로 갈수록 더 독립적인 모듈입니다.

- @ref al-architecture-frontend

- @ref ak-architecture-core

- @ref aj-architecture-stela

- @ref ai-architecture-memlite

- @ref ah-architecture-meta

- @ref ag-architecture-clog

- @ref af-architecture-indep

언어의 핵심 기능은 core module이 담당하고 있어요. 그리고 보시다시피 꽤 상위(많은 모듈에 종속하는)
모듈이므로, 핵심 기능을 수정하려고 한다면 core가 종속하고 있는, 그 밑의 많은 모듈을 필수적으로 학습해야만
합니다. 이건 어쩔 수가 없어요.<br/>
그래서 시간이 충분하시다면, 물론 indep 모듈부터 모든 문서를 천천히 정독하시는 걸 강력하게 추천합니다.

하지만 목적이 분명하고, 시간이 제한되어 있다면, 조금이나마 빠르게 작업할 수 있도록, 시나리오에 맞게 읽고 참조해야할 부분을 소개해드릴꼐요.

참, 각 시나리오별로 어느 모듈의 코드를 수정해야 하는지 적어드릴텐데, 해당 모듈이 종속하는 모듈들은
추가적인 언급이 없어도 필수적으로 읽으셔야 합니다.

---

### 시나리오: 새로운 내장 함수(Native Function) 및 타입을 추가하고 싶어요
- core 모듈을 읽기 위해서 core 모듈이 종속하는 모듈을 순서대로 읽으세요: <br/>
@ref af-architecture-indep -> @ref ag-architecture-clog -> @ref ah-architecture-meta -> @ref ai-architecture-memlite -> @ref aj-architecture-stela

- <b>핵심 가이드</b>: @ref ai-architecture-core 문서의 "Native-Script 브리징" 섹션<b>까지를</b> 자세히 읽어보세요. `tbridger` 클래스가 핵심입니다.

- 그리고 `tbridger`를 사용해서 core 모듈의 `module/core/builtin` 폴더를 수정해야 합니다.

#### 튜토리얼
실제로 함수를 하나 추가해볼께요.


<b>요구사항</b><br/>
Byeol 언어의 `str` 타입에 `foo(msg str)` 라는 함수를 추가해봅시다.<br/>
이 foo()는 c++로 작성할 것이며, 화면에 std::cout 을 사용해서 주어진 인자인 `msg`값으로 `foo says: $msg!\n`를 출력하고 3을 반환해야 한다고 해보죠.


<b>계획</b><br/>
`str`은 c++ 상으로는 `core/builtin/scalar/nStr` 이 표현합니다. 이 파일을 수정해야 겠죠.<br/>
여기서, 주의할 점은 우리가 추가할 foo()는 str 타입에 추가한 것이므로 C++ Runtime에서도 nStr객체로 foo() 함수를 호출할 수 있어야 한다는 겁니다.<br/>
그렇다면 nStr::foo()를 추가하고, 이를 byeol runtime에 노출시켜야 합니다.<br/>
nStr은 baseObj이므로, @ref by::tbridger "tbridger"를 사용해서 Byeol Runtime에 공개해야 겠죠.


<b>코딩</b><br/>
그럼 본격적으로 c++ 코드로 foo()함수를 만들어봅시다. 먼저 `nStr.hpp` 헤더부터.
```foo
    class _nout nStr: public tscalar<std::string>, public tucontainable<nStr> {
        struct _nout nStrType: public ntype {
        public:
            int foo(const std::string& msg); // <-- 추가
            // 이때 tmarshaling에 따라서 인자와 반환형을 결정합니다.
```

함수 body를 채웁시다.
```cpp
@style: language-cpp verified
#include <iostream>

// .....
    int me::foo(const std::string& msg) {
        std::cout << "foo says: " << msg << "\n";
        return 3;
    }
```

좋습니다. 이제 nStr객체를 들고있다면 c++ runtime에서는 `foo("wow")`와 같이 호출할 수 있습니다.<br/>
이제 작성한 foo()를 tbridger를 사용해서 Byeol Runtime으로 연결할 겁니다.<br/>
nStr은 baseObj이므로 이 경우 origin 객체에 의해서 함수가 노출되며, 이 origin 객체는 객체에 속하지 않고 static으로 정의되어 하나를 공유하게 됩니다.<br/>
그래서 nStr::getOrigin() 함수를 먼저 찾아보면, 어디서 origin 객체를 가져오는 지 알 수 있죠.
```cpp
@style: language-cpp verified
    const baseObj& me::getOrigin() const {
        const baseObj& supers = super::getOrigin();
        return &supers == this ? _defaultOrg() : supers; // _defaultOrg()에서 가져오는 군요?
    }

    static const baseObj& _defaultOrg() {
        static tbaseObjOrigin<me> org(tbridger<me>::ctor() // 역시 tbridger를 사용합니다.
          .ctor<nStr>()
          .func("len", &me::len)
          .func<nchar, nidx>(func::GETTER_NAME, &me::get)
          .func("substr", &me::substr)
          .func(func::GETTER_NAME, new getSeqFunc())
          .func("iterate", new iterateFunc())
          // 즉, 대충 여기 어딘가에 우리의 foo() 함수를 정의해야 합니다.
          // 물론 순서는 상관없습니다. 맨 윗줄에 넣어도, 맨 아랫줄에 넣어도.
          .func("foo", &me::foo) // <-- 이 1줄이면 노출이 끝납니다.

          .func("getElemType", new getElemType())
          .subs());
        return org;
    }
```

이제 빌드를 해봐서 테스트 해봅시다.

```bash
@style: language-txt verified
$ ./build/builder.py dbg
```

성공했다면, TC를 넣어서 확인 해봅시다.<br/>
c++ 런타임뿐만 아니라 byeol runtime에서도 확인이 필요하므로 integTest에 넣으면 좋아보입니다.<br/>
test/integTest/core/usual 에 primitive타입을 테스트하는 primitiveTypeIntegTest에 TC를 추가해봅시다.
```cpp
@style: language-cpp verified

TEST_F(primitiveTypeIntegTest, testFooFunc) {
    make()
        .parse(R"SRC(
        main() int
            value := "someString"
            ret value.foo('hello')
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res); // foo(str)의 결과가 여기에 담겨야 겠죠.
    ASSERT_EQ(*res.cast<nint>(), 3); // 3이 나와야 합니다.
}
```

빌드후 TC를 돌려봐야 겠죠?
```
@style: language-txt verified
$ ./build/builder.py dbg
# 빌드 완료후
$ cd /bin # TC 돌릴때는 pack 경로 문제가 있어서 bin 폴더에서 하는게 좋습니다.
$ ./test --gtest_filter="primitiveTypeIntegTest.testFooFunc"

# 잘되면 모든 TC도 한번 돌려보죠.
$ ./test

# 에러가 발생하면 verbose로 로그를 상세히 봐도 좋습니다.
$ ./test --gtest_filter="primitiveTypeIntegTest.testFooFunc" verbose
```

자, 커밋할 준비가 된 것 같네요. 그러면 마지막으로 코드 포맷터를 돌려줍니다.
```
@style: language-txt verified
$ ./build/builder.py format
```

그럼, 이제 commit을 만들고 PR을 올리면 됩니다.

---

### 새로운 문법(Syntax)을 추가하고 싶어요
- core 모듈의 `parser` 클래스를 확인하세요. 그리고 해당 문법을 표현하기 위한 새로운 `expr` 클래스를 만드세요.
- 마지막으로 신규 `expr`을 검증하기 위한 로직을 `verifier`에 추가해야 합니다.
- <b>핵심 가이드</b>: @ref ai-architecture-core 문서의 "파싱"과 "표현식 시스템" 섹션이 도움이 될 것입니다.

---

### 버그를 잡기 위해 로그를 추가하거나, 새로운 로그 기능을 넣고 싶어요.
- 로그 추가라면 `BY_I` 와 같은 매크로에 대한 사용법까지만 보셔도 됩니다.
- 새로운 기능이라면 clog 모듈 문서를 참고하세요.

---

### 리플렉션 기능을 더 추가하고 싶어요.
- byeol runtime 환경에서의 타입 정보는 기본적으로 meta모듈에 의해서 제공됩니다.
- 그리고 core는 기본으로 제공되는 meta 모듈의 `type`을 확장해서 `ntype`이라고 하는 클래스를 사용합니다.
- 마지막으로 이 ntype을 byeol runtime 환경으로 노출하기 위해 core 모듈의 `tbridger` 를 사용하면 됩니다.

---

## 본격적으로 시작해보죠.

문서는 읽는 순서를 고려해서 위에서부터 아래로 구성되어 있습니다.
전체 문서를 다 순서대로 읽으시려면 맨 아래의 `다음 문서`를 계속 읽어나가시면 됩니다.

좀 자랑을 해보자면 다이어그램도 열심히 그려넣고 주석도 많이 채우고 공을 들였고,
분량이 너무 길어지지 않도록 간결하게 쓰려고도 노력을 기울였어요.

입맛에 잘 맞길 바라며, 개발 환경부터 구축하러 떠나보죠.


---
**다음 문서**: @ref ab-dev-env
