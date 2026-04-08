# 간략화된 언어 스펙 {#ac-language-spec}

* 이 문서는 다른 프로그래밍 언어를 다뤄본적이 있는 익숙한 **개발자** 를 대상으로 작성되었습니다.

* Byeol 언어는 현재 일종의 **베타테스트** 기간입니다.

* 언제든지 spec의 **내용이 변경** 될 수 있습니다.

* 각 항목 앞에 ✅ 가 되어있는 항목은 **구현이 완료** 된 겁니다.

* 검증된 예제는 온라인으로 **실행 가능** 합니다. 미검증된 예제는 **빨간색 테두리** 로 표시되어 있습니다.

---

## 언어 디자인 철학

* 빠르게 기능만 익히고 싶은 분은 넘어가셔도 됩니다.

* 관심이 있으신 분은 한번 꼭 @ref design-philosophy "읽어보세요". 이 언어가 왜 이렇게 설계되었는지
이해하는데 큰 도움이 되실겁니다.

---

## 주석

* ✅ 주석은 `#` 으로 표현합니다.

```
{
    "style": "verified runnable language-byeol",
    "shown":
`# 한줄짜리 주석이고,

## 여러줄
주석입니다.##
`,  "code":
`main() void
    # 한줄짜리 주석이고,

    ## 여러줄
    주석입니다.##
    ret
`
}
```

---

## 기본 타입 제공

* ✅  다음의 기본타입(primitive types)이 있습니다.

```
{
    "style": "runnable language-byeol",
    "shown":
`"문자열" # str: 쌍따옴표로 표현합니다.
'문자열' # str: 작은따옴표로도 표현할 수 있습니다.
'He said "Hello"' # 작은따옴표 안에서는 " 를 escaped sequence 없이 사용 가능합니다.
3202302 # int: 4byte 정수입니다.
true # bool
3.5 # flt: 4byte float.
0xff # 255를 뜻하는 16진법 int
017 # 15를 뜻하는 8진법 int
"a" # character type 은 존재하지 않습니다.
"\\0x2B" # ascii 2B(10진수 43)에 해당하는 '+'
"\\43" # ascii '+'. 위와 같은 코드입니다.
# byte도 있어요. 0 ~ 255까지 표현 가능하죠.
# void 도 있습니다만, 물론 void 타입의 변수를 만들 수는 없죠.`,
"code":
`main() void
    print("문자열\n") # str: 쌍따옴표로 표현합니다.
    print('문자열\n') # str: 작은따옴표로도 표현할 수 있습니다.
    print('He said "Hello"\n') # 작은따옴표 안에서는 " 를 escaped sequence 없이 사용 가능합니다.
    print("${3202302}\n") # int: 4byte 정수입니다.
    print("${true}\n")# bool
    print("${3.5}\n") # flt: 4byte float.
    print("${0xff}\n") # 255를 뜻하는 16진법 int
    print("${017}\n") # 15를 뜻하는 8진법 int
    print("a\n") # character type 은 존재하지 않습니다.
    print("${\0x2B}\n") # ascii 2B(10진수 43)에 해당하는 '+'
    print("${\43}\n") # ascii '+'. 위와 같은 코드입니다.
    # byte도 있어요. 0 ~ 255까지 표현 가능하죠.
    # void 도 있습니다만, 물론 void 타입의 변수를 만들 수는 없죠.`
}
```

---

## 프로퍼티 정의

* ✅ 무언가 값을 저장할 수 있는 변수를 byeol언어에서는 프로퍼티라고 합니다.
* ✅ byeol언어에서 타입을 명시적으로 표현할때는 `식별자 <space> 타입명`으로, 항상 띄어쓰기 후 타입을 식별자 뒤에 표현합니다.
* ✅ 따라서 프로퍼티를 정의할때도 위의 타입 규칙을 따릅니다. 즉, 타입은 항상 뒤에오죠.

```
age int # 뒤의 int가 타입이죠.
age = 22 # 누군지 몰라도, 부럽네요.

```

* ✅ 변수를 정의하면서 초기화도 같이 할 수 있는데, 그럴때는 `:=` 기호를 사용합니다.

```
everything int := 42 # int 타입이며, 그 값은 42 입니다.
everything2 := everything # everything2는 everything과 같은 int 타입이며, 값도 같습니다.

```

* ✅ 식별자는 영문자나 `_` 혹은 숫자로 정의합니다만, 첫글자는 오직 영문자만 가능합니다.

---

## StringTemplate

* ✅ StringTemplate도 지원합니다.

```
ch := "+"
print("ch=$ch") # "ch=+"
print("ch=" + ch) # "ch=+"

```

* ✅ 기호가 포함될 경우에는 `{}` 로 감싸세요.

```
print("${a + b}")

```

---

## Map

* ☐  byeol언어는 배열, map, sequence를 기본 제공합니다.
* ☐  Map 객체는 대괄호를 사용해서 정의합니다.

```
# <타입>[<타입>] 으로 Map 타입을 표현합니다.
map int[str] # str이 key, int가 value인 map 입니다.

```

* ✅ 아쉽게도 Map에 대해, Literal 상수를 표현할 수는 없습니다.
* ☐  존재하지 않는 원소에 대괄호를 사용해 접근하면, 자동으로 기본값이
할당됩니다.

* ✅ 모든 Container는 객체이므로, 여러 메소드를 가집니다.

```
map int[str] # str이 key, int가 value인 map을 만듭니다.
map["apple"] = 300
map["banana"] = 700
print(map["apple"]) # "300"입니다. 사과 참 싸네요.

```

---

## 배열

* ✅ 배열타입은 어떻게 표현할까요? 배열은 Key가 항상 int인 Map처럼 표현합니다. 실제로 둘은 매우 유사한 부분이 많습니다.

```
myArray int[] # int 배열입니다.
myArray2 := int[]() # 배열은 객체이므로, 생성자도 있죠.

```

* ✅ 중괄호로 Literal 상수를 표현합니다.

```
myArray3 := {2, 3, 4} # 원소 2, 3, 4를 보고 int배열이라는 걸 유추합니다.

```

* ✅ 모든 container는 유동적인 size를 갖습니다.
* ✅ 전통을 따라, Index는 0부터 시작합니다.
* ✅ 대괄호로 원소에 접근합니다.

```
myArray.add(5)
++myArray[0] == 6 # true

```

---

## 시퀸스

* ✅ `..` 으로 표현합니다. 정수의 범위를 표현합니다.
* ✅ container의 일종이므로, 객체입니다.
* ✅ 끝은 포함되지 않아요(exclusive). 예를들어 1..3 이라고 하면 {1, 2} 2개의 값까지만 의미합니다.

```
"message"[1..3] # "es"
for n in 2..5
    print(n) # 234 출력

```

---

## if

* ✅ if, else를 제공합니다.
* ✅ byeol언어는 소괄호를 싫어합니다. 조건절에 소괄호를 넣지 않습니다.

```
if a == 5
    foo(1)
else if a == 7
    foo(2)
else
    foo(4)

```

---

## Block문

* ✅ offside-rule로 블록문을 표현합니다. 들여쓰기를 해야한단 얘기예요.
* ✅ 블록문은 2 종류로 분류됩니다.
    * 1) 구문과 정의문을 모두 쓸 수 있는 `일반블록문`
    * 2) 정의문만 쓸 수 있는 `정의블록문`
* ✅ 모든 Block문은 Scope를 표현합니다. Scope에 대해서는 나중에 더 얘기해보죠.
* ✅ `:` 는 블록문을 붙여서 쓸 수 있어요.
* ✅ `;` 는 이전 줄과 같은 블록문에 있는 구문을 한줄에 붙여 쓰겠다는 의미입니다.

```
if val == 0 # 2번: 위의 1번과 같은 코드입니다.
    print("1")
    print("2")
# 위 코드를 한줄에 쓸 수도 있습니다:
if val == 0: print("1"); print("2")

```

* ✅ `:`를 응용하면 이렇게도 쓸 수 있어요.

```
# 아래 if는 `:`를 2번합니다.
if val == 0: if val > 0: print("ok")
    else: print("no") # 이 else는 indent를 1번 했죠? 그래서 두번째 if에 대한 else 입니다.

## 풀어쓰면 이런 코드가 됩니다.
if val == 0
    if val > 0
        print("ok")
    else
        print("no")

##

```

* ✅ byeol언어는 비어있는 블록문을 가능하면 피하고 싶습니다.
* 꼭 명시해야하는 상황이라면 `: ;` 로 표현합니다.

```
if val == 0: ;
else: val = 1

```

---

## while

* ✅ 조건식이 참(true) 일 동안 블록문을 반복 실행합니다.

```
# while <조건절>
#  <블록문>
cnt := 0
while cnt++ < 5
    print("forever?")

```

---

## for

* ✅ `for`는 container를 순회합니다.
* ✅ `break`는 가장 최근의 `while` 혹은 `for`을 벗어납니다.
* ✅ `next`는 다음 원소로 넘어갑니다.

```
# for <변수명> in <컨테이너>
#    <블록문>
arr := {1, 2, 3}
for n in arr
    print(n)
    if n == 2
        break # 루프를 빠져나갑니다.
    if n == 1
        next # 다음 원소를 가지고 루프의 처음부터 다시 실행합니다.
