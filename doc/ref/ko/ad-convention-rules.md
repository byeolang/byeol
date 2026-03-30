# Byeol 코딩 규칙 {#ad-convention-rules}

이 문서는 Byeol 프로젝트의 코딩 표준과 규칙을 정의합니다. 모든 기여자는 코드 일관성과 품질을 유지하기 위해 이 규칙을 따라야 합니다.

---

## 시작하기 전에

일단, 간단히 byeol 코드 snippet으로 스타일을 한번 살펴보죠.

```cpp
    str me::eval(const args& a) {
        BY_I_SCOPE("func %s(%s)@%s", getSrc(), getParams(), (void*) this);
        WHEN(!thread::get().isInteractable())
            .err("thread isn't interactable").ret(nerr::newErr(errCode::THERE_IS_NO_FRAMES_IN_THREAD));

        // s is from heap space. but freed by _outFrame() of this class.
        tstr<scope> s = _evalArgs(a) OR.ret(str());
        node& meObj = a.getMe() OR.err("meObj == null").ret(str());
        return _interactFrame(meObj, *s, thread::get().getEx().len() - 1);
    }
```

코드의 독해를 떠나서 스타일만 보셨을 때 어떠셨나요? 아마 포인터와 -> 화살표
연산자, STL등 표준라이브러리, unique_ptr이나 snake case, 중괄호 등에
익숙하셨을텐데 이 코드에서는 <b>일반적인 C++ 컨벤션과 많이 다르다</b>는 느낌을 받으셨을 겁니다.
여기에는 제 개인적인 스타일 취향도 있지만, 분명 의도가 있습니다.


### 왜 일반적인 C++ 컨벤션과 다른가?

Byeol 프로젝트의 C++ 코딩 규칙은 일반적인 C++ 컨벤션(예: Google C++ Style Guide, LLVM Coding Standards)과 <b>의도적으로 다릅니다</b>. <b>Byeol 언어의 핵심 철학을 C++ 구현체에도 그대로 적용</b>하기 위한 선택이에요.

### Byeol 언어의 핵심 철학을 요약하면

Byeol 언어는 간결함을 최우선 가치로 두고 설계되었습니다. 불필요한 복잡성을 제거하고 핵심적인 기능만 남겨, 코드를 읽는 사람이 의도를 빠르게 파악할 수 있도록 합니다. 이와 함께 일관성 또한 중요한 원칙입니다. 한 곳에서 동작하는 방식은 다른 모든 곳에서도 동일하게 동작해야 하며, 이를 통해 학습 곡선을 낮추고 예측 가능한 코드를 작성할 수 있습니다.

또한 Byeol은 작은 개념들을 조합하여 복잡한 기능을 만드는 조합성을 강조합니다. 하나의 기능을 위한 전용 문법을 만들기보다는, 기존의 간단한 개념들을 엮어서 표현할 수 있도록 설계되었습니다. 특히 Byeol에는 클래스라는 개념이 없고, 모든 객체 자체가 타입입니다. 이는 객체와 클래스의 구분을 없애 더 직관적인 프로그래밍을 가능하게 합니다.

마지막으로 Byeol은 작은 장난감처럼 친숙하게 느껴지면서도, 실제로는 완전한 언어 기능을 제공하는 것을 목표로 합니다. 처음 접하는 사람도 쉽게 시작할 수 있지만, 깊이 파고들면 필요한 모든 것을 할 수 있는 언어를 지향합니다.

### C++ 코드에 반영된 철학

이러한 Byeol의 설계 철학은 C++ 코드 스타일에도 크게 반영되어 있습니다. 예를 들어 byeol 언어 컨벤션을 최대한 동일하게 가져가기 위해 변수명은 모두 `camelCase`로 되어 있으며 함수명도 `len()`, `get()`, `make()`처럼 짧지만 의미가 명확한 이름을 선호합니다.

byeol 언어는 클래스가 없고, 객체가 타입을 대신한다는 면에서, 클래스와 변수가 예외 없이 동일한 `camelCase`를 따라야 한다는 네이밍 규칙이 반영되어 있습니다.

```cpp
// 일반적인 C++: PascalCase 클래스 + 장황한 이름
class UserManager {
    std::string getUserNameWithValidation() { /* ... */ }
};

// Byeol C++: camelCase, 간결한 이름
class userManager {
    str getName() { return _name; }
    int len() { return _name.length(); }
};
```

