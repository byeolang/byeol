# 나중을 위해서

* 이곳은 나중에 정식으로 document를 작성하기 전에, 제가 잊지말고 적어야 할
것들을 정리한 것입니다.
* 문서가 정식으로 릴리즈가 되면 물론 분류, 번역되어 올라갑니다.

# Singleton 객체를 반환하는 함수는 `singleton()`으로, 멤버변수는 `singleton` 으로 네이밍합니다.

# 반환형 타입의 convention rule

* byeol은 주어진 type parameter T에 대해 다양한 wrapper 반환형을 지원합니다.
    * T
    * T&
    * T*
    * tmedium<T>
    * tstr<T>
    * tweak<T>
    * tmay<T>
    * tres<T>
    * (`const` 와 같은 qualifier는 제외하였습니다.)
* 어떤 반환형으로 내보낼지는 자유이지만, 특정한 규칙이 존재해야 합니다.
    * 예를들면, 타 C++ 프로젝트처럼 T*로 내보내는 것은, 아주 제한적으로만 허용합니다.

    1. strong smart pointer를 쓰고 싶다면 tstr<T>를 씁니다.
       STL의 unique_ptr와 핵심 동작은 동일하나 변가지 추가된 기능을 제공합니다.
        * API의 look & feel이 byeol 프로젝트 전체적인 API와 일관성이 있습니다.
        * 별도의 memory pool을 통해 속도를 올릴 수 있습니다.
        * `meta` layer을 통한 최적화된 type checking을 지원합니다.

    2. weak smart pointer를 쓰고 싶다면 tweak<T>를 씁니다.

    3. 아무때나 T*로 내보내는 것은 지양합니다.
       가능하면 tstr<T>로 wrapper로 감싸서 내보내는게 메모리 관리 측면에서
       좋습니다. (종종 받는 쪽에서, 감싸지 않는 경우가 있다는 얘기입니다.)
       물론 이 역시 몇가지 예외가 있는데,
        * `new` 를 사용해서 heap에 할당한 객체를 반환하는 경우는 T*로
        내보냅니다. 따라서 반환형이 T* 일때는 `반드시 tstr<T>로 감싸야 하는 구나`
        라는 신호가 됩니다.

        * `bison` 등 외부 툴에 의해서 생성된 코드는 이 규칙에서 제외합니다.

        * `memlite` layer보다 하위 layer도 이 규칙에서 제외합니다.
          tstr<T>를 쓸 수 없으니까 당연하죠.

    4. T&로 내보내는 걸 가장 우선합시다.
        너무나 잘 아는 내용이지만, T*보다는 T&가 좋습니다.
        byeol은 c++ 표준을 준수하므로 T&는 절대로 null이 아닙니다.
        (당연한 내용을 왜 적었지 싶다면, 이전에는 안그랬기 때문이죠..)

    5. 값으로 내보내야 하나, 값이 없는 경우도 표현해야할 때에는 tmay<T>를
       씁니다.

    6. tmay<T>를 써야 하는데, 값이 없는 경우는 별도의 다른 타입이 되었으면 싶을
       경우는 tres<T, R>를 사용합니다.
       tres<T, R> 은 T와 R 둘중 하나를 표현할 수 있습니다.

    7. tmedium<T>는, `뭔지 모른다면 쓰지 마십시오.`
       `OR` 매크로를 편하게 사용하기 위해서만 제공되는 중간 타입입니다.
       일반 적인 용도를 전제로 작성되지 않았습니다.

# 의미를 중복시키지 맙시다.

* 여기 `StudentManager` 라는 클래스가 있습니다. `getStudent(std::string name)`
이라는 함수가 있습니다만, 이름을 굳이 `getStudent()`로 해야하는지 생각해봅시다.

* 다른 getter가 없다면 `get(std::string name)` 으로 충분합니다.
* `StudentManager`라는 클래스명으로 `get()` 을 하면 Student가 나오겠다는 게
직관적으로 이해됩니다.
* 이름이 길다고 해서 무조건 좋은 게 아닙니다. 무조건 줄이자는 얘기도 아닙니다.
* 중복된 의미인가 판단하고 더 간결하고 작은 이름을 가져도 되는지 한번 더 검토 해 주십시오.

# 너무 긴 이름은 오히려 가독성을 해칩니다.

* `StudentRepository` 라는 클래스가 있고, `getSchoolSchedule()` 은 `SchoolSchedule` 객체를 반환합니다.
* `SchoolSchedule`은 `retrieveStudentsByName(std::string name)` 이라는 함수가
있습니다.
* 이 `retreiveStudentsByName()` 을 호출하려면 어떻게 해야 할까요?

```cpp
Student& student =
StudentRepository().getSchoolSchedule().retrieveStudentsByName("Chales");
```

* 일반적으로는 위와 같이 됩니다만, 다음과 같이 줄일 수도 있습니다.

```cpp
auto& student = StudentRepo().getSchedule().getStudentsBy("Chales");
```

* 자주 쓰는 단어라면 `Repo` 와 같은 충분히 이해가능하고 약속된 약어로 대체해볼만
합니다. 처음보는 사람이라도 Repository 겠거니 이해도 가능합니다.
* `getSchoolSchedule()` 에서 Schedule이 여러개가 아니라면 `School`은 불필요한 정보입니다.
* `retrieve` 와 같은 어려운 동사 보다는, 같은 의미라면 `get` 과 같이 간단한
단어를 사용하세요. 쓸데없이 길이만 깁니다.
* `ByName(std::string name)` 에서 뒤에 있는 `name` 으로 `이름으로 가져오겠구나?`
라는 걸 충분히 알 수 있습니다.
    * 나중에 이름 말고 나이나, 성별로도 구분해야 할 경우 `byGender(Gender gender)`
    나 `byAge(int age)` 를 추가할 건가요?
    * 그냥 오버로딩을 사용해서 `getStudentsBy(Gender gender)` 와 `getStudentsBy(int age)` 정도도 충분합니다.
* 이름만 보고 모든 정보를 이해할 수 있게끔 모든 내용을 다 담으려고 하지마세요.
* API명에 아무리 모든 정보를 담도록 길게 작성했다 한들, 결국은 어짜피 반드시
  해당 함수의 header를 찾아서 주석을 찾거나 관련 문서를 읽어야 합니다.
    * 대표적인 이유로는 parameter가 있죠.
* 함수명이 문서가 될 수 없다는 걸 인정하세요. 코드는 `문서는 읽었다는` 전제하에 작성할 수 밖에 없습니다.
* 거듭 언급합니다만, 이 말은 혼자만 알 수 있는, 짧지만 비직관적인 이름으로 다
바꾸라는 내용이 아닙니다.
* 아무 생각없이 `무조건 긴 이름을 최고지!` 라고 생각하는 사람들을 위한 글입니다.

# wrapper 클래스들에 `operator*`를 하면 T& 로 반환합니다. 이 와중에 exception이
발생할 수 있습니다.

* wrapper 클래스들은 `get()` 이라는 함수로 기본적으로 T*를 반환합니다.
* 하지만 사용성 측면에서 T&가 훨씬 좋기 때문에 때로는 강제로 T&로 반환해야 할때
`operator*()` 를 사용합니다.
* 이 과정에서 null에 대한 참조자를 만들 수 있고, 이를 사용하면 UB(Undefined
Behavior) 입니다. 모든 책임은 호출자가 지게 됩니다.

# Column은 120 hard limit, 80 soft limit 입니다.
* 120 columns은 절대로 넘어서는 안됩니다.
* 80 columns 이후부터는 슬슬 개행을 준비하세요.