# 결과: "12"

```

---

## Explicit 캐스팅

* ✅ `as` 를 사용해서 다른 타입으로 변경할 수 있습니다.

```
val1 := 0 # int
val2 := var1 as flt # val2는 flt이고 0.0 값을 갖죠.
0 as str # "0".
"123" as int # 123, 문자를 숫자로 바꿀 수 있습니다.

```

* ✅ 물론 각 객체가 `as`를 했을때 주어진 타입으로 변경하는 코드를 가지고 있어야
합니다. `primitive types`는 대부분 서로 변환이 가능합니다.

---

## Implicit 캐스팅

* ✅ 함수호출시 자동으로 동작하는 캐스팅입니다.
* ✅ 기본 타입끼리는 다음처럼 변환됩니다.

| 타입 | 변환 가능한 타입 |
|------|------------------|
| byte | bool, int        |
| int  | flt, byte, bool  |
| flt  | int, byte, bool  |
| bool | flt, byte, int   |
| str  | N/A              |
| void | N/A              |

* ✅ 위의 표를 Explicit 캐스팅과 착각하시면 곤란합니다. 대부분의 모든 primitive
types는 `as`를 사용하면 서로 변환합니다.

* ✅ 사용자가 만든 타입일 경우, `upcasting`만 지원합니다. 사용자가 Implicit
캐스팅에 뭔가 관여를 할 수는 없습니다.

* ✅ 2단계를 거쳐서 Implicit 캐스팅의 Implicit 캐스팅이 일어나진 않습니다.

```
foo(a flt)
    print(a)
val1 := 27 as byte
val2 := foo(val1) # <-- 에러.
# byte --> flt으로 Implicit 캐스팅이 불가능합니다.
# byte --> int --> flt 이렇게 암시적 캐스팅이 2번 일어나면 에러가 아니었겠죠. 그러나 암시적 캐스팅은 1번만 일어납니다.

```

---

## 연산자들

* ✅ 단형연산자 당연히 지원하고요.

```
var := 3
++var # 4, 전위 연산자도 됩니다.
var-- # 4, 후위도 됩니다.
-var # -3

```

* ✅ 2항도 지원합니다.

```
var1 := 1
var2 := 3
var1 + var2 # 4
var1 - var2 # -2
var1 * var2 # 3
var2 / var1 # 3
var2 % 2 # 1
var1 > var2 # false
var1 <= var2 # true
var2 == (3 - 2) # false, 소괄호를 쓰면 계산 우선순위가 높아져요.

```

* ☐  `==` 는 value equality를, `===` 를 reference equality를 검사합니다.

```
val1 := 1
val2 := 1
val1 == val2 # true
val1 === val2 # false

```

* ✅ 비트연산 지원합니다.

```
var1 := 3 # 3:  0011
var1 >> 1 # 1:  0001
var1 << 2 # 12: 1100
var1 | 4 # 7: 0111
var1 & 2 # 2: 0010
60 ^ 13 # 49: 111100 ^ 001101 = 110001

```

* ✅ 할당연산자도 지원합니다.

```
var1 := 0
var1 = 2 # 2
var1 += 1 # 3
var1 *= 2 # 6
var1 %= 4 # 2
var1 &= 2 # var1 = var1 & 2 --> 2

```

* ✅ 연산자 우선순위는 이렇습니다. 높을 수록 우선됩니다.
* ✅ 도중에 `is`, `in` 과 같이 아직 설명하지 않은 것들이 보일겁니다. 나중에
알려드릴테니 일단 무시하세요.

| 순위 | 연산자          | 예시                   |
|------|-----------------|------------------------|
| 0    | (expr)          | (a \|\| b)             |
| 1    | --, ++, ., []   | a--, a++, a.b, a[b]    |
| 2    | --, ++, -, !, ~ | --a, ++a, -a, !a, ~a   |
| 3    | as, ..          | a as b, a..b           |
| 4    | *, /, %         | a * b, a / b, a % b    |
| 5    | +, -            | a + b, a - b           |
| 6    | &               | a & b                  |
| 7    | ^               | a ^ b                  |
| 8    | \|, is, in      | a \| b, a is b, a in b |
| 9    | >, <, >=,       | a > b, a < b, a >= b   |
|      | <=, ==, !=      | a <= b, a == b, a != b |
| 10   | <<, >>          | a << b, a >> b         |
| 11   | \|\|, &&        | a \|\| b, a && b       |

---

## 함수

* ✅ parameter 목록과 반환형, Block문이 정의된 것을 함수라고 합니다.
* ✅ 프로퍼티때와 마찬가지로, 함수의 반환형 역시 뒤에 옵니다.

```
# foo() 함수는 int, flt, nbool를 인자로 받고 아무것도
# 반환하지 않는(void) 함수입니다.
foo(a int, b flt, c nbool) void
    print(a + b + c) # print문은 기본 제공되는 함수로 화면에 값을 출력합니다.

```

* ✅ 위에서 제가 언급한 함수 정의에 함수명이 없다는 점을 눈치채셨나요?
* ✅ 함수명을 적지 않으면 람다함수가 됩니다. 단, 람다함수는 반드시 함수 호출 안에서 적어야 합니다.
* ☐  람다함수는 parameter타입과 반환형을 생략할 수 있죠.

```
if true
    runLambda(val1, (a, b)
        a + b
    , 33) # indent를 한번 한 후, comma를 적었다는 걸 주의하십쇼.
          # comma의 위치가 상당히 중요합니다.

```

* ☐  앞에 정의한 람다를 다음처럼 한줄에 적을 수 있습니다.

```
runLambda(val1, (a, b): a + b, 33)

```

---

## ret

* `ret` 는 값을 반환하고 함수를 종료합니다.
* ☐  `...` 로 가변 인자를 표현할 수 있는데, 가변인자는 인자목록 끝에 정의해야 합니다.

```
sum(args int...) int
    sum := 0
    for a in args # args 는 int[] 과 같은 겁니다.
        sum += a
    ret sum

```

---

## 객체 정의

* ✅ byeol언어는 OOP언어입니다.
* ✅ 객체를 만드는 방법은 2가지가 있습니다.
    * ✅ 1) 하나는 새로운 객체를 만드는 것이고
    * ✅ 2) 다른 하나는 이미 만든 객체를 복제 하는 거예요.

### 새로운 객체 만들기

* ✅ 다음 처럼 하면 됩니다.

```
# def <객체이름>
#    <정의블록문>
def person
    age := 0 # 정의블록문에는 프로퍼티나 함수 등 정의하는 구문만 올 수 있습니다.
    hello() void # 객체에 함수도 정의할 수 있죠.
        print("I'm foo and $age years old!\n")

```

### 객체 복제하기

* ✅ 객체를 만드는 또다른 방법은 이미 만들어진 객체를 복제하는 건데요.
* ✅ 함수에 대고 호출하면 함수호출이 되듯, 객체에 대고 호출하면 객체 복제가 됩니다.
* ✅ 함수 안에서 이 함수를 소유한 객체를 `me` 로 접근합니다.
* ✅ `me` 는 생략이 가능합니다. 자세한 내용은 `scope`에서 다룹니다.

```
def Person
    age := 0
    say() void: print("I'm ${me.age} yo.\n")
    # 위에서의 me를 사실은 생략할 수 있단 얘기죠.

main() void
    p1 := Person() # (A)
    p1.age = 1
    p2 Person # (A)와 같은 코드입니다.

    for p in {p1, p2, Person()} # `Person()` 처럼 이름없는 객체를 만들 수도 있습니다.
        p.say()

```

---

## 생성자

* ✅ 생성자는 `ctor` 이라고 하는 고유 함수명을 사용합니다.
* ✅ 생성자를 정의할 때 반환형은 생략합니다.

```
def Food
    name str
    price int
    ctor(name str, price int)
        me.name = name
        me.price = price
pizza := Food("pizza", 15000)

```

* ☐  생성자 안에서만 `ctor`로 생성자를 호출할 수 있습니다.
* ✅ 내부적으로 생성자는 객체 생성되고 나서 자동으로 호출됩니다.
* ✅ 즉, 생성자 안에서 `ctor`로 또 호출한다고 해서 객체가 또 생기는 건 아니란 얘기입니다.

```
def Food
    name str
    price int
    ctor()
        print("call constructor!\n")
        ctor("defaultName", 100)
    ctor(name str, price int)
        me.name = name
        me.price = price

print(Food().name) # "defaultName"

```

* ✅ 생성자를 명시하지 않으면 기본 생성자가 정의됩니다.

---

## Special Functions

* ✅ Special Functions는 앞에 `@` 가 붙는 특별한 함수들입니다.
* ✅ 이들은 인터프리터에 의해 특정 시점에 자동으로 호출됩니다.
* ✅ `@ctor`, `@get`, `@set` 등이 존재합니다.

---

## Pack

* ✅ `pack <객체이름>`으로 하면 그 `pack` 객체가 생성됩니다. 그 구문 밑에서
정의한 모든 객체며 함수는 이 `pack`에 속하게 됩니다.

* ✅ 따라서 같은 `pack`을 명시한 소스코드 끼리는 서로 공유가 됩니다.
* ✅ `pack`의 본질은 결국 `def`처럼, 사용자가 정의한 객체에 불과합니다.

```
# file "a.nm" --
pack example # 이 파일은 example이라는 객체를 `pack` 으로 삼았습니다.
# 여기서부터는 사실 example이라는 객체의 내부입니다. 즉 `def example` 과 같습니다.
# -------------------------------
# 여기서부터는 `example`의 정의블록문 입니다.

