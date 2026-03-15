# 테스트 케이스 작성 가이드 {#ak-testing-guide}

byeol 프로젝트에서는 Google Test 프레임워크를 사용하여 테스트를 작성합니다. 테스트는 크게 세 가지 타입으로 나뉘며, 각 타입마다 특정 베이스 클래스를 상속받아야 합니다. Unit Test는 `byeolTest`를, Integration Test는 `byeolIntegTest`를, 그리고 E2E Test는 `byeolE2ETest`를 각각 상속받습니다. 이는 필수 규칙이며, 잘못된 베이스 클래스를 사용하면 테스트가 올바르게 동작하지 않습니다.

전체 테스트 구조는 `module/test/` 디렉토리 아래에 조직되어 있으며, `unit/`, `integ/`, `e2e/` 세 개의 주요 폴더로 구성됩니다. Unit Test는 각 모듈(core, memlite, indep, clog, meta, stela 등)별로 하위 폴더가 있습니다. Integration Test는 `integ/` 아래에 있으며, 일반적인 사용 패턴을 테스트하는 `usual/` 폴더와 오류 케이스를 검증하는 `antipattern/` 폴더로 나뉩니다. E2E Test는 `e2e/` 폴더에 위치합니다.

---

## Unit Test (단위 테스트)

Unit Test는 개별 클래스나 함수의 기능을 독립적으로 검증하는 테스트예요. 다른 모듈에 의존하지 않고 작은 단위의 로직을 세밀하게 테스트할 때 사용합니다. 예를 들어 `args` 클래스의 생성자가 올바르게 동작하는지, `add` 메서드가 예상대로 요소를 추가하는지 등을 확인합니다.

### 파일 위치와 네이밍

Unit Test 파일은 `module/test/unit/<모듈명>/` 디렉토리에 위치하며, 파일명은 `<컴포넌트명>Test.cc` 형식을 따릅니다. 예를 들어 `args` 컴포넌트를 테스트한다면 `argsTest.cc`가 되고, `expr` 컴포넌트라면 `exprTest.cc`가 됩니다. 이러한 네이밍 규칙은 프로젝트 전체에서 일관되게 적용되므로 반드시 준수해야 합니다.

### 기본 구조

Unit Test 파일의 기본 구조는 매우 단순해요. 먼저 `test/byeolTest.hpp` 헤더를 포함하고, `by`와 `std` 네임스페이스를 사용 선언합니다. 그런 다음 `byeolTest`를 상속받는 테스트 클래스를 정의합니다. 이 클래스는 별도의 멤버를 추가하지 않고 빈 구조체로 두는 것이 일반적이에요.

```cpp
#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct argsTest: public byeolTest {};
```

테스트 케이스는 Google Test의 `TEST_F` 매크로를 사용하여 작성합니다. 첫 번째 인자는 테스트 클래스명이고, 두 번째 인자는 테스트 케이스의 이름입니다. 테스트 케이스 이름은 무엇을 테스트하는지 명확하게 드러내야 합니다.

```cpp
TEST_F(argsTest, testDefaultConstructor) {
    args a;

    ASSERT_EQ(a.len(), 0);
    ASSERT_EQ(a.getMe(), nullptr);
}
```

### byeolTest 클래스의 주요 기능

`byeolTest` 클래스는 Google Test의 `::testing::Test`를 상속받으며, 몇 가지 유용한 메서드를 제공합니다. `SetUp()` 메서드는 각 테스트 케이스 실행 전에 자동으로 호출되어 테스트 환경을 초기화하고, `TearDown()` 메서드는 각 테스트 케이스 실행 후에 호출되어 리소스를 정리합니다. 이 메서드들을 오버라이드하여 테스트별 설정을 추가할 수 있습니다.

`negative()` 메서드는 에러 로그 출력을 억제하는 데 사용됩니다. Negative TC에서는 파서나 인터프리터가 의도적으로 에러를 발생시키는데, 시스템의 기본 동작이 에러 발생 시 로그를 출력하는 것이기 때문에 `negative()`를 호출하지 않으면 콘솔창이 지저분해집니다. `negative()`를 호출하면 이러한 에러 로그 출력을 억제하여 깔끔한 테스트 결과를 유지할 수 있습니다. 테스트가 성공할지 실패할지는 `ASSERT_FALSE` 같은 검증 매크로로 판단하는 것이며, `negative()`는 단순히 로그 출력만 제어합니다. `getReport()` 메서드는 현재 에러 리포트를 가져오며, 테스트 실행 중 발생한 오류를 확인할 때 유용합니다.

