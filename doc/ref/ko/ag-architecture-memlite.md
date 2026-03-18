# memlite 모듈 - 커스텀 메모리 관리 {#ag-architecture-memlite}

@ref memlite 모듈은 참조 카운팅을 갖춘 커스텀 메모리 풀 시스템을 제공하여 자동 메모리 관리를 유지합니다.
Object Pool Pattern과 Smart Pointer Pattern을 활용하여 효율적인 메모리 관리를 구현합니다.

@ref memlite 의 궁극적인 목적은, byeol managed 환경을 실행할 수 있는 경량화된 C++ 메모리 관리에 있어요.
따라서 GC 등 추가적인 메모리 관리가 필요로 해지며, 이는 자체 메모리 풀을 가지고 있으며, 인스턴스의
라이프사이클을 관리해야 한다는 것을 시사합니다.

@startuml
package "메모리 관리 계층" {
    class "instancer" as instancer {
        - _pool : pool
        - _watcher : watcher
        ---
        + make(size) : void*
        + destroy(void*) : void
    }

    class "pool" as pool {
        - _chunks : chunks[]
        ---
        + get(size) : chunks*
        + new1(size) : void*
        + del(ptr, size) : void
    }

    class "chunks" as chunks {
        - _chunks : chunk[]
        - _s : ncnt
        - _blkSize : ncnt
        ---
        + new1() : void*
        + del(ptr, size) : void
        + resize() : void
    }

    class "chunk" as chunk {
        - _heap : void*
        - _head : ncnt
        - _len : ncnt
        - _size : ncnt
        - _blkSize : ncnt
        ---
        + new1() : void*
        + del(ptr, size) : void
        + has(ptr) : nbool
        + operator[](idx) : void*
    }

    class "watcher" as watcher {
        - _lives : life[]
        ---
        + reg(instance*) : life*
        + unreg(id) : void
        + get(id) : life*
    }

    class "life" as life {
        - _pt : instance*
        - _strong : ncnt
        - _id : id
        ---
        + onStrong(delta) : void
        + getStrongCnt() : ncnt
        + isBind() : nbool
        + canBind(type) : nbool
    }
}

package "인터페이스" {
    interface "memoryHaver" as memoryHaver {
        + len() : ncnt
        + size() : ncnt
    }

    interface "allocator" as allocator {
        + new1() : void*
        + del(ptr, size) : void
    }
}

package "사용자 계층" {
    class "instance" as instance {
        - _id : id
        ---
        + getId() : id
        + getBindTag() : life&
    }

    class "binder" as binder {
        + bind(instance*) : nbool
        + rel() : void
        + isBind() : nbool
        + get() : instance*
    }

    class "tstr<T>" as tstr
    class "tweak<T>" as tweak
}

note top of instancer
  <b>Facade 패턴:</b>
  복잡한 메모리 관리 서브시스템에
  대한 단순화된 인터페이스 제공

  pool: 로우레벨 메모리 할당
  watcher: 라이프사이클 관리
end note

note right of pool
  <b>크기별 lazy chunks 관리:</b>
  요청 크기별로 chunks를 생성
  같은 크기는 같은 chunks 사용
end note

note right of chunk
  <b>Free List 알고리즘:</b>
  intrusive linked list로
  사용 가능한 블록 추적

  각 빈 블록에 다음 빈 블록의
  인덱스 저장
end note

note bottom of watcher
  <b>Object Pool 패턴:</b>
  life 객체들을 미리 할당해두고
  재사용하여 성능 향상
end note

note right of life
  <b>Reference Counting:</b>
  shared_ptr의 control block과
  유사하지만 instance 자체에
  연결되어 이중 해제 방지
end note

instancer *-- pool : 소유
instancer *-- watcher : 소유
pool *-- "n" chunks : lazy 생성
chunks *-- "n" chunk : 관리
watcher *-- "n" life : 풀 관리
life --> instance : 참조

allocator --|> memoryHaver : 상속
chunk ..|> allocator : 구현
chunks ..|> allocator : 구현
pool ..|> memoryHaver : 구현

binder --> life : 참조
tstr --|> binder : 상속
tweak --|> binder : 상속
instance --> life : getBindTag()

@enduml

@startuml
participant "chunk" as chunk
participant "_heap[0]" as heap0
participant "_heap[1]" as heap1
participant "_heap[2]" as heap2
participant "_heap[3]" as heap3

note over chunk
  <b>초기화 (size=4):</b>
  _head = 0
  _len = 0
  각 원소에 다음 인덱스 저장
end note

chunk -> heap0 : *(_heap[0]) = 1
chunk -> heap1 : *(_heap[1]) = 2
chunk -> heap2 : *(_heap[2]) = 3
chunk -> heap3 : *(_heap[3]) = 4