foo() int # example은 foo() 함수를 갖습니다.
  print("this is pack 'example'\n")
  ret 33

def person # person 이라는 타입도 갖습니다.
  age := 2
  name := "Chales"
  ctor(newAge int): age = newAge

```

* ✅ 이 `pack`은 시스템이 가져올 수 있는(import) module의 최소 단위입니다. 어떻게 `pack`을 import 할 수 있는지는 밑에서 얘기할께요.

* ✅ `main()` 함수는 특정 pack에 대한 진입점 입니다.
* ✅ `main()` 함수는 반환형이 `void` 혹은 `int` 여야 합니다.
* ✅ `main()` 함수는 parameter가 비어있거나 `str[]` 여야 합니다.

```
# file "b.nm" --
pack example
# example의 scope 입니다. 그러니 파일이 달라도 example의 모든 식별자에 바로 접근 가능합니다.

main() void
    a := foo() # 다른 파일에 있는 foo함수를 호출합니다. 같은 객체(example)의 scope을 공유 하니까요.
    print(person(a).age) # 1
    print(example.person(a).age) # 1과 같은 코드입니다.

```

---

## scope

* ✅ 어느 객체가 scope로써 등록되면, 해당 객체가 소유한 하위 식별자 (함수, 프로퍼티, 또다른 객체 등)에
바로 접근할 수 있게 됩니다.
* ✅ 각 scope은 나름의 방법에 의해 시스템에 잠시 등록되었다가 등록이 해제됩니다.
* ✅ Scope이 메모리에서 사라지면 Scope이 각 식별자에 대한 참조도 잃습니다.
* ✅ 모든 참조를 잃어버린 객체는 메모리에서 해제됩니다.
* ✅ byeol언어의 Scope에는 5가지 존재합니다.
  * ✅ 1) local scope: 매 블록문마다 scope이 생성되고, 블록문이 끝나면 scope은 메모리에서 사라집니다.
  함수가 가진 블록문도 여기에 속합니다.
  * ✅ 2) func scope: 블록문을 제외하고 함수가 가진 하위 식별자들을 scope에 등록합니다.
  함수 호출이 끝나면 scope은 등록이 해제됩니다.
  * ✅ 3) obj scope: 함수를 가지고 있는 객체의 scope 입니다. 객체가 사라질때 이 scope도 사라집니다.
  * ✅ 4) pack scope: pack이 제공하는 obj scope 입니다. pack에 속한 obj scope은 해당 pack scope와
  항상 연결되어 있습니다. pack은 사라지지 않습니다.
  * ✅ 5) file scope: 각 소스코드 파일 내에서만 접근 가능한 scope 입니다. 해당 파일의 코드가 실행될때만
  scope이 등록됩니다.
* ✅ scope의 우선순위는 위의 언급한 숫자가 작을 수록 높습니다. (local scope이 제일 높음)
* ✅ 보통 file scope에 외부 pack을 import 하기 위한 코드를 작성합니다.

```
pack mypack
# 여기서부터는 pack scope:
age := 27 # mypack.a

def Person
    # Person에 대한 obj scope:
    name str
    ctor(newName str): name = newName
    hello() void
        # age는 pack scope에서 왔습니다.
        print("I'm $name and $age yo.\n")
        # print("I'm ${me.name} and ${mypack.age} yo.\n") 처럼 해도 되긴 합니다.

foo() str
    # local scope:
    name := "kniz and $age yo"
    ret name
    # 함수가 끝나면 local scope에 의한 name에 대한 참조는 사라지지만, name은 함수 밖으로 반환되므로,
    # 함수에 의한 참조는 남아 있습니다.
    # 그러니 name은 메모리에서 사라지지 않습니다.

main() void
    Person(foo()).hello()
# name에 대한 참조는 이 Person 임시객체가 가졌습니다.
# Person이 소속된 코드가 끝났으므로, Person은 사라집니다.
# Person이 가졌던 name에 대한 참조도 사라지므로, name 또한 사라집니다.

```

* ✅ `pack` 키워드 보다 윗 공간은 file scope 입니다.
* ✅ 하나의 scope 안에서 같은 식별자를 정의할 수 없습니다.
* ✅ 위 얘기는 scope이 다르다면 이름이 같아도 된다는 뜻이죠.

```
# file "a.nm" --
pack example
# 여기서부터는 pack scope.
age := 4

foo() void
  print(age) # "5"

# file "b.nm" --
# -----------------------------------------------
# 여기서부터는 file scope:
age := 5
# 이 age는 이 파일에서만 유효합니다.
# example에도 `age` 라는 프로퍼티가 존재하기 때문에, 사실상 pack scope의 age를 덮어쓰는 효과가 발생합니다.
# 인터프리터가 이 점에 대해 경고를 내보낼 겁니다.

pack example
# -----------------------------------------------
# 여기서부터는 pack scope:
def Person
  age := 3 # 눈치채셨나요? 사실 이 값들은 위에서 말한 우선순위를 나타내고 있습니다.
  hello() void
    print(age) # "3"

main() void
  if true
    age := 1
    print(age) # "1"
  print(age) # "4"
  Person().hello() # "3" 출력.

```

* ☐  obj가 아니라 pack scope 혹은 file scope 에 속한 식별자에 접근할때는 `pack`을 사용합니다.

```
scope := "file"
pack example
scope := "pack"

def A
  age := "obj"
  foo() void
    age := "local"
    print("$age ${me.age} ${pack.age}")
# 결과:
#  local obj file

```

---

## 오버로딩

* ✅ 오버로딩을 지원합니다.

```
add(a str, b str) str
    a + b
add(a int, b int) int
    a + b

```

---

## 평가전략

* ✅ by object 평가전략을 사용합니다.
    * 1) immutable 속성을 갖는 인자의 경우, 값에 의한 복사가(by val)
    * 2) mutable 속성을 갖는 인자의 경우, 참조에 의한 복사가(by reference)
         됩니다.

* ✅ immutable은 primitive 객체에만 적용되는 것으로, 사용자는 자신이 정의한 객체에 immutable 속성을 줄 수 없습니다.

```
def Person
    age := 0