### Google Test 검증 매크로

테스트 결과를 검증할 때는 Google Test가 제공하는 매크로를 사용합니다. 가장 기본적인 `ASSERT_EQ(expected, actual)`는 두 값이 같은지 확인하고, `ASSERT_NE(val1, val2)`는 두 값이 다른지 확인합니다. 불리언 조건을 검사할 때는 `ASSERT_TRUE(condition)`와 `ASSERT_FALSE(condition)`을 사용합니다.

Google Test는 `ASSERT_*`와 `EXPECT_*` 두 가지 계열의 매크로를 제공합니다. `ASSERT_*`는 실패 시 테스트를 즉시 종료하고, `EXPECT_*`는 실패해도 테스트를 계속 진행합니다.

### 실전 예제

다음은 `args` 클래스의 다양한 생성자를 테스트하는 예제예요. 각 테스트는 특정 생성자 하나에 집중하며, 해당 생성자의 동작을 철저히 검증합니다.

```cpp
TEST_F(argsTest, testConstructorWithNarr) {
    narr arr;
    arr.add(new nInt(1));
    arr.add(new nInt(2));
    arr.add(new nInt(3));

    args a(arr);

    ASSERT_EQ(a.len(), 3);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 1);
    ASSERT_EQ(a[1].cast<nInt>()->get(), 2);
    ASSERT_EQ(a[2].cast<nInt>()->get(), 3);
}

TEST_F(argsTest, testMixedArgumentTypes) {
    args a;

    a.add(new nInt(42));
    a.add(new nStr("hello"));
    a.add(new nFlt(3.14f));
    a.add(new nBool(true));

    ASSERT_EQ(a.len(), 4);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 42);
    ASSERT_EQ(a[1].cast<nStr>()->get(), "hello");
    ASSERT_EQ(a[2].cast<nFlt>()->get(), 3.14f);
    ASSERT_EQ(a[3].cast<nBool>()->get(), true);
}
```

---

## Integration Test (통합 테스트)

Integration Test는 여러 컴포넌트가 함께 동작하는 시나리오를 테스트합니다. byeol 언어의 경우 소스 코드를 파싱하고, 검증하고, 실행하는 전체 과정을 통합적으로 테스트하게 됩니다. Unit Test가 개별 클래스의 메서드를 테스트한다면, Integration Test는 실제 byeol 코드를 작성하여 전체 시스템이 올바르게 동작하는지 확인합니다.

### Antipattern Test?

byeol 문법상으로는 올바르나, 숙련된 개발자라면 당연히 불가능하다고 여기는 부분이 있을 수 있어요.
Antipattern Test는 이러한 문법 정의의 허점이 있는 지를 테스트하는 TC를 모아둔 거예요.

### 파일 위치와 네이밍

Integration Test는 `module/test/integ/` 디렉토리 아래에 위치하며, 두 가지 하위 폴더로 나뉩니다. `usual/` 폴더에는 일반적인 사용 패턴을 테스트하는 파일들이 있고, `antipattern/` 폴더에는 오류 케이스를 검증하는 파일들이 있습니다.

일반 Integration Test 파일은 `<기능명>IntegTest.cc` 형식으로 명명합니다. 예를 들어 함수 정의 기능을 테스트한다면 `defFuncIntegTest.cc`, 배열 기능을 테스트한다면 `arrIntegTest.cc`가 됩니다. Antipattern 테스트는 `<기능명>AntiTest.cc` 형식을 사용합니다. 예를 들어 `arrAntiTest.cc`, `defFuncAntiTest.cc` 등입니다.

### 기본 구조

Integration Test는 Unit Test와 달리 익명 네임스페이스를 사용하는 것이 일반적이에요. 이는 테스트 클래스가 해당 파일에만 보이도록 제한하여 링커 충돌을 방지하기 위함입니다.

```cpp
#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defFuncIntegTest: public byeolIntegTest {};
}

TEST_F(defFuncIntegTest, testScenario) {
    // 테스트 코드
}
```

### byeolIntegTest의 빌더 패턴

