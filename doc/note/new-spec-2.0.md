# Major Changes ===========================================================
# 프로퍼티
## 기본 컨셉
* 문법은 다음과 같다.
    * defPropExpr: NAME NAME
        * accessor: "\_" | @empty
        * propGetFunc: accessor "get" blkStmt | accessor "get"
        * propSetFunc: accessor "set" blkStmt | accessor "set"
        * defPropFuncs: propGetFunc propSetFunc | propSetFunc propGetFunc
        * defCustomizedPropExpr: defPropExpr defPropFuncs
* 이제 `get prop.getEval()`, `setter void`를 정의할 수 있다.
* getter, setter에서 `it`을 사용하면 값이 있는 prop으로 parsing 한다.
    * `A B` 처럼 적었다고 해도, getter/setter 안에서 it을 사용했다면 값이 있는 prop이다.
    * `A := B` 처럼 적었는데 `it`을 사용하지 않으면 에러다.
* prop 에 대고 as()을 호출하게 되면 내부에서 getFunc을 호출한 결과를 가지고 캐스팅을 하도록 하자. 이 func을 호출하기전에 당연하지만 가지고 있는 me를 frameInteract 해줘야한다.
* set/get은 뒤에 소괄호를 적지 않는다.
* set의 경우는 코드상에 명시되어 있지 않지만 prop.getEval() 타입으로 정의된 `rhs` 라는 param이 정의되어 있다.
* get/set을 명시하지 않으면 기본동작이 된다.
    * innervalue를 지정한 경우라면 기본동작은 당연히 it = rhs, return it 이 된다.
    * innervalue가 없는 경우라면 기본동작은 당연히 `호출불가` 가 된다.
* 값이 있는 prop의 경우, str \_value 에 실제로 복제된 객체를 넣어둔다. 그리고 prop의 get/set 함수를 call하기 전에 obj에 대해 frameInteract하고 나서 이 \_value가 담긴 scope을 `it` 혹은 `rhs` 등으로 추가한 후 func.run()을 돌리자.
* 일반 함수 안에서도 getter/setter가 있는 prop을 정의할 수 있다.
* 함수는 참조를 가질 수 있지만, prop은 참조를 가질 수 없다. 내 언어는 참조자, 포인터가 없다.
* scope::set() 을 만든다.
* prop::get이 node::onGet() 이다. 이게 불리는 순간은, prop.as()을 호출한 순간이다.
* prop::set이 node::onSet() 이다. 이게 불리는 순간은 `prop = expr` 보다 정확하게는 `scope::set()` 를 호출한 순간이다.
    * assignExpr::run()은 현재 `scope.iterate(name)` 로 iterator를 가져와서 `iter.setVal(expr)` 을 하는 것이다.
    * 어딘가 한번 적었던 것 처럼 iter.setVal은 제거한다.
    * 대신에, `scope.set(name, expr)` 을 만들어서 사용해야 한다.
    * 그리고 scope은 `set()` 안에서 assign 하고 난 뒤에 expr.onSet(rhs) 을 불려주자.
    * node::onGet,onSet()은 아무런 동작도 하지 않는다. 비어있음.
* scope::add() 에서는 node::onSet()을 호출하지 않는다. 이건 assign이 아니니 말이다.
* [?] getter 혹은 setter가 `호출 불가`인 prop을 호출한 것인지는 어떻게 verification 하는가?
## 알고리즘
* scope는 tnmap이라고 되어있고, tnmap은 multimap으로 되어있다.
* multimap으로 된 이유는 오버로딩된 func 때문이다.
* 문제는 multimap으로 되어있으니 scope::add 혹은 scope::set을 쓸 수 없다는 것이다.
    * key만 가지고는, 여러개의 value중 어떤것인지 구분이 안가는 상황이 되니까.
* 현재는 추가는 add()로, set은 iter로 하는 상황이다. 그러니 다음과 같이 고쳐야 한다.
    * tnmap은 multimap으로 유지한다.
    * tnmap은 add와 별도로 set(iter&, val) 이라는 함수를 추가한다.
        * key로 하면 어떤건지 알 방법이 없으므로 iter를 쓰는 것이다.
    * iter는 setVal을 public으로 제공하지 않는다.
    * tbicontainable은 set()을 pure virtual로, tnmap::set()을 virutal로 제공하고 기본동작을 채워놓는다.
        * 이때 set()의 반환형은 bool로 에러가 발생했는지 여부를 반환할 수 있어야 한다.
    * scope은 tnmap에서 상속한 뒤, set() 안에서 super::set()을 부르고 에러가 없으면 rhs.\_onSet()을 불러주자.
    * 마지막으로 assignExpr::run() 에서 scope::set() 을 사용하도록 수정하자.
## abstract prop
* 값이 있는 prop은 abstract가 될 수 없다.
* get, set을 정의만 하고 비워둔다. 이 경우, 앞서 말한대로 호출불가다.
* get과 set이 모두 호출불가이므로, 이 prop은 abstract 하다.
```go
def Base
    age int
        get set
```
* * *
# with
## 기본 컨셉
* 문법의 형태는 이렇다.
```
with-inline-expr: with <inline-expr>
with-block-expr: with
             | with <inline-expr> (<indentBlock>)*
with-expr: with-inline-expr | with-block-expr
```
* 동작의 핵심은 \<inline-expr\>로 eval되는 객체를 현재 scope 위에 얹는 것이다.
* 그래서 해당 객체의 subs 들을 직접 접근할 수 있게 된다.
* with는 indentBlock이 있는가 없는가에 따라서 2가지 형태로 사용된다.
    1. indentBlock이 존재한다면, 해당 블록 내에서만 with가 유효하다.
    2. indentBlock이 없다면, 해당 구문이 사용된 blockExpr 전체에서 유효하다.
* 2번 형태의 문법은 모든 곳에 등장할 수 있지만 1번 형태의 문법은 함수 내에서만 사용이 가능하다.
* 그래서 개념적으로 2가지형태는 같은 기능, 같은 키워드에 속하지만, 파싱할때의 문법적으로는 2개는 엄연히 구분된다.
* 기능은 단순하지만, 매우 다양한 용도로 사용되기 때문에 영향력이 큰 문법이다.
* verifier는 withExpr.itExpr의 추출된 orgObj (즉, abstract 타입) 으로 expand을 해서 검증할 것이다. 반면, 실행단계에서는 withExpr.itExpr을 실제로 돌려서 나온 obj(즉, 복제된 구체객체)를 expand한다.
    * 이 동작은, 검증단계의 verifier가 알지 못했던 함수가 실행단계에서 override되어 들어갈 수 있다는 걸 의미한다.
    * verifier 당시와 다른 함수가 바인딩되는 것이지만, 문제 없다. verifier가 관심이 있는 건, 어디까지나 **호출이 가능한가?** 뿐이기 때문이다.
## with-block-expr
* **함수 내에서** 사용되며, `inline-expr`를 돌려서 나온 객체의 subs들을 바로 접근하도록 해준다.
```go
a := getA(getB(), 33)
print(a.age)
print(a.grade)
a.say()
...
age := 33
with a
    print(age) // local age가 아니라 a.age로 접근한다.
    print(grade)
    say()
```
* 현재의 objScope 보다 위에 위치하도록 해주는 기능이다.
* 이름이 충돌하는 경우 localScope은 항상 objScope보다 위에 있으므로 항상 local variable이 먼저 우선권을 갖는다.
* with 뒤에 \<inline-expr\>을 생략할 수 있다. 이 경우 그저 블록문의 역할만 수행한다.
    * `if true <indentBlock>` 과 같은 기능이다.
* defBlock 바로 밑에다가 이 with-block-expr을 절대로 사용할 수 없다.
* block문이 실행되기 직전에, itExpr은 `me` 와 `it` 으로, withExpr이 실행되기 전의 `me`를 `outer` 라고 각각 변수를 만들어 둔다.
* with 뒤에는 `?` 를 붙여서, itExpr이 nullable이면 skip하고, non null일때만 with를 동작하도록 만들 수 있다.
    * 이 문법은 함수내에도 가능하고, def 안에서도 가능하다.
    * `?`가 사용되지 않았을 경우 itExpr은 반드시 non err type이어야 한다.
        * 이 검증은 verifier가 수행한다.
### 알고리즘
* with는 함수내에 사용될 경우인 이 `with-block-expr`과 `def` block내에서 사용되는 `with-inline-expr`의 경우 각각 다른 로 파싱된다.
* withBlockExpr은 동작이 간단하다. 다음처럼 동작한다.
    1. targetExpr을 run()해서 obj를 뽑아낸다.
    2. obj의 objScope 전체를 뽑아내서 currentFrame의 objScope에 chain을 걸어준다.
    3. 그리고 currentFrame::objScope로 뽑아낸 objScope로 연결한다.
    4. frameInteract(blockExpr) 를 한다. 안에서 localScope을 하나 추가할 것이다.
    5. `it` 변수에 obj를 바인딩해준다. 이것은 localScope에 들어가게 된다.
    6. blockExpr 동작이 끝나면 역순으로 풀어준다.
* 검증도 간단하다.
    1. verifier는 targetExpr의 type이 되는 orgObj를 추출한다.
    2. 해당 orgObj로 위의 withBlockExpr 동작을 수행한다.
    3. 참고로 withBlockExpr은 함수 안에 있으므로 verifier가 수행한다. verifier는 모든 객체의 subs()가 확정되서 나온 상태이므로
       종속성이 모두 해결되어 있다. 그러니 이렇게 동작이 간단한 것이다.
## with-inline-expr 공통
* 이 with-inline-expr 문법은 어디에서 이 expr이 실행되느냐에 따라서 각기 다른 AST가 파싱되어야 한다.
    * 그래서 함수내에서의 inline-expr와 defBlock에서의 inline-expr을 각각 서술한다.
* 반드시 itExpr이 있어야 한다. 생략할 수 없다.
* with-block-expr과 다르게 with 뒤에 `?`를 넣을 수 없다. null일때와 아닐때의 동작이 너무 달라져서 검증이 의미없기 때문이다.
* with를 선언할때 이름을 붙였다면, 그 이름을 사용해서 언제든지 assignExpr을 사용할 수 있다.
    * 이 assignExpr은 with를 매번 새로 하는 효과를 지닌다.
    * 물론, assignExpr을 할때 rhs는 lhs와 같은 타입이거나 더 구체타입어야 한다. 이건 일반적인 assignExpr에 있는 내용이다.
## 함수 내에서 사용되는 with-inline-expr
* withInlineExpr객체로 만들어버리면 된다. 동작은 BlockExpr과 거의 흡사하다. 쉽다.
### 파서는 어떤 객체를 만들어야 할까???
* parser가 객체를 만드는 시점에서는 이 with가 함수에 속한 것인지 def 에 속한것인지 알 수 없다.
* parser는 내가 만드는 게 defBlock인지 block인지조차도 알 수 없다.
* 파서는 어떤 객체를 만들어야 할까?
1. [?] 현재로써 가장 유력한 방법은, 일단 함수라고 가정하는 것이다.
    * 그리고 defBlock이었다면, 다른 객체를 만들어서 교체한다.
2. [?] 더 좋은 방법은?

### 알고리즘
* 함수내에서 사용되는 경우라면, currentFrame::objScope으로 뽑아낸 objScope으로 바로 연결한다.
    * 참고로 함수는 종료시 frame을 모두 폐기하므로, with-inline-expr로 추가한 objScope을 pop()하는 동작은 필요없다.
* 별도의 blockExpr을 돌리는 과정은 없다. blockExpr 자체를 들고 있지 않는다.
* 참고로 AST로 파싱되었을때 with-block-expr에서 생성되는 withBlockExpr과는 다른 클래스다.
* 검증의 경우도 with-block-expr과 흡사하므로 생략한다.
## defBlock 안에서 사용된 with-inline-expr
* with문과 마찬가지로 \<inline-expr\> 로부터 eval한 객체의 objScope 부분을 현재 def 중인 objScope 밑에 깔아주는 기능이다. 하지만 더 복잡하고 어렵다.
* 한가지 예외가 있다. defBlock안에서 선언된 with 로 주입될 objScope은 반드시 `def` 로 현재 만들고 있는 objScope보다 **밑에** 깔려야 한다.
    * 즉 `with` 보다 현재 `def`로 만들 고 있는 objScope이 더 우선순위가 높다. 이는 일반적으로 함수 안에서 사용하는 with와는 **동작이 반대**인 것이다.
* \<inline-expr\> 에는 defAssign이나 defVar처럼 정의문도 올 수 있다. 몇 안되는 정의문이 expr로 올 수 있는 부분이다.
* 이 정의문에는 이름이 붙게 되는데 이름을 붙이면, 나중에 이 이름을 가지고 다른 객체로 expansion할 객체를 변경할 수 있다.
* defBlock에서 사용될 때만 itExpr에 ADT가 orgObj인 expr을 넣을 수 있다.
    * 이때 반드시 모든 생성자마다, **각 생성자를 돌리고 나면 100% 해당 ADT가 구체객체로 채워지도록** 를 코드를 작성해야 한다.
* 이렇게 with를 def 안에서 사용하는 것을 Type Expansion 이라고한다.
* 다중상속을 권장하지 않는다.
    * `with A; with B` 에서 A와 B가 서로 공통의 타입을 가지고 있다면 경고를 내보낸다. 이렇게 쓰지 말라는 의미다.
* scope 안에 with들은 서로 우선순위를 갖지 않는다.
    * 이름 충돌이 생기면 바로 에러다. 어떤 언어는 나중에 with한게 우선순위를 갖기도 하지만, 내 언어는 그렇지 않다.
    * 사용자가 이런 이름 충돌을 해결하려면
        1. 설계를 다시해서 상위타입에 함수가 겹치지 않도록 하거나
        2. defBlock 안에 override 한 함수나 prop을 새로 정의한다.