foo(val1 int ##by val##, val2 person##by ref##) void
    val1 = 22
    val2.age = 22

main() void
    age := 1
    p := person()
    foo(age, p)
    print("age=$age p.age=${p.age}") # age=1 p.age=22

```

---

## 중첩 객체

* ✅ 다른 언어에는 중첩 객체라는 기능이 있습니다.
* ☐  변수 정의가 일반블록문에 올 수 있다면, 당연히 함수나 객체를 정의하는 것도 올 수 있어야 합니다.
* ✅ innerClass는 지원하지 않습니다.
* ✅ 함수 안에서 객체를 정의하더라도 아무것도 capture하지 않습니다.

```
def person
    def house # 중첩 객체
        address str
        ctor(a str): address = a
        introduce(p person) void
            # house는 person과 아무런 관련이 없기 때문에 person에 접근하려면 인자로 받아야 합니다.
            print("hello! I'm ${p.name}")

    myHome := house("korea")
    age := 22
    name := "Sunsin"

main() void
    def mc # 함수안에도 객체를 정의할 수 있습니다. 이 객체는 func scope에 속합니다.
        intro() void
            p person
# 일반 블록문 안에 변수는 정의할 수 있으면서, 클래스/함수를 정의할 수 없다면, 일관성이 없다고 생각되지 않나요?
            p.house.introduce(p)
# Byeol 언어에서는 정의블록문에 사용하는 모든 구문을 일반 블록문에서 사용할 수 있습니다.
    mc().intro()

```

---

## 중첩 함수

* ✅ 객체를 정의할 수 있는 곳에는, 함수도 정의할 수 있어야 합니다.
* ✅ 함수 안에 정의된 함수를 `중첩함수`라고 합니다.

```
def teacher
    checkAttendence() void
        def student
            name str
            me(n str): name = n

        bow(s student) void # bow는 중첩함수입니다.
            print("Glad to see you. I'm ${s.name}\n")

        for s in {student("Marie"), student("Crono"),
                student("Lucca")}
            bow(s)

main() void
  teacher().checkAttendence()
# 결과:
#   Glad to see you. I'm Marie
#   Glad to see you. I'm Crono
#   Glad to see you. I'm Lucca

```

* ✅ 중첩 함수는 외부함수와 같은 obj scope을 사용합니다.
* ☐  중첩 함수는 호출 시점까지의 outer scope을 capture 합니다.

```
def app
    foo() void
        arr := {1, 2, 3} # (1)
        getLenFrom() int
            # if val: print("ok") # err.
            # val은 이 함수 정의보다 뒤에 나옵니다. 그러니 getLenFrom()에서 val을 사용할 수 없습니다.
            arr.len # 호출 시점에서의 밖에 있는 `arr` 배열에 접근합니다.
        print(getLenFrom()) # 이 시점에서의 `arr`은 1번의 `arr`입니다.

        val := true # 이제 val이 정의되었습니다.
        if val
            arr := {4, 5} # (2)
            print(getLenFrom()) # getLenFrom() 안에서 사용되는 `arr`은 (1)의 `arr` 입니다.

```

---

## 클로저

* ✅ 함수를 다른 context로 넘기면(인자로 넣거나, 함수에서 반환) 클로저가 됩니다.
* ✅ 클로저 호출시에는 me 가 필요 없습니다.

```
def app
    func() int
    foo(n int) func
        arr := {1, 2, 3}
        getLenFrom() int
            ret arr.len + n
        # 함수를 넘기면, 이 함수는 클로저가 됩니다.
        ret getLenFrom

    main() void
        closure := app().foo(1)
        closure() # "4"

```

---

## is

* ✅ `A is B` 는 A가 B 타입일때 true가 반환됩니다.

```
value := get()
if value is flt
    print(value as flt)

```

---

## in

* ✅ `A in B` 는 B container의 A가 원소로 들어가 있으면 true 입니다.
* ✅ `in`은 value equality 관점에서 원소가 존재하는지를 검사합니다.

```
arr := {1, 2, 3}
2 in arr # true

```

---

## Pattern Matching

* ☐  패턴매칭을 실행할 구문은 한줄짜리 일반 구문이어야 합니다. 단, 예외적으로 할당정의문도 올 수 있습니다.
* ☐  검사 구문이란, 패턴이 일치하는지 확인하는 블록문이 아닌 한줄짜리 구문을 말합니다.
* ☐  검사 구문에는 `in`, `is`, `비교문`을 lhs만 생략해서 그대로 사용할 수 있습니다.
* ☐  검사 구문에는 `&&`, `||` 를 사용할 수 없습니다.
* ☐  기호나 `in`, `is`로 시작하지 않는 경우, 해당 검사는 `==`로 간주합니다.
* ☐  위의 경우 `,` 를 통해 여러가지 값들 중 하나라도 매칭되는지 검사 할 수 있습니다.
* ☐  fallthrough 를 지원하지 않습니다.

```
# matchers가 다음 중 하나일때,
#    (<inline-expr> [, <inline-expr>]*)* <indentBlock>
#    | in <inline-expr> <indentBlock>
#    | is <type> <indentBlock>
#    | `==` <inline-expr>
#    | `>` <inline-expr>
#    ......
# )
#
# 다음과 같이 사용합니다:
# <inline-expr || def-assign-expr>
#    <matchers>+

while true
    intValue := nextIncreasedInt()
        in {1, 2, 4, 5}: print("$intValue ")
        3, 6: print("*clap* ")
        is int
            print("!")
        > 6: break # for while
        # > 7 && < 9: .... # 컴파일 에러, 검사구문
        # 안에서 `&&` 나 `||` 쓰면 안됩니다
# 결과:
#  1 2 *clap* 4 5 *clap*

```

* ☐  유일하게 예외적으로 inline 구문에 할당정의문도 포함됩니다.

---

## 단축된 할당 문법

* ☐  obj 내 함수를 정의할 때 assignment를 처음부터 실행해놓고 본문을 수행하도록 하는 편의 기능이 있습니다.
* ☐  이때는 파라메터를 정의할 때 타입 없이 `=프로퍼티명`을 적습니다.
* ☐  이 기능을 사용하면 함수의 정의를 비워도 abstract 가 아닙니다.

```
def A
    age := 0
    name str

    # 오리지널 버전:
    ctor(age int, name str)
        me.age = age
        me.name = name
        print("ctor\n")

    # 축약버전:
    ctor(=age, =name)
        print("ctor\n")

```

* ☐  생성자 뿐만 아니라 함수 정의시라면 다 사용합니다.
* ☐  람다에서도 동작합니다.

```
def Button
    ctor(=name)
    name str

def a
    handler onClick
    onClick(btn Button) int # abstract
    setHandler(=handler) void # abstract가 아닙니다.
    setHandler(=onClick) void # err. onClick이라는 프로퍼티는 없습니다.
    dispatch() int: onClick(Button())

main() void
    # `=btn`이 먼저 동작합니다.
    # 그리고 함수 body가 실행됩니다.
    a.setHandler((=btn): print("${btn.name} click"))
    a.dispatch()

```

---

## 변수명을 타입명과 동일하게 하려면 백틱(`)으로 편하게 합니다.