`byeolIntegTest` 클래스는 메서드 체이닝을 지원하는 빌더 패턴 API를 제공합니다. 먼저 `make()` 메서드로 새로운 인터프리터를 생성하고, `parse()` 메서드로 byeol 소스 코드를 파싱합니다. 그런 다음 `shouldParsed()` 또는 `shouldVerified()` 메서드로 결과를 검증합니다.

```cpp
make()
    .parse(R"SRC(
    main() void
        a := 10
)SRC")
    .shouldParsed(true);
```

`parse()` 메서드에는 byeol 소스 코드를 전달하는데, 이때 C++11의 Raw String Literal을 사용합니다. `R"SRC( ... )SRC"` 형식을 사용하면 이스케이프 문자 없이 코드를 작성할 수 있고, 들여쓰기도 그대로 유지되죠. 시작과 끝의 구분자(여기서는 `SRC`)는 반드시 일치해야 합니다.

### 파싱 결과 검증

`shouldParsed(true)`는 파싱이 성공해야 함을 의미하고, `shouldParsed(false)`는 파싱이 실패해야 함을 의미해요. 문법 오류가 있는 코드를 테스트할 때는 `shouldParsed(false)`를 사용합니다. 이 메서드는 실제 결과와 기대값이 일치하면 `true`를 반환하므로, `if`문과 함께 사용하여 파싱 성공 후 추가 검증을 수행할 수 있습니다.

```cpp
if(make()
        .parse(R"SRC(
    main() void
        a := 22
)SRC")
        .shouldParsed(true)) {

    node& res = getSubPack() OR_ASSERT(res);
    // res를 사용한 추가 검증
}
```

`getSubPack()` 메서드는 파싱된 서브 패키지 노드를 반환합니다. 이 노드를 통해 파싱 결과의 AST(추상 구문 트리)에 접근할 수 있습니다. `OR_ASSERT` 매크로는 byeol 프로젝트에서 제공하는 편의 매크로로, 변수가 유효하지 않으면 테스트를 즉시 실패시키고 종료합니다. 이를 사용하면 null 포인터 역참조로 인한 세그멘테이션 폴트를 방지할 수 있습니다.

### Negative 테스트

오류 케이스를 테스트할 때는 `negative()` 메서드를 호출해야 합니다. Negative TC에서는 파서나 인터프리터가 의도적으로 실패하는데, 시스템의 기본 동작이 실패 시 로그를 출력하는 것이기 때문에 `negative()`를 호출하지 않으면 마치 오동작한 것처럼 로그창이 지저분해집니다. `negative()`를 호출하면 이러한 예상된 실패에 대한 로그 출력을 억제하여 테스트 결과를 깔끔하게 유지할 수 있습니다. Antipattern 테스트에서는 거의 모든 테스트 케이스에 `negative()`가 포함됩니다.

```cpp
TEST_F(defFuncIntegTest, distinguishDefineFuncOrCall2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            foo(a, 22) void
                a.doSomething(22)
            foo(a, 22)
    )SRC")
        .shouldParsed(false);
}
```

### shouldParsed vs shouldVerified

byeol 시스템은 파싱(parsing)과 검증(verification) 두 단계로 나뉩니다. 파싱은 문법적으로 올바른지 확인하고, 검증은 타입이 맞는지, 변수가 정의되었는지 등을 확인합니다. 따라서 문법 오류를 테스트할 때는 `shouldParsed(false)`를 사용하고, 타입 오류나 정의되지 않은 변수 등을 테스트할 때는 `shouldParsed(true)` 후에 `shouldVerified(false)`를 사용합니다.

```cpp
TEST_F(arrAntiTest, emptyArrayWithoutType) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {}
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}
```

위 예제에서 빈 배열 `{}`는 문법적으로는 올바르지만(파싱 성공), 타입을 추론할 수 없으므로 검증 단계에서 실패합니다.

### 런타임 에러 테스트

파싱과 검증을 모두 통과하지만 실행 시 에러가 발생하는 경우도 있습니다. 예를 들어 배열 인덱스가 범위를 벗어나는 경우입니다. 이런 경우 `shouldVerified(true)`로 검증을 통과시킨 후, `run()` 메서드로 실제 코드를 실행하고 에러를 검증합니다.

```cpp
TEST_F(arrAntiTest, negativeArrayIndexNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[-1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    nerr& resErr = res.cast<nerr>() OR_ASSERT(resErr);
    ASSERT_EQ(resErr.getErrCode(), errCode::OUT_OF_RANGE);
}
```