### 알고리즘
* 핵심내용
    * `tnchain& tchain::unlink(tnchain& it)` 를 제공한다.
        * 이 tnchain의 link된 chain들을 순차탐색해서 it 과 addr이 같으면 그걸 unlink 한다.
            * 그 it의 prev와 it의 next를 봉합한다. 결과 it는 메모리에서 사라진다.
        * 이 tnchain보다 prev인 chain은 unlink 못한다.
    * orgObj는 state로 관리된다.
        * RELEASED
        * PARSED
        * EXPANDED
        * VERIFIED:
            1. fixOwns 에서 복제한 owns를 만든다.
            2. subs = owns --> shares 로 새로 link 한다.
                * 이 상태는, 생성자를 호출하기 직전의 상태로 돌아간 것이다.
            3. @expand를 제거한다.
        * INITIALIZED: 
            * getState() == VERIFIED였다면, callComplete()를 사용해서 완전객체로 만든다.
            * 참고로 모든 orgObj는 VERIFIED <--> INITIALIZED 상태 변환이 언제든 가역적이어야 한다.
    * slot은 orgObj와 동일한 state 관리 구현코드가 있어야 한다.
        * 따로따로 구현하면 안된다는 얘기다. 재사용 해야지?
    * orgObj와 obj는 모두 \_shares 와 \_owns 를 각각 arr로 가지고 있다. \_subs는 chain으로 가진다.
    * orgObj는 추가로 fixOwns를 arr로 하나 더 갖는다.
    * 자세한 객체 생성 알고리즘은 문서 아래에 별도로 정리하였다.
    * orgObj는 callComplete() 라는 `완전객체 생성자 호출자` 라는 게 있다. prerun 단계에서 생성자의 어떠한 함수를 호출해서 완전객체를 만들것인지를 결정하는 것이다.
        * orgObj::isComplete() 는 \_callComplete == null; 을 반환하면 된다.
    * orgObj의 종속적인 sub들은 mark&eval 알고리즘으로 진행된다. (half 무한루프)
    * orgObj는 expand 단계에서 각 종속적 sub들이 orgObj가 반환되었다는 이상적인 가정을 하고 일단 orgObj를 완성한다.
    * get, set이 있는 prop은 초기식이 있는 것과 없는 것 2가지가 있다. 이것에 따라서 어느 container에 넣어야 하는지가 갈린다.
        * 초기식이 없는, get, set이 있는 prop은 parser가 shares에 넣어두면 된다.
        * 초기식이 없는, get, set도 없는 prop은 parser가 fixOwns에 넣어두면 된다.
        * 초기식이 있는 prop은 get/set이 있건 말건 무조건 @expand행이다.
    * withProperty는 실질적으로 동적에 `expand` 기능을 구현하는 프로퍼티다. 다음의 특징을 갖는다.
        1. meObj는 @set 함수에 이미 넘어와 있다는 것을 기억하자.
        2. withProperty는 생성자는 기본 생성자를 사용한다.
        3. expand 단계에서 `expander` 로부터 orgObj 정보를 받는다.
        4. parser 단계에서 이미 각 orgObj는 directSubs들이 subs()에 가장 먼저 담겨 있는 상태다.
        5. runtime에 withProperty::set(it str)이 불려지면,
            1. `it.sub\<orgObj\>() == true`여야 한다. 체크한다.
            2. meObj.subs().containers를 가져온다.
            3. 신경써야 할 부분은 딱 2개:
                1. directSubs()가 반드시 containers의 chain의 가장 마지막에 있어야 한다
                2. containers 맨 앞에 기존의 expandedObj가 있었을 때
            4. meObj.orgObj.directSubs().len() > 0 이면 반드시 parser가 directSubs를 containers에 넣어뒀을 것이다. 그렇게 가정하라.
            5. 2번의 containers\[0\] == this->expandedObj 였다면
                1. this->expandedObj = it
                2. this->expandedObj->next = containers\[1\] // 참고로 containers\[1\]은 null일 수도 있다.
                3. container = this->expandedObj 하고 끝낸다.
            6. directSubs()가 없었다면:
                * 그냥 `containers.link(this->expandedObj = deepWrap(it))` 하고 끝낸다.
                * deepWrap을 하는 경우는, 이미 expandedObj가 여러 chain으로 구성될 경우를 대비한 것이다.
            7. directSubs()가 있었다면:
                * this->expandedObj = deepWrap(it)
                * this->expandedObj.next = \*containers\[containers\].last
                * containers\[containers.len - 2\].next = this->expandedObj
                * 하고 끝낸다.
            8. 모두 아니라면, 다음의 범용 로직을 돌린다: containers의 container를 반복자로 순회하면서,
                1. container->next == this->expandedObj 가 아니라면 계속 반복한다.
                2. container->next가 expandedObj일 경우라면:
                    1. it.next = container->next->next
                    2. container->next = it.next
                    3. this->expandedObj = it
                    4. 하고 끝낸다.
                3. 여기까지 왔다는 것은 `이 withProp은 expand되었으나, expand한 container를 못찾았습니다.` 라는 뜻이다. 에러를 내보낸다.
        6. meObj의 chain을 새로 바뀐 `it` 으로 다시 갱신한다.
    * defBlock안의 with구문은 다음의 2가지 AST 요소로 파싱된다.
        1. defBlock 안에 정의된 with는 withProperty의 형태로 @expand 안에 sub로 정의된다.
            * 참고로 이 @expand는 expand 단계에서 expander에 의해서 이 meObj의 subs()안에 들어가게 된다.
        2. @commonCtor 라는 곳에 withExpr을 assign하는 코드가 들어가 있다.
    * orgObj는 prerun단계 이후에서 생성자를 호출하기 직전에 @commonCtor을 가장먼저 호출한다.
        * 그 결과 expand에서 orgObj로 러프하게 채워졌던 withExpr이 진짜로 withExpr 초기식을 돌려나온 객체로 채워진다.
* 상세내용
    * 빌드 과정은 parse -> expand -> verification -> prerun -> run 5단계로 이루어진다.
    * parse 단계
        * `def` 키워드를 만나면 orgObj를 생성해서 현재 `pack.orgs` 에 넣어둔다.
            * `pack` 자신조차도 이 orgs에 들어있다.
        * defBlock 안의 with를 만났을 때 패턴매칭을 한다.
            * `with A := bExpr`, `with A B` 처럼 이름이 있는 경우:
                * 공통생성자(@commonCtor) 에 `assignExpr("A", bExpr)` 을 넣어둔다.
                * `subs().add("A", dummyNode()) 로 넣어둔다.
                * `@expand라는 subs 안에 add("A", withProperty(bExpr))` 로 넣어둔다.
                    * 이 함수에 구문들은 expand 단계에서 종속문제를 풀어서 타입을 fix시킬 것이다.
            * `with aExpr`, `with callFoo()` 처럼 이름이 없는 경우:
                * 이름이 절대 겹치지 않는 hash로 돌린 name을 하나 만든다. 이를 aName이라 하자.
                * 공통생성자(@commonCtor) 에 `assignExpr(aName, bExpr)` 을 넣어둔다.
                * `@expand라는 subs 안에 add(aName, withProperty(bExpr))` 로 넣어둔다.
                    * 이 함수에 구문들은 expand 단계에서 종속문제를 풀어서 타입을 fix시킬 것이다.
        * 기타 초기식 expr (defAssignExpr) 을 만났을 때도, 
            1. @expand 안에 key, value 식으로 추가해둔다.
            2. @commonCtor에 assignExpr(key, value) 를 넣어둔다.
        * defVarExpr도 다음과 같이 처리한다.
            * 이 역시 완전객체 생성시에 @commonCtor를 통해서 subs()에 넣어져야 한다.
            1. @expand에 subs().add(name, proxyNode(getExpr(orgName))) 로 넣어둔다.
                * @expand에 넣어두는 이유는, 파싱타임에는 orgName인 orgObj가 없을 지도 모르기 때문이다.
                * proxyNode로 넣어놓아야 orgObj 임에도 isComplete가 가능해진다.
            2. @commonCtor에 \_directSubs.add(name, getExpr(orgName)) 을 넣어둔다.
    * expand 단계
        * expander는 여러 expand 객체를 거느린다.
        * expand 객체는 하나의 orgObj에 대해서 전담한다.
            * orgObj에서 @expand map 에 대한 iterator를 membervariable로 갖는다.
            * 그래서 expand 객체는 언제든지 작업을 중단하고 함수를 빠져나갔다가, 다시 진입했을때 이전에 작업하던 iterator부터 재개할 수 있다.
        * expander::start() 가 불리면,
            * pack.orgs를 org로 순회하면서,
                1. expands\[org\] 로 expand 객체를 가져온다.
                    * 이때 expands\[org\]가 null이라면 새로운 expand 객체를 만들어서 expands에 넣어두고 이를 반환한다.
                2. expand.run() 을 한다.
                    * expand는 org의 @expand 로부터 map을 가져와서, 이 map의 iterator를 뽑아 순회한다.
                    * iterator가 가리키는 pair(key, value) 중 value를 
                    * 각 meObj의 @expand 함수의 각 pair의 value를 타입추론해서 meObj.subs()에 add(key, orgObj) 식으로 옮긴다.
                        * 참고로, expand에서 만든 subs()의 prop 관련 내용들은 어짜피 `run` 단계에서 @commonCtor에 있는 assignExpr에 의해서 교체될 것이다.
                    * 아무튼 이때 각 @expand 안의 value가 `withProperty` 로 판단된 경우,
                        1. withProperty.orgObj를 추론시도한다.
                        2. 실패하면 expand를 중단하고, 다른 expand 객체로 retry한다.
                        3. 성공하면 orgObj.withs 목록에 현재의 withProperty의 orgObj를 push 해둔다.
                        4. 추론한 orgObj로 일단 임시로 withExpand를 해둔다.
                            * 방법은 단순히 `withProperty = orgObj` 를 하는 것이다.
                                * 이렇게 하면 withProperty의 @set을 발동시킨다.
    * verification 단계
        * defVar, defAssign, withExpr 에서 run단계와의 중요한 차이가 있다.
        * 바로 실제로 돌려보는게 아니라, 타입추론된 orgObj를 대상으로 proxyNode를 씌워서 각 동작을 run 한다는 것이다.
    * prerun 단계
        * 모든 pack.orgs에 대해 setState(VERIFED)로 변경한다.
            * 각 orgObj는 자신의 subs()에서 with로 확장된 chain를 날리고, directSub로만 채운다.
    * run 단계
## defBlock에 사용하는 with는 다중상속과 거의 유사하다.
## static 도 with로 구현이 가능하다.
```go
def A // 불완전
    with def
        foo(): ret
    boo(): ret
A.boo() // err. boo의 direct owner인 A는 불완전이니까.
A.foo() // ok. foo()의 direct owner는 A안 있는 def다.
```
## with 뒤에 abstract가 올 수 있는 경우가 있다.
* 모든 생성자에서 어떠한 경로에서도 그 prop이 abstract 대신 다른 구체객체를 채워놓았다면 에러가 아니다.
    * 하나의 생성자라도, 그 생성자에서 하나의 분기에서라도 assign을 하지 않았다면 에러다.
* 예제를 보자.
    ```go
def A: ... // 불완전객체
def B // 불완전객체
    with A
def myObj
    with a A // 불완전을 non errorable 로 만들었다. 이제 이 a를 ctor에서 구체객체로 assign 해야 한다.
    ctor(@B)
        if foo(3)
            a = B
        // else라면 a 에는 비어있다는 얘기가 된다.
    ctor(n int)
        foo(3)
            on true: boo(a = B) // true 가 아닐때는 a에는 아무런 구채객체도 assign 되지 않는다.
// 결과: 컴파일 에러!
    ```
### 핵심
* 이 작업은 verifier 가 직접 하거나, verifier가 다른 전담 클래스를 호출해서 구현한다.
* 분기문의 조건식을 보지 않는다.
* 사전에 ADT로 prop한 list를 만들어둔다. 2라고 하자.
* 처음시작하면 assign 완료된 목록을 저장할 chain을 하나 만든다. 1이라고 하자.
* 코드를 한줄한줄 따라내려가면서, 그 list 중 하나에게 assign을 한 것만 찾는다.
    * 참고로 assign은 `foo(a = b)` 처럼 term으로 있을 수도 있다.
    * assign을 찾았으면 현재의 chain, 1의 inner container에 직접 add한다.
* 블록문이 끝나면 해당 chain을 없앤다.
* 분기가 끝나면(`when` 혹은 `if`) 이 분기Expr들이 가지고 있는 모든 블록문에 대한 chain들을 다 순회하면서 공통적으로 assign을 한 목록을 꺼내서 분기Expr 밖에 있는 chain에 add한다.
    * 분기안에 있는 블록문을 탐색하기 직전에 chain을 만들어서 외부의 chain과 연결한다.
    * 그리고 분기문 안에서 발생한 assign은 방금 만든 chain.innerContainer에만 담는다.
* 만약 블록문에 return이 있다면, 그 즉시 지금까지의 chain안에 있는 assign 들이, 사전에 만든 2를 모두 만족하는지 검사한다.
    * 만족하지 않는다면, 그 즉시 에러다.
* 만족한다면, 이 블록문을 종료한다.
    * 이렇게 종료된 블록문은, 분기Expr안에 있었을 경우에만 분기Expr은 이 블록문을 처음부터 없었던 것처럼 무시한다.
    * 분기Expr이 아닌 블록문에 그냥 return이 있었다면 1과 2를 검사하고 이 작업은 즉시 종료된다.
* visitor로 구현한다.
### 알고리즘
```cpp
class checker : visitor {
    nchain _candidates; // 전체 assign 해야할 목록
    map<string, str> _errs; // 확실한 에러로 판명남.
    struct partialCheck { // 각 blockExpr 당 체크한 목록
    tstr<nchain> done; // assign이 완료됨.
    bool hasRet; // ret/return이 있었음

    partialCheck(nchain& newDone, nbool newHasRet): done(newDone), hasRet(newHasRet) {}
    partialCheck(PartialCheck& parent) {
        done.link(parent.done);
        errs.link(parent.errs);
    }
    partialCheck(): hasRet(false) {}

    partialCheck operator+(const partialCheck& rhs) const {
        partialCheck ret;
        ret.hasRet = hasRet && rhs.hasRet;
        errs += rhs.errs;
        done = done.교집합(rhs.done);
    }

    me& setFunc(func& f) {
        _func.bind(f);
    }
    void start() override {
        _func->accept(...);
    }