* ✅ 변수 정의시에 `button Button`을 `` Button` `` 라고 정의할 수 있습니다.
* ✅ 백틱을 사용할때는 `int` 같은 primitive type을 사용할 수 없습니다.
* ✅ `person[]` 와 같은 `map`, `array`, `seq` 도 사용 할 수 없습니다.
* ✅ 변수 정의시에 사용하므로 함수 parameter에서도, 본문에서도, 프로퍼티 선언시에도 다 됩니다.
* ✅ 불완전객체에 대해서 백틱을 사용하면 첫글자가 소문자로 풀어집니다.
* ☐  함수 타입 접근시에도 사용합니다.

```
def Abc
    foo(n int, x int) int: n + x
    boo(int`) int: int + x # err. int`를 사용할 수 없다.

koo(Abc`) int # --> abc Abc 로 풀어집니다.
    ret abc.foo(1, 2)

```

* ☐  당연히, `` Abe` ``와 `` =Abe` `` 를 같이 쓸 수 없습니다.

---

## 클래스라는 건 없습니다

* ✅ 사용자가 `def`를 통해 정의한 것은 클래스가 아니라 객체입니다. 이를 `origin object`라고 합니다.
* ✅ origin 객체는 2가지로 분류합니다.
  - 하위요소(sub)를 사용할 수 있는 `complete object` 완전객체.
  - 극히 제한된 일부 요소(sub)만 사용할 수 있는 `incomplete object` 불완전객체
* ✅ 완전객체는 origin 객체이면서도 일반적인 객체처럼 사용이 가능한 것을 말합니다.
* ✅ 완전객체를 protected 생성자로 만들면 싱글톤이 됩니다.
* ✅ `def` 시 소괄호를 사용해서 객체완전자를 정의하면 그 origin 객체는 완전객체가 됩니다.

```
# def <식별자> `(` <arguments> `)`
#    <defBlock>
def school # 완전객체입니다.
    student person[]
    enroll(person') void
        student.add(person)

def person(1, "default") # 객체완전자 사용해서 완전객체 로 되었습니다.
    age int
    name str
    ctor(newAge int, name str)
        age = newAge
        me.name = name

main() void
    # school은 완전객체이기 때문에 바로 사용가능합니다.
    school.enroll(person(12, "Charles"))
    print(person.name) # "default"

```

* ☐  불완전객체는 극히 제한된 일부 하위요소(함수, 프로퍼티)만 사용이 가능합니다.
* ☐  불완전객체로 선언하려면 객체명을 대문자로 시작합니다.

```
def Person
    name str
# print(Person.name) # 에러! 불완전객체를 바로 사용할 수 없습니다.
print(Person().name) # ""

```

* ☐  정의할당문을 통해서 새로운 프로퍼티를 만들고, 그곳으로 불완전객체에 대한 참조를 옮길 수 있습니다.
* ☐  하지만 그 프로퍼티또한 불완전객체일 뿐입니다. 그러니 물론 프로퍼티 정의시 이름을 대문자로 시작합니다.

```
def ComplexLongNameBigCompanyParser
    ctor(): ret
    parse()
        ....

ShortParser := ComplexLongNameBigCompanyParser
# ShortParser의 이름이 대문자로 시작해야 합니다.
# ShortParser는 이제 ComplexLongNameBigCompanyParser에 대한 참조를 갖습니다.
# 즉, 인터프리터는 이 둘을 같은 객체라고 생각할 겁니다.

```

* ✅ 불완전객체는 모두 origin 객체 입니다.
* ☐  Origin객체는 할당을 할 수 없습니다.
* ☐  만들어진 불완전프로퍼티 또한 불완전객체와 같은 규칙을 따릅니다.
* ✅ origin 객체여야만 객체를 생성할 수 있는게 아닙니다. 모든 객체는 객체를 생성할 수 있습니다.
* ✅ origin 객체여야만 타입을 표현하는 건 아닙니다. 모든 객체는 타입으로 표현될 수 있습니다.

```
def person()
    name := "default"

main() void
    p1 person
    p1.name = "Chales"
    print(p1.name) # "Chales"

    p2 p1 # p2 := p1() 과 같은 코드입니다.
          # p1()은 p1객체를 복제하라는 의미가 아닙니다.
    print(p2.name) # "default"
    print(person.name) # "default"

    p3 := p1(p1) # 이게 바로 p1객체를 복제하라는 의미죠.
    print(p3.name) # "Chales"

    nestedFunc(p p1) str
        # origin 객체는 타입으로 사용할 수 있습니다.
        # 그러니 당연히 p1도 타입으로 쓸 수 있어야 합니다.
        p.name # 표현식 기반 언어이므로, 블록문의 마지막 라인이 자동으로 반환됩니다.

outerFunc(p p1) str
# 에러! p1은 어디까지나 main() 안에서 정의되었으니 밖에서는 모릅니다.
    p.name

```

* ✅ 이처럼 Byeol 언어는 클래스란 개념이 없습니다.
* ✅ origin 객체란, pack 안에 소속되어 시스템에 Global 하게 알려진 객체 하나에 불과합니다.
* ✅ 그러니 클래스라는 개념은 잊어버리세요.

---

## 스타일로 Attribute 정의

* ✅ attribute / constraint를 byeol 언어에서는 스타일로 대체해서 표현합니다.
* ✅ 예를들어, 식별자이름 앞에 `_` 를 붙이면 `protected` 접근제한자를 의미합니다.

```
def Person
    name := "Diana"
    _age := 38
p Person
print(p.name)
print(p.age) # 에러. 숙녀에게 나이 묻는 거 아닙니다.

```

* ✅ 대문자로만 적으면 상수입니다.

```
def Person
  NAME := "Chales"
p Person
print(p.NAME)
p.NAME = "doraemong" # 에러. NAME의 값은 한번 적으면, 변경될 수 없습니다.

```

* ✅ origin 객체명을 소문자로 시작하면 완전객체 입니다.
* ✅ 소문자로 시작하고, 객체완전자를 적지않으면 알아서 기본생성자를 통한 객체완전자로 인식합니다.

```
# 이제 왜 앞선 예제들이 가끔씩 소문자로 시작했는지 아시겠죠?
def Person
# 객체명이 소문자로 시작한다는 것은, 이 객체의 개발자는 이것을 일반객체처럼 다루길 원한다는 뜻입니다.
# 뒤에 `()` 객체완전자가 있는 것처럼 처리합니다.
    name := ""
    ctor()
        name = "default"

print(person.name) # "default"

```

---

## 타입 추론

* ✅ 변수를 정의할 때, 함수에 인자를 넘길 때 타입을 생략하면 값을 통해 유추합니다.

```
space := 42 # int
spaces := {2, 35.5} # flt
spaces2 := {3, 6.34, "wow"} # 에러! flt -> string은 implicit 캐스팅을 지원하지 않습니다.

```

* ☐  람다의 반환형/param타입도 추론 마찬가지 입니다.

```
onClick(btn Button) int

click(btn Button, onclick onClick) int
    btn.setState(CLICKED)
    onclick(btn)

main() void
    btn Button
    click(btn, (btn): 22) # 파라메터 타입 `Button`와, 반환형 `int`로 추론됩니다.

```

---

## 프로퍼티

* ☐  프로퍼티를 만들 때 `getter`와 `setter`를 지정할 수 있습니다.
* ☐  `getter`와 `setter` 정의할 때 소괄호를 넣지 않습니다.
* ☐  `setter`시 외부에서 넣은 값은 `rhs`에 담겨 있습니다.

```
def person
    age int
        get
            ret 22
        set
            print("$rhs, ")

main() void
    print("${person.age}, ")
    person.age = 33
    print("${person.age}")
# 결과: 22, 33, 22

```

* ☐  값을 초기화한 프로퍼티에도 `getter`, `setter`를 넣을 수 있습니다.
* ☐  초기화한 값은 `it`에 담겨 있습니다.

```
def person
    age := 22
        get: it
        set: it = ++rhs

main() void
    print("${person.age}, ")
    person.age = 33
    print("${person.age}")
# 결과: 22, 34

```

* ☐  `def` 에서만 되는 게 아닙니다. 프로퍼티는 함수에서도 만들 수 있었으므로,
함수에서도 `getter`, `setter`를 지정할 수 있습니다.

* ☐  프로퍼티에 대한 참조를 함수에 넘길 수는 없습니다.

```
main() void
    age := 22
        get: it
        set: it = ++rhs
    foo(age) # 안쪽에서도 age의 get, set이 동작할까요?

foo(n int) # age의 get한 값이 n에 들어갈 뿐입니다.
           # n이 main() 함수 안에 우리가 특별하게 만든 age는 아닙니다.
    print("$n, ")
    n = 33
    print("$n")
# 결과: 22, 33

```

* ☐  `getter`, `setter` 를 생략하면 기본동작으로 실행됩니다.

```
age int # 값이 없는 프로퍼티 입니다.
    get: 22
    # 값이 없는데 set이 없습니다. set 호출은 에러입니다.
age = 34 # 에러!

age1 := 0 # 오! 값이 있습니다.
    get: it
    # set이 없습니다만, 값은 있습니다. 그럼 값에다가 set하는 기본동작을 대신 실행시켜줍니다.
print("before=$age1, after=${age1 = 34}")

age2 int # 초기화 구문이 없는데 `get`, `set` 도 없나요?
         # 그럼 이건 기존에 보던 age2 := int() 과 같은 코드입니다.
age2 = 18 # set 잘됩니다.
print(age2) # get도 잘되죠.

```

---

## 우리는 typedef가 필요 없습니다.

* ✅ 변수 A가 있을때, 별칭 B를 만들고 싶으면 우리는 `B := A` 라고 사용합니다.
* ✅ 타입 A가 있을때, 별칭 B를 만들고 싶으면, 다른 언어에서는 새로운 키워드를 사용해야만 했습니다. 예를들어,

```cpp
// C++
typedef A B;

```

```kotlin
// kotlin
typealias B = A;

```

```csharp
// C#: 똑같진 않지만,
using B = A;

```

* ✅ byeol언어는 클래스가 없습니다. 클래스와 객체를 구분하지 않습니다.
* ✅ 그러니 `typedef`를 사용하고 싶다면 `B := A`를 사용하면 됩니다.
* ☐  불완전 객체도 타입으로 사용이 가능합니다.

---

## it

* ✅ 모든 블록문 키워드는 대상이 되는 expr과 블록문으로 구성됩니다.
* ☐  그 블록문을 실행할때 대상이 되는 expr의 값이 `it` 에 담겨있습니다.
* ☐  `it`은 참조하는 코드가 있어야 생성됩니다.

```
for n in {1, 2, 3}
    if it.size == 3 # it은 container 입니다.
        print(n)

if n > 2
    print(it) # it은 `n > 2` 즉 true 입니다.

a := foo()
    2, 3, 4: print(it.age) # it은 a의 값입니다.

with foo()
    print(it.age) # it은 `foo()` 입니다.

```

---

## 기본 타입도 객체

* ✅ 기본 제공되는 타입들도 객체입니다.
* ✅ 기본 제공되는 container 들도 객체입니다.
* ✅ `A B`는 `A := B()` 와 같습니다.

---

## 표현식 기반 언어

* ✅ byeol언어는 표현식기반 언어입니다. 무슨 말이냐면, 대부분의 구문이 값을 반환합니다.
* ✅ 정의문은 가독성 문제가 발생하므로 표현식이 아닙니다.
* ✅ `ret`, `break` 등 흐름에 영향을 주는 구문또한 표현식이 될 수 없습니다.

```
isGood := true
max := if !isGood
    print("1")
    print("2")
    -1 # 블록문은 마지막 표현식을 반환합니다.
else: print("3"); 10
# 그래서 max는 int 타입이고, -1 아니면 10의 값을 가지죠.

val := for n in 0..max # 반복문은 배열을 반환합니다. val은 int[] 타입입니다.
    n
val.len == 10 # true

b := if foo()
    "don't do this"
# else 가 없으므로 b는 str과 void를 타입중에 하나가 되야합니다.
# 그런데 void 타입은 정의할 수 없으므로 결과적으로 이 구문은 컴파일 에러입니다.

```

* ✅ assignment는 expression이 아닙니다. 혼란을 일으키며, 굳이 지원했을때의 장점이 없습니다.

```
if a = get() # 에러!
foo(a = get(), food = getFood()) # 에러!

# 위의 코드는 다음과 같이 변경되어야 합니다:
a = get()
if a == 0: ....

a = get()
food = getFood()
foo(a, food)

```

---

## with

* ✅ `with`는 byeol언어에서 매우 중요한 위치를 차지하는 기능입니다.
* ✅ 그렇다고 개념이 복잡하거나 어려운 것은 아닙니다. 지극히 단순합니다.
* ☐  `with`는 주어진 정의문 혹은 obj의 scope을 주어진 블록문 내부에서만 최상위 scope으로 추가합니다.
* ✅ 이것을 확장이라고 표현합니다.

```
def Person: age := 22

main() void
    p Person
    print(p.age) # 원래 이렇게 해야 하지만,

    with p # 이제 p의 obj scope이 최상단 scope으로 추가되므로,
           # p객체가 가진 모든 것들에 바로 접근할 수 있습니다.
        print(age) # p.age를 간단하게 호출 할 수 있습니다.
    # 블록문이 끝나면 p의 obj scope은 해제됩니다.

    age := 23
    with p2 Person # 정의문이 올 수 있으므로, 이렇게도 가능합니다.
        print(age) # "23", 물론 local scope은 항상 우선순위가 obj scope 보다 높습니다.

```

* ☐  블록문을 제공하지 않고도 `with`를 사용할 수 있습니다.
* ☐  이 경우, 해당 시점부터 `with` 구문을 가지는 블록문에서 벗어날때까지 지속됩니다.

```
pack example
age := 23 # example.age 입니다.

def Person
    age := 30
    sayHello() void
        print("I'm kniz and $age yo.\n")

main() void
    print(age) # "23"

    p Person
    print(p.age) # "30"

    with Person() # 꼭 `with p` 처럼 써야하는 건 아닙니다.
                  # 임시객체도 객체입니다. 그러니 당연히 여기에 올 수 있습니다.
                  # 블록문을 넣지 않았으니, with 효과는 이 블록문, 즉 main() 전체에 이 시점부터 지속됩니다.
    print(age) # "30"
    sayHello() # "I'm kniz and 30 yo."

```

* ☐  블록문 없는 `with`를 2개 이상 선언할 수 있습니다. inline with 구문이라고 합니다.
* ✅ 일반 `with`는 일반 구문을 담을 수 있는 블록문이 있습니다. 그러니 `with` 자체도 일반 구문입니다.
* ✅ 반면 inline `with`는 무언가 값을 내보내는 것이 아닙니다. 따라서 정의문입니다.
* ☐  이 inline `with` 구문 간에 우선순위는 없습니다. 따라서 확장되는 식별자가 겹치지 않도록 조심해야 합니다.

```
def ceo
    sayHello() void: print("work more\n")

def developer
    sayHello() void: print("want to go my home")

def cat
    meow() void: print("meow meow")

main() void
    with ceo()
    sayHello()

    with cat() # 겹치는 식별자가 없습니다.
    meow() # "meow meow"

    with developer() # 에러! sayHello() 이름이 겹칩니다.
    # developer의 sayHello()가 ceo의 sayHello()를 덮어쓰지 않습니다.
    # 단순히 에러로 간주될 뿐입니다.

```

* ☐  만약 inline with를 하는 대상의 일부분만을 확장하려면 only를 사용합니다.

```
def A
  foo() void: ...
  boo() void: ...

main() void
  a A
  with a only
    foo(void) # a는 foo(), boo() 2개 함수가 있지만 a만 이곳에 확장하고 싶습니다.
  foo() # a.foo()와 같은 코드입니다.

```

* ✅ `with` 는 다른 언어에서도 종종 볼 수 있는 기능입니다.
* ✅ 하지만 byeol언어는 다른 언어처럼 `기능 1개 == 키워드 1개`로 보지 않습니다.
* ✅ 기능마다 키워드를 만들지 말고, 가능하다면 개념을 다 풀어헤쳐서 다시 조립해서 더 간결한 사용법을 만들어야 합니다.
* ✅ 여러분은 이 `with` 하나만 파악함으로써, `상속`, `Delegation`, `import`, `static` 을 모두 사용할 수 있습니다.

---

## 타입 확장으로 활용

* ☐  다시한번 말씀드리면, inline `with`는 정의문입니다.
* ✅ byeol언어는 문법의 예외를 싫어합니다.
* ✅ 정의문은 블록정의문에 올 수 있습니다.
* ✅ `def` 뒤에는 블록정의문이 나와야 합니다.
* ☐  그러니 `with`를 `def` 안에서 사용하는 것은 완전히 허용되어야 합니다.
* ✅ 그러면 과연 `def` 안에서 `with`를 쓰면 어떤 의미를 가질까요?

```
def Person
    age int
    name str

def Developer
    with Person() # 이 Person() 임시객체는 이 객체 안에서 유효합니다.
    sayHello() void
        print("I'm $name and you finally have learned Type Extension :)")

main() void
    with Developer()
        age = 27
        name = "kniz"
        sayHello() # "I'm kniz and you finally have learned Type Extension :)"

```

* ☐  `def` 안에서 `with`는 항상 맨 앞에 나와야 합니다.

```
def Developer
    sayHello() void: ...
    with Person # `with` 규칙에 따르면 이 시점부터의 obj scope 보다 우선됩니다.
        sayHello() void # 이렇게 식별자가 겹치면 앞서 정의한 sayHello()를 호출할 방법이 없습니다.
            ....        # 즉 앞서 정의한 sayHello()는 있을 필요가 없는 함수입니다.
                        # 이런걸 사용해야 하는 시나리오는 없어보이고, 이걸 허용하면 혼란만 가중됩니다.
                        # 그래서 `def` 안에서 `with` 는 반드시 첫줄에 입력할 것을 강제합니다.

```

---

## 오버라이딩

* ☐  오버라이딩을 지원합니다.
* ☐  오버라이딩을 한건지, 새로운 함수를 만든건지 걱정이 된다면, 함수명/프로퍼티명 앞에 `+` 를 붙여보세요. 오버라이딩이 아닐 경우 에러가 발생합니다.
* ☐  `_` 기호와 순서는 상관이 없습니다.

```
def Person
    name str
    sayName() str: name

def Developer
    +name := ""
    _+sayName() str: "Miss $name"

```

---

## `A::B` 함수 호출

* ☐  확장은 상속과 유사합니다.

```
main() void
    b B
    b.boo() # "B::foo()"

def A
    boo() void
        foo() # 여기에 왔을때 me obj는 A가 아니라 b 입니다.
              # 그러니 B::foo()가 호출됩니다.
    foo() void
        print("foo!")

def B
    with a A
    foo() void
        print("B::foo()")

```

* ☐  하지만 분명히 다릅니다.

```
pack example

def A
    boo() void
        foo()
    foo() void
        print("A::foo()")

def B
    with a A
    boo() void
        a.boo() # 1) 개발자는 무한 재귀를 피하려 했습니다.
    foo() void
        print("B::foo()")

main() void
    B().boo() # A::foo() 일까요 B::foo() 일까요?

```

* ☐  `1`에서 `a.boo()` 을 했으므로 A의 boo()가 호출됩니다.
* ☐  문제는 `a.` 을 하면서 boo() 함수가 호출되었을때 me obj가 `b` 에서 `a`로 교체된 것에 있습니다.
* ☐  따라서 boo() 함수 안에서 `foo()`를 하게 되면 `me.foo()` 즉, `a.foo()` 와 같은 호출이 됩니다.
* ☐  그러므로 결과는 `A::foo()` 가 됩니다.
* ☐  이렇게 오버라이딩 한 확장한 타입의 원본 함수를 그대로 호출하려면 `super` 를 사용합니다.

```
def A
    boo() void
        foo()
    foo() void
        print("A::foo()")

def B
    with a A
    boo() void
        super() # 1) 개발자는 무한 재귀를 피하려 했습니다.
    foo() void
        print("B::foo()")

main() void
    B().boo() # "B::foo()"

```

* ☐  하지만 만약, `1`에서 원본 함수가 아니라 확장한 타입의 전혀 다른 함수를 호출하고 싶다면 어떨까요?
* ☐  이런 상황을 해결하기 위해서 `A::B` 를 사용합니다.
* ☐  `A::B`는 `A.B`와 마찬가지로 A에 있는 B에 접근합니다. 단, 이 과정에서 obj scope이 변경되지 않습니다.
* ☐  단 `A`는 현재 `me`가 확장한 타입이어야 합니다.

```
pack example

def A
    boo() void
        foo()
    foo() void
        print("A::foo()")

def B
    with a A
    boo() void
        a::boo() # a에 속한 boo() 함수를 찾아 호출할 뿐, me obj는 유지가 됩니다.
                 # me obj는 `.` 을 찍는 순간 변경되거든요.
    foo() void
        print("B::foo()")

main() void
    B().boo() # ""
    A::foo() # 에러! A는 현재 me인 example과 확장관계가 아닙니다.
    with a A
        a::foo() # "A::foo()"
    a::foo() # 에러! a는 현재 me인 example과 더이상 확장관계가 아닙니다. a.foo()를 하세요.

```

---

## 다중 확장

* ☐  당연히 `with`를 여러개 할 수 있습니다.
* ☐  식별자가 겹칠 경우, override를 하면 해결이 가능합니다.

```
def Person
    name := ""
    sayName() str: name
def KeyboardTyper
    name := ""
    typeKey() str: "\n"
def Developer
    with p Person
    with k KeyboardTyper
    # 이대로는 식별자 name 이 겹칩니다.

    +name str # name을 override 합니다.
        get: p.name # 기본적으로 composition 관계입니다.
                    # 어느쪽 name인지 지칭하려면 프로퍼티명을 적으면 됩니다.
    # 이제 컴파일 에러는 발생하지 않게 됩니다.

```

* ☐  식별자가 겹칠때마다 override를 하는 것은 바람직한 방향이 아닙니다.
* ☐  같은 부모 타입을 가진 객체를 모두 `with` 하게 되면 [DDD](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem)를 하는 것입니다. 이런 경우 에러를 내보냅니다.
* ☐  `with`는 가능하면 1개만 하되, 여러개를 할때는 부모 타입이 겹치지 않는 경우에 사용하길 바랍니다.

---

## static으로 활용

* ☐  불완전객체는 자신의 하위 요소들을 사용하지 못합니다.
* ☐  그런데 안에 완전객체가 중첩객체로 들어가 있으면 어떻게 될까요?

```
def Food
    printName() void
        print("Um... I don't know")

    def factory
        makePizza() Food
            def Pizza
                with Food
                +printName() void # override 입니다.
                    print("I'm pizza!")
            ret Pizza()

main() void
    Food.factory.makePizza().printName() # "I'm pizza!"
    Food.printName() # 에러! 불완전객체의 하위요소를 사용은 안됩니다. 기억하시죠?

```

* ☐  함수/프로퍼티의 실행 가능한가에 대한 표현은 그 함수를 가지고 있는 origin 객체가 불완전이냐 아니냐로 표현됩니다.
* ☐  위의 예제에 with를 섞어주면, 다른 언어에서 static이라고 부르는 기능이 됩니다.

```
def Food
    printName() void
        print("Um... I don't know")
    with def # 이름없는 def는 기본 완전객체입니다.
        makePizza() Food
            def()
                with Food()
                +printName() void
                    print("I'm pizza!")

main() void
    Food.makePizza() # 완전객체의 메소드이므로 이 메소드는 호출가능한 상태입니다.
    Food.printName() # 에러!

```

* ☐  그러나 함수의 정의된 객체가 완전객체이므로 확장된 불완전객체에서도 바로 사용할 수 있다는 점 때문에,
     다음과 같이 헷갈릴 수 있습니다.

```
def a
    foo() void: boo()
    boo() void: print("a::boo()\n")

def B
    with a
    boo() void: print("B::boo()\n")

b.foo() # foo()가 정의된 곳은 b가 아니라 a 이므로
        # foo() 함수를 호출 가능하겠다고 생각할지도 모릅니다.
        # 그렇다면 me obj는 B가 아니라 a로 되어야 하는 거겠죠? a의 함수니까요.
# 결과:
#  B()::boo() # 어라? me obj가 a였다면 a::boo() 가 나와야 했는데요?

```

* ☐  B는 a로부터 확장한 것이므로 B의 함수이기도 합니다.
* ☐  그러니 B객체에 foo()를 호출하면 B가 me obj로 등록되는 게 자연스럽습니다.
* ☐  여기까지의 내용을 응용하면 불완전객체를 프로퍼티로 받아 `with`로 함으로써 static 처럼 사용할 수 있습니다.

```
def A
    age := 5

def B
    with a A # A는 불완전객체이지만, A로부터 복제된 a는 완전객체입니다.
print(B.age)

```

---

## delegation으로 활용

* ☐  아시다시피 프로퍼티는 값을 바꿀 수 있습니다.
* ☐  그렇다면 `with` 한 프로퍼티도 값을 바꿀 수 있어야 합니다.

```
def person
    age := 2

main() void
    p person
    p.age = 65
    with p

    print(age) # "65"
    p = person()
    print(age) # "2"

```

* ☐  이것을 응용하면 다른 언어에서 delegate 키워드로 제공하는 것을 대신할 수 있습니다.

```
def Pet
    name str
    eat() void: print("I can't eat")
    fly() void: print("I can't fly")

def dog
    +name := "sadaharu"
    +eat() void: print("$name loves Liberty")
    +fly() void: print("$name can't fly.. or can I?")

def cat
    +name := "teyandee"
    +eat() void: print("$name loves Cats Me!")
    +fly() void: print("$name can fly for some seconds")

def ProxyPet1
    _realPet Pet

    ctor(realPet'): ret
    eat() void: realPet.eat()
    fly() void: realPet.fly()
    # 이렇게 Pet의 모든 함수에 다 delegate 해줘야 할까요?

def ProxyPet2
    with realPet Pet
    ctor(@realPet): ret
    # 이제 realPet의 obj scope이 모두 확장됩니다.
    # realPet.eat(), realPet.fly() 가 포함됩니다.

main() void
    with myPet := ProxyPet1(dog)
        eat() # "sadaharu love Liberty"
        myPet = ProxyPet2(cat)
        eat() # "teyandee love Cats Me!"

```

* ✅ 물론 일부 함수만 delegate 하고 싶다면 직접 코드를 작성해야 합니다.

---

## import

* ✅ byeol언어는 `import` 키워드가 없습니다.
* ✅ 타 언어에서 import의 기능은 2가지 입니다.
    * 1) Module 의존관계 표현
    * 2) 해당 module의 scope을 현재 파일에 확장