`run()` 메서드는 코드를 실행하고 결과를 반환합니다. 정상 실행 시 결과값을 반환하고, 에러 발생 시 `nerr` 객체를 반환합니다. `res.cast<nerr>()`로 에러 객체로 캐스팅한 후 `getErrCode()`로 에러 코드를 확인할 수 있습니다. `errCode::OUT_OF_RANGE`, `errCode::NULL_REF`, `errCode::TYPE_MISMATCH` 등 다양한 에러 코드가 정의되어 있습니다.

### 실전 예제

다음은 함수 정의와 함수 호출을 구분하는 기능을 테스트하는 예제예요.

```cpp
TEST_F(defFuncIntegTest, distinguishDefineFuncOrCall) {
    if(make()
            .parse(R"SRC(
        foo(x int, y int) void
            ret

        main() void
            a := 22
            foo(a, 22)
    )SRC")
            .shouldParsed(true)) {

        node& res = getSubPack() OR_ASSERT(res);

        const baseFunc& f = res.sub<baseFunc>("main", narr()) OR_ASSERT(f);
        ASSERT_EQ(f.getParams().len(), 0);
        ASSERT_EQ(f.getRet()->getType(), ttype<nVoid>());
    }
}
```

이 테스트는 `foo` 함수 정의와 `main` 함수 정의, 그리고 `foo` 함수 호출이 올바르게 파싱되는지 확인합니다. 파싱된 결과에서 `main` 함수를 찾아 파라미터 개수와 리턴 타입을 검증합니다.

---

## E2E Test (종단간 테스트)

E2E Test는 시스템의 전체 흐름을 종단간(end-to-end)으로 테스트합니다. byeol 프로젝트에서는 주로 CLI 인터페이스를 통해 실제 `.byeol` 파일을 파싱하고 실행하는 시나리오를 테스트합니다. Unit Test와 Integration Test가 내부 API를 직접 호출한다면, E2E Test는 사용자가 실제로 사용하는 방식 그대로 시스템을 검증합니다.

### 파일 위치와 네이밍

E2E Test 파일은 `module/test/e2e/` 디렉토리에 위치하며, `<기능명>E2ETest.cc` 형식으로 명명합니다. 현재 프로젝트에는 `cliE2ETest.cc` 파일이 있습니다.

### 기본 구조

E2E Test는 Unit Test와 비슷하게 네임스페이스 없이 테스트 클래스를 정의합니다.

```cpp
#include "test/byeolE2ETest.hpp"

using namespace by;
using namespace std;

struct cliE2ETest: public byeolE2ETest {};

TEST_F(cliE2ETest, testScenario) {
    // 테스트 코드
}
```

### 테스트 데이터 파일

E2E Test는 실제 파일 시스템의 파일을 사용하므로, 테스트 데이터 파일을 준비해야 합니다. 테스트 데이터 파일은 `module/test/testdata/` 디렉토리에 배치합니다. 빌드 시 `testdata` 디렉토리는 자동으로 `bin/` 폴더에 복사되므로, 테스트 실행 시 별도의 설정 없이 사용할 수 있습니다. 예를 들어 다음과 같은 파일들을 준비할 수 있습니다.

`testdata/a-main.byeol`:
```byeol
main() int
    ret foo() + 32
```

`testdata/a.byeol`:
```byeol
foo() int
    ret 100
```

첫 번째 파일은 `foo` 함수를 호출하지만 정의하지 않으므로 단독으로는 실행할 수 없습니다. 두 번째 파일과 함께 로드되어야 정상 동작합니다.

### parse와 run

`byeolE2ETest`의 `parse()` 메서드는 가변 인자를 받아 여러 파일 경로를 전달받습니다. 첫 번째 인자는 파일 개수이고, 그 다음부터는 파일 경로들입니다.

```cpp
auto res = parse(2, "testdata/a-main.byeol", "testdata/a.byeol").run();
```

`run()` 메서드는 CLI를 실행하고 `programRes` 구조체를 반환합니다. 이 구조체는 `res` 필드(프로그램 종료 코드)와 `rpt` 필드(에러 리포트)를 가지고 있습니다. 정상 실행 시 `res`는 프로그램의 리턴값이 되고 `rpt`는 null입니다. 에러 발생 시 `res`는 0이 아니고 `rpt`는 에러 정보를 담고 있습니다.

