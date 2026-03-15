# meta 모듈 - 런타임 타입 시스템 {#af-architecture-meta}

@ref meta 모듈은 런타임 타입 정보(RTTI)와 리플렉션 기능을 제공합니다. C++의 기본 RTTI보다 더 강력하고
효율적인 타입 시스템을 구현하고 있으며, Byeol 언어의 타입 시스템 기반이 됩니다.

---

## 메타 시스템 개요

메타 시스템은 프로그램 실행 중에 타입 정보를 조회하고 조작할 수 있게 해줍니다. 이는 다음과 같은
목적을 위해 사용됩니다:

1. 타입 계층 구조 탐색 (상속 관계 확인)
2. 타입 정보 조회 (이름, 추상 클래스 여부, 템플릿 여부 등)
3. 동적 인스턴스 생성
4. 효율적인 타입 캐스팅 (dynamic_cast보다 빠름)

메타 시스템의 핵심은 Monostate 패턴을 사용하여 매번 객체를 생성해도 내부 상태는 공유된다는 점입니다.
따라서 `ttype<MyClass>()`를 여러 번 호출해도 추가 비용이 들지 않습니다.

---

## 타입 정보 사용하기

### ttype<T> 클래스

@ref by::ttype "ttype" 클래스는 사용자가 메타 정보를 다루고자할때 진입점이 되는 클래스입니다. @ref by::ttype "ttype" 를
사용할때는 매번 객체를 생성해서 사용해야 합니다. 전체적으로 monostate 패턴으로 설계 되어있어서 매번
객체를 만들어 사용하더라도 값은 공유하기 때문에 추가비용은 들지 않습니다.

@ref meta 모듈의 전체적인 설계에 대해 파악하고자 한다면 핵심이 되는 @ref by::type "type" 을 먼저 살펴보세요.

**ttype 기본 사용 예제**

```
@style: language-cpp verified
class myClass {};

class myDerivedClass : public myClass {
    typedef myClass super;
};

// 타입 정보 조회
const type& typeInfo = ttype<myClass>();
std::cout << typeInfo.getName();  // "myClass" 출력

// 템플릿 여부 확인
bool isTemplate = ttype<myClass>().isTemplate();  // false

// 추상 클래스 여부 확인
bool isAbstract = ttype<myClass>().isAbstract();  // false

// 부모 타입 확인
const type& superType = ttype<myDerivedClass>().getSuper();
std::cout << superType.getName();  // "myClass" 출력

// 인스턴스 생성 (기본 생성자 필요)
myClass* instance = ttype<myClass>().makeAs<myClass>();
delete instance;
```

---

## type 클래스의 기능

@ref by::type "type" 클래스는 @ref meta 모듈의 핵심이 되는 클래스입니다. @ref by::type "type" 에 대한 다음의 기본적인 API를 제공합니다.

### 기본 타입 식별

다음 함수들로 타입의 기본 정보를 얻을 수 있습니다:

- `isTemplate()`: 템플릿 클래스 여부를 반환합니다.
- `isAbstract()`: 추상 클래스 여부를 반환합니다.
- `getName()`: 클래스명을 반환합니다. 이름은 demangle 되어 반환됩니다.

### 클래스 계층 관련 정보

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

**타입 계층 검사 예제**

```
@style: language-cpp verified
class Animal {};

class Dog : public Animal {
    typedef Animal super;
};

class Puppy : public Dog {
    typedef Dog super;
};

// 상속 관계 확인
const type& animalType = ttype<Animal>();
const type& dogType = ttype<Dog>();
const type& puppyType = ttype<Puppy>();

// Animal이 Dog의 부모인가?
bool isParent = animalType.isSuper(dogType);  // true

// Dog가 Animal의 자식인가?
bool isChild = dogType.isSub(animalType);  // true

// Puppy는 Animal의 자식인가? (간접 상속도 체크)
bool isDescendant = puppyType.isSub(animalType);  // true

// 형제 관계는 false
class Cat : public Animal {
    typedef Animal super;
};
bool areSiblings = ttype<Dog>().isSub(ttype<Cat>());  // false
```

### 인스턴스 생성

**make()**

이 type의 기본생성자로 인스턴스를 생성합니다. 만약, 기본생성자가 없다면 nullptr가 반환되니
주의하세요.

### 메타 타입 정보 관리

`init()`으로 초기화를, `rel()`로 해제를 합니다. 이는 `BY_INIT_META` 매크로를 통해서 자동으로
처리가 되는 부분이라 외부에서 직접 호출할 경우는 드뭅니다.