* ☐  byeol언어에서는 1은 manifest를 통해 해결합니다.
* ☐  manifest 파일을 작성해서, 어떠한 외부 pack을 가져올 것인지 정의합니다.
* ✅ mainfest는 byeol 언어를 환경설정 목적으로 특화/경량화한 `leaf` 언어로 작성됩니다.
* ✅ `leaf` 언어는 byeol 언어와 거의 똑같습니다.
* ☐  import 하려는 pack들의 충돌 문제를 완화할 수 있습니다.

```
def manifest
    import fjson@chales.lee:1.2.* as json1
    import gjson@giigle:0.7.* ~ 0.8.5 as json2

```

* ☐  import 기능 2은 `with`나 `:=`를 사용합니다.

```
with openai # 이제 openai pack의 모든 sub가 이 파일에 확장됩니다.

main() void
    p openai.Parser
    openai.parseText(p)

    # 위와 같은 코드 입니다:
    p Parser
    parseText(p)

```

* ☐  하나의 객체만 import 하고 싶다면 `:=` 를 사용하면 됩니다.

```
# import os.openai 하고 싶다면:
openai := os.openai

main() void
    openai.doSomething()

```

* ☐  2개 이상의 여러개의 일부만 import를 하고 싶다면 `with only`를 응용합니다.