```cpp
auto res = parse(2, "testdata/a-main.byeol", "testdata/a.byeol").run();
ASSERT_EQ(res.res, 132);      // 100 + 32 = 132
ASSERT_FALSE(res.rpt);        // 에러 없음
```

### Negative 테스트

E2E Test에서도 `negative()` 메서드를 사용하여 실패를 기대하는 테스트를 작성할 수 있습니다. `negative()`를 호출하면 실패 시 발생하는 로그 출력을 억제하여 테스트 결과를 깔끔하게 유지할 수 있습니다.

```cpp
TEST_F(cliE2ETest, interpretPartialFileNegative) {
    auto res = negative().parse(1, "testdata/a-main.byeol").run();
    ASSERT_NE(res.res, 0);
    ASSERT_TRUE(res.rpt);
}
```

이 테스트는 `a-main.byeol` 파일만 로드하면 `foo` 함수가 정의되지 않아 실패해야 함을 검증합니다.

---

## 테스트 작성 시 주의사항

### 올바른 베이스 클래스 상속

가장 흔한 실수 중 하나는 잘못된 베이스 클래스를 상속하는 것입니다. Unit Test를 작성하면서 `byeolIntegTest`를 상속받거나, Integration Test에 `byeolTest`를 사용하면 필요한 메서드를 사용할 수 없거나 테스트가 올바르게 동작하지 않습니다. 파일 위치와 테스트 타입에 맞는 베이스 클래스를 선택해야 합니다.

### negative() 호출

Antipattern 테스트나 오류를 기대하는 테스트에서는 반드시 `negative()`를 호출해야 합니다. Negative TC에서는 파서나 인터프리터가 의도적으로 실패하는데, `negative()`를 호출하지 않으면 실패 시 발생하는 로그가 그대로 출력되어 마치 오동작한 것처럼 로그창이 지저분해집니다. `negative()`를 호출하면 이러한 예상된 실패에 대한 로그 출력을 억제하여 깔끔한 테스트 결과를 유지할 수 있습니다.

```cpp
// 잘못된 예
make()
    .parse(R"SRC(
    main() void
        arr := {1, 2, 3}
        val := arr[-1]
)SRC")
    .shouldVerified(true);

// 올바른 예
make()
    .negative()
    .parse(R"SRC(
    main() void
        arr := {1, 2, 3}
        val := arr[-1]
)SRC")
    .shouldVerified(true);
```

### OR_ASSERT 사용

OR 매크로는 참조자를 가져올때 `nullptr` 일때는 함수를 종료합니다. 이를 응용한 `OR_ASSERT`는 `nullptr` 일때 테스트를 실패하게 합니다.
필요에 의해서 편하게 사용하면 됩니다.

```cpp
// 기존 코드
node* res = getSubPack();
ASSERT_TRUE(res);
const baseFunc& f = res->sub<baseFunc>("main", narr());

// OR_ASSERT 적용: 
node& res = getSubPack() OR_ASSERT(res); // 참조자로 받을 수 있게 됨.
const baseFunc& f = res.sub<baseFunc>("main", narr()) OR_ASSERT(f);
```

### shouldParsed vs shouldVerified vs run()

세 가지 검증 단계를 명확히 구분해야 합니다. 문법 오류는 `shouldParsed(false)`, 타입 오류나 미정의 변수는 `shouldVerified(false)`, 런타임 오류는 `run()` 후 에러 검증으로 테스트합니다.

### Raw String Literal 구분자

Raw String Literal을 사용할 때 시작 구분자와 끝 구분자가 일치해야 합니다. `R"SRC(...)`로 시작했다면 반드시 `)SRC"`로 끝나야 합니다. 구분자는 `SRC` 외에 다른 문자열을 사용해도 되지만, 일관성을 위해 프로젝트에서는 `SRC`를 사용합니다.

### SetUp/TearDown 오버라이드

`SetUp`이나 `TearDown` 메서드를 오버라이드할 때는 반드시 부모 클래스의 메서드를 호출해야 합니다. 이를 빠뜨리면 부모 클래스의 초기화나 정리 작업이 수행되지 않아 테스트가 올바르게 동작하지 않을 수 있습니다.