    map<blockExpr*, partialCheck> _partials; // 각 blockExpr별로 assign
    vector<blockExpr*> _blockStack;
};

partialCheck& checker::getPartial(blockExpr& b) {
    static partialCheck _stub(_candidates, false);
    if(_partials.has(&b))
        return _partials[&b];
    return _stub;
}

partialCheck& checker::getPartial() {
    return getPartial(_blockStack->front());
}

int checker::onVisit(assignExpr& a) {
    getPartial().done[a.getName()].bind(a.getEval());
}

int checker::onVisit(retExpr& r) {
    partialCheck& partial = getPartial();
    partial.hasRet = true;
    return DONT_VISIT_FURTHER; // 이제 traverse가 더 되지 않음.
}

int checker::onTraverse(blockExpr& b) {
    // 대부분 super와 동일.
    int n = 0;
    narr& stmts = b.getStmts();
    int len = stmts.len();
    for(auto& stmt : stmts) {
        int res = stmt.accept(visitInfo {"", &b, n++, len, i.depth+1}, *this);
        if(res == DONT_VISIT_FURTHER)
            return DONT_VISIT_FURTHER; // 혹은 이 코드를 visitor에 박아놓던가.
    }
}

int checker::onVisit(blockExpr& b) {
    blockExpr& parent = _blockStack.front();
    _blockStack.push_back(&b);
    _partials[&b] = partialCheck(_partials[parent]);
}

int checker::onLeave(blockExpr& b) {
    _blockStack.pop();
    partialCheck& partial = getPartial();
    if(partial.hasRet || &b == &_func.getBlock()) {
        auto rest = // _candidates - partial.done 해서 남는게 있는가?
        _errs.add(rest);
        partial.done.rel();
    }

    _partials.remove(partial);
    return 0;
}

int checker::onVisit(ifExpr& i) {
    auto& thenBlock = getPartial(i.thenBlock);
    auto& elseBlock = getPartial(i.elseBlock);
    getPartial().done.add(thenBlock.done.교집합(elseBlock.done));
}
```
* 분명히 가능한 로직이라고 본다. 다만 간결하게 쉽게 잘짜기란 조금 어렵다.
* 또, 이 로직은 verifier에서만 사용하기 때문에 node같은 runtime에도 사용하는 부분에는 API를 두지 않는게 좋을 것이다.
* * *
# 종속성 해결 알고리즘: mark & deep
## 컨셉
* 종속성 해결이란, parser 단계에서 타입을 확정짓지 못하는, @expand map에 들어가 있는 것들을 최대한 쉽고 빠르게 풀어서 verification 단계에 들어갈 수 있도록 하는 문제다.
* 기존에는 커스텀 타입을 표현하는 데 있어서 `getExpr("A")` 로 파싱하곤 했다.
* 그러나 이건 문제가 있었다. 다음 코드를 보자.
    ```go
    def A
        def B: ... // 1
        foo(b B): ...
    def B: ... // 2
    boo(): foo(B()) // 3
    ```
    * 3번에서 컴파일 에러가 발생하지 않는게 문제다.
    * getExpr("B") 로 검사하는데, foo의 params을 검사할때 foo 함수로 stackFrame을 교체하지 않기 때문에 함수의 stackFrame에서 getExpr("B")를 하는게 아니라 호출자인 boo() 함수 기준으로 stackFrame에서 getExpr("B")를 하기 때문에 항상 2번 객체가 나오기 때문이다.
    * 따라서 `getExpr()` 로 해둔 것은 verification을 가기 전에 제대로된 orgObj로 교체할 필요가 있으며, parser는 이것을 하지 못한다.
* expand는 getExpr()을 적절한 orgObj로 교체하는 역할도 수행한다.
* parser는 기존대로  커스텀 타입에 대한 orgObj를 표현을 `getExpr()`로 해서 share 혹은 own에 넣어둔다.
* 그 대상은 다음과 같다.
    * params나 returntype이 있는 func.
    * getter 혹은 setter를 가진 defVarExpr prop
    * defVarExpr
    * with에 넣어진 defVarExpr
* 반면에 expr에 초기식이 포함된 경우에는 expand과정을 먼저 돌린다. 그러니 @expand 에 넣는다.
    * @expand는 multimap이다.
    * @expand에는 서로 key가 같은 value가 있을 수 있다. 하지만 key와 canRun(args) 까지 같은 value는 절대 있을 수 없다.
* mark & deep은 mark & sweep에서 참고한 것으로 종속성을 계속 따라가면서, 처음 위치로 돌아오게 되면 순환에러를 내보내는 알고리즘이다.
* parse가 끝나면 모든 orgObj를 만들어두긴 한 상황이다. 그러니 getExpr()은 문제없이 교체가 가능하다.
* 그러나 parse는 해당 orgObj의 subs()는 채우지 못한다. 그러니 이 subs()에 접근하는 문제는 종속성 문제다.
* 우리는 이 둘을 구분해야 한다.
## 핵심
* expander는 pack.orgs에 접근할 수 있다.
* frameInteract(obj) 시 obj가 이미 frameInteract 가장 최상단에 위치해 있으면, 스킵한다.
    * 같다는 기준은 frame::getMe()의 주소가 interact할 &obj 와 같은지 비교한다.
    * obj::\_outFrame() 도 호출하지 않도록 해야 한다.
    * A::foo() 안에서 A::boo() 를 호출한다면, objScope은 변하지 않는 다는 것이다. 그리고 그렇게 해도 된다.
* expander는 hashmap<void*, enum> \_marks 를 가지고 있다.
    * 주어진 ptr에 대해 pair가 존재하면 mark가 된 것이다.
    * 여기서 mark란 `의존성 문제 해결을 위해 방문했다!` 라는 뜻이다.
    * enum mark에는 3가지 상태가 있다.
        1. mark가 없음: 한번도 resolveDep을 시도조차 해본적이 없다.
        2. marked: 시도했었고, 잘 되었다. 이제 이 값을 참조만 하면 된다는 것.
        3. marking: 이 코드라인을 종속성 풀려고 했는데, 다시 돌아왔네? 즉 순환종속성 문제다. 이 케이스는 무조건 에러다.
* getExpr(obj, name) 처럼 `A.B` 와 같이 expr 혹은 obj 가 가진 무언가를 접근하는 것은 분명 종속성 문제다.
* `with` 는 종속성이 나올 수 있다. parser는 종속성이 있는지 아닌지 판단이 어렵다. 그러니 일단은 종속성이 있다고 가정하고 @expand에 들어간다.
    * with는 defWithPropExpr로 parse 된다. 이 시점에서는 prop이 아니다.
* `with`는 expand 시점에서 완료한다.
    * 어떠한 초기식은 `with` 가 되었다는 걸 전제로 한 경우도 있기 때문이다.
    * @expand 에서 defWithPropExpr이 다 검증되었다면,
        ```cpp
        defWithPropExpr& e;
        // obj, func frameInteract가 이미 되어있다는 전제하에:
        withPropExpr& new1 = e.run();
        me.owns.add(e.name, new1);
        new1.set(e.getEval()); // property의 setter를 호출하게 된다.
        ```
* 종속성 문제를 지닌 expr은 반드시 초기식이 있는 구문이다. 초기식이 있는 구문이란 반드시 owns에 들어가는 것들이다.
* expander는 visitor다. 다만 함수내부는 보지 않는다. 종속성문제를 야기시키는 부분만 탐색하고 수정한다.
* visitor는 visit -> onVisit -> onTraverse -> onLeave 순으로 진행된다.
* expander는 종속성 체크를 onVisit에서 처리할 것이다. 그러니 코드를 돌려서 sub 를 채우는 동작은 onLeave에서 해야 한다.
* obj::inFrame(orgObj)
    * 안에서 orgObj.pack, orgObj.fileScope을 모두 frameInteract에 넣는 걸 추가한다.
* expand는 2스텝으로 진행한다.
    1. resolveDep을 통해서 종속성 문제를 해결한다. 해결되면 @expand 에 있는 걸 sub로 옮긴다.
    2. 각 orgObj의 own, share에 있는 것들 중, orgObj에 대한 표현을 `getExpr`로 되어있는 걸 모두 적절한 orgObj로 교체한다.
* generic 클래스도 이 expand를 돌려야 한다.
    * 문제는 기존에는  `a A<int>` 와 같이 Generic을 사용하는 `getGenericExpr` 을 했을 경우에만 **런타임에** generic 객체가 만들어졌다는 것이다.
    * 그래서 이제는 expand 들어가기 전에 모든 generic객체를 만들어 놓는 방식을 택한다.
    * pack은 gorgGenerators을 가진다. generic을 genericObj 안에 생성하는 용도다.
    * gorgGenerators는 getGenericExpr, meOrg, func 2개로 구성되어 있다.
    * gorgGenerators.getEval()을 하게 되면 meOrg, func으로 frameInteract 한 다음에 getGenericExpr이 찾은 genericObj::run()을 호출할 거고, 이 함수는 genericObj::\_cache 안에 새로운 genericObj을 만든 뒤, 이걸 반환한다.
    * parser는 getGenericExpr를 만나면, pack 안에 @gorgGenerators를 만들어 채워둔다.
    * expander는 visit(pack) 하기전에 pack.@gorgGenerators에 대해 각각 getEval() 한 것을 pack.org에 넣어둔다.
    * 그리고 나서 expander는 pack.@gorgGenerators를 지운다.
* 종속성을 풀때는 위에서 아래로만 하지 않는다.
* 최적화시에는,
    * 종속성을 풀면서, marked 된 stmt의 순서를 queue에 push 해둔다.
    * 그리고 pack.@gctor을 하나 만들고 여기에 queue에 있는 stmt를 그대로 넣어둔다.
    * pack은 prerun 단계에서 @gctor을 위에서 아래로 순차로 실행만 하면 된다.
## 알고리즘
아래의 알고리즘은 모두 서술된 것은 아니나, 감을 잡기에는 충분하다.
* visit(pack)를 수행한다. 이게 진입점이다. 다른 verifier, visitor와 동일하다.
    * 이 함수가 호출되면, 다른 것보다
        * pack.@gorgGenerators에 대해 각각 getEval() 한 것을 pack.org에 넣어둔다.
        * 그리고 나서 expander는 pack.@gorgGenerators를 지운다.
    * resolveDep(obj& me, const string& name, const args& args = nullable)
        * e.getMe()로 orgObj를 구한다.
        * 그리고 orgObj.@expand\[e.name\] 으로 expr을 꺼내온다.
            * args가 null이 아니면, @expand.sub(e.name, args) 로 한다.
        * expr이 이미 marking 상태인지 확인한다. 맞다면 무조건 에러다. 모든 과정이 즉각 취소되어야 한다.
        * expr이 marked라면 이미 했던 검증이다. return; 한다.
        * 아니라면 expr을 marking으로 변경한다.
        * frameInteract(orgObj)
            * res := expr.accept(this) 한다.
        * res 가 err가 아니라면 expr을 marked로 변경한다.
    * expand(obj& me, const string& name, expr& e)
        * **e가 @expand에 있는 초기식 구문이라고 가정한다.**
        * **e의 종속성 문제가 모두 해결되면 e를 @expand에서 꺼내서 meObj.owns에 넣는다.**
        * res := resolveDep(me, name)
        * res가 에러면 에러를 report하고 바로 이 함수를 종료한다.
        * me.owns에 e.getEval() 자체를 add 한다.
        * 만약 e가 with였다면,
            ```cpp
            defWithPropExpr& e;
            // obj, func frameInteract가 이미 되어있다는 전제하에:
            withPropExpr& new1 = e.run();
            me.owns.add(e.name, new1);
            new1.set(e.getEval()); // property의 setter를 호출하게 된다.
            ```
    * onTraverse(pack)
        * pack.orgs를 하나하나 traverse한다.
    * onVisit(orgObj)
        * orgObj.inFrame() 한다.
        * 이후 만나게 되는 defVarExpr의 getExpr을 orgObj로 교체하는 작업들을 위해서 inFrame을 한다.
    * onTraverse(orgObj)
        * `@expand` 를 찾아서 그 pair들을 하나하나 traverse 한다.
        * stmt가 marked 상태면 continue
        * 아니라면 expand(stmt) 한다.
    * onLeave(orgObj)
        * **이 orgObj에 대한 종속성과 sub에 옮기는 일이 모두 끝났다.**
        * shares, owns를 각각 accept한다.
            * onTraverse(orgObj) 에서는 1PASS만 돌렸다. 즉, @expand --> sub 로 옮기는 작업만 했다는 것이다.
            * 그러니 여기서 2PASS를 한다.
        * orgObj.outFrame() 한다.
    * onVisit(defPropExpr)
        * defPropExpr은 defVarExpr처럼 type이 명시되어있다.
        * 이 type이 getExpr로 되어있으면 orgObj로 교체한다.
    * onVisit(func)
        * params에서 getExpr() 로 되어있으면 교체한다.
        * returnType이 getExpr()로 되어있으면 교체한다.
    * onTraverse(func)
        * 그냥 skip 한다. 함수 안 쪽을 expand 할 필요는 없다.
    * onVisit(visitInfo, defVarExpr)
        * defVarExpr에 getExpr()로 되어있는 부분을 orgObj로 교체한다.
    * defVarExpr, defAssignExpr등은 visitor 클래스 안에 *기본으로* term을 탐색하도록 구현이 되어있다.
    * onVisit(getExpr& e)
        * e.me 가 null이 아니라면 다른 객체에 대해 종속적인 것이다.
        * resolveDep(e.getMe(), e.getName(), e.getArgs()) 를 한다.
    * onLeave(orgObj)
        * onOut(orgObj)
        * subnode에 대한 onVisit, onTraverse 에서 에러가 없었다면 orgObj.setState(EXPANDED)로 변경한다.
* * *
# Pattern matching (패턴매칭)
* 임의의 객체에 대해서 그 값이나 타입이 특정 패턴에 부합할 경우, 별도의 로직을 수행할 수 있도록 분기를 해준다.
* 패턴매칭은 매칭의 대상인 itExpr과 itExpr의 값이 패턴에 매치되는지를 비교하는 matcher, 매치되었을때의 실행할 블록문으로 구성된다.
* 중요한 점은 matcher의 종류가 pattern matching을 위해서 특별히 준비된 것들이어서는 안된다는 점이다.
    * 코틀린은 저런 방식으로 하는데, 결과적으로 암기를 요구한다.
    * namu는 모든 종류의 비교문, 비교관련 keyword를 전부 사용할 수 있는 것을 목표로 한다.
* matcher 문법 사용시의 조건은 다음과 같다.
    * 2항 연산 비교문이어야 한다.
        * `== 5`, `> 23`, `<= 34` 전부 된다는 얘기다.
    * lhs는 생략을 해야 한다.
    * matcher만은 위한 키워드는 존재하지 않는다. ==> 밖에서 쓰는 키워드 & 문법을 살짝 변형해서 matcher로 쓰는 것이다.
    * matcher 안에 `&&` 나 `||` 는 나올 수 없다. ==> verifier가 체크해야 할 것이다.
    * 상위의 matcher 부터 검사한다.
    * matcher에 match 되면 이후의 matcher는 skip 한다.
    * 어떠한 패턴도 매치되지 않는 경우를 위해 `else` 키워드를 사용할 수 있다.
    * 즉, 일반적으로 matcher는 가장 첫 token이 `in`, `is` 같은 키워드이거나, `<`, `>` 같은 기호일 것이다.
    * 만약 어떠한 기호도, 키워드도 나오지 않았다면, 이건 다중 비교문으로 사용하는 것이다.
        * 다중비교문이란 주어진 lhs가 `,`로 구분되는 여러개의 rhs 중 하나라도 일치되면(`==`) true로 판단하는 비교문이다.
        * 결과적으로 사용자는 matcher를 `1, 3, 4` 처럼 쓸 수 있게 된다.
        * 이 다중비교문은 matcher에서만 사용한다.
* itExpr은 기본 한줄짜리(inline) 일반 구문이어야 한다. 단, 아주 예외적으로 할당정의문도 올 수 있다.
* 패턴매칭은 대상expr의 최종 eval 값에 아무런 영향을 주지 못한다. 즉 대상expr은 대상expr만으로 평가된다.
    * 예를보자.
        ```go
        me.status // 1
            CREATED, PROGRESS: 33 // 2
        ```
    * 2에서 `33` 값으로 eval된다.
    * 그렇다고 해서 1를 실행한 결과가 integer 33이 되지는 않는다. 1의 결과는 그져, `me.status` 일 뿐이다.
    ```go
    b := if val: 33 else 55 // 1
    ```
    * 이건 a와는 다른 결과다. b는 `if` expr이 eval된 값으로 할당된다.
* 다음과 같이 사용한다.
```go
/*
<inline-expr>
    ( <inline-expr>[, <inline-expr>]* <indentBlock>
    | <is> <name> <indentBlock>
    | <in> <inline-expr> <indentBlock>
    | `>` <inline-expr> <indentBlock>
    | `==` <inline-expr> <indentBlock>
    | `<` <inline-expr> <indentBlock>
    | .....
    | <else> <indentBlock>)+
*/
a := me.status // defAssign 최종 결과값인 `me.status`를 실행한 결과를 놓고 패턴매칭을 돌린다.
    CREATED, PROGRESS, defaultStatus() // `,` 로 연결도 가능하다. `a == CREATED || a == PROGRESS || a == defaultStatus()` 라는 뜻이된다.
        doSomething()
    DONE: ...
    is int: ...
    in ABANDON..lastState(): ...
    else: ....
```
* Scalar나 Elixer, Ruby 수준의 고수준 Pattern Matching은 지원하지 않는다.
* 대상값은 반드시 1줄짜리 표현식이어야 한다. 블록문에 대해서 다시 패턴매칭을 사용할 수는 없다.
* 정의문에는 사용이 일반적으로 불가능하다. 하지만 예외로 할당정의문만은 정의문이지만 패턴매칭의 대상값으로 사용이 가능하다.
* 모든 sub keyword들은 blockExpr을 실행하기 전에 `it` 이라는 변수에 대상값을 할당한 상태로 만들어 놓는다.
* failthrough를 지원하지 않는다.
* 대상값이나 `is`, `in`에는 한줄짜리(inline) expr만 올 수 있다.
* `<targetExpr>[, <targetExpr>]* blockExpr`
    * targetExpr은 값이다. itExpr값과 일치되면 해당 값일때 뒤에 나올 블록문을 수행한다.
    * targetExpr 뒤에 , 를 붙여서 다른 값을 또 넣을 수 있다.
* `is <type> blockExpr` 뒤에 타입과 일치하면 blockExpr을 수행한다.
* `in <expr> blockExpr` expr은 container로 eval되어야 한다.
    * container.has(value)를 해서 true가 반환되면 blockExpr을 수행한다.
    * expr 뒤에 , 를 붙여서 다른 값을 또 넣을 수 있다.
* `else blockExpr` 나머지 모든 패턴이 들어맞지 않음면 이게 실행된다.
## sub keyword는 별도의 block문을 생성한 것이다.
* `is`, `on`, `in` 에 매치되어서 blockExpr을 돌린다고 했을때 해당 blockExpr문에서 생성한 local변수는 blockExpr이 종료됨과 동시에 사라진다.
* * *
# err
## err의 정의
* err는 특수 객체다.
* err에서 expand를 했다면, 그 객체는 err다.
* err는 `?` 로 표현된다.
    * 즉 `?`는 null이 아니라 err에 대한 합타입을 의미한다.
## err는  합타입이다.
* 타입 뒤에 `?` 가 붙으면 해당 타입은 main 타입과 err 타입의 합타입이라는 뜻이다.
    * 합타입에 포함될 특정 err 타입을 지정할 수는 없다. 예를들어 `int?socketErr` 처럼은 불가능하다.
    * `int?` 는 integer 값도 포함할 수 있지만, 어떠한 종류의 err 도 포함될 수 있다.
* `?` 가 붙은 타입은 safe-navigation에 사용될 수 있다.
* `?` 를 붙여서 변수를 정의할 수도 있다.
## 나중에는 err에 대한 SmartCast를 지원한다.
* 물론 쉽지는 않을 것이다. 하지만 구현 난이도를 제외하고 순수하게 내가 만든 문법과 harmony를 이루는, 개념적으로만 이 문제를 바라본다면, 이 기능은 반드시 들어가야만 한다.
* 일단 저 수준 (앞에서 `if res is err` 해두면 합타입이 아니라 순수타입으로 타입 추론) 으로 구현하는 것을 1차 목표로 한다.
* if 외에 다른 케이스들은 뭐가 있는지 철저하게 다른 언어들을 벤치마킹 해보자.
## err의 반환
* 함수는 err을 반환하거나 main 값을 반환해야 한다.
    * 2가지를 동시에 하거나, 둘 다 안하는 것은 안된다.
* main 타입으로 err의 일종을 지정할 수는 없다.
    * 예를들면 `foo() socketErr` 은 불가능 하다.
    * 꼭 에러를 반환하고 싶다면 `void?` 로 반환형을 지정하자.
    * verifier가 이런 종류의 에러를 처리한다.
## Safe navigation을 지원한다.
* `A?.B` 은 A가 err 이 아닐 경우에 B에 접근하거나 B를 실행한다.
* 함수 인자에 `?`가 쓰일 경우에는 해당 인자가 `err`가 아닐 경우에만 호출이 된다. 이 함수호출의 eval은 `함수의 eval + err` 이 된다.
    * 함수 호출 밖에서는 사용할 수 없다. 문법이 모호해지거나 eval타입이 errorable type인지 아닌지 한번에 파악이 안된다.
    * 예를들어 with에 `?` 를 붙일 수 있도록 해버리면 `def` 안에서 with를 사용할때도 `?` 를 붙이려는 사람이 나타날 것이다.
    * 그런데 def 안의 with를 해버렸을때의 verifier은 꽤 어려운 문제다.
* 함수호출 인자에 `?` 가 사용되었고, 그 `?`가 붙은 인자가 실제로 err가 반환되었을 경우, 그 즉시 함수호출의 최종 eval 값은 err가 된다.
    * 뒤에 다른 인자들에 대해 평가하지 않는다.
    ```go
    foo(n int, f flt, b bool) str
    foo(boo()?, koo(), hoo()?)
        // 이 함수호출의 eval Type은 str? 다.
        // boo()에서 err가 반환되었을 경우 foo()는 그 즉시 err를 반환한다. koo(), hoo()는 실행되지 않는다.
    ```
* 함수호출(`runExpr`) 인자에 `?` 가 사용되었을 경우, 해당 함수호출의 getEval()은 함수반환형 + ? 타입이 된다.
## 에러의 처리는 지연처리와 패턴매칭으로 한다.
* 지연처리는 합타입인 채로 들고 가다가 그걸 반환하거나, safe-navigation을 사용하거나 나중에 처리하는 방법이다.
## exception
* 함수의 반환형에 `?`로 err가 발생한다고 명시하지 않아도 err를 생성해서 반환할 수 있다. 이러한 에러를 exception이라고 한다.
    * 구현상으로 exception은 err와 완전히 별도의 클래스다. 이걸로 각 AST의 구성요소는 현재 exception인지 그냥 err인지를 파악한다.
    * ex는 panic으로 이어지는 런타임 에러를 표현한다.
* ex는 `case` 를 통해서만 처리될 수 있다.
* 처리되지 않은 ex는 최종적으로 panic 상태를 일으킨다.
    * panic은 콜스택, panic 원인을 찍고 프로그램을 종료한다.
    * panic은 절대로 복구될 수 없다.
* ex는 `ret`으로 바로 호출자 함수로 올릴 수는 있다.
* ex는 다른 변수에 담을 수 없다. 지연처리 할 수 없기 때문이다.
* ex를 담는 변수를 만들 수도 없다.
* ex가 defAssign의 초기식 도중에 발생했다면, 그리고 `case`로 ex를 처리하려고 했다면 각 ex를 처리하는 모든 `case` 에 그 non err 타입에 적절한 값을 반드시 넣어줘야한다.
```go
foo(b B, n int) int: ...
getElem(n int) int
    arr[n] // n이 -1이면 outOfBoundErr가 발생한다.

a := foo(b?, boo()) // a int? 타입. b가 err일 경우 그 즉시 동작을 skip한다. 따라서 boo()는 호출되지 않는다.
val := getElem(-1) // val은 int 타입. 하지만 runtimeErr가 발생하면 F/C가 발생한다.

val2 := getElem(-1)
    $getElem is outOfBoundErr: ....
```
### ex 마스킹
* expr에서 일부 term(lhs, rhs) 에서 ex가 발생했다면 해당 expr은 동작을 즉시 중단하고 그 ex를 바로 run()함수에서 반환한다.
### 알고리즘
* runExpr을 돌리면 실행된 func은 ex를 반환할 수 있다.
* `when` 은 targetExpr로부터 반환된 ex와 매칭되는 `case`를 발견한 경우 해당 case를 실행한 결과를 반환한다. 이 과정에서 ex는 사라진다.
* err 마스킹을 구현하기 위해서 모든 expr은 자신들의 term(lhs, rhs, params)에 ex가 반환되었다면, expr의 동작을 즉시 중단하고 run()의 반환값으로 그 ex를 반환한다.
* assignExpr, defAssignExpr만 예외로 ex가 rhs에 발견되면, 그 ex값을 lhs에 넣는다. 그리고 물론 ex를 run()에 반환한다.
* blockExpr은 만약 stmt를 하나 run()하고 반환되는 값이 ex일 경우 panic 상태로 진입한다.
    * panic상태로 들어가면 에러를 수집하거나 콜스택을 수집하거나 한다.
* 다음 예시는 위 내용을 정리한 것이다.
```go
foo(n int, f flt, b bool) str
koo() flt
a := foo(boo(), koo(), hoo())
// 만약 koo()에서 ex가 반환되었다면 runtime err다.
// foo() 함수호출인 runExpr은 term 중 하나인 koo()에서 ex가 나왔으므로 runExpr는 자기 자신의 run() 값을 ex로 바로 내보낸다.
// defAssignExpr("a", runExpr()) 은 runExpr의 run() 결과 runtimeErr가 나왔지만, assign계열의 expr은 예외적으로 rhs에 ex가 왔을때는 동작을 계속 진행한다. 그 결과 a에는 ex가 할당 된다.
// blockExpr은 defAssignExpr의 실행결과 ex가 반환된 것을 보고는 panic에 들어간다.
// whenExpr이 defAssignExpr을 감싸고 있었다면 상황이 또 달랐을 것이다.
```
## Err 관리 메커니즘
* 메커니즘을 설계할때의 포인트는 다음과 같다.
    * err객체냐 exception이냐 하는 것은 class로 구분하는 게 아니라 예상치 못하게 생성되었는가? 여부로 결정된다.
    * 참고로 native 에서 발생한 에러는 대게 exception이다.
    * verbose 모드를 키게 되면 에러의 상세한 정보가 출력되어야 한다. 그리고 에러가 생성된 순간에 정보를 출력하기도 해야 한다.
    * 이 사실로부터 알 수 있는 것은, err 클래스 자체가 자기자신이 `지금 외부 환경을 보니까, 내가 상세한 정보를 찍어야 하나 본데?`
    * 라고 판단하는 것 보다는, err클래스를 담는 관리자 클래스가 이를 판단하는 게 훨씬 관리가 유리하다.
    * 이 관리자 클래스는 errReport 클래스이며, 이 클래스를 `verbose`가 적용된 클래스로 교체만 해주면 나머지 코드는 수정할 필요가 없게 된다.
    * Exception은 handling되지 않으면 즉각적으로 F/C가 되어야 한다. 이를 감지하는 것은 매 line by line으로 실행하는 role을 가진 blockExpr이다.
    * exception으로 판단된 err객체는 callTrace 정보를 생성해두고 있어야 한다. 이 역시 위의 컨셉대로 유도하면,
    * err객체 자신이 callTrace를 생성해두는 것이 아니라, errReport가 필요하다고 판단되면 err객체의 callTrace를 생성해두도록 한다.
    * 이는 err인지 exception인지 구분하는 코드를 없앨 뿐 아니라, 필요한 시점에서만 callTrace 정보를 생성하도록 한다.
    * native 코드에서 에러가 발생한 경우, 로그를 찍을 것인지 에러객체를 만들것인지를 선택할 수 있어야 한다.
## Err 관리 수정을 위한 TODOLIST
* 위에서 부터 아래로 독립적인 Task 순으로 작성한다.
* 반환형으로 에러를 내보내지 않고도 에러가 발생한 시점이나 발생한 객체들을 전달 할 수 있어야 한다.
### logger
* [o] logger의 filters는 dumpFormat에 쓰이지말고, dump()에 넣도록 하자.
### thread
* thread는 하나의 실행Unit에 전역적으로 저장되는 객체를 갖는다.
### frames, errReport, slots
* [o] thread 는 thread** _get로 관리하는 대신, _set()을 만들자. 더 직관적이다.
* [o] thread는 _get을 삭제한다.
* [o] thread에 errReport를 set,get하도록 추가한다.
* NAMU_E는 기존대로 로그만 찍는 매크로다.
### verboseGraphVisitor
* [o] verboseGraphVisitor를 만든다. 이 graphVisitor만 돌려도 알고싶은 모든 정보가 다 기록되도록.
* [o] 색깔이 왜 안보일까? 색깔을 넣으면 더 좋겠다.
## src
* [o] srcFile 이라는 클래스는 nm, cpp source code 파일에 대한 정보를 갖는다.
* source code가 managed라면 아쉽게도 filename만 가지고 있을 것이다.
* fileName, path, file의 내용 등이 포함될 수 있다.
* 나중에 C-REPL을 구현할때 이 srcFile을 compare해서 차이가 나는 부분만을 catch 하는 식으로 갈 것이다.
* [o] src 객체는 srcFile, name, pos 가 들어가게 된다.
    * 그리고 아마도 src객체는 srcFile에 등록을 시켜놓아야, srcFile은, 자신이 변동했을때 자신에게 의존하는 orgNode들에게
      전파할 수 있게 되 것이다.
    * C-REPL을 구현할때 이 아이디어가 맞는지 다시 생각해보자. 지금은 등록할 필요가 없다.
* [o] node::getSrc() 를 추가한다.
* [o] 파서는 이렇게 모든 func과 obj를 만들때 src객체를 새롭게 만들어서 넣어둬야 한다.
* [o] expr 같이 이름이 없는 것들은 그냥 ""로 넣어둔다.

### dumpable
* [o] dumpable를 만든다. thread, starter, err, errReport에 넣는다.
* [o] 용어의 정리: dump는 크래시 등으로 객체정보를 다 쏟아내는 걸 말하자. log는 한줄을 그냥 찍는 거다.
    * log를 사용하는 클래스에는 logger, err 가 있다.

### err
* [o] logger::callstack 기능은 제거하자. 사용자는 platformAPI를 바로 쓰도록 하자.
* [o] err는 일부 메소드를 Managed에 올려야 한다.
    * err : public baseObj 로 한다.
    * cast, as, is는 별도로 지원할 필요 없다.
* [o] err는 생성자에서 `err를 야기시킨 node` 를 하나 인자로 받는다.
* err는 frame정보를 갖지 않는다. 어느 파일에서 에러가 발생했는지만 기록한다.
* [o] err가 제공하는 기능은 다음과 같다.
    1. [o] initCallTraces(): thread::frames로부터 callTrace를 생성해서 vector<callTrace>로 가지고 있는다.
        * [o] 이 부분은 1번만 호출하면 된다.
        * [o] errReport.addEx()에서 호출해준다.
        * [o] verboseErrReport일 경우에는 add() 처럼 일반 에러로써 들어갈 때도 makeCallTraces()를 호출한다.
    2. [o] platformAPI::callstack 기능을 이용해서 vector<string> 으로 한 걸 한줄한줄 "\tat %s" 로 format 변환한걸 nStr에 담아서
       가지고 있는다.
    3. [o] callTrace와 callstack의 결과는 값으로 가지면안된다. err객체 복제될때마다 deepcopy 되니까. tstr<>로 가지도록 하자.
    4. [o] err::dump()는 다음과 같이 보여줘야 한다.
        * 예:
        ```text
            err23(NO_PROS_ERR): no pros error found!
                at foo(a int, b flt) in someFile.nm:23
                at getMnagd(flt, str) in blablaFile.nm:22
                --------------------------------
                at obj::run
                at getExpr::getEval
                at starter::run
        ```
    5. [o] err::log() 는 다음과 같이 보여줘야 한다.
        ```text
            err23(NO_PROS_ERR): no pros error found!
        ```
    6. [o] 색깔이 적용이 되어야 한다.

* [o] callTraces는 objName, funcName, srcPos 를 갖는다.
* [o] 이제, err는 managed calltrace를 쉽게 만들 수 있게 되었다.
    1. frames를 순회하면서,
        2. frame객체.func.getSrc()를 가져온다. 여기서 name, pos, srcFile.fileName 을 가져오면,
        ```text
            at <funcName>(argStr) in <fileName>:<row>
        ```
        을 출력하 수 있게 된다.

## errReport
* [o] errReport는 dump()가 호출되면 각 err에 대해서 err::log()를 호출하도록 한다, add(err)시에는 err::log()를 호출시킨다.
* [o] verboseErrReport도 add로 에러나 Exception이 들어오면 verboseGraphVisitor로 찍어버린다.
* [o] verboseErrReport는 dump()시, add()시 err::dump()를 호출한다. callstack도 매번 나오게 된다.
* [o] cpp에서 발생하는 에러에는 크게 2종류가 있다.
    1. [o] 로깅만 하고 별도의 기본 값으로 대체가 가능한 것이다.
           이 경우는 NAMU_W 나 NAMU_E로 로그만 찍는다.
    2. [o] 익셉션으로써 대처를 해야 하는 것이다.
        * [o] 이 경우는 thread::getReport()에 err객체를 생성해서 추가한다.
        * cpp에서 만드는 err객체는 거의 다가 err가 아니라 예상치 못하게 발생한 exception이다.
        * 다만, 중요한 것은 "exception 발생 ---> F/C " 가 아니다. 바깥 코드에서 `on` 을 통해서 처리를 하고 있을 수도 있다.
        * [o] errReport는 add()는 에러용, addEx()는 exception으로써 넣는 API 2개를 제공한다.
            * 하나로 하지 못하는 이유는, exception 이란느 클래스를 만들지 못하기 때문이다.
            * NAMU에서는 동일한 의미를 뜻하는 err일지라도 예상치 못하게 출현하면 이것은 exception이 된다.
            * 관리를 위해, 그리고 빠른 exception이 발생했는지 여부를 매번 물어야 하는데 이 속도를 높이기 위해서
              별도의 함수, 별도의 container를 준비하는 것이다.
        * [o] exception을 담을 변수는 container여야 한다.
            * exception이 발생하고 --> 이를 위한 `on` block에서 다시 exception이 발생한느 상황도 있을 수 있기 때문이다.
        * [o] 그리고 blockExpr은 한줄을 끝낸 뒤에 thread::errReport에게 'exception이 있었나요?' 를 질의하면 된다.
        * `on`의 문법을 떠올려 보면 알겠지만, 익셉션이 발생할 수있는 `targetExpr`이 on expr 안에 들어가 있는 형태다.
        * 그러니 한줄을 실행했는데 exception이 생겼다는 것은 이 exception은 이 함수에서는 앞으로도 처리가 불가능한 exception이라는 얘기다.
        * namu 언어에서는 exception을 발생시킬 수도 있고, 밖에서 이걸 처리할 수도 있지만,
          exception을 변수에 할당시켜서 exception을 줄인다거나, 다시 함수밖으로 rethrow를 한다거나가 불가능하다.
        * [o] 따라서 blockExpr은 처음 run() 시점에서 errReport의 exception count를 센다.
        * [o] 그리고 한줄을 run 한 후의 errReport의 exception count가 변동이 생겼다면, 바로 모든걸 중단시켜버리면 된다.
            * 이렇게 하면 exception이 발생 ---> 발생한 것을 `on` blockExpr를 통해 처리 ---> 처리 중에 다시 exception 발생 ---> 이번엔 실패
            ---> 프로그램 바로 중단, 중단한 exception 2개를 모두 출력
            * 위와 같은 것도 가능해진다.
        * [o] 만약 `is ...err` 와 같이 exception에 대한 handling을 적어놓은 게 있다면, 이 blockExpr이 끝나고 나서,
              errReport에 등록된 가장 과거의 exception count를 하나 줄인다.
        * [o] starter는 중단 된 이유가 exception 때문이었다면 exception을 모두 dump() 시켜버리면 된다.

### starter
* [o] starter는 main함수를 시작하기 전에 thread::setReport(errReport()) 로 항상 교체해둔다. verbose가 아니다.
* [o] starter는 signal을 등록한다. SEGfault 가 발생하면 그 signal로 에러객체를 만든다. --> 에러객체.dump()를 한다.
* [o] F/C가 발생하게 되면 실행중인 모든 걸 verboseGraphVisitor로 다 찍어버린다.
    * c++에서 signal() 을 사용하면 죽기 직전에 뭔가를 하는게 가능하다.
    * 단, SIGINT는 windows에서 호출하면 안되는 등, 예외처리가 필요하다.
* [o] starter는 등록한 signal이 handling 되어서 프로그램을 종료해야 한다고 판단된 경우, 거기서 지역객체로 err를 하나 만들고,
      그 err::dump()를 호출한다.

## macro
* [o] NAMU_EX 는 익셉션을 뜻한다. 에러 객체를 생성해서 thread::errReport에 넣어둔다.
    * 넣는 과정에서 thread::errReport가 verboseErrReport였다면 넣는 순간 에러를 찍는다.

## interpreter
* [o] interpreter는 다음과 같이 로그Level을 관리한다.
    * 1. quite(기본값): logger off, errReport객체 사용
    * 2. verbose: logger on, verboseErrReport(err객체가 add되는 순간, 상세 정보 출력) 객체 사용
    * verboseFlag는 interpreter.setVerbose(true)만 하면 된다.
        * 위의 1, 2번의 동작은 모두 interpreter 안쪽에서 처리된다.
    * test/main.cc 에도 verbose가 이미 있다.
        * `verbose` 라고 입력을 해야 namuSyntaxTest::setVerbose(true)를 해둔다.
        * namuSyntaxTest는 interpreter를 내부에서 생성할때 interpreter::setVerbose(_isVerbose) 를 한다.
* [o] interpreter의 log structure, log level을 따로 관리하지 말고, 그냥 verbose면 다 찍는 걸로 하자.
* * *
# 함수
## 람다 문법
* tuple과 body 가 같이 있을때 이걸 함수라고 한다.
* 람다는 일반 적인 함수 정의 문법에서 이 정체성에 해당하는 요소만 남긴채 나머지를 생략한 구조를 띄고 있다.
* 문법은,
```go
/* `(` <name | name name>[, <name | name name>]* `)` [name]* indentBlock */
foo(23, (x, y) void: x + y; doSomething(), 33)
foo(23, (x, y): x + y; doSomething(), 33)
foo(23, (x int, y int) void
    x + y
    doSomething()
, 33)
```
* 람다는 함수 호출 안에서만 사용한다.
* 람다는 paramlist에서 타입을 생략할 수 있다. 이 생략은 일반적인 함수를 작성할때의 생략과는 다른 것이다.
    * 일반적인 함수에서 생략을 하게 되면, prop에 있는 값에 assign 하라는 뜻이 담겨있다.
    * 일반적인 함수는 타입을 생략할 수 없다.
* 여담으로, `:`과 소괄호 `)` 가 같이 쓰였을때, 람다의 body가 어디까지인지를 파싱하는 게 상당히 에러가 잘 나기 쉬운, 고생좀 할 것 같다는 생각이 든다.
### 알고리즘
* 아직 정하지 않았다.
## 함수 정의시 반환형을 생략할 수 없다.
* 반환형에 대한 타입추론도 지원하지 않는다.
* 이걸 지원하게 되는 순간 종속성 해결에 굉장히 많은 퍼포먼스가 요구되기 때문에, 그정도로 유용한 기능은 아니고, 사용자도 독해에 오해의 소지가 있다고 판단해서, 기능을 제외한다.
## overloading을 지원한다
* 구현은 문제없을테고. overloading을 구현할때 아래의 내용들을 잘 잊지말고 구현하도록 하자.
* [ ] 체크리스트1: node.subAll(string& name)이 효율이 떨어진다.
    * 자주 사용하는 건데, name일때는 multimap의 전용함수를 쓰면 더 빠르거든.
* [ ] 체크리스트2: sub duplication verification할때 `무조건 2개 이상의 같은 key가 있는가`로 하고 있다. 이러면 안된다. msg를 던지고, 해당 msg를 처리할 수 있는 무엇인가 2개인가를 봐야한다.
## 함수접근을 표현할때는 `A.B(C D)` 를 사용한다.
* 여기서 `C` 는 실제로는 사용되지 않는 값이다. 단순히 파서가 파싱할때 함수호출인지 함수 접근인지를 구분하려고 넣은 것이다.
* 만약 함수와 같은 이름을 가진 다른 함수나prop이 없을 경우 이름으로만 접근이 가능하다.
* 이름이 있어서 모호할 경우는 `A.B(C D)` 처럼 param name, type을 모두 적어서 표현한다.
    * 이름만 적어서 모호한 경우는, 해당 expr이 할당될 변수나 param의 타입을 모두 고려해서 모호함을 최대한 없애서 검증해야 한다.
    * 모호함이 해결되면 verifier는 추론한 정보를 AST에 채운다.
    * 추론을 시도해봐도 모호하면, 에러로 판단한다.
* param이 없는 함수라면 `A.B(void)` 로 표현한다.
```go
def a
    foo(name str) void
    foo(age int) void
    foo() void