note over chunk, heap3
  초기 상태: [1, 2, 3, 4]
  _head = 0 (다음 할당 가능 인덱스)
end note

== 첫 번째 할당: new1() ==

chunk -> heap0 : ptr1 = _heap + (_head * blkSize)
activate heap0 #lightgreen

note right of chunk
  _head를 _heap[_head] 값으로 업데이트
  _head = _heap[0] = 1
  _len = 1
end note

note over heap0, heap3
  상태: [<b>사용중</b>, 2, 3, 4]
  _head = 1
end note

== 두 번째 할당: new1() ==

chunk -> heap1 : ptr2 = _heap + (_head * blkSize)
activate heap1 #lightgreen

note right of chunk
  _head = _heap[1] = 2
  _len = 2
end note

note over heap0, heap3
  상태: [<b>사용중</b>, <b>사용중</b>, 3, 4]
  _head = 2
end note

== 세 번째 할당: new1() ==

chunk -> heap2 : ptr3 = _heap + (_head * blkSize)
activate heap2 #lightgreen

note right of chunk
  _head = _heap[2] = 3
  _len = 3
end note

note over heap0, heap3
  상태: [<b>사용중</b>, <b>사용중</b>, <b>사용중</b>, 4]
  _head = 3
end note

== 첫 번째 블록 해제: del(ptr1) ==

note right of chunk
  1. ptr1 위치에 현재 _head 저장
  *ptr1 = _head = 3

  2. _head를 해제한 블록 인덱스로 업데이트
  _head = (ptr1 - _heap) / blkSize = 0

  3. _len 감소
  _len = 2
end note

chunk -> heap0 : *(_heap[0]) = 3
deactivate heap0

note over heap0, heap3
  상태: [3, <b>사용중</b>, <b>사용중</b>, 4]
  _head = 0

  <b>Free List 구조:</b>
  _heap[0] → _heap[3] → _heap[4] (없음)
end note

== 네 번째 할당: new1() ==

chunk -> heap0 : ptr4 = _heap + (_head * blkSize)
activate heap0 #lightgreen

note right of chunk
  <b>재사용:</b>
  해제된 블록 0을 재사용
  _head = _heap[0] = 3
  _len = 3
end note

note over heap0, heap3
  상태: [<b>사용중</b>, <b>사용중</b>, <b>사용중</b>, 4]
  _head = 3

  <b>메모리 지역성 향상:</b>
  해제된 블록을 즉시 재사용
end note

note over chunk, heap3
  <b>Free List 알고리즘의 특징:</b>

  1. <b>Intrusive Linked List:</b>
     빈 블록 자체에 다음 빈 블록 인덱스 저장
     별도의 메타데이터 불필요

  2. <b>O(1) 할당/해제:</b>
     _head만 업데이트하면 되므로 상수 시간

  3. <b>메모리 지역성:</b>
     최근 해제된 블록을 먼저 재사용

  4. <b>단편화 방지:</b>
     고정 크기 블록 사용
end note

@enduml

@startuml
actor "클라이언트" as client
participant "new" as new
participant "instancer" as instancer
participant "pool" as pool
participant "chunks" as chunks
participant "chunk" as chunk
participant "watcher" as watcher
participant "life" as life
participant "instance" as instance
participant "binder" as binder

== 인스턴스 생성 단계 ==

client -> new : MyClass* obj = new MyClass()
activate new

new -> instancer : operator new(size)
activate instancer

note right of instancer
  <b>1. 메모리 할당:</b>
  pool을 통해 메모리 확보
end note

instancer -> pool : new1(size)
activate pool

pool -> pool : get(size)
note right of pool
  해당 크기의 chunks를 찾음
  없으면 lazy 생성
end note

pool -> chunks : new1()
activate chunks

chunks -> chunks : 가용 chunk 검색
note right of chunks
  _chunks[_s]부터 순회
  가용 메모리 없으면 resize()
end note

chunks -> chunk : new1()
activate chunk

note right of chunk
  <b>Free List 알고리즘:</b>
  ptr = _heap + (_head * blkSize)
  _head = _heap[_head]
  _len++
end note

chunk --> chunks : void* ptr
deactivate chunk

chunks --> pool : void* ptr
deactivate chunks

pool --> instancer : void* ptr
deactivate pool

note right of instancer
  <b>2. 생명주기 관리:</b>
  watcher에 등록하여 life 할당
end note

instancer -> watcher : reg(ptr)
activate watcher

watcher -> watcher : 사용 가능한 life 찾기
note right of watcher
  life 풀에서 빈 life 할당
  Object Pool 패턴
end note

watcher -> life : 초기화
activate life

note right of life
  _pt = ptr
  _strong = 0
  _id = 생성 (tagN, chkN, serial)
end note

life --> watcher : life*
watcher --> instancer : life*
deactivate watcher