```cpp
void SetUp() override {
    byeolIntegTest::SetUp();  // 먼저 부모 호출 필수
    arr::_cache.clear();
}

void TearDown() override {
    // 정리 작업
    byeolIntegTest::TearDown();  // 나중에 부모 호출 필수
}
```

### 테스트 독립성

각 테스트 케이스는 다른 테스트에 의존하지 않고 독립적으로 실행 가능해야 합니다. 전역 변수나 공유 상태를 사용하면 테스트 실행 순서에 따라 결과가 달라질 수 있으므로 피해야 합니다. Google Test는 테스트 실행 순서를 보장하지 않으므로, 순서에 의존하는 테스트는 작성하지 않아야 합니다.

### 한 가지만 테스트

하나의 TC는 하나의 주제만 테스트해야 합니다. 하나의 TC에 여러 개의 주제를 테스트하면 TC가 실패했을 때 어느 부분에서 에러가 발생했는지 찾기가 어렵습니다. 예를 들어 생성자 테스트, 메서드 테스트, 엣지 케이스 테스트는 각각 별도의 TC로 작성하는 것이 좋습니다. 각 TC는 명확한 하나의 목적을 가져야 하며, 실패 시 즉시 무엇이 잘못되었는지 알 수 있어야 합니다.

### 명확한 테스트 이름

테스트 케이스 이름만 보고도 무엇을 테스트하는지 알 수 있어야 합니다. `test1`, `test2` 같은 이름 대신 `testDefaultConstructor`, `testConstructorWithNarr` 같이 구체적인 이름을 사용합니다.

### Negative 테스트 포함

정상 케이스뿐만 아니라 오류 케이스도 반드시 테스트해야 합니다. 시스템이 잘못된 입력에 대해 적절히 에러를 발생시키는지 확인하는 것도 중요합니다. Antipattern 테스트를 통해 문법 자체에 허점이 있는지도 평가하면 좋습니다.

### 리소스 정리

테스트가 생성한 리소스는 반드시 정리해야 합니다. 테스트간 오염되지 않기 위함입니다.

---

## 디버깅

### 특정 테스트 케이스만 실행하기

전체 테스트를 실행하면 시간이 오래 걸리므로, 개발 중에는 특정 테스트 케이스만 실행하는 것이 효율적입니다. Google Test는 `--gtest_filter` 옵션을 제공하여 필터링을 지원합니다.

```bash
cd bin
./test --gtest_filter="argsTest.testDefaultConstructor" verbose
```

와일드카드를 사용하여 패턴 매칭도 가능합니다. 예를 들어 `argsTest`의 모든 테스트를 실행하려면 `"argsTest.*"`를, 이름에 `Integ`가 포함된 모든 테스트를 실행하려면 `"*IntegTest.*"`를 사용합니다.

### verbose 모드

명령줄 끝에 `verbose`를 추가하면 상세한 로그를 볼 수 있습니다. 테스트가 실패했을 때 원인을 파악하는 데 매우 유용합니다.

### 디버거 사용

심각한 버그나 세그멘테이션 폴트가 발생하면 디버거를 사용해야 합니다. Linux에서는 GDB를, macOS에서는 LLDB를 추천합니다.

```bash
cd bin
gdb ./test

(gdb) run --gtest_filter="argsTest.testDefaultConstructor" verbose
(gdb) break argsTest.cc:10
(gdb) continue
(gdb) print variable_name
(gdb) backtrace
```

macOS LLDB:
```bash
lldb ./test

(lldb) run --gtest_filter="argsTest.testDefaultConstructor" verbose
(lldb) breakpoint set --file argsTest.cc --line 10
(lldb) continue
(lldb) print variable_name
(lldb) bt
```

### 에러 리포트 확인

테스트 실행 중 발생한 에러는 `getReport()` 메서드로 확인할 수 있습니다. 디버깅 목적으로 에러 리포트를 콘솔에 출력하면 문제를 빠르게 파악할 수 있습니다.

```cpp
errReport& rpt = getReport();
if(rpt) {
    std::cout << rpt.toString() << std::endl;
}
```

---

### 테스트 실행

빌드가 완료되면 `bin` 디렉토리에 실행 파일이 생성됩니다.

```bash
cd bin
./test
```

전체 테스트를 실행하면 시간이 오래 걸리므로, 개발 중에는 `--gtest_filter`를 사용하여 필요한 테스트만 실행하는 것이 좋습니다.

---

**다음 문서**: @ref al-build-structure