```
# 아래를 하고 싶다면:
#  import os.openai.Parser
#  import os.openai.parseText
#  import os.openai.generator
with os.openai only
    Parser
    parseText
    generator

main() void
    p Parser
    parseText(p, "rawStringwow!")

```

---

## err

* ✅ byeol언어에 에러는 개념적으로 2가지로 분류됩니다.
    * 1) 사전에 에러가 발생할 수 있다고 고지한 에러
    * 2) 고지하지 않고 발생한 에러. 익셉션이라고 합니다.
* ✅ 익셉션을 포함하여 모든 에러는 Err 객체로부터 확장입니다.
* ✅ Err객체는 msg, callstack등 기본적인 API를 가지고 있습니다.

---

## errorable type

* ☐  사전에 에러가 발생할 수 있다고 고지하면, 좀 더 완만한 처리가 가능합니다.
* ☐  에러는 `?` 로 표현됩니다. 즉 타입에 에러가 발생할 수 있다는 `?` 를 붙이면, 미리 에러가 발생할 수 있다고
     고지할 수 있습니다.
* ☐  이를 ErrorableType이라고 합니다.
* ☐  참고로, nul 도 err를 확장한 객체에 불과합니다.

```
getElem(key str) int? # int일 수도 있고 에러(? 기호) 일 수도 있습니다.
    if key == "": ret outOfBoundErr
    ret map[key]

main() void
    elem int? := nul
    elem = getElem("pizza") # elem은 int? 타입입니다.
        is nul: elem = -1
        is outOfBoundErr: elem = -1
    # err였을 경우 elem에는 -1이 들어갑니다.

```

* ☐  ErrorableType은 결과적으로 처리를 지연시키는 효과를 가집니다.

```
getElem(key str) int? # int일 수도 있고 에러(? 기호) 일 수도 있습니다.
    if key == "": ret outOfBoundErr
    ret map[key]

foo() void? # void?라... 이상한가요? 그런데 말이 되는 타입입니다.
    elem int? := nul
    elem = getElem("pizza") # elem은 int? 타입입니다.
        is err: ret it # 에러가 나기만 했다면 에러로 반환합니다.

main() void
    if foo() is err
        print("error!") # getElem에서 발생한 에러가 이시점까지 지연되었습니다.

```

---

## safe-navigation

* ☐  `?.` 를 사용하면, 에러일때만 함수를 호출할 수 있습니다.

---

## exception이란?

* ✅ 같은 에러라도 고지하지 않은 상태에서 갑자기 에러가 반환하면, 이를 익셉션이라고 합니다.
* ☐  한 줄의 구문을 처리하는 도중에 익셉션이 발생하면, 나머지 처리를 모두 건너뛰고 바로 한 라인의 최종 결과가 됩니다.
* ☐  외부에서 익셉션에 대한 유일한 대처방법은 `pattern matching`을 사용하는 겁니다.
* ☐  일반적인 언어와 달리 try, catch가 없습니다. 기능이 부족하다는 관점이 아니라, 불필요한 개념을 압축한 것이라고 생각해주세요.
* ☐  블록문에 대한 예외처리를 할 수 없습니다. 한줄마다 해야 합니다.

```
arr := getArrayFrom(...) # 얼마나 size가 되는지 모릅니다.
print(arr[3]) # 그래도 일단 접근해보자!
              # 만약 size가 3 이하일 경우, array 객체로부터 outOfBoundErr가 발생합니다.
              # outOfBoundErr는 print() 호출등을 모두 건너뛰고 바로 이 `print(arr[3])`
              # 라인의 최종 결과물이 됩니다.
    is outOfBoundErr
        print(it.msg) # it은 outOfBoundErr 객체입니다.
        ret
doSomethingWhenNoError()

```

* ✅ 익셉션이 처리되지 못하면, 그 시점부터 callstack이 남겨집니다.
* ☐  만약 정의할당 혹은 할당 연산 도중에 익셉션이 발생했다면, 예외처리시 `ret`를 하거나 정상적인 값으로 채워야 합니다.

```
def arr
    get(n int) int # 반환형은 errorable type이 아닙니다.
        if n == 3: ret outOfBoundErr # 그런데 갑자기 err를 반환합니다. 즉 익셉션입니다.
        if n > 4: ret nul

main() void
    val := arr.get(3) # val의 타입은 함수의 반환형으로 추론되어야 합니다.
                      # 그러니 int?가 아니라 int 입니다.
        is outOfBoundErr # 익셉션에 대한 예외처리를 시작합니다.
            # val에는 int타입인데도 err가 들어가 있는 상태입니다.
            val = -1 # 그러니 다시 int 타입으로 채워넣지 않으면 에러입니다.
        is nul
            # 이번에는 함수를 종료했습니다.
            ret

```