note right of instancer
  <b>3. ID 부여:</b>
  vault에 인스턴스 등록
end note

instancer -> instancer : vault[ptr] = id

instancer --> new : void* ptr
deactivate instancer

new -> instance : 생성자 호출
activate instance

instance -> instance : _id = vault[this]
note right of instance
  vault에서 id 가져오기
end note

instance --> new : 초기화 완료
deactivate instance

new --> client : MyClass* obj
deactivate new

== 바인딩 단계 ==

client -> binder : tstr<MyClass> ptr(obj)
activate binder

binder -> instance : getBindTag()
instance --> binder : life&

binder -> life : onStrong(+1)
activate life

note right of life
  <b>Reference Counting:</b>
  _strong++
  _strong = 1
end note

life --> binder : void
deactivate life

binder --> client : 바인딩 완료
deactivate binder

== 사용 단계 ==

client -> binder : ptr->someMethod()
binder -> instance : someMethod()
instance --> binder : 결과
binder --> client : 결과

== 소멸 단계 ==

client -> binder : ptr.rel() 또는 스코프 종료
activate binder

binder -> life : onStrong(-1)
activate life

note right of life
  <b>Count 감소:</b>
  _strong--
  _strong = 0
end note

life -> life : if(_strong == 0)
note right of life
  참조 카운트가 0이 되면
  인스턴스 소멸 시작
end note

life -> instance : delete _pt
activate instance

instance -> instance : ~MyClass() 소멸자 호출

instance -> instancer : operator delete(ptr)
activate instancer

note right of instancer
  <b>1. 생명주기 해제:</b>
  watcher에서 등록 해제
end note

instancer -> watcher : unreg(id)
activate watcher

watcher -> life : 사용 가능 상태로 표시
note right of life
  _pt = nullptr
  _strong = 0
  _id 초기화

  나중에 재사용 가능
end note

watcher --> instancer : void
deactivate watcher
deactivate life

note right of instancer
  <b>2. 메모리 반환:</b>
  pool에 메모리 반환
end note

instancer -> pool : del(ptr, size)
activate pool

pool -> chunks : del(ptr, size)
activate chunks

chunks -> chunk : del(ptr, size)
activate chunk

note right of chunk
  <b>Free List 업데이트:</b>
  *ptr = _head
  _head = (ptr - _heap) / blkSize
  _len--

  메모리 초기화 안함
  (재사용 위해)
end note

chunk --> chunks : void
deactivate chunk

chunks --> pool : void
deactivate chunks

pool --> instancer : void
deactivate pool

instancer --> instance : void
deactivate instancer

instance --> life : void
deactivate instance

life --> binder : void

binder --> client : 소멸 완료
deactivate binder

note over client, life
  <b>메모리 관리 사이클 완료:</b>

  1. 생성: instancer → pool → chunks → chunk (메모리 할당)
           instancer → watcher → life (생명주기 관리)

  2. 바인딩: binder → life (_strong 증가)

  3. 소멸: life (_strong == 0) → delete instance
          instancer → watcher (life 반환)
          instancer → pool → chunks → chunk (메모리 반환)

  <b>핵심:</b>
  - Object Pool: life와 chunk 재사용
  - Reference Counting: 자동 메모리 관리
  - Free List: O(1) 할당/해제
end note

@enduml

---

## 바인딩 시스템 사용하기

### binder 클래스

@ref by::binder "binder" 클래스는 일반화된 바인딩 클래스로 @ref instance 클래스를 상속한 클래스로부터 생성된 모든 객체를
바인딩할 수 있습니다. Smart Pointer Pattern을 구현하여 reference counting으로 적절한 시점에 객체를 소멸시키며,
표준 라이브러리에 잘 정의된 std::weak_ptr과 같은 기능을 @ref by::tweak "tweak" 가, std::shared_ptr은 @ref by::tstr "tstr" 이 각 담당합니다.

shared_ptr를 이미 잘 알고 있다면 아래와 같이 사용할 수 있다는 걸 쉽게 이해할 수 있을 거예요.

### 기본 사용법

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

위는 아주 기본적인 API만 사용한 지나치게 정석적인 예제예요. 실제로는 이보다는 더 간략하게 쓰는
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

<b>tstr 기본 사용 예제</b>

```
@style: language-cpp verified
class Person : public instance {
public:
    Person(int newAge) : age(newAge) {}
    int age;

    const type& getType() const override { return ttype<Person>::get(); }
    clonable* clone() const override { return new Person(age); }
};

// 1. 생성과 동시에 바인딩
tstr<Person> person1(new Person(25));
person1->age;  // 25

// 2. isBind()로 바인딩 상태 확인
tstr<Person> person2;  // 빈 바인더
person2.isBind();  // false

// 3. bind()로 나중에 바인딩
person2.bind(new Person(30));
person2.isBind();  // true

// 4. get()으로 원시 포인터 접근
Person* raw = person2.get();
raw->age;  // 30

// 5. rel()로 바인딩 해제
person2.rel();
person2.isBind();  // false
person2.get();     // nullptr

// 6. 스코프를 벗어나면 자동으로 소멸
{
    tstr<Person> person3(new Person(40));
    // person3->age 사용 가능
}  // 여기서 reference count가 0이 되어 Person 객체 자동 소멸
```