---

## 메타 정보의 생성과 관리

### 메타 정보가 어떻게 생성되나

`isTemplate()` 이나 `isAbstract()`, `getName()` 같은 타입정보는 @ref by::ttypeBase "ttypeBase" 에서
메타프로그래밍을 통해 채워줍니다. 따라서 @ref by::type::init() "init()"이 존재하는 이유는 클래스 계층을 구성하기
위해서입니다.

그리고 그 계층을 구성하는 핵심은 **모든 클래스는 typedef로 super를 정의해야 한다**라는 제약사항으로
해결합니다. 모든 클래스에 `super`가 존재한다면, @ref by::ttype::init() "init()"도 호출할 수 있기 때문에
다음과 같은 간단한 코드로 클래스 계층을 재귀적으로 구성할 수 있게 됩니다.

**참고**: 부모 클래스가 없는 최상위 클래스는 `typedef adam super;`로 정의하여 @ref by::adam "adam" 에 연결됩니다.
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

### 메타 정보 자동 생성

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

---

## adam - 최상위 타입

@ref by::adam "adam" 클래스는 모든 타입 계층의 루트입니다. @ref by::type "type" 의 클래스 계층구조 상 아무런 부모도 없는
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

이를 해결하고자 메타 정보를 구성할때 어떠한 부모클래스도 없는 클래스라면 부모를 @ref by::adam "adam" 으로 정의합니다.
이제 다음과 같이 작성할 수 있게 됩니다.

```
@style: language-cpp verified
for(const auto& t : ttype<adam>().getSubs())
    cout << t.getName() << "\n";
```

**adam을 활용한 타입 순회 예제**

```
@style: language-cpp verified
// 부모가 없는 여러 클래스들 정의
class Shape {
    // 부모가 없으므로 자동으로 adam이 부모가 됨
};

class Vehicle {
    // 부모가 없으므로 자동으로 adam이 부모가 됨
};

class Animal {
    typedef adam super;  // 명시적으로 adam을 부모로 지정
};

// 모든 최상위 타입들을 순회
const types& allTopLevelTypes = ttype<adam>().getSubs();

for(const type* t : allTopLevelTypes) {
    std::cout << "Top-level type: " << t->getName() << std::endl;
}
// 출력: Shape, Vehicle, Animal 등 모든 최상위 클래스들

// adam을 통해 특정 타입이 최상위 타입인지 확인
const type& shapeType = ttype<Shape>();
bool isTopLevel = (shapeType.getSuper() == ttype<adam>());  // true
```

---

## 메타 타입 확장하기

@ref by::type "type" 은 기본적으로도 비교적 많은 타입 정보를 제공하지만, byeol 처럼 언어를 다루는 경우에는 parameter나
return type과 같이 추가적인 정보를 담고 싶은 경우가 있습니다.

이때 쉽게 생각하면 @ref by::type "type" 을 상속받은 클래스를 만들면 되는거 아닌가라는 아이디어를 떠올리기 쉽지만
`ttype<T>`에서 언급한 것처럼, 항상 사용자의 최종 진입점은 `ttype<T>`에 접근하면서 시작되어야
합니다. 문제는 @ref by::ttype "ttype" 의 코드를 @ref meta 모듈을 종속하는 쪽에서 수정할 수는 없기 때문에 상속으로는
불가능하고 메타 타입을 주입하는 형태로 문제를 해결합니다.

핵심 코드는 @ref by::ttypeBase "ttypeBase" 에 있습니다.

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

tmetaTypeDef는 T에 typedef metaType이 있을 경우에는 해당타입을 반환하고, 없으면 @ref by::type "type" 을 반환합니다.
@ref by::ttype "ttype" 은 @ref by::ttypeBase "ttypeBase" 를 상속하며, @ref by::ttypeBase "ttypeBase" 는 바로 tmetaTypeDef<T>::is를 상속합니다.

이걸 통해서 만약 class T에 대해 ttype<T>를 호출하는 순간, class T의 개발자가 `typedef metaType
MyType;` 처럼 새로운 MyType 클래스를 선언하여 추가하면 해당 ttype<T>()로 객체를 만들었을때 MyType을
기반으로 해서 만들어지게 됩니다.

실제로 이 기능은 @ref core 모듈에서 @ref by::ntype "ntype" 을 주입하기 위해 사용합니다. 자세한 내용은 @ref by::ntype "ntype" 을 참조하세요.

---

**다음 문서**: @ref ag-architecture-memlite