이처럼 C++ 코드를 작성하면서도 Byeol 언어의 철학을 일관되게 유지함으로써, 언어 사용자가 구현체 코드를 볼 때도 같은 사고방식으로 접근할 수 있도록 했습니다.

혹시 관심이 있다면 byeol 언어의 설계 철학에 대한 자세한 내용은 [디자인 철학 문서](https://byeol.io/guide/generated/ko/design-philosophy.html)를 참조하세요.

### 코딩 스타일은 clang-format으로 자동 적용

이 문서에 명시된 코딩 스타일은 clang-format을 통해 자동으로 적용되고 검증됩니다. 커밋을 제출하면 CI가 자동으로 `builder.py format` 명령을 실행하여 프로젝트 루트의 `.clang-format` 파일에 정의된 규칙대로 코드가 작성되었는지 확인합니다. 만약 스타일 위반이 발견되면 빌드가 실패하므로, 커밋 전에 로컬에서 미리 포맷을 적용해두면 좋아요.

로컬에서 포맷을 적용하려면 프로젝트 루트에서 다음 명령을 실행하면 됩니다:

> ./build/builder.py format

이를 통해 CI에서 스타일 위반으로 빌드가 실패하는 상황을 미리 방지할 수 있어요.

---

## 네이밍 규칙

### 클래스와 변수: camelCase

클래스명과 변수명은 항상 소문자로 시작하는 `camelCase`를 사용합니다.

#### ✅ 올바른 예제

```cpp
class userManager { /* ... */ };  // 클래스명
int myAge = 25;                   // 변수명
std::string userName;             // 변수명
bool isValid;                     // 변수명
```

#### ❌ 잘못된 예제

```cpp
class UserManager { };     // 틀림! (PascalCase 사용 금지)
class user_manager { };    // 틀림! (snake_case 사용 금지)
int my_age;                // 틀림! (snake_case 사용 금지)
```

### 매크로: UPPER_SNAKE_CASE

매크로와 `#define` 상수는 항상 `UPPER_SNAKE_CASE`를 사용합니다.

#### ✅ 올바른 예제

```cpp
#define MAX_BUFFER_SIZE 1024          // 상수
#define IS_DEBUG_MODE 1               // 조건부 컴파일
#define SQUARE(x) ((x) * (x))         // 함수형 매크로
```

#### ❌ 잘못된 예제

```cpp
#define maxBufferSize 1024     // 틀림! (camelCase 사용 금지)
#define MaxBufferSize 1024     // 틀림! (PascalCase 사용 금지)
```

### non public accessor를 위한 언더스코어 접두사

private, protected 접근제한자를 가진 멤버 변수나 함수는 언더스코어(`_`)로 시작합니다.

```cpp
// ✅ 올바른 예제
class userManager {
public:
    std::string getName() { return _name; }
    void setAge(int age) { _age = age; }

private:
    std::string _name;
    int _age;
    bool _isActive;
};

// ❌ 잘못된 예제
class userManager {
    std::string name;   // 틀림! 멤버 변수는 _로 시작
    int age;            // 틀림!
    bool isActive;      // 틀림!
};
```

### 네이밍 철학

간결하지만 의미가 명확한 이름을 선호합니다.

#### ✅ 올바른 예제

```cpp
// 간결하고 명확한 이름
class node { };              // ⭐ 좋음
class visitor { };           // ⭐ 좋음
int get();                   // ⭐ 좋음
void make();                 // ⭐ 좋음
bool eval();                 // ⭐ 좋음

// 약어도 의미가 명확하고, 자주 통용되는 단어면 OK
class ast { };               // Abstract Syntax Tree - 좋음
int len = str.len();         // len - 좋음
void init();                 // initialize - 좋음
```

#### ❌ 불필요하게 긴 예제

```cpp
// 너무 장황함 - 피할 것
class abstractSyntaxTreeNode { };        // 너무 김! ast 또는 astNode 사용
int getUserNameLengthValue();            // 너무 김! getUserNameLen() 충분
void processAndValidateUserInput();      // 너무 김! 두 함수로 분리
bool checkIfUserIsValid();               // 너무 김! isUserValid() 충분

// vs 간결한 버전
class astNode { };                       // ⭐ 좋음
int getUserNameLen();                    // ⭐ 좋음
void processInput();                     // ⭐ 좋음
void validateInput();                    // ⭐ 좋음
bool isUserValid();                      // ⭐ 좋음
```

#### 일반적인 네이밍 가이드

```cpp
bool isValid;              // Boolean - is, has, can 등으로 시작
int getAge();              // Getter - get + 명사
void setAge(int age);      // Setter - set + 명사
void parse();              // 동작 함수 - 동사로 시작
std::vector<user> users;   // 컬렉션 - 복수형
```

### 간단하고 직관적인 단어 선호

복잡하거나 어려운 단어보다는 직관적이고 쉬운 단어를 선호합니다. 누구나 쉽게 이해할 수 있는 일상적인 단어를 사용하면 코드의 가독성이 높아집니다.

```cpp
// ✅ 좋은 예 - 간단하고 직관적
void make();           // generate보다 선호
void add(user u);      // enroll보다 선호

// ❌ 나쁜 예 - 불필요하게 복잡
void generate();       // make가 더 직관적
void enroll(user u);   // add가 더 간단
void commence();       // start가 더 쉬움
void terminate();      // stop이 더 직관적
```

실제 코드 사례:

```cpp
// ✅ Byeol 프로젝트 실제 사용 예
class nodeFactory {
    node* make();              // 노드 생성
    void add(node* n);         // 노드 추가
};

class visitor {
    void visit(node& n);
};
```

### 자주 사용하는 동사

함수명에는 프로젝트에서 자주 사용되는 동사들을 우선적으로 사용합니다. 같은 의미를 가진 동사를 특별한 이유 없이 혼용하면 안 됩니다.

**자주 사용되는 동사 목록**:
- **add** - 원소 추가
- **del** - 원소 삭제
- **get** - 값 가져오기
- **set** - 값 설정
- **make** - 객체 생성
- **init** - 초기화
- **rel** - 해제/정리 (release)
- **eval** - 평가/실행
- **sub** - 하위 요소 접근
- **bind** - 바인딩
- **clone** - 복제
- **len** - 길이 반환
- **singleton** - 싱글톤 접근
- **work** - 작업 수행

**접두사 패턴**:
- **is** + 형용사 - Boolean 술어 (예: `isValid()`, `isEnd()`, `isNul()`)
- **has** + 명사 - 존재 여부 확인 (예: `has()`, `hasError()`)
- **can** + 동사 - 가능 여부 확인 (예: `canBind()`, `canMarshal()`)
- **on** + 동작 - 이벤트 핸들러 (예: `onVisit()`, `onScan()`, `onGet()`)
- **as** + 타입 - 타입 캐스팅 (예: `asInt()`, `asStr()`, `asBool()`)
- **to** + 타입 - 타입 변환 (예: `toStr()`, `toMgd()`, `toNative()`)

예를 들어 원소를 추가하는 함수는 `add()` 또는 `addElem()`을 사용하고, `insert`, `push`, `append` 등을 임의로 섞어 쓰지 않습니다.

```cpp
// ✅ 올바른 예 - 일관된 동사 사용
class container {
    void add(int value);       // 원소 추가
    void del(int index);       // 원소 삭제
    int get(int index);        // 값 가져오기
    void set(int index, int value);  // 값 설정
    int len();                 // 길이 반환
    container* clone();        // 복제
};

// ❌ 잘못된 예 - 동의어 혼용
class container {
    void add(int value);       // 추가
    void insert(int value);    // 추가 (혼란!)
    void push(int value);      // 추가 (혼란!)
    void append(int value);    // 추가 (혼란!)
    // → 모두 add로 통일하거나, 명확한 의미 차이가 있어야 함
};
```

단, 명확한 의미 차이가 있으면 다른 동사 사용 가능합니다.

```cpp
// ✅ 의미 차이가 명확한 경우
class list {
    void add(int value);           // 끝에 추가
    void insert(int index, int value);  // 특정 위치에 삽입 (의미 차이 명확)
};

class stack {
    void push(int value);          // 스택의 전통적인 인터페이스
    int pop();                     // 스택의 전통적인 인터페이스
};
```

---

## 코드 스타일

### 줄 길이와 들여쓰기

#### 줄 길이 제한

가독성을 위해 가능한 100 컬럼 이내로 작성하되, 부득이한 경우 120 컬럼까지 허용됩니다.
- **Soft Cap (권장)**: 100 컬럼 이내
- **Hard Cap (강제)**: 120 컬럼 이내 (clang-format 강제)


```sh
// ✅ 좋은 예 - 100 컬럼 이내 (권장)
/**
 * @brief 사용자 정보를 검증하고 데이터베이스에 저장합니다
 * @param user 검증할 사용자 객체
 * @return 성공 시 true, 실패 시 false
 */

// ⚠️ 허용되지만 비권장 - 120 컬럼 이내
/** 
 * @brief 사용자 정보를 검증하고 유효한 경우 데이터베이스에 저장하며 저장 후 로그를 남기고 캐시를 업데이트합니다
 */

// ❌ 나쁜 예 - 120 컬럼 초과 (clang-format 오류)
/**
 * @brief 이 함수는 사용자 정보를 검증하고 유효한 경우 데이터베이스에 저장하며 저장 후 로그를 남기고 캐시를 업데이트하며 이메일 알림을 전송합니다
 */

// ✅ 개선 - 여러 줄로 분리
/**
 * @brief 사용자 정보를 검증하고 데이터베이스에 저장합니다
 * @details 검증 성공 시 데이터베이스에 저장하고,
 * 저장 후 로그를 남기며 캐시를 업데이트합니다.
 */
```

#### 들여쓰기: 4 스페이스

```cpp
// ✅ 올바른 들여쓰기
class myClass {
    void myMethod() {
        if(condition) {
            for(int i = 0; i < 10; i++) {
                processItem(i);
            }
        }
    }
};

// ❌ 탭 사용 금지
class myClass {
→   void myMethod() {  // 탭 사용 - 틀림!
→   →   process();
→   }
};
```

#### 중괄호 스타일: 같은 줄에 배치

중괄호 `{`는 선언과 같은 줄에 배치합니다 (K&R style).

```cpp
// ✅ 올바른 스타일 - 같은 줄
void myFunction() {
    // ...
}

class myClass {
public:
    void method() {
        // ...
    }
};

if(condition) {
    doSomething();
}

// ❌ 잘못된 스타일 - 다음 줄
void myFunction()
{  // 틀림!
    // ...
}

class myClass
{  // 틀림!
    // ...
};
```

#### 포인터/참조 정렬: 왼쪽 정렬

`*`와 `&`는 타입 쪽(왼쪽)에 붙입니다.

```cpp
// ✅ 올바른 스타일
int* ptr;
const std::string& name;
node* getNode();
void process(const obj& o);

// ❌ 잘못된 스타일
int *ptr;          // 틀림!
int * ptr;         // 틀림!
const std::string &name;  // 틀림!
node *getNode();   // 틀림!
```

**여러 변수 선언 시 주의**:

```cpp
// ⚠️ 주의: 한 줄에 여러 포인터 선언 시 각각 * 필요
int* a, *b, *c;  // a, b, c 모두 포인터

// ✅ 권장: 명확성을 위해 각각 선언
int* a;
int* b;
int* c;
```

#### 괄호 앞 공백: 공백 없음

함수 호출, 조건문 등의 괄호 `(` 앞에 공백을 넣지 않습니다.

```cpp
// ✅ 올바른 스타일
if(condition) { }
for(int i = 0; i < 10; i++) { }
while(running) { }
myFunction(arg1, arg2);
sizeof(int);

// ❌ 잘못된 스타일
if (condition) { }      // 틀림!
for (int i = 0; ...) { }  // 틀림!
myFunction (arg1, arg2);  // 틀림!
sizeof (int);            // 틀림!
```

#### 네임스페이스 들여쓰기: 들여쓰기 적용

네임스페이스 내부 코드도 들여쓰기를 적용합니다.

```cpp
// ✅ 올바른 스타일
namespace by {
    class node {
        void eval();
    };

    void helper() {
        // ...
    }
}  // namespace by

// ❌ 잘못된 스타일
namespace by {
class node {  // 틀림! 들여쓰기 없음
    void eval();
};

void helper() {  // 틀림! 들여쓰기 없음
    // ...
}
}
```

#### 짧은 함수/블록: 한 줄 허용

간단한 함수나 블록은 한 줄로 작성할 수 있습니다.

```cpp
// ✅ 한 줄 함수 (getter/setter 등)
int getAge() { return _age; }
void setAge(int age) { _age = age; }
bool isEmpty() { return _size == 0; }

// ✅ 짧은 if문
if(ptr) return;
if(x < 0) x = 0;
if(!isValid()) { logError(); return false; }

// ❌ 여러 구문을 가진 블록문은 한줄로 하지 말것.
{ int tmp = a; a = b; b = tmp; }

// ❌ 복잡한 로직은 여러 줄로
int calculate() {  // 한 줄로 하지 말 것
    int result = 0;
    for(int i = 0; i < 10; i++) result += i;
    return result;
}
```

#### 1줄 블록문: 중괄호 생략

블록문 본문이 1줄일 경우, 중괄호를 절대로 적지 않습니다. 블록문 표현식과 본문이 합쳐서 충분히 짧다면 한 줄에 작성할 수 있습니다.

```cpp
// ✅ 올바른 예 - 1줄이면 중괄호 없음
if(condition)
    doSomething();

for(int i = 0; i < 10; i++)
    process(i);

while(isRunning)
    update();

// ✅ 충분히 짧으면 한 줄로
if(ptr) return;
if(x < 0) x = 0;
for(int i = 0; i < 10; i++) process(i);
while(hasNext()) advance();

// ❌ 잘못된 예 - 1줄인데 중괄호 사용
if(condition) {
    doSomething();  // 틀림! 중괄호 제거해야 함
}

for(int i = 0; i < 10; i++) {
    process(i);     // 틀림! 중괄호 제거해야 함
}

// ✅ 2줄 이상이면 중괄호 필요
if(condition) {
    doSomething();
    doAnotherThing();
}

for(int i = 0; i < 10; i++) {
    process(i);
    log(i);
}
```

#### 접근 제어자 순서

클래스 내부에서는 함수를 먼저, 변수를 나중에 배치합니다. 
생성자와 연산자, 변수는 접근 제어자를 서술하여 단락을 구분합니다.

```cpp
// ✅ 올바른 예제
class myClass {
public:
    // 생성자
    myClass();
    myClass(int value);
    ~myClass();

public:
    // 연산자
    myClass& operator=(const myClass& other);
    bool operator==(const myClass& other) const;

public:
    // 일반 public 함수
    void process();
    int getValue() const;
    void setValue(int v);

protected:
    // protected 함수
    void onInitialize();
    void onDestroy();

private:
    // private 함수
    void init();
    void cleanup();
    bool validate();

private:
    // 멤버 변수는 마지막에
    int _value;
    std::string _name;
    bool _isValid;
};
```

#### Early-Return Pattern 선호

중첩된 조건문보다 early-return 패턴을 선호합니다.

```cpp
// ❌ 나쁜 예 - 중첩된 조건문
str evalGeneric(const args& a) {
    std::string key = makeKey(a);
    if(!key.empty()) {
        if(!isSelfMaking(key)) {
            // 실제 처리 로직
            if(!_cache.count(key)) makeGeneric(key, a);
            return _cache[key];
        } else {
            logError("error: you tried to clone self generic object.");
            return tstr<obj>();
        }
    } else {
        logError("key is empty");
        return tstr<obj>();
    }
}

// ✅ 좋은 예 - Early-Return 적용
str evalGeneric(const args& a) {
    std::string key = makeKey(a);
    if(key.empty()) {
        logError("key is empty");
        return tstr<obj>();
    }
    if(isSelfMaking(key)) {
        logError("error: you tried to clone self generic object.");
        return tstr<obj>();
    }

    // 실제 처리 로직
    if(!_cache.count(key)) makeGeneric(key, a);
    return _cache[key];
}

// ✅ 더 좋은 예 - WHEN 매크로 사용
str evalGeneric(const args& a) {
    std::string key = makeKey(a);
    WHEN(key.empty()).err("key is empty").ret(tstr<obj>());
    WHEN(isSelfMaking(key)).err("error: you tried to clone self generic object.").ret(tstr<obj>());

    // 실제 처리 로직
    if(!_cache.count(key)) makeGeneric(key, a);
    return _cache[key];
}
```

#### TO 매크로 사용

TO는 safe navigation을 C++에 구현한 것입니다. 이를 사용하여 간결한 코드를 작성하는 것이 매우 권장됩니다.

```cpp
// 일반적인 코드 - 매번 null 체크 필요
int getBrushColorCode(Resource r) {
    auto* pallete = r.getPallete();
    if(!pallete) {
        log("pallete is null");
        return -1;
    }

    auto* canvas = pallete->getCanvas();
    if(!canvas) {
        log("canvas is null");
        return -1;
    }

    Brush* brush = canvas->getBrush(BrushType.SYSTEM);
    if(!brush) {
        log("brush is null");
        return -1;
    }

    return brush->getColorCode();
}

// ✅ TO 매크로 사용 - 체이닝으로 간결하게
int getBrushColorCode(Resource r) {
    int* code = r TO(getPallete()) TO(getCanvas())
                  TO(getBrush(BrushType.SYSTEM)) TO(getColorCode());
    if(!code) {
        log("code is null");
        return -1;
    }

    return *code;
}
```

`WHEN`, `TO`등 매크로에 대한 자세한 내용은 @ref ae-architecture-overview 문서를 참조하세요.


---

## 문서화 표준

### Doxygen 주석 기본 규칙

#### /** 스타일만 사용

```sh
// ✅ 올바른 스타일
/**
 * @brief 파일을 읽습니다
 * @param path 파일 경로
 * @return 파일 내용
 */

// ❌ 금지된 스타일
/// @brief 파일을 읽습니다
/// @param path 파일 경로
/// @return 파일 내용
```

#### @ 접두사 사용

```sh
// ✅ 올바른 예
/**
 * @brief 사용자를 생성합니다
 * @param name 사용자 이름
 * @return 생성된 사용자 포인터
 */

// ❌ 잘못된 예 - \ 사용 금지
/**
 * \brief 사용자를 생성합니다   // 틀림!
 * \param name 사용자 이름       // 틀림!
 */
```

### 파일 주석

#### @file 태그

모든 헤더 파일 최상단에 `@file`을 추가합니다. 클래스가 없고 `#define`이나 `typedef`만 있는 경우에만 파일 설명을 추가합니다.

```sh
// 예제 1: 클래스가 있는 헤더 - 설명 불필요
/** @file */

#pragma once
#include "base.h"

/**
 * @ingroup core
 * @brief AST 노드를 표현하는 클래스
 */
class astNode {
    // ...
};
```

```sh
// 예제 2: 매크로만 있는 헤더 - 설명 필요
/**
 * @file
 * 문자열 조작을 위한 유틸리티 매크로
 *
 * 문자열 연결, 변환, 포매팅 등의 기능을 제공하는
 * 매크로 모음입니다.
 */

#pragma once

#define STR_CONCAT(a, b) a##b
#define STR_TO_UPPER(s) /* ... */
#define STR_FORMAT(fmt, ...) /* ... */
```

```sh
// 예제 3: typedef만 있는 헤더 - 설명 필요
/**
 * @file
 * 타입 별칭 정의
 *
 * 프로젝트 전반에서 사용되는 공통 타입 별칭을 정의합니다.
 */

#pragma once

typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::shared_ptr<node> nodePtr;
```

### 클래스 주석

클래스 주석에는 `@ingroup` (모듈 그룹), `@brief` (간단한 설명), `@details` (상세 설명) 태그를 포함합니다.

#### 예제 1: 간단한 클래스

```sh
/**
 * @ingroup core
 * @brief AST 노드의 기본 클래스
 * @details 모든 AST 노드가 상속받는 베이스 클래스입니다.
 * 노드 순회, 타입 체킹 등의 기본 기능을 제공합니다.
 */
class astNode {
public:
    virtual ~astNode() = default;
    virtual void accept(visitor& v) = 0;
};
```

#### 예제 2: 복잡한 클래스 - 여러 단락

```sh
/**
 * @ingroup memlite
 * @brief 메모리 풀 관리자
 * @details 고성능 메모리 할당을 위한 풀 기반 메모리 관리자입니다.
 *
 * 이 클래스는 고정 크기 메모리 블록을 미리 할당하고 재사용하여
 * 빈번한 메모리 할당/해제의 오버헤드를 줄입니다.
 *
 * 각 풀은 특정 크기의 객체를 관리하며, 요청된 크기에 따라
 * 적절한 풀을 자동으로 선택합니다. 스레드 안전성은 보장되지 않으므로
 * 다중 스레드 환경에서는 외부 동기화가 필요합니다.
 */
class memoryPool {
public:
    void* allocate(size_t size);
    void deallocate(void* ptr);
};
```


### 함수 주석

함수 주석에는 `@brief` (간단한 설명), `@param` (매개변수 설명), `@return` (반환값 설명) 태그를 필요에 따라 포함합니다.

```sh
// 예제 1: 간단한 함수 - @param 생략 가능
/** 
 * @brief 사용자 이름을 반환합니다
 */
std::string getName() const {
    return _name;
}

// 예제 2: @param과 @return이 필요한 경우
/**
 * @brief 사용자를 생성하고 검증합니다
 * @param username 사용자 이름 (3-20자의 영숫자)
 * @param email 이메일 주소 (게스트는 nullptr 가능)
 * @param age 나이 (0-150 범위)
 * @return 성공 시 사용자 포인터, 실패 시 nullptr
 */
user* createUser(const std::string& username, const char* email, int age);
```

### code 블록 사용

복잡한 API나 사용법이 명확하지 않은 경우 예제 코드 제공해야 합니다.

```sh
/**
 * @brief 설정 파일을 로드하고 파서를 초기화합니다
 * @details 복잡한 초기화 순서가 필요하므로 아래 예제를 참고하세요
 * @code
 *  configLoader loader;
 *  if(loader.load("config.stela")) {
 *      parser p;
 *      p.setConfig(loader.getConfig());
 *      p.init();
 *      auto ast = p.parse("main.by");
 *  }
 * @endcode
 */
```

### ref로 다른 코드 참조

```sh
/**
 * @brief @ref parser 를 사용하여 파일을 파싱합니다
 * @param p @ref parser 인스턴스
 * @return 파싱된 @ref astNode
 */
astNode* parseWithParser(parser& p);
```

### 개발 노트는 Doxygen 밖에

```sh
// ✅ 올바른 예
// TODO: 에러 핸들링 추가 필요
// FIXME: 메모리 누수 존재
// HACK: 임시 해결책, 나중에 리팩토링 필요
/**
 * @brief 파일을 읽습니다
 * @param path 파일 경로
 * @return 파일 내용
 */
std::string readFile(const std::string& path);

// ❌ 잘못된 예
/**
 * @brief 파일을 읽습니다
 * TODO: 에러 핸들링 추가 필요        // 틀림!
 * FIXME: 메모리 누수 존재            // 틀림!
 * @param path 파일 경로
 * @return 파일 내용
 */
std::string readFile(const std::string& path);
```

### 자명한 코드에는 주석 금지

```sh
// ❌ 나쁜 예 - 불필요한 주석
/** @brief 이름을 가져옵니다 */
std::string getName() { return _name; }  // 너무 당연함!

/** @brief 나이를 설정합니다 */
void setAge(int age) { _age = age; }     // 너무 당연함!

/** @brief i를 1 증가시킵니다 */
i++;                                      // 너무 당연함!

// ✅ 좋은 예 - 주석 없음 (자명함)
std::string getName() { return _name; }
void setAge(int age) { _age = age; }
i++;
```

```sh
// ✅ 좋은 예 - 주석이 필요한 경우
/**
 * @brief 캐시를 고려하여 값을 가져옵니다
 * @details 캐시에 값이 있으면 캐시에서, 없으면 DB에서 조회하고
 * 결과를 캐시에 저장합니다. TTL은 5분입니다.
 * @return 사용자 이름
 */
std::string getName() {
    if(_cache.has("name"))
        return _cache.get("name");

    auto name = _db.query("SELECT name FROM users WHERE id=?", _id);
    _cache.set("name", name, 300);  // 5분 TTL
    return name;
}
```

---

## 헤더 파일 포함

헤더 파일을 포함할 때는 가능한 전방 선언(forward declaration)을 사용하여 불필요한 의존성을 줄이는 것을 권장합니다. 포인터나 참조로만 사용하는 타입은 전방 선언만으로 충분합니다.

```cpp
// ✅ 전방 선언 사용
// file: moduleA.h
class moduleB;  // 전방 선언

class moduleA {
    moduleB* _b;  // 포인터만 사용하므로 전방 선언으로 충분
};

// ❌ 불필요한 include
// file: moduleA.h
#include "moduleB.h"  // 포인터만 사용하는데 include - 비권장

class moduleA {
    moduleB* _b;
};
```

---

**다음 문서**: @ref ae-architecture-overview