---

## std::shared_ptr 대비 장점

이쯤되면, 아마도 왜 shared_ptr를 사용하지 않고 굳이 tstr을 만들었는가에 대해 의문을 가질 것입니다.
shared_ptr이 제공하지 못하는 몇가지 장점이 있기 때문입니다.

### reference counting 블록은 인스턴스 자체에 붙어있다

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

byeol에서는 reference counting을 위한 클래스를 @ref by::life "life" 라고 하며, 이는 @ref by::watcher "watcher" 에 의해 인스턴스마다
별도로 제공됩니다. @ref by::watcher "watcher" 는 내부에 @ref by::life "life" 객체들을 배열로 미리 대량 할당해둔 풀(pool)을 관리합니다.
새로운 인스턴스가 바인딩될 때 사용 가능한 @ref by::life "life" 를 할당하고, 인스턴스가 소멸되면 해당 @ref by::life "life" 를
사용 가능(available) 상태로 표시하여 나중에 재사용합니다. 동일한 인스턴스에 대해서는 항상 같은
@ref by::life "life" 가 할당되므로 이중 해제 문제가 발생하지 않습니다.

### ADT 제공

@ref by::tstr "tstr" 과 @ref by::tweak "tweak" 는 같은 @ref by::binder "binder" 기반클래스를 갖기 때문에 @ref by::binder "binder" 타입으로 범용적인 로직을 구현할 수 있습니다.

```
@style: language-cpp verified
void me::rel(binder& me) { // me가 tstr인지 tweak인지 상관없다.
    WHEN(!me.isBind()) .ret();

    life* l = me._getBindTag();
    if(l) l->_onStrong(-1);
}
```

@ref by::binder "binder" 는 abstract 하므로 객체 생성이 불가능합니다. @ref by::tstr "tstr" 이나 @ref by::tweak "tweak" 로 이미 생성된 바인더들을 범용적인
로직을 작성할때만 의의를 갖습니다.

<b>tstr과 tweak 강한/약한 참조 예제</b>

```
@style: language-cpp verified
class Data : public instance {
public:
    int value;
    const type& getType() const override { return ttype<Data>::get(); }
    clonable* clone() const override { return new Data(); }
};

// 1. tstr은 강한 참조 (strong reference)
tstr<Data> strong(new Data());
strong->value = 100;

const life& lifeTag = strong->getBindTag();
lifeTag.getStrongCnt();  // 1

// 2. tweak은 약한 참조 (weak reference)
tweak<Data> weak(*strong);
lifeTag.getStrongCnt();  // 여전히 1 (weak은 strong count를 증가시키지 않음)

// 3. weak은 strong이 유효한 동안만 접근 가능
weak->value;  // 100, strong이 살아있으므로 접근 가능

// 4. strong을 해제하면 weak도 무효화
strong.rel();
weak.isBind();  // false
weak.get();     // nullptr - 원본 객체가 소멸되었으므로 접근 불가

// 5. 여러 strong 참조는 count 증가
tstr<Data> strong1(new Data());
tstr<Data> strong2 = strong1;
tstr<Data> strong3 = strong1;
strong1->getBindTag().getStrongCnt();  // 3

strong2.rel();
strong1->getBindTag().getStrongCnt();  // 2

strong3.rel();
strong1->getBindTag().getStrongCnt();  // 1
```

### 동적 타입 체킹

@ref by::binder "binder" 는 ADT이며 클래스 템플릿 조차 아닙니다. 따라서 binder::bind() 함수는 parameter가 @ref by::instance "instance"
타입으로 되어있습니다. 이 말은 tstr<A>라고 할지라도 bind(new B()); 컴파일 에러가 발생하지 않는다는
걸 의미합니다.

bind() 안쪽에서 @ref meta 모듈을 사용하여 동적으로 타입을 검사해서 올바른 경우만 인스턴스가 바인딩 됩니다.
타입이 일치하지 않을 경우, bind()는 `false`를 반환하고 바인딩을 수행하지 않습니다. 따라서 타입
안정성이 필요한 경우 bind()의 반환값을 확인해야 합니다.

### 자체 메모리 풀 사용

인스턴스 할당시 모든 작업은 @ref by::instancer "instancer" 를 시작으로 이뤄집니다. 자체 메모리 풀을 사용함으로써 heap보다
빠른 할당/해제가 가능합니다.