a.foo // err. 모호하다.
a.foo(x str) // ok. `x`는 실제로는 어디에서도 사용하지 않는다.
a.foo(void) // ok? 이렇게 쓰면 예외로 `foo()` 를 찾으라는 명시적인 구문이 된다.
onFoo() void // body가 없고 반환형이 있다. 즉, 이건 decl이다. 메소드 호출이 아니라.
init(fooer onFoo) void
    fooer()
init(boo(5).foo) // 1: init의 param은 onFoo다 --> onFoo는 param이 없어야 한다. --> 아 그럼 a.foo중에서 foo() void가 되어야 하겠다.
// 타입추론이 이렇게 되어야 한다.
```
## 간편할당문법
* 간편 초기화 문법은 me의 prop에 assign 하는 것을 생략해주는 문법이다.
* 함수를 정의할때 다음과 같이 사용한다.
```go
def a
    age int
    grade flt
    init(age, grade, transparent bool) void // 1
        // 아무런 코드도 없지만 실제로는,
        // age = newAge
        // transparent = newTransparent
        // 가 각각 들어있는 것이다.
        ret
```
* 사용법은, 1번 라인처럼 함수의 parameter를 정의할 때 `A B` 같은 defVarExpr 가 아니라 `A` 같은 getExpr 로 정의하는 것이다.
* 여기서 단축된 age, grade는 반드시 객체 `a`에 속한 것이어야 한다.
* prop의 set/get을 제외한, 생성자를 비롯해서 모든 종류의 함수에 다 사용될 수 있다.
    * prop의 set/get은 어짜피 함수의 형태의 문법이 아니기 때문에 자연스럽게 문제에서 배제될 것이다.
### 알고리즘
* verifier에서 defVarExpr인지, getExpr인지, 2가지 패턴 중 하나가 아닐 경우에만 에러로 탐지한다.
* 알고리즘은 syntactic sugar로 한다.
    1. parser는 onFunc() 에서 func의 param을 정해야한다.
    2. parser는 onFunc에서 args는 arr& 로써 받아들인다. 일반적으로 `age int` 라고 하면, `arr[0] = defVarExpr("age", getExpr("int"))` 와 같은 식으로 들어가게 된다.
    3. parser는 list에 defVarExpr가 아니라 getExpr로 들어있을 경우, `a int` 가 아니라 `a` 라고만 적은 것이다. 이 경우 2가지를 처리해준다. parser가.
          1. `param.add(param("$unamed_param_1", getExpr("a"')))` 추가한다.
          2. 생성자 앞부분에 `assignExpr("a", "$unamed_param_1")` 을 추가한다.
      5. preEval단계에서 모든 함수의 params는 orgObj로 교체된다.
          1. 위 예시를 바탕으로 예를 들면 `getExpr("a")` 을 본 preEvaluator는 me로 stackFrame을 교체한 상태에서 getExpr을 eval한다. 결과 나온 orgObj안의 'a' 로 param과 str의 `type`을 교체한다.
          2. 이때 me의 objScope이 아니라 pack에서 접근한 경우 에러를 내야한다.
* prop이 nullable이어도 이 기능은 정상동작한다.
* verifier는 이 간편할당문법을 발견하면 getExpr()로 나온 prop이 반드시 me의 objScope에 속해있는지 여부를 확인해야한다.
* 그래서 getExpr::getEval()을 해서 결과로 null인가 아닌가만 체크하면 절대로 안된다. 적어도 pack은 제거한 상태에서 질의하자.
## 특수함수
* 객체에 이름이 이미 정해져 있는 함수들이다. 사용자가 다른 목적을 가지고 함부로 이름을 붙일 수 없는 함수명이다.
* interpreter에 의해서 자동으로 호출되는 것들이 여기에 해당한다.
    1. `ctor()` : 생성자를 의미한다.
    2. `end()` : `end` 키워드를 통해서 자동으로 호출되는 함수다.
## abstract func
* signature만 적고 함수 body를 생략한다.
```go
foo() void
foo(n int, a flt) A
```
## 새로운 `A::B()` 함수 호출 문법
### 컨셉
* 함수 호출시 args에 들어있는 getMe()로 frameInteract를 실시한다.
* 그러나 때로는 me는 currentFrame에 있는 me를 그대로 쓰면서 함수호출만 해야 하는 경우가 있다. 그럴때 사용한다. 다음과 같은 문법이다.
```bison
scopedRunExpr: <expr> `::` <funcCall>
```
* 보다시피 함수호출에 대해서만 지원하는 문법이다.
* 프로퍼티에 대해서는 단순히 `A.B` 를 사용하면 된다. 동일한 효과다.
* 이 문법은 `먼저 A의 orgType은 현재 me가 with 한 객체고, A에 있는 B를 호출할 건데, 이때의 frame의 me는 교체하지말고 지금 그대로 유지해줘` 라는 뜻이다.
* 즉 일반적으로 내 클래스에도 foo()가 있고 super클래스에도 foo()가 있는 상황에서 super의 foo()을 호출하고 싶은 특수한 상황에서 사용된다.
* super클래스에스 foo()가 있고, 내가 그걸 확장해서 안쪽에서 foo()를 하고 싶으면, 그냥 foo() 나 me.foo() 를 하면 된다.
* 게다가 만약 호출하고 싶은 함수가 같은 함수라면, 즉 오버라이딩 한 B::foo()에서 a::foo()를 호출하고 싶은 상황이라면, super() 를 사용하면 된다.
* 단, with한 객체 중에서 2개이상 동일한 함수&signature를 가진 함수가 있다면 동작하지 않는다. 직접 A::B()로 해야 한다.
* 이름만 같아도 안된다. 분명히 signature라고 했다. B::foo() 에서 A::foo(int) 를 호출 할 수 없다.
* 다음과 같이 사용한다.
```go
def A
    foo() void: koo()
    koo() void: print("no")
def B
    with a A
    koo() void: print("ok")
    foo() void: print("no")
    boo() void: a::foo()
// 아래처럼 했을때, ok가 나오도록 해야한다.
B().boo()
```
### 알고리즘
* scopedRunExpr이라고 따로 클래스를 만들지 않고 runExpr 클래스를 사용한다.
* runExpr은 node me, node subOwner, node sub, args 4개의 인자로 구성한다. 다음과 같은 뜻이다.
    * me: me.run(sub, args) 를 하게 될때 사용하는 `me` 다. 여기다가 run()을 때릴꺼다.
    * subOwner: subOwner를 지정하면, 여기서 sub, args를 통해서 subNode를 찾을 거다. 그리고 me.run() 하는 대신에 subNode.run()을 할꺼다.
* 일반적으로 subOwner는 null이다. 이 경우 runExpr::run()에서는 getMe().run(sub, args) 를 하고 끝낼꺼다.
* subOwner가 null이 아니라면, subOwner.sub(sub, args)로 subNode를 찾아서 subNode.run(args) 를 할꺼다.
* me 가 null이면 frame에서 가져오는 동작은 기존과 동일하다.
* * *
# Type
## Block문 타입추론
* 때때로 block문은 임의의 타입으로 사용되어야 할때가 있다.
    ```go
    a := if val // a는 int다.
        22
    else
        55
    ```
    * a는 if문으로 eval 되며, if문은 thenblock과 elseblock 각 블록문의 eval된 값의 공통 merged 된 타입으로 eval 된다.
    * 이처럼, 때로는 block문에게 어떠한 타입으로 eval 되는 것인지를 추론하거나 요구해야할 상황이 생긴다.
* block문의 type을 eval 컨셉은 이렇다.
    * block문 도중에 `return`이 나오면 해당 블록문은 아무런 타입을 eval하지 않는다. 타입이 `null` 인 셈이다. 일종의 추론 실패인 셈이다.
        * 참고로 verifier는 onVisit(returnExpr&) 에서 return이 함수의 반환형과 일치하는 지를 항상 확인한다.
            * 그러니 blockExpr의 타입을 추론하는 데 있어서는 무시해도 된다.
    * `void` 와는 다르다. `void`는 아무것도 아닌 타입으로 정상적으로 추론되었다는 뜻이다.
    * c++에서의 `_type == null` 과는 다른 의미다. 이 뜻은 아직 아무런 추론 시도를 하지 않았다는 의미이다.
* `if else` 처럼 여러개의 blockExpr을 들고있는 표현식은 여러개의 blockExpr의 도출된 type을 merge 한 것을 자신의 type으로 확정한다.
    * 만약 한쪽 blockExpr이 null을 반환하는 경우에는, 그 blockExpr을 merge 하지 않는다.
* `if` 나 `when` 에서 else를 넣지 않은 경우는 어떠한 타입으로도 확정되지 않는 경우가 존재한다는 것이다.
    * 이때의 `if`, `when`의 eval type은 null이 된다.
* `null` 로 eval된 타입을 다른 값에 할당하려고 시도하면 에러를 반환한다.
    * null로 eval 된 타입을 아무런 동작을 하지 않고 놔둔다면 물론 에러가 아니다.
### 알고리즘
* expr::getEval()이 null을 반환한다는 것은 `이 expr을 evalType을 무시해주세요` 라는 뜻이다.
* `blockExpr::getEval()` 이 호출되면 마지막 stmt의 getEval()만 수행한 결과를 그대로 내보낸다.
    * 함수는 반환형을 반드시 적어야 하도록 했기 때문에 이렇게 blockExpr은 마지막 stmt의 대한 getEval()만 하면 되는 것이다.
* `if else`, `when` 처럼 여러개의 blockExpr을 들고있는 표현식은 여러개의 blockExpr의 도출된 type을 merge 한 것을 자신의 type으로 확정한다.
    * 만약 한쪽 blockExpr이 null을 반환하는 경우에는, 그 blockExpr을 merge 하지 않는다.
* `if` 나 `when` 에서 else를 넣지 않은 경우, 그 expr의 getEval()은 null을 내보낸다.
* `null` 로 eval된 타입을 다른 값에 할당하려고 시도하거나 함수의 getEval()로 null이 반환되면 에러를 반환한다.
    * 다시말하면, 다른 상황에서 null이 반환된다면 그건 에러가 아니다.
* expr::getEval()은 궁극적으로 타입추론을 담당한다.
* 타입추론은 verifier에서도 사용되고 expand에서도 사용된다.
## 참조자 타입 검증
* prop이나 함수의 인자가 의미하는 타입에 올바른 객체가 들어가있는지를 verifier가 검증할 수 있어야 한다.
    * isA 관계도 고려해서 검증한다.
    * 여기서 isA는 `with로 expand 되었는가` 를 체크하는 것이다.
* 이를 구현하기 위해서 몇가지 요구사항이 있다.
    1. 기존대로 native type은 `ntype` 클래스가, managedType은 `orgObj` 가 처리한다.
    2. verifier는 `def myObj: a A` 처럼 defAssign 혹은 defVar시에 expr(여기서는 `a A`)의 orgObj(즉, `A`)를 proxyNode에 감싸서 넣어둔다.
        * 이제 `myObj["a"].isSub(a)` 로 is-A 관계를 바로 알 수 있다.
    3. parser가 orgObj를 만들때 mgdType도 하나 만들어서 orgObj에 넣어준다. 이렇게 하면 mgdType은 자동으로 unique 성을 띄게 된다.
        * orgObj가 unique하게 관리되기 때문이다.
* 즉 verifier가 생각하는 mgdType이란 결국 orgObj다.
* 같은 선상에서 생각해보면 verification 단계에서는 절대로 `null`을 참조자에 넣어두면 안된다.
    *  `def MyObj: a A := null` 의 코드가 있다면 expand 혹은 verification단계에서 `a`는 `null`이 아니라 orgObj인 `A`가 들어가 있어야 한다.
## Multiple Parents
* with에 의해서 사실상 다중상속이 가능하게 되었다.
* getSuper()는 getSupers()로 변경하고 여러개의 부모를 반환할 수 있어야 한다.
* getSupers()는 getAncestors() 로 변경해서 의미를 더 명확하게 표현하도록 한다.
### 알고리즘
* 자세하게 작성되지는 않았다. 막연히 될거라고 생각했던 건가? 확실히 알아낸 건 다음과 같다.
    1. getAncestors()에 들어가는 mgdType은 그때그때 객체를 만들어서 넣으면 안된다.
    2. 반드시 orgObj에 들어있는 그 unique한 애들을 넣어서 바인딩을 잘 해줘야 한다.
## managedType 접근 오류 수정
* 사용자가 작성한 객체에 접근하거나 param의 타입으로 놓거나 할때마다 `getExpr`을 사용해왔다.
* 그러나 getExpr::getEval()로 managedType인 orgObj를 추출하려고 하면 currentStackFrame에서 찾는다.
* 즉, currentStackFrame을 적절하게 교체해주지 않고 무작정 getOrgObj를 뽑아내려고 하면 어떨때는 전혀 다른 객체가 나오는 오류가 있었다.
* 이걸 해결하기 위해서 expand 단계에서 expander는 코드를 순회하면서 외부에 노출된 타입들을 전부 제대로 eval한 뒤에 orgObj를 교체해주는 작업을 수행해야한다.
    * 여기서 말하는 외부에 노출된 타입이란 다음과 같은 orgObj의 directSub들로, getExpr 혹은 Param 객체다.
        * prop의 타입
        * func의 반환형
        * func의 params
* 물론, parser는 이전처럼 getExpr로 채워서 보내주면 된다.
### 최적화 방안이 있어야 한다.
* 이 managedType orgObj로 기존의 getExpr들을 교체하는 작업은 퍼포먼스를 요구한다.
* 그리고 type을 추출해내는 이유는 오로지 verifier가 검증하기 위해서다.
    * 다시 말하면 검증과정이 끝나면 이 타입정보는 필요가 없다. tstr(mgdType, node)로 할 필요도 없다. 그냥 str(node) 로 해버려도 괜찮다.
    * 동작에는 아무런 영향을 끼치지 못한다.
* 그러니 pack 형태로 배포한 것을 사용할때는 이러한 type을 추출해서 교체하는 작업이 애초에 발생하지 않도록 최적화를 해줘야 한다.
* 일단 드는 생각은 **optimize 된 library나 pack을 만들때는 파일 맨 앞에 orgObj 트리 구조를, 이름만으로 구성해놓는 방법**이다. 이렇게 하면 먼저 orgObj를 다 만들고 알맹이를 채우면 되기 때문에, 파싱하면서 동시에 바인딩도 해버리면 된다.
* 바인딩이 이미 끝나있을 것이므로, verifier도 필요없고, 굳이 바인딩이 잘 되었는지를 체크할 필요도 없다. 바인딩을 체크할 필요가 없으므로 바인딩체크를 위한 managedType 추출 과정도 필요가 없게 되는 것이다.
* 나중에 최적화가 필요할때 더 구체적으로 생각을 해보도록 하자.
* * *
# Coding Style로 Attribute를 표현한다.
* 기호를 앞에 넣어서 attribute를 표현하는 경우, 실제로 해당 식별자에 접근할때는 해당 기호를 적지 않아도 좋다.
* 기호를 2개 이상 사용할때는, 기호 간의 우선순위는 없다.
## `_` 를 앞에 붙이면 protected로 인식한다.
* 이후, 해당 변수에 접근할때는 `_`를 적지 않아도 된다.
* 두번 연속해서 `__`를 적으면 맨 앞의 `_` 만 attribute로 인식한다. 따라서 식별자에 접근할대는 `_`를 한번 앞에 적어야 한다.
## 불완전객체는 대문자 CamelCase를 따른다.
* 기본 생성자가 없는 경우, 대문자 CamelCase로 org객체명을 작성해서 불완전객체임을 알리지 않으면 컴파일 에러다.
* c++ 코드도 클래스명은 대문자CamelCase로 작성한다.
## override는 앞에 `+` 를 붙여서 표기한다.
* prop/func 앞에 표기한다.
* override인데, override라고 명시하지 않으면 컴파일 에러다.
## 대문자로만 prop을 만들면 그것은 상수다.
* 상수는 primitive type만 가능하다.
    * 배열도 안되고, class도 안된다.
* 상수라고 할지라도 해당 변수가 참조하는 객체는 변경이 가능하다. 여기서 말하는 상수란 어디까지나 해당 참조자가 변경되지 않는다는 보장이다.
    * cpp로 치면 `const A&` 가 아니라 `A* const` 라는 뜻이다.
* 구별을 위해서, 사용자는 *1글자 대문자* 짜리 prop/variable을 만들 수 없다.
### 알고리즘
* verifier는 onVisit(assignExpr& e)에서 e.lhs 에 getExpr이 들어있고 getExpr 의 subName이 모두 대문자로 되어있고 2글자 이상이라면 이 getExpr은 상수에 접근하는 것이므로 에러다.
* * *
# pack
* 구현을 보면 pack을 대신해서 slot을 대대적으로 쓰고 있는데 slot은 node 상속받고 있다.
* slot은 obj \_pack이라는 걸 가지고 있고 이것을 proxy로 하고 있는데 그냥 pack을 obj에서 상속받으면 되는 걸로 보인다.
* 키워드 `pack` 보다 이전에 작성된 defStmt는 해당 파일 내에서만 유효한 것들이다. 이것을 fileScope이라고 한다.
* `pack` 이후로 작성된 것들은 pack orgObj에 들어가는, pack내에서 모두 공유되는 것들이다. packScope이라고 한다.
* packScope는 func이 붙어있는 것이다. obj에 붙어있는 것이 아니다.
* file scope이 pack scope보다 우선순위가 뒤다.
* filescope에 있는 것과 완전히 충돌하는 식별자가 pack scope에 존재한다면 경고를 내보내주는 것이 적절할 것이다.
## 알고리즘
* 다음의 순으로 frame이 구성되어야 한다.
    * \[localScopes\] --> objScope --> \[expandedObjScopes\] --> fileScope --> packScope
* parser는 소스코드 1개에 대해 File 객체를 만들어 놓는다.
* parser는 func을 make() 할때 어느 File 객체로부터 나온 것인지를 생성자에 참조를 넣어준다.
* File은 node의 일종이다. filename, srcLine 등 C-REPL를 위한 정보를 갖는 반면, subs()로 fileScope의 객체를 반환할 수 있다.
    * c++에서 가져온 bridgeCpp 같이 File은 존재할 수 있어도 subs()는 없는 것들이 있다.
    * 이때는 subs()로 dumChain()을 반환하면 된다.
* frame은 이제 setFile(), getFile()을 가진다.
* mgdFunc은 \_file.subs()를 해서 len()이 > 1이면  currentFrame.setFile()을 해서 objScope의 맨 뒤가 fileScope을 가리키도록 변경한다.
* * *
# 객체 정의

## 완전객체정의
### 컨셉
* 사용자는 객체명의 첫글자를 대문자/소문자 로 함으로써 (불)완전객체로 만들겠다는 의도를 보여준다.
* 객체의 코드가 그 의도와 부합하는 지를 verifier가 체크한다.
* 문법은 이렇다.
    ```go
    def <NAME> ( `(` (expr, expr...)* `)` )* <indentBlock>
    ```
* 객체명 뒤에 소괄호 부분을 CompleteCall, `완전생성호출` 이라고 한다.
* 완전생성호출이 없으면, 파서는 자동으로 기본생성자를 호출하는 것처럼 인식한다.
    * 생성자는 subs()에 바로 들어가므로, 파서는 `def` 가 끝난 시점에서 기본생성자가 있는지, 완전생성호출이 명시되어 있는지를 체크할 수 있다.
    * 기본생성자도 없으면 파서는 완전생성호출을 만들지 않는다.
    * 완전생성호출을 지정하지 않았고, 기본생성자는 있다면, 객체명을 소문자로 했을때만 완전생성호출을 자동으로 만들어준다.
    * def 작업이 끝나기 직전에, 완전생성호출과 객체명이 대문자로 시작했는지를 비교한다.
        * 완전생성호출이 있으면 반드시 소문자여야 한다.
        * 완전생성호출이 없으면 반드시 대문자여야 한다.
* 완전생성호출이 사용되는 것은 오직, run 단계에서 orgObj.setState(INITIALIZED) 로 이행된 순간이다.
## 객체 생성
### 컨셉
* 이전에는 prototype 기반 으로 동작했지만, 이제 더이상 그렇게 동작하지 않는다.
* 생성자 호출 != 객체 생성
* 생성자는 객체 생성을 담당하지 않는다. 객체의 초기화만 담당한다.
* 객체생성은 obj::run() 안에서 담당한다.
* obj::run() 을 호출하면 안에서 객체를 생성하고 적절한 생성자를 명시적으로 호출한다.
* obj는 기존대로 owns, shares, subs 3개를 갖는다.
* orgObj는 추가로 fixOwns를 갖는다.
    * 완전객체로 변하지 않은, proxyNode()가 들어가 있는 상태의 owns 배열이다.
* with에 대한 연결은 생성자(사용자의 생성자 혹은 @commonCtor) 안쪽에서 처리한다.
* withExpr, defAssignExpr 은 초기식에 해당하며, 다음처럼 한다.
    1. 초기식을 getEval() 한 orgObj을 proxyNode() 에 감싸서 owns에 넣어둔다. 이 값은 이제 verifier가 사용할 것이다.
    1. 초기식을 assignExpr 하는 코드를 @commonCtor에 담긴다. 생성자가 호출되면 기존의 proxyNode() 부분을 map.set() 을 통해서 교체할 것이다.
* proxyNode::clone()은 자신을 복제한 객체에 orgObj를 얇은 복사해서 넣는 구현이다.
* scope::clone()은 각 elem에 대해 clone() 을 해서 넣는 코드다.
* 초기식이 아닌 owns는 owns에 그대로 담긴다.
* tnmap은 set() 함수를 제공한다. 만약 set시도했으나 add를 한적이 없다면 에러를 반환한다.
* assignExpr은 iter를 가져와서 setVal()을 하지 않는다. 대신에 subs().set()을 수행한다.
* iter.setVal()을 필요가 없다면 제거한다. getVal()은 놔두고.
* obj::\_assign()은 현재 구현이 정말 맞는지 확인이 필요하다
    * [?] 각 elem에 대해 map.set()을 하는게 맞지 않나?
    * [?] rhs.owns.deepClone()을 하는게 정말 맞나?
    * [?] \_isComplete = true로 하는 게 맞나? 완전이냐 불완전이냐 하는 것은 callComplete()를 호출해서 생성된 객체인가 아닌가로 갈리는 것이다. 원본이 불완전인데 그걸 assign 한다고 해서 이게 완전이 되는게 아니다.
### 알고리즘
* obj::run() 안쪽에서 객체를 생성할때 다음처럼 한다.
        1. orgObj::clone()을 한다.
            1. ret.owns = \_fixOwns.clone()
            2. ret.shares = tnchain(shares)
            3. ret.subs = tchain(ret.owns)
            4. ret.subs.next = ret.shares
        2. 생성자를 호출한다.
            1. run()에 들어온 args에 적합한 ctor을 찾는다.
            2. ctor에 호출하기 직전에 commonCtor을 호출한다.
            3. ctor을 호출한다.
        3. 생성자 / commonCtor에서 `with` 를 통해 다시 다른 obj::run() 이 호출될 수 있다. 이 경우 위의 과정이 재귀적으로 진행된다.
* with 교체는 런타임에도 언제든 발생가능하다. 자세한 알고리즘은 문서 윗부분에 별도 항목으로 정리하였다.
## 생성자도 상속된다.
* 상속도 되고 override도 지원된다.
* 생성자는 그냥 함수다.
* 생성자는 파싱될때 반환형도 채워진다.
```go
def A
    ctor(): ...
    ctor(n bool): ..
    ctor(n flt): ...
def B
    with A
    ctor(n int): ...
    ctor(n bool): ..
fakeA A := B(2)
b := B(1)
// 일때,
c := fakeA(3) // err. verify 단계에서 a는 A타입. A에는 ctor(int)가 없다.
d := fakeA(true) // ok. A타입에 ctor(bool)이 있다. verifier는 d가 A라고 생각할것이다. run 단계에서는 a는 실제 B이므로 B(true)가 된다.
e := b(3.5) // ok. 생성자도 상속된다. verifier는 e가 A라고 생각할 것이다.
f := fakeA(3.5) // ok
```
* 만약 `A생성자` 를 했는데 B객체가 나오는 예상치 못한 결과가 껄끄럽다면 orgObj를 통해 생성자를 호출하도록 코드를 작성해라.
* 불완전 orgObj에서 호출하는 생성자는 상속되지 않는다.
    * 불완전 orgObj는 자신의 shares에서만 CTOR_NAME을 찾아서 canRun(), run()을 구현한다.
    * 완전 orgObj는 그냥 obj하고 똑같이 동작한다.
## 명시적 생성자 호출
* 생성자 안에서만 다른 생성자를 명시적으로 호출할 수 있다. `ctor` 로 생성자명을 붙여서 호출한다.
* 생성자 호출이 꼭 앞에 나올 필요는 없다. 중간에도, 끝에도 나와도 되고, 몇번이나 호출되어도 된다.
* 생성자 호출은 객체 생성이 아니다.
* obj::run()이 불리면,
    * obj 객체를 생성한다.
    * obj.@commonCtor을 부른다.
    * obj를 적합한 생성자에 호출한다.
## 불완전 객체의 정의
* `def <indentBlock>` 만 적고 이름은 적지 않은 경우는 완전객체로 취급한다.
    * completeCall은 orgObj() 와 같은 것이다. 빈 생성자를 호출하려고 한다.
    * 만약 빈 생성자가 없을 경우에는 이름을 적어야만 한다.
* 예시
    ```go
    def A // 대문자로 시작하므로 불완전객체다.
        age := foo() + 3
        ctx Context // 생성자에서 채워질 것이다.
        ctor(ctx): ret
        foo() A: ret me // 빈함수지?
        grade A
            get: ...
        with def// 완전객체다.
            help() void: ...
    A.age // ok
    A.ctx // err
    A.foo() // err
    A.grade // err
    A.static.help() // ok.
    ```
* 기본적으로 rtti는 불완전객체에 접근해서 사용하는 방식으로 제공하지 않는다.
    * 왜냐하면 get, set이 있는 프로퍼티 때문이다. 이러한 프로퍼티는 접근 자체가 get을 호출해버린다.
* 불완전객체는 애초에 사용이 가능한/호출이 가능한 요소만 접근을 허용한다. 그것들은 다음과 같다.
    1. 생성자 호출
    2. 중첩된 완전객체에 대한 public 한 접근
    3. 정의할당문을 사용해서 불완전객체에 대한 참조를 생성
        * 불완전객체에 대한 할당문을 사용할 수 없다.
            1. 문법이 더러워진다. 같은 타입이라도 불완전속성,완전속성을 표현할 수 있어야 한다. (함수반환형 등)
            2. 굳이 불완전객체까지 완전객체로 해야할 이유가 없다. `모든 불완전객체는 origin이다.` 라는 전제를 지키는 게 도움이 된다.
        * 불완전객체 대한 참조를 생성하려면 정의할당문을 통해서만 가능하다. 이때 프로퍼티명을 대문자로 시작해야 한다.
        * 이렇게 만들어진 프로퍼티는 사실상 불완전객체인 origin 객체 취급을 하면 된다.
        ```go
        Gson := com.google.test.Gson // 정의할당문일때만 rhs가 불완전일때만, 프로퍼티명을 대문자로 시작할 수 있다.
        ```
        * 불완전객체에 할당을 할 수 없다. origin객체를 재할당한다고? 어림도 없지.
        * 불완전객체를 함수밖으로 `ret` 할 수 없다.
            * 왜냐하면, 아까도 말했듯이 이게 가능하려면 타입이라는 걸 정의할때 불완전속성을 표현할 수 있어야 하기 때문이다.
            * 그래야 받는 쪽에서 A타입인데 불완전한A인지 완전한A인지 구분에서 verifier가 에러 체크를 할 수 있게 된다.
### 알고리즘
* defAssignExpr::run() 에서는 rhs에 있는 origin Obj를 참조채로 local/obj Scope에 넣는다. Easy!
* 각 orgObj는 자신이 originObj이고 불완전한지 아닌지를 표시해두고 있다.
* 그러니 불완전객체의 요소를 접근만했는가 사용도 했는가 그래서 에러인가 아닌가는
  orgObj와 runExpr 얘네들이 생각할 문제다.
* verifier 상에서의 defAssignExpr::run()에서는 verifier가 proxyNode()를 만든다고 했었다.
    * 이때 proxyNode를 만들지 않고 직접 orgObj를 scope에 넣으면 된다.
    * 즉, runtime일때와 동작이 거의 동일 할 것이다. Easy!
* `retExpr`은 반환할 rhs가 불완전객체라면 무조건 에러!
* 다음처럼 case by case로 대응한다.
* runExpr:
     1. args 중에 불완전객체가 있으면 안된다.
     2. meObj가 불완전이면 안된다.
* assignExpr: lhs, rhs 둘 중 하나라도 불완전이 있으면 안된다.
* returnExpr: rhs가 불완전이면 안된다.
* asExpr: lhs 안됨. rhs는 OK
* getExpr:
    1. meObj가 불완전객체다. currentFrame 이 아니여야 하고,
    2. meObj.sub(subName) 한 결과가 초기식이 없는 prop혹은 함수라면, 에러다.
        * 어떻게 초기식이 없는 prop인지 알 수있는가? ==> **아마도 위의 다른 기능들을 해결하다보면 orgObj에 이미 정보가 있을것으로 추측된다. 없다면? 만들자.**
### 다음의 코드로 검증하라
```go
def A
    age int
    say() void
        print("hello world!")
foo() A
    ret A // 여기!
main() void
    foo().say()
```
## run단계에서 완전객체는 최초 접근시 생성된다.
* 완전객체 생성이 의미하는 바는 orgObj.setState(INITIALIZED) 로 VERIFIED에서 상태가 이행되는 것이다.
* 이렇게 상태를 변경시키면, 알아서 orgObj.getCallComplete().run() 을 해서 자신에 대해 생성자를 실행시켜서 준비상태로 만든다.
* 완전객체는, verified가 되면 setState(VERIFIED)가 된다.
    * 앞서 말한대로, 이상태에 들어오면 fixOwn을 바탕으로 해서 생성자가 호출되기만 하면 완전객체가 될 수 있는 초기상태로 만들어 놓는다.
* run단계에 진입 후, 어디선가 orgObj에대해 as() 혹은 subs(), getOwns(), getShares(), clone() 중 함수 1개라도 건드리면 바로 INITIALIZED 상태로 이행한다.
    * 참고로 이 과정에서 초기식이 담겨있는 @commonCtor을 실행할 것이다.
    * 생성자 안의 일부 구문이 `a := a.B` 와 같이 다른 완전객체에 대해 접근하는 경우, 다시 재귀적으로 해당 객체는 INITIALIZED 상태로 이행되어야 한다.
## verifier는 함수를 호출코드를 보면, 그 함수를 directly 하게 owns/shares로 가진 obj가 isComplete 한지 체크해야 한다.
```go
def A // incomplete
    with def // complete
        foo(): ...
    boo(): ...
A.boo() // err.
A.foo() // ok.
```
### 알고리즘
**<<채우기>>**
* * *
# `aka` 는 없앤다. `A := B` 와 새롭게 바뀐 불완전객체에 대한 접근으로 해결한다.
* * *
# Abstract
* prop과 func, orgObj를 abstract 하다고 선언할 수 있다.
* 선언된 요소를 들고 있는 orgObj는 isAbstract()가 true로 된다.
* 그 orgObj를 확장한 orgObj는 abstract한 요소를 모두 채워넣지 않으면 여전히 isAbstract() = true 다.
* orgObj가 isAbstract() = true인 obj를 객체로 만들려고 시도하면 (verifier가 `A()`를 발견하면) 컴파일 에러다.
* abstract로 선언한 것도 일단 shares에 넣어둔다.
* 선언문법은 이미 각 항목에 서술하였다.
* abstract 요소를 run에서 호출하게 되면 런타임에러다.
## 몇개의 abstract을 해결했는가에 대한 알고리즘
* abstract 요소를 몇개 정의한 A가 있다. B는 A를 확장했다. 이때 B가 isAbstract()인지 아닌지 verifier의 계산 알고리즘을 서술한다.
### 핵심
* abstract 한 요소를 본 parser는 dummy로 만들어서 shares에 그대로 보관된다. 즉 verifier는 abstract한 요소들을 추적할 수 있다.
    * abstract prop은 값이 없는 prop이다. abstract 하지 않더라도 당연히 share로 가야 한다.
* orgObj.getAbstracts() 하면 getState()가 VERIFIED 이상일때, lazy하게 평가한다. 이 작업은 꽤 느린 작업일 것이다. 그래서 cache를 만든다.
* getAbstracts()는 abtract한 node들을 반환한다.
* obj는 shares, ows, subs를 가지고 있다. subs는 owns --> shares --> with한Subs.... 로 구성된다.
* 이 정보를 응용하면 subs에 어느 지점부터 with한 Subs 즉 super클래스들의 chain을 반환할 수 있다. 이 함수를 obj::getBaseSubs()라고 하자.
* abstract한 객체로부터 run() 즉, 객체생성을 시도하는 코드를 verifier가 보면 컴파일 에러를 내보낸다.
### 알고리즘
* obj::getAbstracts()
    * return orgObj.getAbstracts()
* orgObj::getAbstracts()
    * if(state < VERIFIED) return dummyArr;
    * if(\_abstracts != null) return \_abstracts;
    * \_abstracts := array()
    * \_abstracts.add(with한orgObj마다.getAbstracts()) // 없으면 add 안함.
    * 각 \_abstracts에 대해,
        * shares/owns에 똑같은 signature로 있으면, 그 node를 \_abstract에서 제거.
    * return \_abstracts
* * *
# import
* 별도의 manifest 파일에 어떠한 외부 pack을 사용한다는 상세한 내용을 적는다.
    * 이 내용에는 pack의 식별자, version, 작성자 등이 모두 포함된다. 그래서 pack 충돌을 최소화 한다.
* manifest에 의해서 pack이 import 되면 sandbox로 로드 된다. 전체 시스템에 영향을 주지 않는다.
    * 같은 pack인데 버전에 따라서 여러개를 들고 있을 수 있다.
    * namu 코드에서는 무슨 버전의 pack이라는 명시가 없어도 된다. manifest에만 적으면 된다.
* import는 해당 외부 pack의 subs들을 펼치는 역할도 같이 한다. 그 역할은 defAssign, with를 사용하면 모두 대체 가능하다.
* 기본 pack은 이미 load가 된 상태로 진행된다.
* 추가로 외부 pack을 가져오고 싶다면 manifest를 만드는 것 외에는 방법이 없다.
* [?] seedling에서 추가하게 될 버전명시 문법을 아예 namu로 가져온 뒤에 namu에서 어딘가에 manifest란 객체를 def 하도록 하는 방법도 있다.
      이 import 부분은 생각을 좀 더 해봐야 한다.
* * *
* import를 := 로 대신하는 것은 좀 귀찮은 일이다.
```go
import os.BigClassName // python.
BigClassName := os.BigClassName // namu.
```
* `with`는 `only` 라는 키워드를 제공하고, 일부 항목에 대해서만 delegation을 지원하는 기능이 있다.
* 이 기능을 응용하면 import를 거의 완벽하게 대체할 수 있다.
* 참고로 기존의scope을 재사용하는 기존 with와는 동작 방식이 아예 다르다. 새로 scope을 만들어 내는 것에 가깝다.
```go
with os only
    BigClassName
    AnotherBigClassName
// 다른 언어:
//  import os.BigClassName
//  import os.AnotherBigClassName
// namu 언어에서 :=로 대체:
//  BigClassName := os.BigClassName
//  AnotherBigClassName := os.AnotherBigClassName
// 과 같은 코드다.

// with only는 with가 사용되는 곳이면 다 사용할 수 있다.
// 다음과 같이도.
def A
    foo(n int) void
    foo(n int, x int) void
    boo() void
def B
    with a A only
        foo(n int) void
        boo() void
def c
    with @B
c.foo(1, 2) // 에러! 이런 함수는 c에 없다.
```
## 알고리즘 ==> **직접 delegation하는 scope을 만든다.**
* 먼저 namu 예제 코드를 짜보자.
    ```go
    def A
        koo() void
        boo() void
    def B
        with a A only
            koo() void

    A().koo() // 컴파일 에러!
    ```
* 이것을 cpp로 가구현하면 이렇게 된다.
    ```cpp
    // at loweventer.cpp --
    WithOnlyExpandExpr* me::onWithOnly(expr& it, blockExpr& blk) {
        // it 에 대해서는 기존의 with 와 동일하게 처리한다.
        // 여기서는 blk만 집중적으로 처리한다.
        return new WithOnlyExpr(blk) // blk에는 프로퍼티의 접근, 함수의 접근인, getExpr만 들어있어야 한다.
                                     // 다른게 들어있다면 조금 뒤에 에러를 내보낼 거다.
    }

    // run()을 하면 런타임에 scope에 있는 me obj안에다가 blockExpr의 delegation
    // 들을 만들어서 넣는다.
    // parser가 run을 한다. 그리고 이 항목은 날린다.
    class WithOnlyExpandExpr : public BlockExpr {
        str run(const args& a) override {
            // WithOnlyExpandErr이 run 안에서 동작한다는 것은,
            // 이 expand를 하려면 run이 되도록 stackFrame을 구축해놓으라는 얘기다.
            obj& me = currentFrame.getMe() // 그래서 현재 frame에서 me를 뽑아온다.
            realContainer = me.realContainer;
            for(s : getStmts()) {
                GetExpr& cast = s.cast<GetExpr>();
                if(nul(cast)) {
                    onErr("프로퍼티나 함수의 선언만 올 수 있습니다.");
                    continue;
                }
                _expand(realContainer, s));
            }
            return str();
        }
        void _expand(narr& scope, const GetExpr& e) {
            str expanded
            node* new1 = null;
            if (e.isForFunc()) {
                // 함수의 반환형을 알아내야 하는데, 이걸 알려면 함수를 get으로 직접 찾아야 한다.
                // getExpr의 현재 값을 참조하는 것만으로는 반환형 정보를 알 수 없다.
                func& f = e.run();
                // FuncDelegation은 주어진 함수를 바로 호출하는 간이 위임전용함수다.
                new1 = new FuncDelegation(f);
            } else {
                new1 = new Property(/*get:*/e, /*set:*/new AssignExpr(e, new GetExpr("rhs")));
            }
            scope.add(e.getSubName(), *new1);
        }
    }

    defBlock* me::onDefBlock(defBlock& s, node& candidate) {
        WithOnlyExpr& withOnly = candidate.cast<WithOnlyExpr>();
        if(!nul(withOnly)) {
            // 여기서, parser가 expand를 할 수 없다.
            // 보면 WithOnlyExpandExpr을 보면 알겠지만, withonly에 대해 expand를 하려면,
            // run()을 실행시켜야 한다. 즉, expander에서 돌려야 한다는 뜻.
            // 그러니 여기서는 expander가 나중에 catch할 수 있도록 `@expand`
            // 함숭에 넣어두면 된다.
            s.sub<MgdFunc>("@expand").getBlock().add(withOnly);
        }
    }
    ```
* 일단 scope과 하나의 함수/프로퍼티에 대해 각각 delegation하는 뭔가가 추가가 된다는 점은 단점이다.
    * 속도도 느려지고
    * 메모리도 더 먹을 것이다.
* 그러나 이렇게 scope을 한번 구축하고 나면 도중에 delegation 하는 프로퍼티에 다른 값이 들어가거나
* 2개 이상 withonly를 사용하는 복잡한 상황에서도
* 동일한 벙법으로 verification을 할 수 있고, 동일하게 run을 할 수 있게 되었다.
* 그리고 with only는 일부분을 참조하는 것이므로 새로 delegation하는 요소가 많지는 않을 거라는 생각도 있다.
# end 키워드
## 컨셉
* 다음의 문법을 가진다.
    ```go
    endExpr: `end` <indentBlock>
    ```
* end가 소속된 블록문이 종료되는 시점에 이 indentBlock이 수행된다.
* end구문까지 가야 end가 효과를 발휘한다. end까지 가기전에 return이 불려지면, end 구문을 만나지 못하면 의미가 없다.
* end를 2개 이상 만난 경우, 먼저 만난게 나중에 호출된다. 즉 STACK 구조다.
## 알고리즘
### 핵심
* 이제 blockExpr은 `vector<blockExpr> _ends` 를 갖는다.
* frame은 getCurrentBlockExpr()을 추가한다.
* `endExpr`은 run() 되면 frame.getCurrentBlockExpr().getEnds().pushFront(this->indentBlock)을 등록한다. STACK구조이니,  맨 앞에 들어가야 한다. pushFront.
* blockExpr은 자신의 run()을 종료하기 직전에 \_ends에 있는 것들을 popFront 해가면서 위에서부터 하나씩 run() 한다.
* * *
# Minor Changes ===========================================================
# #확장된_visitInfo
* visitInfo를 preMap으로 만든다.
* preMap은 내가 만든 용어인데, predefined 된 key/value에만 값이 할당되고, 나머지 pair에 대해서는 기본값이 나간다는 뜻이다.
* string이 일반적이니까 string으로 하면 되겠지.
## 알고리즘
1. visitInfo은 preMap 객체 1개를 가진다.
2. visitor는 preMap객체를 돌려 쓴다. 매번 만드는 게 아니라 부모로부터 받은 걸 그냥 나도 쓴다는 뜻.
3. 처음 visitor가 할당하는 preMap은 dummyPreMap이다. 모든 동작이 가능한 것처럼 외부에 보인다. get, set, add 등
    하지만 dummyMap인 만큼, 실제로 새로운 key를 만들거나 value를 메모리에 넣거나하는 동작은 일체 하지 않는다.
    그래서 아무리 add/set을 하더라도 get을 하면 기본값인 빈 string만 나온다.
4.  상위노드 (여기서는 obj) 에서 진짜 preMap을 하나 만들고, 하위노드로부터 어떠한 정보를 받고 싶은지를 key/value pair를 미리 활성화 해둔다.
5. 하위노드들은 자신들의 모든 행동들에 대해서 일일이 preMap에 남기려고 시도한다.
    그러나 preMap인 만큼, 상위노드가 미리 활성화 해두지 않은 정보일 경우에는 add/set이 아무런 동작을 하지 않으므로 퍼포먼스가 하락하지 않는다.
* * *
# 객체 생성 알고리즘
* 이전에는 prototype 기반 으로 동작했지만, 이제 더이상 그렇게 동작하지 않는다.
* 생성자 호출 != 객체 생성
* obj::run() 을 호출하면 안에서 객체를 생성하고 적절한 생성자를 명시적으로 호출한다.
* obj는 기존대로 owns, shares, subs 3개를 갖는다.
* orgObj는 추가로 fixOwns를 갖는다.
    * 완전객체로 변하지 않은, proxyNode()가 들어가 있는 상태의 owns 배열이다.
* with에 대한 연결은 생성자(사용자의 생성자 혹은 @commonCtor) 안쪽에서 처리한다.
* withExpr, defAssignExpr 은 초기식에 해당하며, 다음처럼 한다.
    1. 초기식을 getEval() 한 orgObj을 proxyNode() 에 감싸서 owns에 넣어둔다. 이 값은 이제 verifier가 사용할 것이다.
    1. 초기식을 assignExpr 하는 코드를 @commonCtor에 담긴다. 생성자가 호출되면 기존의 proxyNode() 부분을 map.set() 을 통해서 교체할 것이다.
* proxyNode::clone()은 자신을 복제한 객체에 orgObj를 얇은 복사해서 넣는 구현이다.
* scope::clone()은 각 elem에 대해 clone() 을 해서 넣는 코드다.
* 초기식이 아닌 owns는 owns에 그대로 담긴다.
* tnmap은 set() 함수를 제공한다. 만약 set시도했으나 add를 한적이 없다면 에러를 반환한다.
* assignExpr은 iter를 가져와서 setVal()을 하지 않는다. 대신에 subs().set()을 수행한다.
* iter.setVal()을 필요가 없다면 제거한다. getVal()은 놔두고.
* obj::\_assign()은 현재 구현이 정말 맞는지 확인이 필요하다
    * [?] 각 elem에 대해 map.set()을 하는게 맞지 않나?
    * [?] rhs.owns.deepClone()을 하는게 정말 맞나?
    * [?] \_isComplete = true로 하는 게 맞나? 완전이냐 불완전이냐 하는 것은 callComplete()를 호출해서 생성된 객체인가 아닌가로 갈리는 것이다. 원본이 불완전인데 그걸 assign 한다고 해서 이게 완전이 되는게 아니다.
* obj::run() 안쪽에서 객체를 생성할때 다음처럼 한다.
        1. orgObj::clone()을 한다.
            1. ret.owns = \_fixOwns.clone()
            2. ret.shares = tnchain(shares)
            3. ret.subs = tchain(ret.owns)
            4. ret.subs.next = ret.shares
        2. 생성자를 호출한다.
            1. run()에 들어온 args에 적합한 ctor을 찾는다.
            2. ctor에 호출하기 직전에 commonCtor을 호출한다.
            3. ctor을 호출한다.
        3. 생성자 / commonCtor에서 `with` 를 통해 다시 다른 obj::run() 이 호출될 수 있다. 이 경우 위의 과정이 재귀적으로 진행된다.
* with 교체는 런타임에도 언제든 발생가능하다. 자세한 알고리즘은 문서 윗부분에 별도 항목으로 정리하였다.
* * *
# 변수 정의시에 이름에 `int` 같은 primitive type의 이름도 적을 수 있다.
```go
foo(int int) int
    ret 3
int int := 22
```
* * *
# 변수명을 타입명과 동일하게 하려면 `@`으로 편하게 할 수 있다.
* 변수 정의시에 `@Abe`라고 정의할 수 있다.
* `@Abe`는 앞자리를 소문자로 한, `abe Abe`로 풀어진다.
* 변수 정의시에 사용하므로 함수 parameter에서도, 본문에서도, 프로퍼티 선언시에도 다 된다.
* 함수 타입 접근시에도 사용한다.
```go
def A
    foo(n int, x int) int: n + x

A.foo(n int, x int).name // ok
A.foo(@int, @int).name // ok
A.foo(int, int).name // 에러! 함수 호출처럼 보인다, 그치? 컴파일러는 구분이 불가능하다.
```
* * *
# 함수 밖에 있는 변수에 먼저 assignment를 하고 나서 함수를 실행하고자 할때는 `=변수명` 을 한다.
* obj 내 함수를 정의할 때 assignment를 처음부터 실행해놓고 본문을 수행하도록 하는 편의 기능이 있다.
* 이때는 파라메터를 정의할 때 타입 없이 `=프로퍼티명`을 적는다.
* 함수명은 적을 수 없다. 메모리가 없으니까.
* 이 기능을 사용하면 함수의 정의를 비워도 abstract 가 아니다.
```go
def A
    age := 0
    name str

    // 오리지널 버전:
    ctor(age int, name str)
        me.age = age
        me.name = name
        print("ctor\n")

    // 축약버전:
    ctor(=age, =name)
        print("ctor\n")
```
* 함수 정의시에는 반드시 반환형을 적어야 하므로 파서는 모호성 오류를 일으키지 않는다.
* 생성자 뿐만 아니라 함수 정의시라면 다 사용 가능하다.
* 람다에서도 동작한다.

```go
def a
    listener onClick
    onClick(@Button) int // abstract
    setListener(listener) void // abstract가 아니다.
    setListener(=onClick) void // 에러! onClick이라는 프로퍼티는 없다.

main() void
    btn Button
    a.setListener((=btn): print("${btn.name} clicked!")) // btn = btn이 먼저 동작한다.
    a.dispatch()
```
# 당연히, `@Abe` 와 `=Abe`를 같이 쓸 수 없다.
```go
=@abe
```
* abe는 프로퍼티를 정의하는게 아니므로 프로퍼티 명1개만 필요하다.
* 그러니 `@abe`로 프로퍼티명과 타입을 같이 서술하고자 하는게 의미가 없다.
* * *

# `:`는 indent, dedent를 생략한다. `;` 는 앞의 구문과 같은 레벨의 stmt임을 알린다.
### 알고리즘
* 문제는 dedent를 알기 어렵다는 것이다.
* 예를 보자.
    ```go
    a := if value == true: 55 else: 22
    ```
    * [?] `55 else` 사이에 dedent가 있어야 한다. 그게 가능한가?
* * *
# Multiline String을 `"""` 로 지원한다.
* * *
# `?`로 safe navigation을 지원한다.
* 인자 뒤에 `?` 만 붙이면, 해당인자가 null이 아닐때 해당라인이 실행되게 한다.
```go
foo(getAOrNull()?, getB()?) // 2개 중 하나라도 null인 경우 실행하지 않는다.
```
## 알고리즘
```cpp
class safeNullNode : public baseObj {}
```
* 를 만든다.
* mgdFunc::\_evalArgs()에서 p.getOrigin().as\<node\>()의 결과가 safeNullNode면 mgdFunc::run()을 그냥 로그 없이 tstr(safeNullNode::singleton) 만 넣어서 종료한다.
* * *
# c++ 식 null check를 지원하지 않는다
* null은 false를 의미하지 않는다.
* * *
# 정의문은 expression이 아니다
* 변수의 정의를 expression으로 했을때의 demerit가 너무 많다.
    * 정말로 온갖 문법에서 에러가 발생한다.
    * 게다가 변수의 정의가 함수 인자 깊숙이 나올 수 있으므로 한눈에 파악이 어려운 곳이 많다.
* * *
# lhs여부는 지금과 마찬가지로 case by case로 verification 한다.
* lvalue냐 rvalue냐 하는 문제는 생각보다 복잡하다. 근본적으로 런타임에만 해결이 가능하다. 그러니 lvalue를 완벽히 탐지해낼 생각은 하지 않는게 좋다.
    * 타입 T에 대한 lvalue 타입, rvalue 타입을 만들어내지 않는 이상(C++처럼 하지 않는 이상), 대다수 언어들이 하고 있는 lvalue 구분이라는 건 헛점이 반드시 존재한다.
* **지금처럼 verifier는 assignExpr을 검증할때 lhs가 getExpr, getElementExpr** 만 2가지 케이스일때만 lhs라고 판단하도록 구현한다.
* * *
# Meta정보 접근은 API로 한다.
* Meta 정보는 별도의 reflection API 처럼 해서 접근한다. `A.age.name` 처럼 접근하지 못한다. 이유가 있는데,
    1. age가 class였다면 `name` 이라는 prop은 너무 흔히 사용 가능한 이름이다. 사용자는 `name` 이라는 prop을 만들지 못한다는 큰 제약을 가져야 한다.
    2. 어떠한 언어도 `A.age.name` 처럼 가능하게 해주지 않는다. `A.class.getProp("name")` 처럼 해주지. 이유가 있다.
        * relfection을 사용한다는 건 동적으로 범용적으로 돌려야 할때 주로 사용한다.
        * `A.age.name` 은 근데 `name` 이라는 접근하려는 prop의 이름을 정적으로 확정한 것이다.
        * 즉, `A.age.name` 처럼 사용하는 케이스 자체가 별로 없을 거라는 것이다. `A.class.getProp(name)` 을 사람들을 더 원할 거라는 것.
    3. 바로 이, 불완전객체에 대한 접근여부를 탐지하는 로직의 구현이 어려워진다.
* 정확히 어떤 API가 될지, 구현은 어떻게 될지 아무런 생각이 없다.
* * *
# Verifier는 orgObj를 proxyNode를 통해서 만들어 놓고 검증한다.
* ProxyNode는 1개의 함수를 제외하고는 모든 함수를 proxy로 전달만 한다.
* 그 1개의 함수란 바로 isComplete() 다. 항상 true를 반환한다.
* verification 도중 defAssign 처럼 변수를 생성해야 하는 경우, ProxyNode() 객체를 생성해서 넣어두자.
* 그리고 orgObj 안에 있는 각 prop 들은 실제 값을 run되서 들어가 있는게 아니라 getEval()로 타입만 확정된 orgObj이 들어가 있게끔 한다.
* * *
# for문은 마지막 실행 결과를 반환하는 게 아니라, 배열을 반환한다.
* 매번 forExpr 실행시마다 배열을 생성해서 넘기는 것은 퍼포먼스를 많이 요구한다.
* verifier는 forExpr을 검증할때 배열을 생성해서 넘겨야 하는지를 판단해서 forExpr에 set 주도록 한다.
```cpp
str forExpr::getEval() {
    return new arr(_block.getEval());
}
```
* * *
# Fixes ==================================================================
# visitor는 \_visited를 onTraverse(node) 에서만 구현해 놓았다.
* 커스텀하게 onTraverse를 해둔 경우는 \_visited가 동작안한다.
* * *
# generalizer.cpp:14, 15 라인은 필요가 없다
* * *
# CPP 파일명의 첫글자는 대문자로 한다
* * *
# defVarExpr은 defPropExpr로 고치자.
* 우리는 모든 변수가 prop 이다.
* * *
# BridgeObj
* BridgeObj는 baseObj에서 상속받도록 하자.
* * *
# 잘못된 타입을 str로 cast해도 오류가 발생하지 않는다.
```go
def A
    age int
    main() void
        print("age = " + A() as str) // 여기!
```
* as int로 하면 정상적으로 메시지가 나온다.
err10(OBJ_WRONG_ARGS) 8:5: I can't run with this argument types '{none}'. they should be 'N4namu4nStrE'
err42(IMPLICIT_CAST_NOT_AVAILABLE) 8:11: couldn't implicit cast N4namu4nStrE type as N4namu4nIntE
* str일때만 아무런 메시지도 없고, 죽지도 않고. 아무런 값도 나오지 않는다.
* * *
# baseFunc, func로 이름을 cpp 클래스명을 각각 변경하자
* * *
# runExpr의 잘못된 구현?
* me obj를 구해서 `me.run(NAME, args)` 를 하기만 하면 되야 하지 않았나...
* 지금 구현은 이렇다.
    1. runExpr은 node me, node sub, args 3개를 가진다. 이중에서 sub는 getExpr이 들어있다.
    2. sub.setMe(me) 한다.
    3. sub.as() 를 하면 안에서 sub.run()이 돌면서 sub.me 에서 sub,args를 만족하는 sub(즉 func)을 반환한다.
    4. 반환한 func.run(args)를 한다.
* NAME대신 expr을 받는 부분은 어쩔 수 없다고 생각한다. `int[]` 나 `A<B, C>` 같은게 되어야 하니까 string만으로는 안된다.
* 그러나 위의 구조는 고쳐야 한다.
* `A::B()` 문법을 지원하면서 자연스럽게 고쳐질 것으로 보인다.
* * *