* ☐  만약 local scope에 속하지 않은 프로퍼티를 할당연산 도중에 exception의 예외처리를 하면, 반드시 이 프로퍼티의 값을 채워야 합니다.

```
def person
    age int
    foo() void
        age = arr().get(3) # 안에서 outOfBoundErr가 exception이 발생합니다.
            is err
                age = -1 # age는 지역변수가 아닙니다.
                         # 따라서 바로 ret를 해버리면 age에는 여전히 exception이 담겨있게 됩니다.
                         # 그래서 age를 적절한 int의 값으로 채우지않으면 컴파일 에러가 발생합니다.
                ret

```

---

## abstract

* ☐  프로퍼티나 함수를 abstract로 선언할 수 있습니다.
* ☐  프로퍼티나 함수를 abstract로 선언하면 해당 식별자를 가진 객체는 자동으로 abstract로 정의됩니다.
* ☐  abstract 객체를 확장한 객체는 모든 abstract 요소를 오버라이딩해야 객체로 생성할 수 있습니다.
* ✅ abstract 함수는 body를 명시하지 않으면 됩니다.

```
add(a int, b int) int # 이 함수는 abstract 합니다. 이 함수를 바로 호출 할 수 없습니다.
foo(cl add) int
  ret cl() # 타입으로 사용가능합니다.

main() void
  foo((a, b): a + b)

```

* ☐  abstract 프로퍼티는 2가지 방법으로 정의합니다.
* ☐  첫번째 방법은 `get; set;` 으로 get과 set의 구현부를 비워주는 겁니다.
* ☐  프로퍼티를 정의하고 `get`, `set`을 정의한다는 것은, 이 프로퍼티의 get, set 이 발생할때 값을 내보내는 대신, 개발자가 정의할 적절한 동작의 결과를 대신 내보내겠다는 의미일 것입니다.
* ☐  바로 그 초기값이 없는 프로퍼티를 만들어 놓고 `get`, `set`을 생략해버리면 인터프리터 입장에서 그 기본 동작으로, 어떤 것도 대신 해줄 수 없습니다. 즉 호출시 컴파일 에러로 처리하는 게 최선입니다.
* ☐  그러니 `get`, `set`을 정의할 뿐 구현부를 비워놓으면 `get`도 `set` 도 불가능한, abstract 한 프로퍼티가 됩니다.

```
def Base
    age int
        get; set; # 초기값이 없는데 getter, setter를 지정한다는 건, 사용자가 이 프로퍼티에 대한 get과 set을
                  # 채우겠다는 뜻 일겁니다.
                  # 그런데 정작 구현부가 없습니다. 아무런 동작을 채우지 않았으므로, 이 프로퍼티를 사용하지
                  # 못하게 됩니다. 즉 사용하지 못하지만, interface 로써의 프로퍼티가 존재하는 셈이고,
                  # 우리는 이걸 abstract 라고 표현합니다.

```

* ☐  두번째 방법은 abstract 타입으로 정의하는 겁니다.

```
def Base # abstract를 가지고 있으니 불완전객체로 정의해야 했습니다.
    fly() void # abstract 함수입니다.

def Derived
    b Base # abstract 타입으로 프로퍼티를 만들었고, 초기값도 넣지 않았습니다.
           # 그렇다고 errorable type (Base?) 도 아닙니다. Base? 였다면 nul이라도 대신 넣어줄 수 있습니다.
           # 즉 b는 자연스럽게 abstract 프로퍼티가 됩니다.

```

* ☐  abstract 객체로부터 객체를 생성하고 싶다면 반드시 abstract 요소를 모두 오버라이딩으로 채워야 합니다.

```
def Base
    age int: get;set # abstract 프로퍼티
    fly() void # abstract 함수

def derived
    with Base # abstract 타입인 Base로부터 확장했습니다. Base의 abstract 요소인 age, fly()를 모두 채워야 합니다.
    +age := 1
    +fly() void: print("fly!\n")
    # 오버라이딩으로 채웠습니다. derived는 더이상 abstract 객체가 아닙니다.

main() void
  b Base := derived()
  print(b.age) # "0"

```

* ☐  abstract 타입으로 프로퍼티를 만들었어도 생성자에서 채우면 abstract가 아닙니다.

```
def Base # abstract 객체 입니다.
    fly() void # abstract 함수입니다.

def Derived
    _b Base # abstract 타입으로 프로퍼티를 만들었으니 Derived는 abstract 객체가 됩니다.
    ctor(newB Base)
        b = newB # 그런데 생성자에서 abstract 한 프로퍼티에 구체값을 채워줬습니다.
# 더이상 Derived는 abstract 하지 않습니다.

```

* ☐  인터프리터는 생성자에서 abstract 프로퍼티를 할당연산으로 채워넣었는가 아닌가를 지능적으로 판단합니다.
* ☐  다음은 그 조건들 입니다.
    * 1) 생성자 안에서 오버라이딩 되지 않은 모든 abstract 프로퍼티를 생성자에서 채워넣어야 abstract 객체가 아니게 됩니다.
    * 2) 생성자 안에서 `if`, `Pattern Matching`을 사용한 경우, 어떠한 경로로 분기하더라도 abstract 프로퍼티를 모두 채워넣어야 합니다.
    * 3) 분기 도중 `ret` 을 하는 경우, ret 되는 시점에서 모든 abstract 프로퍼티에 채워넣어져 있어야 합니다.
    * 4) 모든 생성자가 위 조건을 만족해야 합니다.
    * 5) 분기문에서의 조건은 고려하지 않습니다. 즉 `if true` 같이 절대로 `else` 분기하지 않는다고 하더라도, 인터프리터는 이를 고려하지 않습니다.
* ☐  abstract 한 불완전객체인지, 구체타입이지만 개발자가 원하지 않아서 불완전객체인지를 키워드로 구별할 방법은 없습니다.
* ☐  인터프리터는 코드를 보고 abstract 요소가 있는지 구분하고 파악하고 있습니다.
* ☐  확장한 객체의 abstract 요소를 모두 오버라이딩하지 않고 객체를 생성하려고 시도하면 에러로 어떠어떠한 abstract 요소를 채우지 않았다고 알려줍니다.

* ✅ byeol언어는 모든 언어들의 편의 기능을 제공하는 것에는 관심이 없습니다.
* ✅ byeol언어는 불편하지는 않으면서, 어떻게 하면 종래의 개념과 문법을 간결하게 풀어낼 수 있을지에 더 관심이 있습니다.
* ✅ 그래서 byeol언어는 다른 언어들에 있는 몇가지 문법/개념을 일부러 제공하지 않습니다.

---

## 함수타입 대신 메소드을 사용

* ✅ byeol언어는 함수타입을 제공하지 않습니다.
* ✅ 함수에 대한 정의 자체가 함수에 대한 타입입니다.

```
def Person # 우리는 객체를 정의했고,
  name str
p Person # 정의했으니 타입으로 쓸 수 있습니다.

# 그렇다면,
foo(n int) int: ret n # 함수를 정의했으니,
dispatch(listener foo) int # 함수를 타입으로 쓰는게 맞지 않을까요?
  ret listener(3)

main() void
  print(dispatch((n): n + 1)) # "4"

```

* ☐  함수 이름만 가지고 구분이 되지 않으면 signature를 적어야 합니다.

```
foo(n int) int: ret 0
foo(n flt) flt: ret 0.1
dispatch(listener foo(n int)) int
  ret listener(3)

```

* ✅ 하지만 그보다는 이름을 바꿉시다.

```
OnEvent(n int): int
dispatch(listener OnEvent) int
  ret listener(3)

```

---

## 제네릭

* ✅ origin 객체를 정의할때 이름 뒤에 \<타입인자목록\> 을 추가함으로써 제네릭 객체를 정의할 수 있습니다.

```
# def 객체이름<타입인자목록>
#    <정의블록문>
def adder<T, R>()
  add(T a, T b) R -> a + b

int1 := adder<int, int>.add(1, 2)
str2 := adder<str, str>.add("nice", "guy")

main() void
  print("$int1 \n")
  print(str2)
# 결과:
#  3
#  niceguy

```

* ✅ 함수에 대한  제네릭은 지원하지 않아요.
* ✅ Type erasure 같은 불완전한 구현이 아닙니다.

---

## enumeration

* ☐  TODO

---

## Annotation

* ☐  Annotation은 `:` 으로 표현할 예정입니다.

---

# 끝맺으면서

* 간결한 언어입니다.

* byeol:
    * pack, def, in, as, is, for, while, next, break, with, if, else, ret, enum, get, set, end, only

* C:
    * break, case, const, continue, return, default, do, while, else, enum, extern, for, goto, if,
    * signed, sizeof, static, struct, switch, typedef, union, unsigned, volatile, while

* python:
    * await, else, import, pass, break, except, in, raise, class, finally, is, return, and, continue,
    * for, lambda, try, as, def, from, nonlocal, while, assert, del, global, not, with, async, elif,
    * if, or, yield

---
**다음 문서**: @ref ad-design-philosophy