### 속도 개선

shared_ptr의 알고리즘은 같은 shared_ptr 끼리 공유되는 reference counting 정보를 heap에 보관하고
공유하는 것입니다. heap보다 빠른 자체 메모리 풀을 사용하고, 바인딩 속도를 조금이나마 최적화한다면 속도를
개선할 여지가 있습니다.

참고로, binding은 byeol에서 가장 많은 퍼포먼스 비용을 차지하는 핫스팟중에 하나입니다.

### 추가 정보 제공

shared_ptr은 heap에 reference counting 정보를 보관하는 객체를 생성하고 이를 공유합니다. 반면 @ref memlite
모듈은 @ref by::watcher "watcher" 클래스를 통해서 이미 메모리는 할당된, 빈 @ref by::life "life" 하나를 내어주고, 그곳을 해당
@ref by::instance "instance" 의 reference counting 공간으로 활용합니다.

만약 이후, GC와 같은 기능이 추가되면 인스턴스마다 추가적으로 생명주기와 관련된 정보를 필요로 할 여지가
있습니다. shared_ptr와 달리 각 인스턴스의 생명주기 정보 또한 자체적으로 관리하고 있기 때문에 그런
요구사항에도 적절하게 대응할 수 있습니다.

---

## 메모리 풀 아키텍처

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

### chunk - 최소 할당 단위

@ref by::chunk "chunk" 클래스는 @ref memlite 에서 메모리를 실제로 할당 가능한 최소 단위 클래스입니다. 모든 메모리 관리는
@ref by::chunk "chunk" 들을 엮어서 수행합니다.

@ref by::chunk "chunk" 는 메모리가 flexible하게 늘어나도록 하는 _resize() 함수가 있지만, @ref memlite 의 컨셉상 이를 public으로
공개하지 않습니다. 결과적으로 @ref by::chunk "chunk" 의 메모리는 객체 생성시 고정되며, 추가 메모리가 필요하다면 @ref by::chunk "chunk"
객체를 더 생성해서 운영해야 합니다.

<b>chunk 기본 사용 예제</b>

```
@style: language-cpp verified
// int를 100개 담을 수 있는 chunk 생성
chunk myChunk(sizeof(int), 100);

// 상태 확인
myChunk.len();        // 0 (아직 할당 안함)
myChunk.size();       // 100 (최대 용량)
myChunk.getBlkSize(); // sizeof(int)와 동일하거나 큰 값 (정렬됨)

// 메모리 할당
int* ptr1 = (int*) myChunk.new1();
*ptr1 = 42;

int* ptr2 = (int*) myChunk.new1();
*ptr2 = 57;

myChunk.len();  // 2

// 인덱스로 접근
int* elem0 = (int*) myChunk[0];
*elem0;  // 42

int* elem1 = (int*) myChunk[1];
*elem1;  // 57

// 메모리 해제 (순서 무관)
myChunk.del(ptr2, 0);
myChunk.len();  // 1

myChunk.del(ptr1, 0);
myChunk.len();  // 0

// chunk 전체 해제
myChunk.rel();
myChunk.size();  // 0
```

<b>Block size</b>

@ref chunk 는 생성시 block size와 size 2개를 입력받습니다. blockSize는 메모리에 인스턴스 하나가 차지하게 될
최소 단위 크기입니다. 반면 size는 그러한 인스턴스가 몇개 까지 들어갈 지를 정합니다.

예를들어 만약 int64만 100개 담는 chunk를 만든다고 한다면, 다음과 같이 됩니다.

```
@style: language-cpp verified
// int64 크기(8바이트)의 블록 100개를 담는 chunk 생성
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

<b>real block size</b>

실제 메모리 할당시에는 block size 대신 real block size를 사용하는데, 이는 최적화에 따른 것입니다. CPU
연산시 1이나 2바이트 등 작은 단위로 메모리 할당해서 계산하는 것보다 CPU 아키텍처에 맞게 정렬(padding)하는 것이
더 효율적입니다. 예를 들어, 64비트 CPU에서는 8바이트 단위로 정렬되며, 3바이트를 요청해도 실제로는
8바이트가 할당됩니다. 이는 메모리 접근 속도를 최적화하기 위한 것입니다.

<b>ArrayList 구현</b>

@ref by::chunk "chunk" 는 배열 기반 리스트(ArrayList)로 직접 구현되어 있습니다. 크기가 고정되어 있지만 크기 내에서는
List처럼 추가 삭제가 자유로우며 임의접근 속도는 Array처럼 빠릅니다. 내부적으로 Free List 알고리즘을 사용하여
사용 가능한 메모리 블록을 추적합니다. 각 비어있는 블록은 다음 빈 블록의 인덱스를 저장하는 intrusive linked list
구조로 연결됩니다.

알고리즘은 다음과 같습니다:

1. 각 원소의 byte 크기는 real block size 이상이어야 한다는 전제조건을 갖습니다. 이는 앞서 설명한 real block size로
   인한 것으로, CPU 아키텍처에 따라 1바이트를 할당하더라도 실제로는 4바이트 이상으로 정렬되기 때문입니다.

2. 각 원소들은 모두 동일한 byte 크기를 갖습니다.

3. @ref by::chunk "chunk" 는 각 블록을 기본적으로는 void*로 취급하지만 값이 없을 경우에는 int타입으로 취급합니다.
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

### chunks - 다중 chunk 관리

@ref by::chunks "chunks" 객체는 여러개의 @ref by::chunk "chunk" 의 인스턴스 관리를 담당합니다. @ref by::chunk "chunk" 는 생성시 고정된 크기만 메모리를
활용하기 때문에 @ref by::chunks "chunks" 가 여러개의 @ref by::chunk "chunk" 를 추가/삭제 함으로써 유동적으로 메모리를 관리합니다.

<b>chunks 역시 고정된 메모리만 제공한다</b>

@ref by::chunks "chunks" 는 @ref by::chunk "chunk" 들을 추가하거나 삭제하므로, @ref by::chunk "chunk" 가 각 셀마다 고정된 크기만을 사용하기 때문에 @ref by::chunks "chunks" 또한
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

<b>pool과의 연계</b>

최초 메모리 요청을 받는 곳은 @ref by::pool "pool" 클래스입니다. 해당 객체에서 할당해야할 memory size를 받으면, 해당
memory size를 처리할 수 있는 @ref by::chunks "chunks" 인스턴스를 lazy로 가져와, new1()를 요청하게 됩니다.
@ref by::chunks::new1() "new1()"에서는 메모리 할당이 가능한 @ref by::chunk "chunk" 를 찾고, 없을 경우는 추가로 @ref by::chunk "chunk" 를 생성합니다.

<b>가용 chunk 검색 알고리즘</b>

가장 최근에 메모리를 할당한 @ref by::chunk "chunk" 가 추가로 할당 할 가능성이 가장 높습니다. 멤버변수 `_s`는 바로 최근에
할당한 @ref by::chunk "chunk" 의 인덱스를 가지고 있습니다.

만약 _chunks[_s]에 가용 메모리가 없을 경우 _s를 ++ 합니다. 이후 마치 원형배열처럼, _chunks의 끝은
처음과 이어져 있다고 보면 됩니다. 그래서 다시 _s가 순회직전의 _s로 값이 같아질 때까지도 가용 메모리가
없다면, @ref by::chunks "chunks" 전체에 가용 메모리가 없는 상태이므로 resize()에 들어갑니다.

<b>vector를 쓰면 안된다</b>

당연한 건데, vector는 heap으로 관리되므로 자체 메모리 풀을 만든다면서 vector를 사용해서는 안됩니다.
차후 수정 예정입니다.

### pool - 크기별 메모리 관리

@ref by::pool "pool" 클래스는 외부로부터 메모리 할당 요청시 가장 최초로 처리하는 클래스입니다. 내부적으로 @ref by::chunks "chunks" 에
대한 배열을 가지고 있으며, @ref by::chunks "chunks" 는 @ref by::chunk "chunk" 를 가지고 있으므로, 사실상 로우레벨의 메모리 관련 클래스를
모두 관리하는 셈입니다.

<b>pool은 할당 가능한 size 별로 lazy하게 chunks를 가진다</b>

자체 메모리 풀을 만들때 중요한 포인트는, 같은 사이즈의 메모리를 한 곳에 나열함으로써 속도를 높이는
것입니다. @ref by::chunks "chunks" 는 블록이라는 개념이 있어서 각 블록은 미리 지정된 크기의 메모리만 할당/해제 될 수
있습니다.

@ref by::pool "pool" 은 @ref by::chunks "chunks" 를 만들때 블록의 크기를 고정해서 생성하며, 외부에 의해서 특정 사이즈의 메모리 할당을
요청받으면, 해당 크기의 블록을 담당하는 @ref by::chunks "chunks" 를 찾습니다. 없을 경우 lazy 하게 생성합니다.

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

### instancer - 메모리 관리 조정자

@ref by::instancer "instancer" 클래스는 low level로 메모리를 관리하는 @ref by::pool "pool" 클래스와, @ref by::instance "instance" 들의 라이프사이클을
관리하는 @ref by::watcher "watcher" 를 가지고 있습니다. Facade 패턴을 사용하여 복잡한 메모리 관리 서브시스템에 대한
단순화된 인터페이스를 제공합니다.

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

---

## 인스턴스 생명주기 관리

### instance 클래스

@ref by::instance "instance" 클래스는 @ref memlite 모듈의 자체 memory pool에 의해서 관리되는 객체의 기반 클래스입니다.
@ref by::instance "instance" 클래스를 상속해야만 @ref by::binder "binder" 를 통해 weak pointer나 strong pointer로 참조 할 수 있습니다.
@ref by::instance "instance" 의 식별은 `id`를 통해서 이뤄집니다.

<b>id 부여 알고리즘</b>

@ref memlite 에서 가장 취약한 부분을 고르라면 바로 이 id 부여 알고리즘입니다. 인스턴스 생성은 memory pool을
관리하는 @ref by::instancer "instancer" 에 의해서 이뤄집니다. 이때 @ref by::instancer "instancer" 는 `vault`라고 하는 @ref by::instance "instance" 내부의 클래스에
@ref by::instance "instance" 주소와 id를 map에 push 합니다. instance::operator new()가 불리면 안쪽에서는 vault에게
`map[this]`와 같은 코드로 id값을 가져오는 방식입니다.

얼핏 괜찮아 보이지만 단점이 많습니다:

1. 생성자 단계에서 vault에 접근해서 값을 가져오는 방식 자체가 안전하다고 보긴 어렵습니다.
2. 느립니다.

최초 구현은 vector로만 되어있었으며 FIFO로 관리했었으나, 생성자 안에서 다른 객체를 생성하는 경우에는
추가되는 id의 순서가 FIFO가 아니게 되면서 ID가 꼬이는 문제가 있었습니다.

<b>속도에 있어서 instance 클래스의 중요성</b>

byeol에서 가장 빈번히 하는 작업은 객체를 생성하면서 id를 부여하거나 binding을 하는 작업입니다. 이 부분은
개선 예정이며, 더 나은 알고리즘에 대한 아이디어를 환영합니다.

### id 클래스

@ref by::id "id" 클래스는 64bit integer로 되어있는 @ref by::instance "instance" 식별자입니다. tagN은 @ref by::life "life" 를 식별하며, chkN은
몇번째 @ref by::chunk "chunk" 인지를 나타내며 serial은 객체 검증에 사용됩니다.

<b>serial은 프로세스 실행 도중 instance 객체의 생성횟수다</b>

@ref by::pool "pool" 과 @ref by::chunk "chunk" 를 먼저 봤다면 알겠지만, 자체 메모리 풀을 사용하기 때문에 메모리가 해제 될때는 소멸자만
호출할 뿐, 모든 메모리를 초기화 하지 않습니다.

그러니 이전에 할당해서 사용후 소멸된 데이터가 그대로 남아있으며, 심지어 이 데이터에 접근도 가능합니다.
(이미 사용한 데이터에 접근시 exception이나 UB가 된다면 weak pointer나 strong pointer를 구현한 @ref by::binder "binder" 를
구현할 수 없었을 것입니다)

@ref by::binder "binder" 에서는 이렇게 해서 가져온 데이터가 정말로 유효한 데이터인지 구분하기 위해서 serial을 추가로
비교합니다.

<b>id 구성 요소 확인 예제</b>

```
@style: language-cpp verified
class MyData : public instance {
public:
    int value;
    const type& getType() const override { return ttype<MyData>::get(); }
    clonable* clone() const override { return new MyData(); }
};

MyData* obj = new MyData();

// id 구조 확인
id objId = obj->getId();
objId.tagN;    // watcher의 life 배열 인덱스
objId.chkN;    // chunks의 chunk 인덱스
objId.serial;  // 생성 카운터 (유효성 검증용)
objId.isHeap();  // true - heap에 할당된 객체

// life 정보 접근
const life& lifeTag = obj->getBindTag();
lifeTag.getId();  // objId와 동일
```

<b>tagN은 life 객체에 접근할때 사용한다</b>

@ref by::watcher "watcher" 는 자신의 배열에서 tagN 번째 @ref by::life "life" 객체를 가져올때 이 값을 사용합니다.

<b>chkN은 chunk 객체를 가져올 때 사용한다</b>

@ref by::pool "pool" 은 먼저 id와 매핑된 @ref by::instance "instance" 의 size를 계산해 @ref by::chunks "chunks" 를 가져옵니다. 그리고 @ref by::chunks "chunks" 는 자신의 chkN
번째 원소인 메모리블록을 반환합니다. 외부에서는 전달 받은 메모리 주소와 serial 값을 비교해서 같은
인스턴스인지를 검증합니다.

### life 클래스

@ref by::pool "pool" 클래스가 로우레벨 관점에서 블록 단위로 메모리를 관리하는 클래스라면, @ref by::watcher "watcher" 컴포넌트는 각
블록의 정보를 유기적으로 관리하는 클래스입니다.

@ref by::life "life" 는 @ref by::pool "pool" 에 할당되어있는 주소값(_pt)와 reference counting을 위한 값들을 갖습니다. _strong은
reference counting을 위한 값이며, _pt는 @ref by::pool "pool" 에 할당받은 인스턴스를 직접 가리킵니다. _id는 객체를
식별하기 위한 값으로 자세한 내용은 @ref by::id "id" 를 참고하세요.

### watcher 클래스

@ref by::watcher "watcher" 클래스는 메모리 관리의 한 축을 담당하는 클래스로, 생성된 객체의 라이프사이클을 관리합니다.
@ref by::instance "instance" 가 생성될때마다 @ref by::watcher "watcher" 는 @ref by::life "life" 객체를 추가로 할당해 reference counting으로 객체의
소멸시점을 판별합니다.

<b>reference counting</b>

@ref by::binder "binder" 에 의해서 @ref by::instance "instance" 가 바인딩 될때마다 @ref by::life "life" 가 count 하는 strong 값을 1 증가시킵니다. @ref by::binder "binder" 가
@ref by::instance "instance" 를 rel() 할때 count를 1 감소하며, 0이 되는 순간 delete로 메모리에서 해제합니다. @ref by::instance "instance" 는
`operator delete()`를 통해 @ref by::instancer "instancer" 에게 메모리 해제 작업을 실행하도록 합니다.

<b>watcher와 life 사용 예제</b>

```
@style: language-cpp verified
class MyData : public instance {
public:
    int age;
    const type& getType() const override { return ttype<MyData>::get(); }
    clonable* clone() const override { return new MyData(); }
};

// heap에 객체 생성
MyData* obj = new MyData();

// life 정보 확인
const life& lifeTag = obj->getBindTag();
lifeTag.isBind();  // true
lifeTag.getId().serial;  // > 0
lifeTag.getId().isHeap();  // true

// chunk 정보 접근
const chunk& chk = lifeTag.getChunk();
chk.has(*obj);  // true - 이 chunk에 obj가 있음
chk.len();  // > 0 - 할당된 블록 개수
chk.size();  // > 0 - 총 용량

// 바인딩 가능 타입 확인
lifeTag.canBind(ttype<instance>());  // true
lifeTag.getBindable();  // instance의 type 반환
```

---

## 메모리 관리 인터페이스

이 인터페이스들은 Interface Segregation Principle을 따라 설계되었습니다. 클라이언트가 사용하지 않는
메서드에 의존하지 않도록, 메모리 조회 기능(@ref by::memoryHaver "memoryHaver")과 메모리 할당 기능(@ref by::allocator "allocator")을
별도의 인터페이스로 분리했습니다.

### memoryHaver 클래스

@ref by::memoryHaver "memoryHaver" 클래스는 memory pool에서 일정 메모리를 직접 혹은 간접적으로 소유하고 있으며, 그 메모리를
READ 가능한 클래스들의 기본 인터페이스를 정의합니다. 그래서 메모리의 크기나, 상태 등을 알 수 있는
인터페이스로 정의되어 있습니다.

<b>간접적으로 소유하다?</b>

해당 객체가 직접 메모리를 할당받아 사용하는 것이 아니라, 내부에 멤버변수로 있는 다른 객체들이 담당하는
경우가 있습니다. 그리고 메모리의 할당은 내부 멤버변수들을 통해 직접해야 한다면, 그 클래스는 @ref by::memoryHaver "memoryHaver" 만
상속받아야 합니다. 만약 할당도 가능하다면 @ref by::allocator "allocator" 를 상속하면 됩니다.

<b>len과 size</b>

할당 가능한 메모리의 크기는 size로 표현하며, 그 중에서 할당한 메모리는 len으로 표현됩니다. void* 및 byte
단위로만 제어하는 것을 전제로 합니다.

@ref by::memoryHaver "memoryHaver" 의 파생클래스들은 자신들이 담당하는 메모리의 사이즈가 제각기 다르다는 것에 주의하세요.

### allocator 클래스

@ref by::allocator "allocator" 클래스는 @ref by::memoryHaver "memoryHaver" 를 상속하고 있다는 점에서 알다시피 관리하는 메모리의 상태나 크기를
측정할 수 있으면서, 추가적으로 메모리를 할당/소멸 할 수 있는 클래스입니다.

모든 메모리는 void* 및 바이트 관점에서만 바라본다는 @ref memlite 컨셉에 맞게, new(), del()의 파라메터는
void*만 제공합니다.

<b>memlite 전용의 공통 인터페이스의 네이밍 컨벤션</b>

할당은 new1() (new one 이라는 뜻입니다.), 해제는 del()를 사용합니다. 이 네이밍은 @ref memlite 뿐만 아니라
byeol 프로젝트 내부에서 자주 사용됩니다.

---

<b>다음 문서</b>: @ref ah-architecture-stela
